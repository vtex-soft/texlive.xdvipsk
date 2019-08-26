/* Copyright (c) 2016 Arunas Povilaitis VTeX Ltd. All rights reserved.
*  This code is released  under the BSD 3-Clause license
*/

/*
* COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:
*
* If you modify make2unc you may insert additional notices immediately following
* this sentence.
*
*
* This code is released under  the BSD 3-Clause license
* and with the following additions to the disclaimer:
*
*    There is no warranty against interference with your enjoyment of the
*    program or against infringement.  There is no warranty that our
*    efforts or the program will fulfill any of your particular purposes
*    or needs.  This program is provided with all faults, and the entire
*    risk of satisfactory quality, performance, accuracy, and effort is with
*    the user.
*
*
* The utility make2unc is supplied "AS IS".  The Author
* and VTeX Ltd. disclaim all warranties, expressed or implied,
* including, without limitation, the warranties of merchantability and of
* fitness for any purpose.  The Author and VTeX Ltd.
* assume no liability for direct, indirect, incidental, special, exemplary,
* or consequential damages, which may result from the use of this
* utility, even if advised of the possibility of such damage.
*
* Permission is hereby granted to use, copy, modify, and distribute this
* source code, or portions hereof, for any purpose, without fee, subject
* to the following restrictions:
*
*   1. The origin of this source code must not be misrepresented.
*
*   2. Altered versions must be plainly marked as such and must not
*      be misrepresented as being the original source.
*
*   3. This Copyright notice may not be removed or altered from any
*      source or altered source distribution.
*
* END OF COPYRIGHT NOTICE, DISCLAIMER, and LICENSE.
*/

/*
* This utility uses:
*
* mupdf open source library. See http://mupdf.com for details.
* mupdf is used under GNU Affero General Public License.
*
*/


#include <stdio.h>
#if defined(__linux__)
#include <cstring>
#else
#include <string>
#endif
#include <map>
#include <vector>
#include <list>

#include "mupdf/fitz.h"
#include "mupdf/pdf.h"
//#include "mupdf/pdf/name-table.h"

#define BANNER \
"This is make2unc 1.0 Copyright 2016 VTeX Ltd"
#define BANNER2 "(www.vtex.lt)"
#define OTFMAPFILEDIR ".xdvipsk/"

#define CMAP_BEGIN "\
%%!PS-Adobe-3.0 Resource-CMap\n\
%%%%DocumentNeededResources: ProcSet(CIDInit)\n\
%%%%IncludeResource: ProcSet(CIDInit)\n\
%%%%BeginResource: CMap(%s)\n\
%%%%Title: (%s)\n\
%%%%Version: 1.000\n\
%%%%EndComments\n\
/CIDInit /ProcSet findresource begin\n\
12 dict begin\n\
begincmap\n\
"

#define CMAP_END "\
endcmap\n\
CMapName currentdict /CMap defineresource pop\n\
end\n\
end\n\
%%EndResource\n\
%%EOF\n\
"

#define CMAP_CSI_FMT "/CIDSystemInfo <<\n\
  /Registry (%s)\n\
  /Ordering (%s)\n\
  /Supplement %s\n\
>> def\n"


/* DIRSEP is the char that separates directories from files */
#if defined MSDOS || defined OS2 || defined(ATARIST) || defined(WIN32)
#define DIRSEP "\\"
#else
#define DIRSEP "/"
#endif

#if defined(WIN32)
#include <io.h>
#define access _access
#else
#include <unistd.h>
#endif

struct Options {
  bool show_config;
  bool run_silently;
  bool linearize;
  bool writelog;
  std::string exe_path;
  std::string cmap_directory;
  std::string rez_name;
  FILE *log_file;
};

struct mrange
{
	unsigned int low, high, len, n, out[8];
};

struct cmap_block
{
	char type;		//c - bfchar, r - bfrange
	unsigned int size;
	mrange r[100];
};

int log_records_count;

