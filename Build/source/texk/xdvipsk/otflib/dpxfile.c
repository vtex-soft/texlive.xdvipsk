/* This is xdvipsk, an eXtended version of dvips(k) by Tomas Rokicki.

	Copyright (C) 2016 by VTeX Ltd (www.vtex.lt),
	the xdvipsk project team - Sigitas Tolusis and Arunas Povilaitis.

    Program original code copyright (C) 2007-2014 by Jin-Hwan Cho and 
	Shunsaku Hirata, the dvipdfmx project team.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/

#include <config.h>

#ifdef _MSC_VER
#include <kpathsea/dirent.h>
#endif

#include <time.h>

#include "system.h"
#include "error.h"
#include "mem.h"

#include "dpxutil.h"
#include "mfileio.h"

#include "dpxfile.h"
//#include "dpxcrypt.h"
#define MAX_KEY_LEN 16

#include <kpathsea/lib.h>
#include <string.h>
#ifdef WIN32
#include <io.h>
#include <process.h>
#include <wchar.h>
#else
#if HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
#define WEXITSTATUS(val) ((unsigned)(val) >> 8)
#endif
#ifndef WIFEXITED
#define WIFEXITED(val) (((val) & 255) == 0)
#endif
#endif

int keep_cache = 0;


/* Kpathsea library does not check file type. */
static int qcheck_filetype (const char *fqpn, dpx_res_type type);

/* For testing MIKTEX enabled compilation */
#if defined(TESTCOMPILE) && !defined(MIKTEX)
#  define MIKTEX        1
#  define PATH_SEP_CHR  '/'
#  define _MAX_PATH     256

static int
miktex_get_acrobat_font_dir (char *buf)
{
  strcpy(buf, "/usr/share/ghostscript/Resource/Font/");
  return  1;
}

static int
miktex_find_file (const char *filename, const char *dirlist, char *buf)
{
  int    r = 0;
  char  *fqpn;

  fqpn = kpse_path_search(dirlist, filename, 0);
  if (!fqpn)
    return  0;
  if (strlen(fqpn) > _MAX_PATH)
    r = 0;
  else {
    strcpy(buf, fqpn);
    r = 1;
  }
  RELEASE(fqpn);

  return  r;
}

static int
miktex_find_app_input_file (const char *progname, const char *filename, char *buf)
{
  int    r = 0;
  char  *fqpn;

  kpse_reset_program_name(progname);
  fqpn = kpse_find_file  (filename, kpse_program_text_format, false);
  kpse_reset_program_name("dvipdfmx");

  if (!fqpn)
    return  0;
  if (strlen(fqpn) > _MAX_PATH)
    r = 0;
  else {
    strcpy(buf, fqpn);
    r = 1;
  }
  RELEASE(fqpn);

  return  r;
}

static int
miktex_find_psheader_file (const char *filename, char *buf)
{
  int    r;
  char  *fqpn;

  fqpn = kpse_find_file(filename, kpse_tex_ps_header_format, 0);

  if (!fqpn)
    return  0;
  if (strlen(fqpn) > _MAX_PATH)
    r = 0;
  else {
    strcpy(buf, fqpn);
    r = 1;
  }
  RELEASE(fqpn);

  return  r; 
}

#endif /* TESTCOMPILE */

#ifdef  MIKTEX
#ifndef PATH_SEP_CHR
#  define PATH_SEP_CHR '\\'
#endif
static char  _tmpbuf[_MAX_PATH+1];
#endif /* MIKTEX */

