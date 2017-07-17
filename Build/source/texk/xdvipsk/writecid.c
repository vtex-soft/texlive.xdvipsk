/*
 *   Creates CID font resource from a SFNT tables based font file.
 */
#include "dvips.h" /* The copyright notice in that file is included too! */
/*
 *   The external declarations:
 */
#include "protos.h"
#include "protos_add.h" /* external declarations */

#include "mem.h"
#include "error.h"
#include "dpxfile.h"
#include "sfnt.h"
#include "cmap.h"

#include "tt_aux.h"
#include "tt_cmap.h"
#include "tt_table.h"
#include "tt_post.h"

#include "cff_types.h"
#include "cff_dict.h"
#include "cff.h"

#include "cidsysinfo.h"
#include "cidtype0.h"
#include "cidtype2.h"

#if defined(WIN32)
#include <io.h>
#include <direct.h>
#define access _access
#define mkdir _mkdir
#else
#include <sys/stat.h> 
#include <sys/types.h> 
#endif

#define CID_HEXLINE_WIDTH 64
#define cid_putchar(c)    fputc(c, bitfile)

static long hexline_length;

/* from Adobe Technical Specification #5014, Adobe CMap and CIDFont Files */
/* Specification, Version 1.0. */
static const char *cid_othersubrs[] = {
	"[ {} {} {}",
	"  { systemdict /internaldict known not",
	"    { pop 3 }",
	"    { 1183615869 systemdict /internaldict get exec dup",
	"      /startlock known",
	"      { /startlock get exec }",
	"      { dup /strlck known",
	"        { /strlck get exec }",
	"        { pop 3 }",
	"        ifelse",
	"      }",
	"      ifelse",
	"    }",
	"    ifelse",
	"  } bind",
	"  {} {} {} {} {} {} {} {} {}",
	"  { 2 { cvi { { pop 0 lt { exit } if } loop } repeat }",
	"       repeat } bind",
	"]",
	NULL
};


static void end_hexline(void)
{
    if (hexline_length == CID_HEXLINE_WIDTH) {
        fputs("\n", bitfile);
        hexline_length = 0;
    }
}

static void cid_outhex(BYTE b)
{
    static const char *hexdigits = "0123456789ABCDEF";
    cid_putchar(hexdigits[b/16]);
    cid_putchar(hexdigits[b%16]);
    hexline_length += 2;
    end_hexline();
}

static void cid_outbin(BYTE b)
{
    cid_putchar(b);
}