void
writelogrecord(FILE *Exe_Log, const char *s)
{
	char *str, *s1;

	if (Exe_Log && s) {
		str = (char *)malloc(strlen(s) + 30);
		strcpy(str, "Error: ");
		s1 = (char *)s;
		while (*s1 == ' ')
			s1++;
		strcat(str, s1);
		if (str[strlen(str) - 1] != '\n') {
			strcat(str, "\n");
		}
		fputs(str, Exe_Log);
		fflush(Exe_Log);
		log_records_count++;
		free(str);
	}
}

bool
IsLittleEndian() {
	union {
		unsigned int i;
		unsigned char c[4];
	} u;
	u.i = 1;
	return (u.c[0] != 0);
}

unsigned short swap2(unsigned short value)
{
	char	*ptr, c;

	ptr = (char *)&value;

	c = *ptr;
	*ptr = *(ptr + 1);
	*(ptr + 1) = c;

	return(value);
}

unsigned int swap(unsigned int value)
{
	unsigned char	*ptr, c;

	ptr = (unsigned char *)&value;
	c = *(ptr + 1);
	*(ptr + 1) = *(ptr + 2);
	*(ptr + 2) = c;

	c = *ptr;
	*ptr = *(ptr + 3);
	*(ptr + 3) = c;

	return(value);
}

int
sputx(unsigned char c, char **s, char *end)
{
	char hi = (c >> 4), lo = c & 0x0f;

	if (*s + 2 > end)
		return 0;
	**s = (hi < 10) ? hi + '0' : hi + '7';
	*(*s + 1) = (lo < 10) ? lo + '0' : lo + '7';
	*s += 2;

	return 2;
}

int
sputi(unsigned int c, char **s, char *end)
{
	unsigned short val1;
	unsigned int val2;
	int n, j;
	char *p;

	if (c <= 0xFFFF) {
		if (IsLittleEndian())
			val1 = swap2(c);
		else
			val1 = c;
		p = (char *)&val1;
		n = 2;
	}
	else {
		if (IsLittleEndian())
			val2 = swap(c);
		else
			val2 = c;
		p = (char *)&val2;
		n = 4;
	}
	for (j = 0; j < n; j++) {
		sputx(p[j], s, end);
	}
	return 1;
}