static int exec_spawn (char *cmd)
{
  char **cmdv, **qv;
  char *p, *pp;
  char buf[1024];
  int  i, ret = -1;
#ifdef WIN32
  wchar_t **cmdvw, **qvw;
#endif

  if (!cmd)
    return -1;
  while (*cmd == ' ' || *cmd == '\t')
    cmd++;
  if (*cmd == '\0')
    return -1;
  i = 0;
  p = cmd;
  while (*p) {
    if (*p == ' ' || *p == '\t')
      i++;
    p++;
  }
  cmdv = xcalloc (i + 2, sizeof (char *));
  p = cmd;
  qv = cmdv;
  while (*p) {
    pp = buf;
    if (*p == '"') {
      p++;
      while (*p != '"') {
        if (*p == '\0') {
          goto done;
        }
        *pp++ = *p++;
      }
      p++;
    } else if (*p == '\'') {
      p++;
      while (*p != '\'') {
        if (*p == '\0') {
          goto done;
        }
        *pp++ = *p++;
      }
      p++;
    } else {
      while (*p != ' ' && *p != '\t' && *p) {
        if (*p == '\'') {
          p++;
          while (*p != '\'') {
             if (*p == '\0') {
                 goto done;
             }
             *pp++ = *p++;
          }
          p++;
        } else {
          *pp++ = *p++;
        }
      }
    }
    *pp = '\0';
#ifdef WIN32
    if (strchr (buf, ' ') || strchr (buf, '\t'))
      *qv = concat3 ("\"", buf, "\"");
    else
#endif
      *qv = xstrdup (buf);
/*
    fprintf(stderr,"\n%s", *qv);
*/
    while (*p == ' ' || *p == '\t')
      p++;
    qv++;
  }
#ifdef WIN32
  cmdvw = xcalloc (i + 2, sizeof (wchar_t *));
  qv = cmdv;
  qvw = cmdvw;
  while (*qv) {
    *qvw = get_wstring_from_fsyscp(*qv, *qvw=NULL);
    qv++;
    qvw++;
  }
  *qvw = NULL;
  ret = _wspawnvp (_P_WAIT, *cmdvw, (const wchar_t* const*) cmdvw);
  if (cmdvw) {
    qvw = cmdvw;
    while (*qvw) {
      free (*qvw);
      qvw++;
    }
    free (cmdvw);
  }
#else
  i = fork ();
  if (i < 0)
    ret = -1;
  else if (i == 0) {
    if (execvp (*cmdv, cmdv))
      _exit (-1);
  } else {
    if (wait (&ret) == i) {
      ret = (WIFEXITED (ret) ? WEXITSTATUS (ret) : -1);
    } else {
      ret = -1;
    }
  }
#endif
done:
  qv = cmdv;
  while (*qv) {
    free (*qv);
    qv++;
  }
  free (cmdv);
  return ret;
}

/* ensuresuffix() returns a copy of basename if sfx is "". */
static char *
ensuresuffix (const char *basename, const char *sfx)
{
  char  *q, *p;

  p = NEW(strlen(basename) + strlen(sfx) + 1, char);
  strcpy(p, basename);
  q = strrchr(p, '.');
  if (!q && sfx[0])
    strcat(p, sfx);

  return  p;
}

#ifdef  MIKTEX
static char *
dpx_find__app__xyz (const char *filename,
                    const char *suffix, int is_text)
{
  char  *fqpn = NULL;
  int    r;
  char  *q;

  q = ensuresuffix(filename, suffix);
  r = miktex_find_app_input_file("dvipdfmx", q, _tmpbuf);
  if (!r && strcmp(q, filename))
    r = miktex_find_app_input_file("dvipdfmx", filename, _tmpbuf);
  if (r) {
    fqpn = NEW(strlen(_tmpbuf) + 1, char);
    strcpy(fqpn, _tmpbuf);
  }
  RELEASE(q);

  return  fqpn;
}

static char *
dpx_foolsearch (const char  *foolname,
                const char  *filename,
                int          is_text)
{
  char  *fqpn = NULL;
  int    r;

  r = miktex_find_app_input_file(foolname, filename, _tmpbuf);
  if (r) {
    fqpn = NEW(strlen(_tmpbuf) + 1, char);
    strcpy(fqpn, _tmpbuf);
  }

  return  fqpn;
}
#else /* !MIKTEX */
#  define TDS11DOC "http://www.tug.org/ftp/tex/tds-1.1/tds.html#Fonts"
static void
insistupdate (const char      *filename,
              const char      *fqpn,
              const char      *foolname,
              kpse_file_format_type foolformat,
              kpse_file_format_type realformat)
{
}