static char *rand_string(char *str, size_t size)
{
	size_t n;
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (size) {
        --size;
        for (n = 0; n < size; n++) {
#ifdef WIN32
            int key = rand() % (int) (sizeof(charset) - 1);
#else
            int key = random() % (int) (sizeof(charset) - 1);
#endif
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

static void writecidtype0(cff_font *cffont, CIDSysInfo *csi, struct cidbytes *cdb, BYTE *binarydata, long bin_size)
{
	char *str, start[256];
	long i, j, k, cnt;
	double dbl, dbl_prev;
	s_SID sid;
	struct fddata *incid;
    int isbold=false;

	fprintf(bitfile, "%%%%BeginResource: font (%s)\n",cffont->fontname);
	fprintf(bitfile, "%%%%Title: (%s)\n",cffont->fontname);
	fprintf(bitfile, "%%%%Version: 1 0\n");
	linepos = 1;
	newline();
	fprintf(bitfile, "/CIDInit /ProcSet findresource begin\n");

	fprintf(bitfile, "20 dict begin\n");

	fprintf(bitfile, "/CIDFontName /%s def\n",cffont->fontname);
	if ( cff_dict_known(cffont->topdict,"CIDFontVersion") )
		dbl = cff_dict_get(cffont->topdict, "CIDFontVersion", 0);
	else
		dbl = 1;
	fprintf(bitfile, "/CIDFontVersion %g def\n",dbl);
	if ( cff_dict_known(cffont->topdict,"CIDFontType") )
		dbl = cff_dict_get(cffont->topdict, "CIDFontType", 0);
	else
		dbl = 0;
	fprintf(bitfile, "/CIDFontType %g def\n",dbl);

	fprintf(bitfile, "/CIDSystemInfo 3 dict dup begin\n");
	fprintf(bitfile, "  /Registry (%s) def\n",csi->registry);
	fprintf(bitfile, "  /Ordering (%s) def\n",csi->ordering);
	fprintf(bitfile, "  /Supplement %d def\n",csi->supplement);
	fprintf(bitfile, "end def\n");

	fprintf(bitfile, "/FontBBox [");
	dbl = cff_dict_get(cffont->topdict, "FontBBox", 0);
	fprintf(bitfile, " %g",dbl);
	dbl = cff_dict_get(cffont->topdict, "FontBBox", 1);
	fprintf(bitfile, " %g",dbl);
	dbl = cff_dict_get(cffont->topdict, "FontBBox", 2);
	fprintf(bitfile, " %g",dbl);
	dbl = cff_dict_get(cffont->topdict, "FontBBox", 3);
	fprintf(bitfile, " %g",dbl);
	fprintf(bitfile, " ] def\n");

	cnt = 1;
	if ( cff_dict_known(cffont->topdict,"Notice") )
		cnt++;
	if ( cff_dict_known(cffont->topdict,"FullName") )
		cnt++;
	if ( cff_dict_known(cffont->topdict,"FamilyName") )
		cnt++;
	if ( cff_dict_known(cffont->topdict,"Weight") )
		cnt++;
	if ( cff_dict_known(cffont->topdict,"ItalicAngle") )
		cnt++;
	if ( cff_dict_known(cffont->topdict,"isFixedPitch") )
		cnt++;

	if ( cnt ) {
		fprintf(bitfile, "/FontInfo %d dict dup begin\n",cnt);
		if ( cff_dict_known(cffont->topdict,"Notice") ) {
			sid = (s_SID)cff_dict_get(cffont->topdict, "Notice", 0);
			str = cff_get_string(cffont,sid);
			fprintf(bitfile, "  /Notice (");
			for ( k=0; k<strlen(str); ++k ) {
				if (  str[k]<' ' ||  str[k]>=0x7f ||  str[k]=='\\' ||  str[k]=='(' ||  str[k]==')' ) {
					fputc('\\',bitfile);
					fputc('0'+(str[k]>>6),bitfile);
					fputc('0'+((str[k]>>3)&0x7),bitfile);
					fputc('0'+(str[k]&0x7),bitfile);
				} else
					fputc(str[k],bitfile);
			}
			fprintf(bitfile, ") readonly def\n");
		}
		if ( cff_dict_known(cffont->topdict,"FullName") ) {
			sid = (s_SID)cff_dict_get(cffont->topdict, "FullName", 0);
			str = cff_get_string(cffont,sid);
			fprintf(bitfile, "  /FullName (%s) readonly def\n",str);
		}
		if ( cff_dict_known(cffont->topdict,"FamilyName") ) {
			sid = (s_SID)cff_dict_get(cffont->topdict, "FamilyName", 0);
			str = cff_get_string(cffont,sid);
			fprintf(bitfile, "  /FamilyName (%s) readonly def\n",str);
		}
		if ( cff_dict_known(cffont->topdict,"Weight") ) {
			sid = (s_SID)cff_dict_get(cffont->topdict, "Weight", 0);
			str = cff_get_string(cffont,sid);
			fprintf(bitfile, "  /Weight (%s) readonly def\n",str);
		}
		if ( cff_dict_known(cffont->topdict,"ItalicAngle") ) {
			dbl = cff_dict_get(cffont->topdict, "ItalicAngle", 0);
			fprintf(bitfile, "  /ItalicAngle %g def\n",dbl);
		}
		if ( cff_dict_known(cffont->topdict,"isFixedPitch") ) {
			dbl = cff_dict_get(cffont->topdict, "isFixedPitch", 0);
			fprintf(bitfile, "  /isFixedPitch %s readonly def\n",dbl > 0 ? "true" : "false");
		}
		fprintf(bitfile, "  /FSType 8 def\n");
		fprintf(bitfile, "end readonly def\n");
	}

	fprintf(bitfile, "/CIDMapOffset %d def\n",cdb->cidmapoffset);
	fprintf(bitfile, "/FDBytes %d def\n",cdb->fdbytes);
	fprintf(bitfile, "/GDBytes %d def\n",cdb->gdbytes);
	fprintf(bitfile, "/CIDCount %d def\n",cdb->cidcnt);

    fprintf(bitfile, "/FDArray %d array\n",cdb->fdcnt);
    for ( i=0; i<cdb->fdcnt; ++i ) {
		fprintf(bitfile,"dup %d\n",i);
		fprintf(bitfile, "%%ADOBeginFontDict\n");
		fprintf(bitfile, "15 dict begin\n");

		fprintf(bitfile, "  /FontName /%s def\n",cffont->fontname);
		fprintf(bitfile, "  /FontType 1 def\n");
		fprintf(bitfile, "  /FontMatrix [");
		if ( cff_dict_known(cffont->topdict,"FontMatrix") ) {
			dbl = cff_dict_get(cffont->topdict, "FontMatrix", 0);
			fprintf(bitfile, " %.3f",dbl);
			dbl = cff_dict_get(cffont->topdict, "FontMatrix", 1);
			fprintf(bitfile, " %.3f",dbl);
			dbl = cff_dict_get(cffont->topdict, "FontMatrix", 2);
			fprintf(bitfile, " %.3f",dbl);
			dbl = cff_dict_get(cffont->topdict, "FontMatrix", 3);
			fprintf(bitfile, " %.3f",dbl);
			dbl = cff_dict_get(cffont->topdict, "FontMatrix", 4);
			fprintf(bitfile, " %.3f",dbl);
			dbl = cff_dict_get(cffont->topdict, "FontMatrix", 5);
			fprintf(bitfile, " %.3f",dbl);
		}
		else 
			fprintf(bitfile, " 0.001 0.0 0.0 0.001 0.0 0.0");
		fprintf(bitfile, " ] def\n");
		fprintf(bitfile, "  /PaintType 0 def\n");
		fprintf(bitfile, "  %%ADOBeginPrivateDict\n");
		fprintf(bitfile, "  /Private 14 dict dup begin\n");
		fprintf(bitfile, "    /MinFeature {16 16} def\n");
		fprintf(bitfile, "    /password 5839 def\n");

		incid = &cdb->fds[i];
		for ( j=0; j<cffont->privat[i]->count; j++ ) {
			if ( strcmp(cffont->privat[i]->entries[j].key,"BlueValues") == 0 ) {
				fprintf(bitfile, "    /BlueValues [");
				dbl_prev = 0;
				for ( k=0; k<cffont->privat[i]->entries[j].count; k++ ) {
					dbl = cffont->privat[i]->entries[j].values[k];
					if ( k != 0 )
						dbl += dbl_prev;
					dbl_prev = dbl;
					fprintf(bitfile, " %g",dbl);
				}
				fprintf(bitfile, " ] def\n");
				break;
			}
		}
		for ( j=0; j<cffont->privat[i]->count; j++ ) {
			if ( strcmp(cffont->privat[i]->entries[j].key,"OtherBlues") == 0 ) {
				fprintf(bitfile, "    /OtherBlues [");
				dbl_prev = 0;
				for ( k=0; k<cffont->privat[i]->entries[j].count; k++ ) {
					dbl = cffont->privat[i]->entries[j].values[k];
					if ( k != 0 )
						dbl += dbl_prev;
					dbl_prev = dbl;
					fprintf(bitfile, " %g",dbl);
				}
				fprintf(bitfile, " ] def\n");
				break;
			}
		}
		for ( j=0; j<cffont->privat[i]->count; j++ ) {
			if ( strcmp(cffont->privat[i]->entries[j].key,"StdHW") == 0 ) {
				dbl = cffont->privat[i]->entries[j].values[0];
				fprintf(bitfile, "    /StdHW [%g] def\n",dbl);
				break;
			}
		}
		for ( j=0; j<cffont->privat[i]->count; j++ ) {
			if ( strcmp(cffont->privat[i]->entries[j].key,"StemSnapH") == 0 ) {
				fprintf(bitfile, "    /StemSnapH [");
				dbl_prev = 0;
				for ( k=0; k<cffont->privat[i]->entries[j].count; k++ ) {
					dbl = cffont->privat[i]->entries[j].values[k];
					if ( k != 0 )
						dbl += dbl_prev;
					dbl_prev = dbl;
					fprintf(bitfile, " %g",dbl);
				}
				fprintf(bitfile, " ] def\n");
				break;
			}
		}
		for ( j=0; j<cffont->privat[i]->count; j++ ) {
			if ( strcmp(cffont->privat[i]->entries[j].key,"StdVW") == 0 ) {
				dbl = cffont->privat[i]->entries[j].values[0];
				fprintf(bitfile, "    /StdVW [%g] def\n",dbl);
				break;
			}
		}
		for ( j=0; j<cffont->privat[i]->count; j++ ) {
			if ( strcmp(cffont->privat[i]->entries[j].key,"StemSnapV") == 0 ) {
				fprintf(bitfile, "    /StemSnapV [");
				dbl_prev = 0;
				for ( k=0; k<cffont->privat[i]->entries[j].count; k++ ) {
					dbl = cffont->privat[i]->entries[j].values[k];
					if ( k != 0 )
						dbl += dbl_prev;
					dbl_prev = dbl;
					fprintf(bitfile, " %g",dbl);
				}
				fprintf(bitfile, " ] def\n");
				break;
			}
		}
		for ( j=0; j<cffont->privat[i]->count; j++ ) {
			if ( strcmp(cffont->privat[i]->entries[j].key,"BlueShift") == 0 ) {
				dbl = cffont->privat[i]->entries[j].values[0];
				fprintf(bitfile, "    /BlueShift %g def\n",dbl);
				break;
			}
		}
		for ( j=0; j<cffont->privat[i]->count; j++ ) {
			if ( strcmp(cffont->privat[i]->entries[j].key,"BlueScale") == 0 ) {
				dbl = cffont->privat[i]->entries[j].values[0];
				fprintf(bitfile, "    /BlueScale %g def\n",dbl);
				break;
			}
		}
		if ( cff_dict_known(cffont->topdict,"Weight") ) {
			sid = (s_SID)cff_dict_get(cffont->topdict, "Weight", 0);
			str = cff_get_string(cffont,sid);
			if (strcmp(str,"Bold")==0 ||
				strcmp(str,"Heavy")==0 ||
				strcmp(str,"Black")==0 ||
				strcmp(str,"Grass")==0 ||
				strcmp(str,"Fett")==0)
				isbold = true;
		}
		if ( cff_dict_known(cffont->privat[i],"ForceBold") ) {
			dbl = cff_dict_get(cffont->privat[i],"ForceBold",0);
			fprintf(bitfile, "    /ForceBold %s def\n",dbl>0 ? "true" : "false");
		}
		else if ( isbold ) {
			fprintf(bitfile, "    /ForceBold true def\n");
		}
		fprintf(bitfile, "    /LanguageGroup 1 def\n");
		if ( incid->leniv != -1 ) {
			fprintf(bitfile, "    /lenIV %d def\n",incid->leniv);
		}
		fprintf(bitfile, "    /OtherSubrs");
		j = 0;
		while ( str = (char *)cid_othersubrs[j] ) {
			fprintf(bitfile, "      %s\n",str);
			j++;
		}
		fprintf(bitfile, "    def\n");

		fprintf(bitfile, "    /SubrMapOffset %d def\n",incid->subrmapoff);
		fprintf(bitfile, "    /SDBytes %d def\n",incid->sdbytes);
		fprintf(bitfile, "    /SubrCount %d def\n",incid->subrcnt);

		fprintf(bitfile, "  end def\n");
		fprintf(bitfile, "  %%ADOEndPrivateDict\n");

		fprintf(bitfile, "  currentdict end\n");
		fprintf(bitfile, "  %%ADOEndFontDict\n");
		fprintf(bitfile, "  put\n");
	}
	fprintf(bitfile, "def\n");

	sprintf(start,"(Hex) %d StartData\n",bin_size);
	fprintf(bitfile, "%%%%BeginData: %d Binary Bytes\n",strlen(start) + bin_size * 2 + 1);
//	sprintf(start,"(Binary) %d StartData ",bin_size);
//	fprintf(bitfile, "%%%%BeginData: %d Binary Bytes\n",strlen(start) + bin_size);
	fprintf(bitfile, start);
	hexline_length = 0;
	for ( i=0; i<bin_size; i++ ) {
		cid_outhex(binarydata[i]);
//		cid_outbin(binarydata[i]);
	}
	fputc('>',bitfile);
	linepos = 1;
	newline();
	fprintf(bitfile, "%%%%EndData\n");

	fprintf(bitfile, "%%%%EndResource\n");
	fprintf(bitfile, "/%s /TeX_Identity-H [/%s] composefont pop\n", cffont->fontname, cffont->fontname);
}

static char *get_sfnt_name(sfnt *sfont, char *name)
{
	char *dest, *ret;
	USHORT destlen, k, n;
	USHORT namelen = 0;

	dest = NEW(64000,char);
	destlen = 64000;

	if ( strcmp(name,"Version") == 0 ) {			//id=5
		namelen = tt_get_name(sfont, dest, destlen, 3, 1, 0x409u, 5);
	}
	else if ( strcmp(name,"Notice") == 0 ) {		//id=0
		namelen = tt_get_name(sfont, dest, destlen, 3, 1, 0x409u, 0);
	}
	else if ( strcmp(name,"FullName") == 0 ) {		//id=4
		namelen = tt_get_name(sfont, dest, destlen, 3, 1, 0x409u, 4);
	}
	else if ( strcmp(name,"FamilyName") == 0 ) {	//id=1
		namelen = tt_get_name(sfont, dest, destlen, 3, 1, 0x409u, 1);
	}
	else if ( strcmp(name,"Weight") == 0 ) {		//id=2
		namelen = tt_get_name(sfont, dest, destlen, 3, 1, 0x409u, 2);
	}
	else
		return NULL;
	if ( namelen == 0 )
		return NULL;
	dest[namelen] = '\0';
	k = namelen;
	for (n = 0; n < namelen; n++) {
		if (dest[n] == 0) {
			memmove(dest + n, dest + n + 1, namelen - n - 1);
			k--;
		}
	}
	dest[k] = '\0';
	if (strlen(dest) == 0)
		return NULL;
	ret = NEW(strlen(dest) + 1, char);
	strcpy(ret,dest);
	free(dest);
	return ret;
}

static void dumpsfnt(BYTE *sfnt_tbl,ULONG length) {
    int ch, ch1;
	ULONG i, bytesout;

    if ( length&1 )
		ERROR("SFNT table length should not be odd");

    while ( length>65534 ) {
		dumpsfnt(sfnt_tbl,65534);
		length -= 65534;
    }

    fprintf( bitfile, " <\n  " );
	bytesout = 0;
    for ( i=0; i<length; ++i ) {
		ch = sfnt_tbl[i];
		if ( ch==EOF )
			break;
		if ( bytesout>=62 ) {
			fprintf( bitfile, "\n  " );
			bytesout = 0;
		}
		ch1 = ch>>4;
		if ( ch1>=10 )
			ch1 += 'A'-10;
		else
			ch1 += '0';
		putc(ch1,bitfile);
		ch1 = ch&0xf;
		if ( ch1>=10 )
			ch1 += 'A'-10;
		else
			ch1 += '0';
		putc(ch1,bitfile);
		++bytesout;
    }
    fprintf( bitfile, "\n  00\n >\n" );
}

/*
 * Computes the max power of 2 <= n
 */
static unsigned
max2floor (unsigned n)
{
  int val = 1;

  while (n > 1) {
    n   /= 2;
    val *= 2;
  }

  return val;
}

/*
 * Computes the log2 of the max power of 2 <= n
 */
static unsigned
log2floor (unsigned n)
{
  unsigned val = 0;

  while (n > 1) {
    n /= 2;
    val++;
  }

  return val;
}

static ULONG *makeglyphslocation(sfnt *sfont, SHORT indexToLocFormat, halfword num_glyphs) {
	struct sfnt_table_directory *td;
	int  idx, i, j;
	ULONG *loca;

	td = sfont->directory;

	for (idx = 0; idx < td->num_tables; idx++) {
		if ( memcmp(td->tables[idx].tag, "loca", 4) == 0 )
			break;
	}
	if ( idx >= td->num_tables )
		return NULL;

	loca = NEW(num_glyphs,ULONG);
	if (!td->tables[idx].data) {
		sfnt_seek_set(sfont, td->tables[idx].offset); 
		if (indexToLocFormat == 0) {
			for (i = 0; i <= num_glyphs; i++)
				loca[i] = 2*((ULONG) sfnt_get_ushort(sfont));
		} else if (indexToLocFormat == 1) {
			for (i = 0; i <= num_glyphs; i++)
				loca[i] = sfnt_get_ulong(sfont);
		} else {
			free(loca);
			return NULL;
		}
	}
	else {
		if (indexToLocFormat == 0) {
			for (i = 0; i <= num_glyphs; i++) {
				unsigned short pair = td->tables[idx].data[2 * i];
				loca[i] = 2 * ((pair << 8) | td->tables[idx].data[2 * i + 1]);
			}
		} else if (indexToLocFormat == 1) {
			for (i = 0; i <= num_glyphs; i++) {
				loca[i] = 0;
				for (j=i * 4; j<i * 4 + 4; j++) {
					loca[i] = (loca[i] << 8) | td->tables[idx].data[j];
				}
			}
		} else {
			free(loca);
			return NULL;
		}
	}

	return loca;
}

static void writecidtype2(sfnt *sfont, struct tt_head_table *head, struct tt_hhea_table *hhea,
						  CIDSysInfo *csi, char *usedchars, char *fontname, long maxcid)
{
	char *str;
	long k;
	double dbl;
	struct sfnt_table_directory *td;
	long offset, nb_read, length, len_pad;
	int  i, j, sr;
	char *p, *p1;
	BYTE *wbuf, wtbl[1024], padbytes[4] = {0, 0, 0, 0};

	struct tt_post_table *post;

	post = tt_read_post_table(sfont);
	if (!post) {
		return;
	}

	fprintf(bitfile, "%%%%BeginResource: font (%s)\n",fontname);
	fprintf(bitfile, "%%%%Title: (%s)\n",fontname);
	fprintf(bitfile, "%%%%Version: 1 0\n");
	linepos = 1;
	newline();
	fprintf(bitfile, "/CIDInit /ProcSet findresource begin\n");

	fprintf(bitfile, "16 dict begin\n");

	fprintf(bitfile, "/CIDFontName /%s def\n",fontname);
	fprintf(bitfile, "/CIDFontType 2 def\n");
	fprintf(bitfile, "/FontType 11 def\n");

	fprintf(bitfile, "/CIDSystemInfo 3 dict dup begin\n");
	fprintf(bitfile, "  /Registry (%s) def\n",csi->registry);
	fprintf(bitfile, "  /Ordering (%s) def\n",csi->ordering);
	fprintf(bitfile, "  /Supplement %d def\n",csi->supplement);
	fprintf(bitfile, "end def\n");

	fprintf(bitfile, "/FontMatrix [ 1 0 0 1 0 0 ] def\n");
	fprintf(bitfile, "/PaintType 0 def\n");

	fprintf(bitfile, "/FontBBox [");
	dbl = (double)head->xMin / (hhea->ascent + hhea->descent);
	fprintf(bitfile, " %.4f",dbl);
	dbl = (double)head->yMin / (hhea->ascent + hhea->descent);
	fprintf(bitfile, " %.4f",dbl);
	dbl = (double)head->xMax / (hhea->ascent + hhea->descent);
	fprintf(bitfile, " %.4f",dbl);
	dbl = (double)head->yMax / (hhea->ascent + hhea->descent);
	fprintf(bitfile, " %.4f",dbl);
	fprintf(bitfile, " ] readonly def\n");

	fprintf(bitfile, "/FontInfo 11 dict dup begin\n");
	if ( str = get_sfnt_name(sfont,"Version") ) {
		fprintf(bitfile, "  /Version (%s) readonly def\n",str + 8);
	}
	if ( str = get_sfnt_name(sfont,"Notice") ) {
		fprintf(bitfile, "  /Notice (");
		for ( k=0; k<strlen(str); ++k ) {
			if (  str[k]<' ' ||  str[k]>=0x7f ||  str[k]=='\\' ||  str[k]=='(' ||  str[k]==')' ) {
				fputc('\\',bitfile);
				fputc('0'+(str[k]>>6),bitfile);
				fputc('0'+((str[k]>>3)&0x7),bitfile);
				fputc('0'+(str[k]&0x7),bitfile);
			} else
				fputc(str[k],bitfile);
		}
		fprintf(bitfile, ") readonly def\n");
	}
	if ( str = get_sfnt_name(sfont,"FullName") ) {
		fprintf(bitfile, "  /FullName (%s) readonly def\n",str);
	}
	if ( str = get_sfnt_name(sfont,"FamilyName") ) {
		fprintf(bitfile, "  /FamilyName (%s) readonly def\n",str);
	}
	if ( str = get_sfnt_name(sfont,"Weight") ) {
		fprintf(bitfile, "  /Weight (%s) readonly def\n",str);
	}
	dbl = (double)post->italicAngle / 65536;
	fprintf(bitfile, "  /ItalicAngle %g def\n",dbl);
	dbl = post->isFixedPitch;
	fprintf(bitfile, "  /isFixedPitch %s def\n",dbl > 0 ? "true" : "false");
	dbl = (double)post->underlinePosition / (hhea->ascent+hhea->descent);
	fprintf(bitfile, "  /UnderlinePosition %g def\n",dbl);
	dbl = (double)post->underlineThickness / (hhea->ascent+hhea->descent);
	fprintf(bitfile, "  /UnderlineThickness %g def\n",dbl);
	if ( hhea->ascent != 8*(hhea->ascent+hhea->descent)/10 ) {
		fprintf(bitfile, "  /ascent %d def\n",hhea->ascent);
	}
	fprintf(bitfile, "  /FSType 8 def\n");
	fprintf(bitfile, "end readonly def\n");

    fprintf( bitfile, "/sfnts [\n" );

	td  = sfont->directory;
	wbuf = NEW(12 + 16 * td->num_kept_tables,BYTE);

	/* Header */
	p  = (char *) wbuf;
	p += sfnt_put_ulong (p, td->version);
	p += sfnt_put_ushort(p, td->num_kept_tables);
	sr = max2floor(td->num_kept_tables) * 16;
	p += sfnt_put_ushort(p, sr);
	p += sfnt_put_ushort(p, log2floor(td->num_kept_tables));
	p += sfnt_put_ushort(p, td->num_kept_tables * 16 - sr);

	/*
	* Compute start of actual tables (after headers).
	*/
	offset = 12 + 16 * td->num_kept_tables;
	for (i = 0; i < td->num_tables; i++) {
		/* This table must exist in FontFile */
		if (td->flags[i] & SFNT_TABLE_REQUIRED) {
			if ((offset % 4) != 0) {
				offset += 4 - (offset % 4);
			}
			p1 = (char *) wtbl;
			memcpy(p1, td->tables[i].tag, 4);
			p1 += 4;
			p1 += sfnt_put_ulong(p1, td->tables[i].check_sum);
			p1 += sfnt_put_ulong(p1, offset);
			p1 += sfnt_put_ulong(p1, td->tables[i].length);
			memcpy(p, wtbl, 16);
			p += 16;
			offset += td->tables[i].length;
		}
	}
	length = 12 + 16 * td->num_kept_tables;
	dumpsfnt(wbuf,length);
	free(wbuf);

	offset = 12 + 16 * td->num_kept_tables;
	wbuf = NULL;
	for (i = 0; i < td->num_tables; i++) {
		if (td->flags[i] & SFNT_TABLE_REQUIRED) {
			if ((td->tables[i].length % 4) != 0)
				len_pad  = 4 - (td->tables[i].length % 4);
			else
				len_pad = 0;
			if ( (memcmp(td->tables[i].tag,"glyf",4) == 0) && (td->tables[i].length > 65534) ) {
				ULONG *loca = makeglyphslocation(sfont,head->indexToLocFormat,maxcid+1);
				ULONG last = 0;
				length = 0;
				wbuf = NEW(65534,BYTE);
				for ( j=0; j<maxcid+1; ++j ) {
					if ( loca[j+1]-last > 65534 ) {
						dumpsfnt(wbuf,loca[j]-last);
						last = loca[j];
						length = 0;
					}
					else {
						memcpy(wbuf + length, td->tables[i].data + loca[j], loca[j + 1] - loca[j]);
						length += loca[j + 1] - loca[j];
					}
				}
				if ( len_pad > 0 )
					memcpy(wbuf + length,padbytes,len_pad);
				dumpsfnt(wbuf,length + len_pad);
				RELEASE(td->tables[i].data);
				td->tables[i].data = NULL;
				free(loca);
				free(wbuf);
			}
			else {
				if (!td->tables[i].data) {
					if (!sfont->stream)	{
						if ( wbuf )
							RELEASE(wbuf);
						ERROR("Font file not opened or already closed...");
						return;
					}
					length = td->tables[i].length;
					wbuf = NEW(length + len_pad,BYTE);
					sfnt_seek_set(sfont, td->tables[i].offset); 
					nb_read = sfnt_read(wbuf, td->tables[i].length, sfont);
					if (nb_read < 0) {
						if ( wbuf )
							RELEASE(wbuf);
						ERROR("Reading file failed...");
						return;
					}
				}
				else {
					length = td->tables[i].length;
					wbuf = NEW(length + len_pad,BYTE);
					memcpy(wbuf, td->tables[i].data, td->tables[i].length);
					RELEASE(td->tables[i].data);
					td->tables[i].data = NULL;
				}
				if ( len_pad > 0 )
					memcpy(wbuf + length,padbytes,len_pad);
				dumpsfnt(wbuf,length + len_pad);
				free(wbuf);
				wbuf = NULL;
				/* Set offset for next table */
				offset += td->tables[i].length + len_pad;
			}
		}
	}
    fprintf( bitfile, "  ] def\n" );

	fprintf(bitfile, "  /CharStrings 1 dict dup begin\n");
	fprintf(bitfile, "    /.notdef 0 def\n");
	fprintf(bitfile, "  end readonly def\n");

//	sr = maxcid+1;
//    for ( i=0; i<maxcid+1; ++i )
//		if ( is_used_char2(usedchars,i) )
//			sr++;
//	fprintf( bitfile, "  /CIDMap %d dict dup begin\n", sr );
//    for ( i=0; i<maxcid+1; ++i )
//		if ( is_used_char2(usedchars,i) )
//		    fprintf( bitfile, "    %d %d def\n", i, i );
//	fprintf(bitfile, "  end readonly def\n");

    fprintf( bitfile, "  /CIDCount %d def\n", maxcid+1 );
    fprintf( bitfile, "  /GDBytes %d def\n", maxcid+1>65535?3:2 );
    fprintf( bitfile, "  /CIDMap 0 def\n" );
//    fprintf( bitfile, "  /CIDMap 0 def\n" );

	fprintf( bitfile, "currentdict end dup /CIDFontName get exch /CIDFont defineresource pop\nend\n" );

	fprintf(bitfile, "%%%%EndResource\n");
	fprintf(bitfile, "/%s /TeX_Identity-H [/%s] composefont pop\n",fontname,fontname);

	RELEASE(post);
}

int writecid(charusetype *p)
{
	char *d;
    struct resfont *rf;
	FILE         *fp;
	char         *path;
	sfnt         *sfont;
	cff_font *cffont;
	unsigned      offset = 0;
	int is_dfont = 0, index = 0;
    int   n;
	CIDSysInfo csi;
	struct tt_maxp_table *maxp;
	struct tt_head_table *head;
	struct tt_hhea_table *hhea;
	CMap *code_to_cid_cmap;
	halfword    num_glyphs;
	char xname[16];
	BYTE *buffer;
	long buf_size, maxcid;
	int cidCount;
	struct cidbytes cdb;
	otfcmaptype *otfcmap;

    curfnt = p->fd;
    rf = curfnt->resfont;
	code_to_cid_cmap = NULL;
#ifdef KPATHSEA
	d = kpse_var_expand(rf->Fontfile);
#else
	d = expandfilename(rf->Fontfile);
#endif
	if ( d == NULL )
		return 0;

	if ((path = dpx_find_dfont_file(d)) != NULL &&
		(fp = fopen(path, "rb")) != NULL)
		is_dfont = 1;
	else if (((path = dpx_find_opentype_file(d)) == NULL
         && (path = dpx_find_truetype_file(d)) == NULL)
         || (fp = fopen(path, "rb")) == NULL) {
		char *msg = concatn("! Cannot proceed without the font: ", d, NULL);
		error(msg);
	}
	free(d);
    realnameoffile = path;
    strcpy(name, realnameoffile);
	index = rf->index;
	if (is_dfont)
		sfont = dfont_open(fp, index);
	else
		sfont = sfnt_open(fp);
	if (sfont->type == SFNT_TYPE_TTC) {
		offset = ttc_read_offset(sfont, index);
		if (offset == 0) {
			ERROR("Invalid TTC index for font %s",curfnt->resfont->PSname);
		}
	}
	else if (sfont->type == SFNT_TYPE_DFONT) {
		offset = sfont->offset;
	}
	if (sfnt_read_table_directory(sfont, offset) < 0) {
		ERROR("Could not read OpenType/TrueType table directory for font %s",rf->PSname);
	}
	rand_string(xname,9);
	csi.registry = mymalloc(6);
	csi.ordering = mymalloc(24);
	strcpy(csi.registry,"Adobe");
	strcpy(csi.ordering, "Identity");
	csi.supplement = 0;
	switch ( rf->otftype ) {
	case 1:
	case 3:
	case 4:
		//Make Type2 CID Font (SFNT)
		maxp       = tt_read_maxp_table(sfont);
		num_glyphs = maxp->numGlyphs;
		RELEASE(maxp);
		if (num_glyphs > 0) {
			head = tt_read_head_table(sfont);
			hhea = tt_read_hhea_table(sfont);
			if ( cid_partialdownload ) {
				CIDFont_type2_dofont(rf->PSname,sfont,rf->index,(char *)p->bitmap,&maxcid);
			}
			else 
				maxcid = num_glyphs - 1;
			CIDFont_type2_checktables(sfont);
			writecidtype2(sfont,head,hhea,&csi,(char *)p->bitmap,rf->PSname,maxcid);
			RELEASE(head);
			RELEASE(hhea);
		}
		break;
	case 2:
		//Make Type0 CID Font (CFF)
		offset = sfnt_find_table_pos(sfont, "CFF ");
		if (offset > 0) {
			maxp       = tt_read_maxp_table(sfont);
			num_glyphs = maxp->numGlyphs;
			RELEASE(maxp);
			if (num_glyphs > 0) {
				cffont = cff_open(sfont->stream, offset, 0);
				if (!cffont)
					ERROR("Could not open CFF font for font %s", rf->Fontfile);
				if (!cid_partialdownload) {
					for (n = 1; n < num_glyphs; n++)
						add_to_used_chars2((char *)p->bitmap, n);
				}
				if ( cffont->flag & FONTTYPE_CIDFONT )
					buf_size = CIDFont_type0_dofont(rf->PSname,cffont,(char *)p->bitmap,&cdb,&buffer);
				else
					buf_size = CIDFont_type0_t1cdofont(rf->PSname,cffont,(char *)p->bitmap,&cdb,&buffer);
				writecidtype0(cffont,&csi,&cdb,buffer,buf_size);
				RELEASE(cdb.fds);
				RELEASE(buffer);
				cff_close(cffont);
			}
		}
		break;
	}
	if ( !noToUnicode && (((rf->cmap_fmt == 4) || (rf->cmap_fmt == 12)) || (rf->luamap_idx >= 0) )) {
		char *expath, *cmap_name, *cmap_ext, *tmp, cmap_tmp[260];
#ifdef KPATHSEA
		char *path = xdirname(fulliname);
#else
		char *path = mymalloc(strlen(fulliname) + 1);
		strcpy(path,fulliname);
		for ( n=(int)strlen(path); n>=0; n-- ) {
			if ( (path[n] == '\\') || (path[n] == '/') )
				break;
		}
		if ( n < 0 ) {
			strcpy(path,".");
		}
		else
			path[n] = 0;
#endif
		expath = mymalloc(strlen(path) +32);
		strcpy(expath, path);
		strcat(expath, "/");
		strcat(expath, OTFMAPFILEDIR);
		free(path);
		if (access(expath, 0) == -1)
#if defined(_WIN32) || defined(_WIN64)
			mkdir(expath);
#else
			mkdir(expath, ACCESSPERMS);
#endif

		strcpy(csi.registry, "TeX");
		sprintf(csi.ordering, "%s+%03d", xname, curfnt->psname);
		cmap_name = mymalloc(260);
		for (n = strlen(fulliname); n >= 0; n--) {
			if ((fulliname[n] == '\\') || (fulliname[n] == '/'))
				break;
		}
		n++;
		strcpy(cmap_tmp, &fulliname[n]);
		tmp = strrchr(cmap_tmp, '.');
		if (tmp)
			*tmp = 0;
		sprintf(cmap_name, "%s-cid%03d", cmap_tmp, curfnt->psname);
		cmap_ext = mymalloc(16);
		strcpy(cmap_ext, "tounicode");
		p->bitmap[0] &= 0xFF7F;
		otf_create_ToUnicode_stream(sfont,rf->PSname,code_to_cid_cmap,&csi,(char *)p->bitmap,expath,cmap_name,cmap_ext,p->map_chain);
		if ( code_to_cid_cmap )
			CMap_release(code_to_cid_cmap);
		index = curfnt->psname;
		HASH_FIND_INT(OTF_list, &index, otfcmap);  /* id already in the hash? */
		if (otfcmap==NULL) {
			otfcmap = (otfcmaptype*)malloc(sizeof(otfcmaptype));
			otfcmap->index = index;
			otfcmap->fontname = (char *)malloc(strlen(curfnt->resfont->PSname) + 1);
			strcpy(otfcmap->fontname,curfnt->resfont->PSname);
			otfcmap->cmapname = (char *)malloc(strlen(cmap_name) + 1);
			strcpy(otfcmap->cmapname, cmap_name);
			otfcmap->cmapext = (char *)malloc(strlen(cmap_ext) + 1);
			strcpy(otfcmap->cmapext, cmap_ext);
			HASH_ADD_INT(OTF_list, index, otfcmap);
		}
		if (expath)
			free(expath);
		free(cmap_name);
		free(cmap_ext);
	}
	free(csi.registry);
	free(csi.ordering);
	sfnt_close(sfont);
	fclose(fp);
	return 1;
}