int print_cmap(fz_context *ctx, fz_buffer *out, pdf_cmap *cmap,
	std::string &registry, std::string ordering, std::string supplement)
{
	pdf_range *ranges = cmap->ranges;
	pdf_xrange *xranges = cmap->xranges;
	pdf_mrange *mranges = cmap->mranges;
	int i, r, x, m;
	unsigned int k;
	int *ptr, len;

	std::list<cmap_block> body;
	std::list<cmap_block>::iterator itbody;
	cmap_block block;

	m = 0;
	block.size = 0;
	block.type = 'u';

	for (r = 0; r < cmap->rlen; r++) {
		while (mranges && (ranges[r].low > mranges[m].low) && (mranges[m].low > 0)) {
			if (block.type == 'u')
				block.type = 'c';
			if ((block.type != 'c') || (block.size > 99)) {
				body.push_back(block);
				block.size = 0;
				block.type = 'c';
			}
			block.r[block.size].low = mranges[m].low;
			block.r[block.size].high = mranges[m].low;
			block.r[block.size].n = 2;
			ptr = &cmap->dict[cmap->mranges[m].out];
			len = *ptr++;
			block.r[block.size].len = len;
			memcpy(block.r[block.size].out, ptr, len * sizeof(unsigned int));
			block.size++;
			if (m < cmap->mlen)
				m++;
			else
				break;
		}
		if (ranges[r].low == ranges[r].high) {
			if (block.type == 'u')
				block.type = 'c';
			if ((block.type != 'c') || (block.size > 99)) {
				body.push_back(block);
				block.size = 0;
				block.type = 'c';
			}
			block.r[block.size].low = ranges[r].low;
			block.r[block.size].high = ranges[r].high;
			block.r[block.size].n = 2;
			block.r[block.size].len = 1;
			block.r[block.size].out[0] = ranges[r].out;
			block.size++;
		}
		else {
/*
			if (block.type == 'u')
				block.type = 'r';
			if ((block.type != 'r') || (block.size > 99)) {
				body.push_back(block);
				block.size = 0;
				block.type = 'r';
			}
			block.r[block.size].low = ranges[r].low;
			block.r[block.size].high = ranges[r].high;
			block.r[block.size].n = 2;
			block.r[block.size].len = 1;
			block.r[block.size].out[0] = ranges[r].out;
			block.size++;
*/
			if (block.type == 'u')
				block.type = 'c';
			if (block.size > 99) {
				body.push_back(block);
				block.size = 0;
				block.type = 'c';
			}
			for (k = 0; k < (unsigned int)(ranges[r].high - ranges[r].low + 1); k++) {
				block.r[block.size].low = ranges[r].low + k;
				block.r[block.size].high = ranges[r].low + k;
				block.r[block.size].n = 2;
				block.r[block.size].len = 1;
				block.r[block.size].out[0] = ranges[r].out + k;
				block.size++;
				if (block.size > 99) {
					body.push_back(block);
					block.size = 0;
					block.type = 'c';
				}
			}
		}
	}

	for (x = 0; x < cmap->xlen; x++) {
		while (mranges && (xranges[x].low > mranges[m].low) && (mranges[m].low > 0)) {
			if (block.type == 'u')
				block.type = 'c';
			if ((block.type != 'c') || (block.size > 99)) {
				body.push_back(block);
				block.size = 0;
				block.type = 'c';
			}
			block.r[block.size].low = mranges[m].low;
			block.r[block.size].high = mranges[m].low;
			block.r[block.size].n = 2;
			ptr = &cmap->dict[cmap->mranges[m].out];
			len = *ptr++;
			block.r[block.size].len = len;
			memcpy(block.r[block.size].out, ptr, len * sizeof(unsigned int));
			block.size++;
			if (m < cmap->mlen)
				m++;
			else
				break;
		}
		if (xranges[x].low == xranges[x].high) {
			if (block.type == 'u')
				block.type = 'c';
			if ((block.type != 'c') || (block.size > 99)) {
				body.push_back(block);
				block.size = 0;
				block.type = 'c';
			}
			block.r[block.size].low = xranges[x].low;
			block.r[block.size].high = xranges[x].high;
			block.r[block.size].n = 4;
			block.r[block.size].len = 1;
			block.r[block.size].out[0] = xranges[x].out;
			block.size++;
		}
		else {
/*
			if (block.type == 'u')
				block.type = 'r';
			if ((block.type != 'r') || (block.size > 99)) {
				body.push_back(block);
				block.size = 0;
				block.type = 'r';
			}
			block.r[block.size].low = xranges[x].low;
			block.r[block.size].high = xranges[x].high;
			block.r[block.size].n = 4;
			block.r[block.size].len = 1;
			block.r[block.size].out[0] = xranges[x].out;
			block.size++;
*/
			if (block.type == 'u')
				block.type = 'c';
			if (block.size > 99) {
				body.push_back(block);
				block.size = 0;
				block.type = 'c';
			}
			for (k = 0; k < (unsigned int)(xranges[x].high - xranges[x].low + 1); k++) {
				block.r[block.size].low = xranges[x].low + k;
				block.r[block.size].high = xranges[x].low + k;
				block.r[block.size].n = 2;
				block.r[block.size].len = 1;
				block.r[block.size].out[0] = xranges[x].out + k;
				block.size++;
				if (block.size > 99) {
					body.push_back(block);
					block.size = 0;
					block.type = 'c';
				}
			}
		}
	}

	if (block.size > 0) {
		if (block.type == 'r') {
			body.push_back(block);
			block.size = 0;
			block.type = 'c';
		}
		while (m < cmap->mlen) {
			if ((block.type != 'c') || (block.size > 99)) {
				body.push_back(block);
				block.size = 0;
				block.type = 'c';
			}
			block.r[block.size].low = mranges[m].low;
			block.r[block.size].high = mranges[m].low;
			block.r[block.size].n = 2;
			ptr = &cmap->dict[cmap->mranges[m].out];
			len = *ptr++;
			block.r[block.size].len = len;
			memcpy(block.r[block.size].out, ptr, len * sizeof(unsigned int));
			block.size++;
			m++;
		}
		if (block.size > 0) {
			body.push_back(block);
			block.size = 0;
			block.type = 'u';
		}
	}
	else {
		if (block.type == 'u')
			block.type = 'c';
		while (m < cmap->mlen) {
			if ((block.type != 'c') || (block.size > 99)) {
				body.push_back(block);
				block.size = 0;
				block.type = 'c';
			}
			block.r[block.size].low = mranges[m].low;
			block.r[block.size].high = mranges[m].low;
			block.r[block.size].n = 2;
			ptr = &cmap->dict[cmap->mranges[m].out];
			len = *ptr++;
			block.r[block.size].len = len;
			memcpy(block.r[block.size].out, ptr, len * sizeof(unsigned int));
			block.size++;
			m++;
		}
		if (block.size > 0) {
			body.push_back(block);
			block.size = 0;
			block.type = 'u';
		}
	}

	if (body.size() == 0)
		return 0;

	std::string cmapName = registry;
	cmapName.append("-");
	cmapName += ordering;
	cmapName.append("-");
	cmapName += supplement;
	char str[1024], *wbuf;
	sprintf(str, CMAP_BEGIN, cmapName.c_str(), cmapName.c_str());
	fz_append_data(ctx, out, str, strlen(str));
	sprintf(str, CMAP_CSI_FMT, registry.c_str(), ordering.c_str(), supplement.c_str());
	fz_append_data(ctx, out, str, strlen(str));
	sprintf(str, "/CMapName /%s def\n", cmapName.c_str());
	fz_append_data(ctx, out, str, strlen(str));
	sprintf(str, "/CMapType %d def\n", 2);
	fz_append_data(ctx, out, str, strlen(str));
	if (cmap->wmode != 0) {
		sprintf(str, "/WMode %d def\n", cmap->wmode);
		fz_append_data(ctx, out, str, strlen(str));
	}

	/* codespacerange */
	sprintf(str, "%d begincodespacerange\n", cmap->codespace_len);
	fz_append_data(ctx, out, str, strlen(str));
	memset(str, 0, sizeof(str));
	wbuf = &str[0];
	for (i = 0; i < cmap->codespace_len; i++) {
		*(wbuf)++ = '<';
		sputi(cmap->codespace[i].low, &(wbuf), wbuf + 10);
		*(wbuf)++ = '>';
		*(wbuf)++ = ' ';
		*(wbuf)++ = '<';
		sputi(cmap->codespace[i].high, &(wbuf), wbuf + 10);
		*(wbuf)++ = '>';
		*(wbuf)++ = '\n';
	}
	fz_append_data(ctx, out, str, strlen(str));
	fz_append_data(ctx, out, "endcodespacerange\n", strlen("endcodespacerange\n"));

	/* CMap body */
	for (itbody = body.begin(); itbody != body.end(); itbody++) {
		unsigned int i, m;
		cmap_block *block;
		block = &(*itbody);
		switch (block->type) {
		case 'c':
			sprintf(str, "%d beginbfchar\n", block->size);
			fz_append_data(ctx, out, str, strlen(str));
			for (i = 0; i < block->size; i++) {
				memset(str, 0, sizeof(str));
				wbuf = &str[0];
				*(wbuf)++ = '<';
				sputi(block->r[i].low, &(wbuf), wbuf + 10);
				*(wbuf)++ = '>';
				*(wbuf)++ = ' ';
				*(wbuf)++ = '<';
				if (block->r[i].len == 1) {
					if (block->r[i].out[0] > 0x10000) {
						unsigned int val = block->r[i].out[0] - 0x10000;
						unsigned int val0 = (val >> 10) + 0xD800;
						unsigned int val1 = (val & 0x000003ff) + 0xDC00;
						sputi(val0, &(wbuf), wbuf + 10);
						sputi(val1, &(wbuf), wbuf + 10);
					}
					else
						sputi(block->r[i].out[0], &(wbuf), wbuf + 10);
				}
				else {
					for (m = 0; m < block->r[i].len; m++) {
						sputi(block->r[i].out[m], &(wbuf), wbuf + 10);
					}
				}
				*(wbuf)++ = '>';
				*(wbuf)++ = '\n';
				fz_append_data(ctx, out, str, strlen(str));
			}
			fz_append_data(ctx, out, "endbfchar\n", strlen("endbfchar\n"));
			break;
		case 'r':
			sprintf(str, "%d beginbfrange\n", block->size);
			fz_append_data(ctx, out, str, strlen(str));
			for (i = 0; i < block->size; i++) {
				memset(str, 0, sizeof(str));
				wbuf = &str[0];
				*(wbuf)++ = '<';
				sputi(block->r[i].low, &(wbuf), wbuf + 10);
				*(wbuf)++ = '>';
				*(wbuf)++ = ' ';
				*(wbuf)++ = '<';
				sputi(block->r[i].high, &(wbuf), wbuf + 10);
				*(wbuf)++ = '>';
				*(wbuf)++ = ' ';
				*(wbuf)++ = '<';
				if (block->r[i].len == 1) {
					if (block->r[i].out[0] > 0x10000) {
						unsigned int val = block->r[i].out[0] - 0x10000;
						unsigned int val0 = (val >> 10) + 0xD800;
						unsigned int val1 = (val & 0x000003ff) + 0xDC00;
						sputi(val0, &(wbuf), wbuf + 10);
						sputi(val1, &(wbuf), wbuf + 10);
					}
					else
						sputi(block->r[i].out[0], &(wbuf), wbuf + 10);
				}
				else {
					for (m = 0; m < block->r[i].len; m++) {
						sputi(block->r[i].out[m], &(wbuf), wbuf + 10);
					}
				}
				*(wbuf)++ = '>';
				*(wbuf)++ = '\n';
				fz_append_data(ctx, out, str, strlen(str));
			}
			fz_append_data(ctx, out, "endbfrange\n", strlen("endbfrange\n"));
			break;
		}
	}

	fz_append_data(ctx, out, CMAP_END, strlen(CMAP_END));
	return 1;
}