static char *
dpx_find__app__xyz (const char *filename,
                    const char *suffix, int is_text)
{
  char  *fqpn = NULL;
  char  *q;

  q    = ensuresuffix(filename, suffix);
  fqpn = kpse_find_file(q,
                         (is_text ?
                            kpse_program_text_format : kpse_program_binary_format), 0);
  if (!fqpn && strcmp(q, filename))
    fqpn = kpse_find_file(filename,
                           (is_text ?
                              kpse_program_text_format : kpse_program_binary_format), 0);   
  RELEASE(q);

  return  fqpn;
}

static char *
dpx_foolsearch (const char  *foolname,
                const char  *filename,
                int          is_text)
{
  char  *fqpn = NULL;

  kpse_reset_program_name(foolname);
  fqpn = kpse_find_file  (filename,
                          (is_text ?
                              kpse_program_text_format :
                              kpse_program_binary_format),
                          false);
  kpse_reset_program_name("dvipdfmx");

  return  fqpn;
}
#endif /* MIKTEX */

static char *dpx_find_fontmap_file  (const char *filename);
static char *dpx_find_agl_file      (const char *filename);
static char *dpx_find_sfd_file      (const char *filename);
static char *dpx_find_cmap_file     (const char *filename);
static char *dpx_find_enc_file      (const char *filename);
static char *dpx_find_iccp_file     (const char *filename);

FILE *
dpx_open_file (const char *filename, dpx_res_type type)
{
  FILE  *fp   = NULL;
  char  *fqpn = NULL;

  switch (type) {
  case DPX_RES_TYPE_FONTMAP:
    fqpn = dpx_find_fontmap_file(filename);
    break;
  case DPX_RES_TYPE_T1FONT:
    fqpn = dpx_find_type1_file(filename);
    break;
  case DPX_RES_TYPE_TTFONT:
    fqpn = dpx_find_truetype_file(filename);
    break;
  case DPX_RES_TYPE_OTFONT:
    fqpn = dpx_find_opentype_file(filename);
    break;
  case DPX_RES_TYPE_PKFONT:
    break;
  case DPX_RES_TYPE_CMAP:
    fqpn = dpx_find_cmap_file(filename);
    break;
  case DPX_RES_TYPE_ENC:
    fqpn = dpx_find_enc_file(filename);
    break;
  case DPX_RES_TYPE_SFD:
    fqpn = dpx_find_sfd_file(filename);
    break;
  case DPX_RES_TYPE_AGL:
    fqpn = dpx_find_agl_file(filename);
    break;
  case DPX_RES_TYPE_ICCPROFILE:
    fqpn = dpx_find_iccp_file(filename);
    break;
  case DPX_RES_TYPE_DFONT:
    fqpn = dpx_find_dfont_file(filename);
    break;
  case DPX_RES_TYPE_BINARY:
    fqpn = dpx_find__app__xyz(filename, "", 0);
    break;
  case DPX_RES_TYPE_TEXT:
    fqpn = dpx_find__app__xyz(filename, "", 1);
    break;
  }
  if (fqpn) {
    fp = MFOPEN(fqpn, FOPEN_RBIN_MODE);
    RELEASE(fqpn);
  }

  return  fp;
}


static char *
dpx_find_iccp_file (const char *filename)
{
  char  *fqpn = NULL;

  fqpn = dpx_find__app__xyz(filename, "", 0);
  if (fqpn || strrchr(filename, '.'))
    return  fqpn;

  fqpn = dpx_find__app__xyz(filename, ".icc", 0);
  if (fqpn)
    return  fqpn;

  fqpn = dpx_find__app__xyz(filename, ".icm", 0);

  return  fqpn;
}


static char *
dpx_find_fontmap_file (const char *filename)
{
  char  *fqpn = NULL;
  char  *q;

  q = ensuresuffix(filename, ".map");
#ifdef  MIKTEX
  fqpn = dpx_find__app__xyz(q, ".map", 1);
#else /* !MIKTEX */
  fqpn = kpse_find_file(q, kpse_fontmap_format, 0);
  if (!fqpn) {
    fqpn = dpx_find__app__xyz(q, ".map", 1);
    if (fqpn)
      insistupdate(q, fqpn, "dvipdfmx",
                   kpse_program_text_format, kpse_fontmap_format); 
  }
#endif /* MIKETEX */
  RELEASE(q);

  return  fqpn;
}


static char *
dpx_find_agl_file (const char *filename)
{
  char  *fqpn = NULL;
  char  *q;

  q = ensuresuffix(filename, ".txt");
#ifdef  MIKTEX
  fqpn = dpx_find__app__xyz(q, ".txt", 1);
#else /* !MIKTEX */
  fqpn = kpse_find_file(q, kpse_fontmap_format, 0);
  if (!fqpn) {
    fqpn = dpx_find__app__xyz(q, ".txt", 1);
    if (fqpn)
      insistupdate(q, fqpn, "dvipdfmx",
                   kpse_program_text_format, kpse_fontmap_format); 
  }
#endif /* MIKETEX */
  RELEASE(q);

  return  fqpn;
}


/* cmap.sty put files into tex/latex/cmap */
static char *
dpx_find_cmap_file (const char *filename)
{
  char  *fqpn = NULL;
  static const char *fools[] = {
    "cmap", "tex", NULL
  };
  int    i;

#if  defined(MIKTEX)
  /* Find in Acrobat's Resource/CMap dir */
  {
    char  _acrodir[_MAX_PATH+1];
    char  *q;
    int    r;

    memset(_acrodir, 0, _MAX_PATH+1);
    r = miktex_get_acrobat_font_dir(_acrodir);
    if (r &&
        strlen(_acrodir) > strlen("Font")) {
      /* ....\Font\ */
      q = strrchr(_acrodir, PATH_SEP_CHR);
      if (q && q[1] == '\0')
        q[0] = '\0';
      q = strrchr(_acrodir, PATH_SEP_CHR);
      if (q && !strcmp(q + 1, "Font")) {
        sprintf(q, "%cCMap%c", PATH_SEP_CHR, PATH_SEP_CHR);
        r = miktex_find_file(filename, _acrodir, _tmpbuf);
        if (r) {
          fqpn = NEW(strlen(_tmpbuf) + 1, char);
          strcpy(fqpn, _tmpbuf);
        }
      }
    }
    memset(_tmpbuf, 0, _MAX_PATH+1);
  }
#else
  fqpn = kpse_find_file(filename, kpse_cmap_format, 0); 
#endif

  /* Files found above are assumed to be CMap,
   * if it's not really CMap it will cause an error.
   */
  for (i = 0; !fqpn && fools[i]; i++) { 
    fqpn = dpx_foolsearch(fools[i], filename, 1);
    if (fqpn) {
#ifndef  MIKTEX
      insistupdate(filename, fqpn, fools[i],
                   kpse_program_text_format, kpse_cmap_format); 
#endif
      if (!qcheck_filetype(fqpn, DPX_RES_TYPE_CMAP)) {
        WARN("Found file \"%s\" for PostScript CMap but it doesn't look like a CMap...", fqpn);
        RELEASE(fqpn);
        fqpn = NULL;
      }
    }
  }

  return  fqpn;
}


/* Search order:
 *   SFDFONTS (TDS 1.1)
 *   ttf2pk   (text file)
 *   ttf2tfm  (text file)
 *   dvipdfm  (text file)   
 */
static char *
dpx_find_sfd_file (const char *filename)
{
  char  *fqpn = NULL;
  char  *q;
  static const char *fools[] = {
    "ttf2pk", "ttf2tfm", NULL
  };
  int    i;

  q    = ensuresuffix(filename, ".sfd");
#ifndef  MIKTEX
  fqpn = kpse_find_file(q, kpse_sfd_format, 0);
#endif /* !MIKTEX */

  for (i = 0; !fqpn && fools[i]; i++) { 
    fqpn = dpx_foolsearch(fools[i], q, 1);
#ifndef  MIKTEX
    if (fqpn)
      insistupdate(filename, fqpn, fools[i],
                   kpse_program_text_format, kpse_sfd_format); 
#endif
  }
  RELEASE(q);

  return  fqpn;
}