void AddUnicodeCmaps(fz_context *ctx, pdf_document *doc, pdf_obj* cmaps_array, const std::string &cmaps_path) {
	int i, k; 

	std::string cmaps_dir = cmaps_path;
	if (!cmaps_dir.empty()) {
		if ((cmaps_dir.at(cmaps_dir.size() - 1) != '\\') && (cmaps_dir.at(cmaps_dir.size() - 1) != '/'))
			cmaps_dir.append(DIRSEP);
	}

	std::map<std::string, std::list<pdf_cmap *> *> cmaps_map;
	std::pair<std::string, std::list<pdf_cmap *> *> cmap_info;
	std::map<std::string, std::list<pdf_cmap *> *>::iterator itcmap;

	std::map<std::string, pdf_cmap *> cmaps_obj_map;
	std::pair<std::string, pdf_cmap *> cmap_obj_info;
	std::map<std::string, pdf_cmap *>::iterator itcmap_obj;

	int length = pdf_array_len(ctx, cmaps_array);

	for (k = 0; k < length; k++) {
		pdf_obj *cmaps = pdf_array_get(ctx, cmaps_array, k);
		int n = pdf_dict_len(ctx, cmaps);
		for (i = 0; i < n; i++) {
			pdf_obj *key = pdf_dict_get_key(ctx, cmaps, i);
			pdf_obj *val = pdf_dict_get_val(ctx, cmaps, i);
			std::string cmapFile = cmaps_dir;
			cmapFile.append(pdf_to_str_buf(ctx, val));
			if (access(cmapFile.c_str(), 0) == -1)
				continue;
			fz_try(ctx)
			{
				fz_stream *stm = fz_open_file(ctx, cmapFile.c_str());
				pdf_cmap *pmap = pdf_load_cmap(ctx, stm);
				fz_drop_stream(ctx, stm);
				itcmap = cmaps_map.find(pdf_to_name(ctx, key));
				if (itcmap == cmaps_map.end()) {
					cmap_info.first = pdf_to_name(ctx, key);
					cmap_info.second = new std::list<pdf_cmap *>;
					cmap_info.second->push_back(pmap);
					cmaps_map.insert(cmap_info);
				}
				else
					itcmap->second->push_back(pmap);
			}
			fz_catch(ctx)
			{
				continue;
			}
		}
	}

	for (itcmap = cmaps_map.begin(); itcmap != cmaps_map.end(); itcmap++) {
		cmap_obj_info.first = itcmap->first;
		if (itcmap->second->size() == 1) {
			cmap_obj_info.second = itcmap->second->front();
			cmaps_obj_map.insert(cmap_obj_info);
		}
		else {
			std::list<pdf_cmap *>::iterator itobj;
			pdf_cmap *new_cmap = pdf_new_cmap(ctx);
			pdf_set_cmap_wmode(ctx, new_cmap, 0);
			pdf_add_codespace(ctx, new_cmap, 0, 65535, 2);
			for (k = 1; k < 65536; k++) {
				int rez[32];
				for (itobj = itcmap->second->begin(); itobj != itcmap->second->end(); itobj++) {
					pdf_cmap *orig_cmap = *itobj;
					int len = pdf_lookup_cmap_full(orig_cmap, k, rez);
					if (len == 0)
						continue;
					else {
						pdf_map_one_to_many(ctx, new_cmap, k, rez, len);
						break;
					}
				}
			}
			for (itobj = itcmap->second->begin(); itobj != itcmap->second->end(); itobj++)
				pdf_drop_cmap(ctx, *itobj);
			pdf_sort_cmap(ctx, new_cmap);
			cmap_obj_info.second = new_cmap;
			cmaps_obj_map.insert(cmap_obj_info);
		}
		delete itcmap->second;
	}

	int xref_len = pdf_xref_len(ctx, doc);

	for (i = 1; i < xref_len; i++)
	{
		pdf_xref_entry *entry = pdf_get_xref_entry(ctx, doc, i);
		if (entry->type == 0 || entry->type == 'f')
			continue;

		fz_try(ctx)
		{
			pdf_obj *dict = pdf_load_object(ctx, doc, i);
			if (pdf_is_dict(ctx, dict)) {
				pdf_obj *obj = pdf_dict_get(ctx, dict, PDF_NAME_Type);
				if (pdf_name_eq(ctx, obj, PDF_NAME_Font)) {
					obj = pdf_dict_get(ctx, dict, PDF_NAME_Subtype);
					if (pdf_name_eq(ctx, obj, PDF_NAME_Type0)) {
						obj = pdf_dict_get(ctx, dict, PDF_NAME_ToUnicode);
						if (!obj) {
							obj = pdf_dict_get(ctx, dict, PDF_NAME_BaseFont);
							if (obj) {
								std::string fntName = pdf_to_name(ctx, obj);
								if ((fntName.size() > 7) && (fntName.at(6) == '+'))
									fntName = fntName.substr(7);
								int pos = fntName.rfind("-TeX_Identity-H");
								if (pos > 0)
									fntName = fntName.substr(0, pos);
								else {
									pos = fntName.rfind("-Identity-H");
									if (pos > 0)
										fntName = fntName.substr(0, pos);
								}
								itcmap_obj = cmaps_obj_map.find(fntName);
								if (itcmap_obj == cmaps_obj_map.end())
									continue;
								pdf_cmap *orig_cmap = itcmap_obj->second;
								fz_buffer *buf = fz_new_buffer(ctx, 1024);
								std::string registry = "TeX";
								std::string ordering = pdf_to_name(ctx, obj);
								if (pos > 0) {
									if ((ordering.size() > 7) && (ordering.at(6) == '+'))
										pos += 7;
									ordering = ordering.substr(0, pos);
								}
								if ((ordering.size() > 7) && (ordering.at(6) == '+'))
									ordering.at(6) = '-';
								std::string supplement = "0";
								if (!print_cmap(ctx, buf, orig_cmap, registry, ordering, supplement))
									continue;
								pdf_dict_put_drop(ctx, dict, PDF_NAME_ToUnicode, pdf_add_stream(ctx, doc, buf, NULL, 0));
								fz_drop_buffer(ctx, buf);
							}
						}
					}
				}
			}
		}
		fz_catch(ctx)
		{
			continue;
		}
	}
	for (itcmap_obj = cmaps_obj_map.begin(); itcmap_obj != cmaps_obj_map.end(); itcmap_obj++) {
		pdf_drop_cmap(ctx, itcmap_obj->second);
	}
}