static char *
dpx_find_enc_file (const char *filename)
{
  char  *fqpn = NULL;
  char  *q;
  static const char *fools[] = {
    "dvips", NULL
  };
  int    i;

  q = ensuresuffix(filename, ".enc");
#ifdef  MIKTEX
  if (miktex_find_psheader_file(q, _tmpbuf)) {
    fqpn = NEW(strlen(_tmpbuf) + 1, char);
    strcpy(fqpn, _tmpbuf);
  }
#else
  fqpn = kpse_find_file(q, kpse_enc_format, 0);
#endif /* MIKTEX */

  for (i = 0; !fqpn && fools[i]; i++) { 
    fqpn = dpx_foolsearch(fools[i], q, 1);
#ifndef  MIKTEX
    if (fqpn)
      insistupdate(filename, fqpn, fools[i],
                   kpse_program_text_format, kpse_enc_format); 
#endif
  }
  RELEASE(q);

  return  fqpn;
}

static int
is_absolute_path(const char *filename)
{
#ifdef WIN32
  if (isalpha(filename[0]) && filename[1] == ':')
    return 1;
  if (filename[0] == '\\' && filename[1] == '\\')
    return 1;
  if (filename[0] == '/' && filename[1] == '/')
    return 1;
#else
  if (filename[0] == '/')
    return 1;
#endif
  return 0;
}

char *
dpx_find_type1_file (const char *filename)
{
  char  *fqpn = NULL;

  if (is_absolute_path(filename))
    fqpn = xstrdup(filename);
  else
    fqpn = kpse_find_file(filename, kpse_type1_format, 0);
  if (fqpn && !qcheck_filetype(fqpn, DPX_RES_TYPE_T1FONT)) {
    RELEASE(fqpn);
    fqpn = NULL;
  }

  return  fqpn;
}


char *
dpx_find_truetype_file (const char *filename)
{
  char  *fqpn = NULL;

  if (is_absolute_path(filename))
    fqpn = xstrdup(filename);
  else
    fqpn = kpse_find_file(filename, kpse_truetype_format, 0);
  if (fqpn && !qcheck_filetype(fqpn, DPX_RES_TYPE_TTFONT)) {
    RELEASE(fqpn);
    fqpn = NULL;
  }

  return  fqpn;
}


char *
dpx_find_opentype_file (const char *filename)
{
  char  *fqpn = NULL;
  char  *q;

  q = ensuresuffix(filename, ".otf");
#ifndef MIKTEX
  if (is_absolute_path(q))
    fqpn = xstrdup(q);
  else
    fqpn = kpse_find_file(q, kpse_opentype_format, 0);
  if (!fqpn) {
#endif
    fqpn = dpx_foolsearch("dvipdfmx", q, 0);
#ifndef  MIKTEX
    if (fqpn)
      insistupdate(filename, fqpn, "dvipdfmx",
                   kpse_program_binary_format, kpse_opentype_format); 
  }
#endif
  RELEASE(q);

  /* *We* use "opentype" for ".otf" (CFF). */
  if (fqpn && !qcheck_filetype(fqpn, DPX_RES_TYPE_OTFONT)) {
    RELEASE(fqpn);
    fqpn = NULL;
  }

  return  fqpn;
}


char *
dpx_find_dfont_file (const char *filename)
{
  char *fqpn = NULL;

  fqpn = kpse_find_file(filename, kpse_truetype_format, 0);
  if (fqpn) {
    int len = strlen(fqpn);
    if (len > 6 && strncmp(fqpn+len-6, ".dfont", 6)) {
      fqpn = RENEW(fqpn, len+6, char);
      strcat(fqpn, "/rsrc");
    }
  }
  if (!qcheck_filetype(fqpn, DPX_RES_TYPE_DFONT)) {
    RELEASE(fqpn);
    fqpn = NULL;
  }
  return fqpn;
}

static char _sbuf[128];
/*
 * SFNT type sigs:
 *  `true' (0x74727565): TrueType (Mac)
 *  `typ1' (0x74797031) (Mac): PostScript font housed in a sfnt wrapper
 *  0x00010000: TrueType (Win)/OpenType
 *  `OTTO': PostScript CFF font with OpenType wrapper
 *  `ttcf': TrueType Collection
 */
static int
istruetype (FILE *fp)
{
  int   n;

  rewind(fp);
  n = fread(_sbuf, 1, 4, fp);
  rewind(fp);

  if (n != 4)
    return  0;
  else if (!memcmp(_sbuf, "true", 4) ||
           !memcmp(_sbuf, "\0\1\0\0", 4)) /* This doesn't help... */
    return  1;
  else if (!memcmp(_sbuf, "ttcf", 4))
    return  1;

  return  0;
}
      
/* "OpenType" is only for ".otf" here */
static int
isopentype (FILE *fp)
{
  int   n;

  rewind(fp);
  n = fread(_sbuf, 1, 4, fp);
  rewind(fp);

  if (n != 4)
    return  0;
  else if (!memcmp(_sbuf, "OTTO", 4))
    return  1;
  else
    return  0;
}

static int
ist1binary (FILE *fp)
{
  char *p;
  int   n;

  rewind(fp);
  n = fread(_sbuf, 1, 21, fp);
  rewind(fp);

  p = _sbuf;
  if (n != 21)
    return  0;
  else if (p[0] != (char) 0x80 || p[1] < 0 || p[1] > 3)
    return  0;
  else if (!memcmp(p + 6, "%!PS-AdobeFont", 14) ||
           !memcmp(p + 6, "%!FontType1", 11))
    return  1;
  else if (!memcmp(p + 6, "%!PS", 4)) {
#if  0
    p[20] = '\0'; p += 6;
    WARN("Ambiguous PostScript resource type: %s", (char *) p);
#endif
    return  1;
  }
  /* Otherwise ambiguious */
  return  0;
}

/* %!PS-Adobe-x.y Resource-CMap */
static int
ispscmap (FILE *fp)
{
  char  *p;
  p = mfgets(_sbuf, 128, fp); p[127] = '\0';
  if (!p || strlen(p) < 4 || memcmp(p, "%!PS", 4))
    return 0;
  for (p += 4; *p && !isspace((unsigned char)*p); p++);
  for ( ; *p && (*p == ' ' || *p == '\t'); p++);
  if (*p == '\0' || strlen(p) < strlen("Resource-CMap"))
    return  0;
  else if (!memcmp(p, "Resource-CMap", strlen("Resource-CMap")))
    return  1;
  /* Otherwise ambiguious */
  return  0;
}

static int
isdfont (FILE *fp)
{
  int i, n;
  unsigned long pos;

  rewind(fp);

  get_unsigned_quad(fp);
  seek_absolute(fp, (pos = get_unsigned_quad(fp)) + 0x18);
  seek_absolute(fp, pos + get_unsigned_pair(fp));
  n = get_unsigned_pair(fp);
  for (i = 0; i <= n; i++) {
    if (get_unsigned_quad(fp) == 0x73666e74UL) /* "sfnt" */
      return 1;
    get_unsigned_quad(fp);
  }
  return 0;
}
      
/* This actually opens files. */
static int
qcheck_filetype (const char *fqpn, dpx_res_type type)
{
  int    r = 1;
  FILE  *fp;
  struct stat sb;

  if (!fqpn)
    return  0;

  if (stat(fqpn, &sb) != 0)
    return 0;

  if (sb.st_size == 0)
    return 0;

  fp = MFOPEN(fqpn, FOPEN_RBIN_MODE);
  if (!fp) {
    WARN("File \"%s\" found but I could not open that...", fqpn);
    return  0;
  }
  switch (type) {
  case DPX_RES_TYPE_T1FONT:
    r = ist1binary(fp);
    break;
  case DPX_RES_TYPE_TTFONT:
    r = istruetype(fp);
    break;
  case DPX_RES_TYPE_OTFONT:
    r = isopentype(fp);
    break;
  case DPX_RES_TYPE_CMAP:
    r = ispscmap(fp);
    break;
  case DPX_RES_TYPE_DFONT:
    r = isdfont(fp);
    break;
  default:
    break;
  }
  MFCLOSE(fp);

  return  r;
}