int ModifyPdfForUnicodes(const std::string& name, const Options& options) {
	fz_context *ctx;
	pdf_write_options opts = { 0 };
	pdf_document *doc;
	bool renfile = false;
	char buff[512];

	std::string source(name);
	std::string result(options.rez_name);
	std::string src_path("");
	int pos = source.rfind("\\");
	if (pos == -1)
		pos = source.rfind("/");

	if (pos > 0) {
		src_path = source.substr(0, pos + 1);
	}
	src_path.append(OTFMAPFILEDIR);

	if (access(name.c_str(), 0) == -1) {
		sprintf(buff, "PDF file %s not found.\n", name.c_str());
		fprintf(stderr, buff);
		if (options.writelog && options.log_file)
			writelogrecord(options.log_file, buff);
		return 1;
	}
	else {
		if (!options.run_silently)
			fprintf(stderr, "Modifying PDF file %s.\n", name.c_str());
	}

	ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
	if (!ctx)
	{
		sprintf(buff, "Cannot initialise MuPDF context");
		fprintf(stderr, buff);
		if (options.writelog && options.log_file)
			writelogrecord(options.log_file, buff);
		return 1;
	}

	fz_try(ctx)
	{
		fz_register_document_handlers(ctx);
		doc = pdf_open_document(ctx, name.c_str());
		if (pdf_needs_password(ctx, doc)) {
			sprintf(buff, "File %s requires password", name.c_str());
			fprintf(stderr, buff);
			if (options.writelog && options.log_file)
				writelogrecord(options.log_file, buff);
			return 1;
		}
	}
	fz_catch(ctx)
	{
		sprintf(buff, "MuPDF error opening file %s", name.c_str());
		fprintf(stderr, buff);
		if (options.writelog && options.log_file)
			writelogrecord(options.log_file, buff);
		return 1;
	}

	fz_try(ctx)
	{
		opts.do_linear = options.linearize ? 1 : 0;
		opts.do_garbage = 1;
		opts.do_compress = 1;
		if (doc->linear_obj)
			opts.do_linear = 1;
		if (result.empty()) {
			result = source;
			result.replace(result.rfind("."), 4, "_copy.pdf");
			renfile = true;
		}
		pdf_obj* root = pdf_dict_get(ctx, pdf_trailer(ctx, doc), PDF_NAME_Root);
		if (root == NULL) {
			sprintf(buff, "Possibly damaged file %s", name.c_str());
			fprintf(stderr, buff);
			pdf_drop_document(ctx, doc);
			if (options.writelog && options.log_file)
				writelogrecord(options.log_file, buff);
			return 1;
		}
		pdf_obj *lua = pdf_new_name(ctx, doc, "LuaTexCmaps");
		pdf_obj* cmaps = pdf_dict_get(ctx, root, lua);
		if (cmaps == NULL) {
			//Save PDF only
			pdf_save_document(ctx, doc, result.c_str(), &opts);
		}
		else {
			//Add ToUnicode streams to fonts
			if (!pdf_is_array(ctx, cmaps)) {
				sprintf(buff, "For file %s CMaps list is not array", name.c_str());
				fprintf(stderr, buff);
				pdf_drop_document(ctx, doc);
				if (options.writelog && options.log_file)
					writelogrecord(options.log_file, buff);
				return 1;
			}
			if (options.cmap_directory.empty())
				AddUnicodeCmaps(ctx, doc, cmaps, src_path);
			else
				AddUnicodeCmaps(ctx, doc, cmaps, options.cmap_directory);
			pdf_dict_del(ctx, root, lua);
			pdf_save_document(ctx, doc, result.c_str(), &opts);
		}
		pdf_drop_obj(ctx, lua);
	}
	fz_catch(ctx)
	{
		sprintf(buff, "Internal MuPDF error processing file %s", name.c_str());
		fprintf(stderr, buff);
		if (options.writelog && options.log_file)
			writelogrecord(options.log_file, buff);
		return 1;
	}

	fz_try(ctx) {
		pdf_drop_document(ctx, doc);
	}
	fz_catch(ctx)
	{
		sprintf(buff, "MuPDF error closing file %s", name.c_str());
		fprintf(stderr, buff);
		if (options.writelog && options.log_file)
			writelogrecord(options.log_file, buff);
		return 1;
	}
	fz_drop_context(ctx);
	if (renfile) {
		remove(source.c_str());
		rename(result.c_str(), source.c_str());
	}

	return 0;
}

bool ParseCommandLine(const std::vector<std::string>& args,
                      Options* options, std::list<std::string>* files) {
  if (args.empty()) {
    return false;
  }

  options->exe_path = args[0];

  size_t cur_idx = 1;
  for (; cur_idx < args.size(); ++cur_idx) {
    const std::string& cur_arg = args[cur_idx];
    if (cur_arg == "-h") {
      options->show_config = true;
    } else if (cur_arg == "-l") {
		options->linearize = true;
	}
	else if (cur_arg == "-q") {
		options->run_silently = true;
	}
	else if (cur_arg == "-g") {
		options->writelog = false;
	}
	else if (cur_arg.size() > 3 && cur_arg.compare(0, 3, "-c=") == 0) {
	  if (!options->cmap_directory.empty()) {
        fprintf(stderr, "Duplicate -c argument\n");
        return false;
      }
	  options->cmap_directory = cur_arg.substr(3);
	} else if (cur_arg.size() > 3 && cur_arg.compare(0, 3, "-o=") == 0) {
	  if (!options->rez_name.empty()) {
        fprintf(stderr, "Duplicate -o argument\n");
        return false;
      }
	  options->rez_name = cur_arg.substr(3);
    } else if (cur_arg.size() >= 1 && cur_arg[0] == '-') {
      fprintf(stderr, "Unrecognized argument %s\n", cur_arg.c_str());
      return false;
    } else
      break;
  }
  for (size_t i = cur_idx; i < args.size(); i++) {
    files->push_back(args[i]);
  }
  return true;
}

static const char usage_string[] =
    "Usage: make2unc [OPTION] [PDF_FILE]...\n"
    "  -h        - show these options and exit\n"
	"  -q        - run this programm silently\n"
//	"  -l        - make result PDF WEB optimized (linearized)\n"
	"  -g        - don't write log file\n"
	"  -c=<path> - path to find ToUnicode cmaps. If absent - same path as PDF\n"
    "  -o=<file> - output file name. If absent - overrides source PDF\n";

int main(int argc, const char* argv[]) {
  std::vector<std::string> args(argv, argv + argc);
  Options options;
  int retval = 0;
  std::list<std::string> files;
  options.show_config = false;
  options.run_silently = false;
  options.linearize = false;
  options.writelog = true;
  options.log_file = NULL;
  log_records_count = 0;
  if (!ParseCommandLine(args, &options, &files)) {
	fprintf(stderr, "%s %s\n\n", BANNER, BANNER2);
    fprintf(stderr, "%s", usage_string);
    return 1;
  }

  if ( options.show_config ) {
	fprintf(stderr, "%s %s\n\n", BANNER, BANNER2);
    fprintf(stderr, "%s", usage_string);
    return 1;
  }

  if (files.empty()) {
    fprintf(stderr, "%s %s\n", BANNER, BANNER2);
    fprintf(stderr, "No input file(s).\n");
    return 1;
  }

  if (!options.run_silently)
	fprintf(stderr, "%s %s\n\n", BANNER, BANNER2);

  while (!files.empty()) {
    std::string filename = files.front();
    files.pop_front();
	if (options.writelog) {
		std::string log_name = filename;
		int n = filename.find_last_of('.');
		if (n > 0)
			log_name = filename.substr(0, n);
		log_name.append(".make2unc.log");
		options.log_file = fopen(log_name.c_str(), "w");
	}
	if (ModifyPdfForUnicodes(filename, options) == 1)
		retval = 1;
	if (options.log_file) {
		fclose(options.log_file);
		options.log_file = NULL;
	}
  }

  return retval;
}
