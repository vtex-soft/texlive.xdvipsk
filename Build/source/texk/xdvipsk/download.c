/*
 *   Code to download a font definition at the beginning of a section.
 * hacked by SPQR 8.95 to allow for non-partial downloading
 *
 */
//AP--begin
//#include "dvips.h" /* The copyright notice in that file is included too! */
#include "xdvips.h" /* The copyright notice in that file is included too! */
//AP--end
#ifndef DOWNLOAD_USING_PDFTEX
#define DOWNLOAD_USING_PDFTEX
#endif
#ifndef DOWNLOAD_USING_PDFTEX
#include "t1part.h"
#endif
#define DVIPS
/*
 *   The external declarations:
 */
#include "protos.h"

static unsigned char dummyend[8] = { 252 };

/*
 *   We have a routine that downloads an individual character.
 */
static int lastccout;
quarterword *unpack_bb(chardesctype *c, integer *cwidth, integer *cheight,
                                        integer *xoff, integer *yoff) {
   quarterword *p = c->packptr;
   halfword cmd = *p++;
   if (cmd & 4) {
      if ((cmd & 7) == 7) {
         *cwidth = getlong(p);
         *cheight = getlong(p + 4);
         *xoff = getlong(p + 8);
         *yoff = getlong(p + 12);
         p += 16;
      } else {
         *cwidth = p[0] * 256 + p[1];
         *cheight = p[2] * 256 + p[3];
         *xoff = p[4] * 256 + p[5];
         if (*xoff > 32767)
            *xoff -= 65536 ;
         *yoff = p[6] * 256 + p[7];
         if (*xoff > 32767)
            *xoff -= 65536 ;
         p += 8;
      }
   } else {
      *cwidth = *p++;
      *cheight = *p++;
      *xoff = *p++;
      *yoff = *p++;
      if (*xoff > 127)
         *xoff -= 256;
      if (*yoff > 127)
         *yoff -= 256;
   }
   return p ;
}
static void
downchar(chardesctype *c, shalfword cc)
{
   register long i, j;
   integer cheight, cwidth;
   register long k;
   register quarterword *p;
   register halfword cmd;
   integer xoff, yoff;
   halfword wwidth = 0;
   register long len;
   int smallchar;

   cmd = *(c->packptr) ;
   p = unpack_bb(c, &cwidth, &cheight, &xoff, &yoff) ;
   if (c->flags & BIGCHAR)
      smallchar = 0;
   else
      smallchar = 5;
   if (compressed) {
      len = getlong(p);
      p += 4;
   } else {
      wwidth = (cwidth + 15) / 16;
      i = 2 * cheight * (long)wwidth;
      if (i <= 0)
         i = 2;
      i += smallchar;
      if (mbytesleft < i) {
         if (mbytesleft >= RASTERCHUNK)
            (void) free((char *) mraster);
         if (RASTERCHUNK > i) {
            mraster = (quarterword *)mymalloc((integer)RASTERCHUNK);
            mbytesleft = RASTERCHUNK;
         } else {
            k = i + i / 4;
            mraster = (quarterword *)mymalloc((integer)k);
            mbytesleft = k;
         }
      }
      k = i;
      while (k > 0)
         mraster[--k] = 0;
      unpack(p, (halfword *)mraster, cwidth, cheight, cmd);
      p = mraster;
      len = i - smallchar;
   }
   if (cheight == 0 || cwidth == 0 || len == 0) {
      cwidth = 1;
      cheight = 1;
      wwidth = 1;
      len = 2;
      if (compressed)
         p = dummyend;  /* CMD(END); see repack.c */
      else
         mraster[0] = 0;
   }
   if (smallchar) {
      p[len] = cwidth;
      p[len + 1] = cheight;
      p[len + 2] = xoff + 128;
      p[len + 3] = yoff + 128;
      p[len + 4] = c->pixelwidth;
   } else
/*
 *   Now we actually send out the data.
 */
      specialout('[');
   if (compressed) {
      specialout('<');
      mhexout(p, len + smallchar);
      specialout('>');
   } else {
      i = (cwidth + 7) / 8;
      if (i * cheight > 65520) {
         long bc = 0;
         specialout('<');
         for (j=0; j<cheight; j++) {
            if (bc + i > 65520) {
               specialout('>');
               specialout('<');
               bc = 0;
            }
            mhexout(p, i);
            bc += i;
            p += 2*wwidth;
         }
         specialout('>');
      } else {
         specialout('<');
         if (2 * wwidth == i)
            mhexout(p, ((long)cheight) * i + smallchar);
         else {
            for (j=0; j<cheight; j++) {
               mhexout(p, i);
               p += 2*wwidth;
            }
            if (smallchar)
               mhexout(p, (long)smallchar);
         }
         specialout('>');
      }
   }
   if (smallchar == 0) {
      numout((integer)cwidth);
      numout((integer)cheight);
      numout((integer)xoff + 128); /* not all these casts needed. */
      numout((integer)yoff + 128);
      numout((integer)(c->pixelwidth));
   }
   if (lastccout + 1 == cc) {
      cmdout("I");
   } else {
      numout((integer)cc);
      cmdout("D");
   }
   lastccout = cc;
}
/*
 * Output the literal name of the font change command with PostScript index n
 */
static char goodnames[] =
   "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
void
makepsname(register char *s, register int n)
{
   n--;
   *s++ = 'F' + n / (sizeof(goodnames)-1);
   *s++ = goodnames[n % (sizeof(goodnames)-1)];
   *s++ = 0;
}
void
lfontout(int n)
{
   char buf[10];
   char *b = buf;
   *b++ = '/';
   makepsname(b, n);
   cmdout(buf);
}

/*
 *   And the download procedure.
 */
void
//AP--begin
//download(charusetype *p, int psfont)
download(charusetype *p)
//AP--end
{
   double scale;
   int seq;
   register int b, i;
   register halfword bit;
   register chardesctype *c;
   int cc, maxcc = -1, numcc;
   double fontscale;
   char name[10];
   lastccout = -5;
   name[0] = '/';
//AP--begin
//   makepsname(name + 1, psfont);
   name[1] = 0;
   if (p->fd->resfont) {
      makepsname(name + 1, p->fd->psname);
   }
//AP--end
   curfnt = p->fd;
//AP--begin
//   curfnt->psname = psfont;
//AP--end
   if (curfnt->resfont) {
      struct resfont *rf = curfnt->resfont;
      int non_empty=0;
//AP--begin
      if (rf->otftype) {
          cmdout(name);
		  if (rf->specialinstructions[0] == '{')
			cmdout(rf->specialinstructions);
		  else {
			  char *p = strstr(rf->specialinstructions,"{");
			  if (p)
				cmdout(p);
		  }
          fontscale = curfnt->scaledsize * conv;
          sprintf(nextstring, "%g", fontscale);
          cmdout(nextstring);
          strcpy(nextstring, "/CID+");
          strcat(nextstring, rf->PSname);
          cmdout(nextstring);
          cmdout("ct_cid");
          rf->sent = 1;
          return;
      }
//AP--end
      for (b=0; b<16; b++)
        if(p->bitmap[b] !=0)
            non_empty =1;
      if(non_empty==0 && curfnt->iswide == 0 && curfnt->codewidth==1)
        return;
      cmdout(name);
/* following code re-arranged - Rob Hutchings 1992Apr02 */
//AP--begin
      cc = 255;
//      c = curfnt->chardesc + 255;
      c = find_chardesc(curfnt, cc);
//AP--end
      numcc = 0;
      i = 0;
      for (b=15; b>=0; b--) {
         for (bit=1; bit; bit<<=1) {
             if (p->bitmap[b] & bit) {
               if (i > 0) {
                  numout((integer)i);
                  specialout('[');
                  i = 0;
               }
               numout((integer)c->pixelwidth);
               c->flags |= EXISTS;
               numcc++;
            } else {
               i++;
               c->flags &= ~EXISTS;
            }
            cc--;
//AP--begin
//            c--;
            c = find_chardesc(curfnt, cc);
//AP--end
         }
      }
      if (i > 0) {
         numout((integer)i);
         specialout('[');
      }
      specialout ('{');
      if (rf->specialinstructions)
         cmdout(rf->specialinstructions);
      specialout ('}');
      numout((integer)numcc);
      /*
       *   This code has been bogus for a long time.  The fix is
       *   straightforward.  The input, curfnt->scaledsize, is the
       *   desired size of the font in dvi units.  The output is the
       *   fontscale, which is the height of the font in output units
       *   which are simply pixels.  Thus, all we need to do is multiply
       *   by the default generic conv, which properly takes into
       *   account magnification, num/den, output resolution, and so on.
       *   Sorry this bug has been in here so long.   -tgr
       */
      fontscale = curfnt->scaledsize * conv;
      sprintf(nextstring, "%g", fontscale);
      cmdout(nextstring);
      strcpy(nextstring, "/");
      strcat(nextstring, rf->PSname);
      cmdout(nextstring);
/* end of changed section - Rob */
      cmdout("rf");
      rf->sent = 1;
      return;
   }
/*
 *   Here we calculate the largest character actually used, and
 *   send it as a parameter to df.
 */
   cc = 0;
   numcc = 0;
   for (b=0; b<16; b++) {
      for (bit=32768; bit!=0; bit>>=1) {
         if (p->bitmap[b] & bit) {
            maxcc = cc;
            numcc++;
         }
         cc++;
      }
   }
   if (numcc <= 0)
      return;
   fontscale = ((double)(curfnt->scaledsize)) / 65536.0;
   fontscale *= (mag/1000.0);
   newline();
   fprintf(bitfile, "%%DVIPSBitmapFont: %s %s %g %d\n", name+1, curfnt->name,
                     fontscale, numcc);
   scale = fontscale * DPI / 72.0 ;
   seq = -1 ;
   if (encodetype3)
      seq = downloadbmencoding(curfnt->name, scale, curfnt) ;
   cmdout(name);
   numout((integer)numcc);
   numout((integer)maxcc + 1);
/*
 *   If we need to scale the font, we say so by using dfs
 *   instead of df, and we give it a scale factor.  We also
 *   scale the character widths; this is ugly, but scaling
 *   fonts is ugly, and this is the best we can probably do.
 */
   if (curfnt->dpi != curfnt->loadeddpi) {
      numout((integer)curfnt->dpi);
      numout((integer)curfnt->loadeddpi);
      if (curfnt->alreadyscaled == 0) {
//AP--begin
//         for (b=0, c=curfnt->chardesc; b<256; b++, c++)
//            c->pixelwidth = (c->pixelwidth * 
//      (long)curfnt->dpi * 2 + curfnt->loadeddpi) / (2 * curfnt->loadeddpi);
          for (b = 0; b<256; b++) {
              c = find_chardesc(curfnt, b);
              c->pixelwidth = (c->pixelwidth *
                  (long)curfnt->dpi * 2 + curfnt->loadeddpi) / (2 * curfnt->loadeddpi);
          }
//AP--end
         curfnt->alreadyscaled = 1;
      }
      cmdout("dfs");
   } else
      cmdout("df");
//AP--begin
//   c = curfnt->chardesc;
//AP--end
   cc = 0;
   for (b=0; b<16; b++) {
      for (bit=32768; bit; bit>>=1) {
//AP--begin
          c = find_chardesc(curfnt, cc);
//AP--end
         if (p->bitmap[b] & bit) {
            downchar(c, (shalfword)cc);
            c->flags |= EXISTS;
         } else
            c->flags &= ~EXISTS;
         c++;
         cc++;
      }
   }
   cmdout("E");
   newline() ;
   if (seq >= 0)
      finishbitmapencoding(name, scale) ;
   fprintf(bitfile, "%%EndDVIPSBitmapFont\n");
}

#ifdef DOWNLOAD_USING_PDFTEX
/*
 *   Magic code to deal with PostScript font partial downloading.
 *   We track the encodings we've seen so far and keep them in these
 *   structures.  We rely on writet1 to load the encodings for us.
 */
static struct seenEncodings {
   struct seenEncodings *next;
   const char *name;
   char **glyphs;
} *seenEncodings;

/*
 *   Load a new encoding and return the array of glyphs as a vector.
 *   Linear search.
 */
static char **getEncoding(char *encoding) {
   struct seenEncodings *p = seenEncodings;
   while (p != 0)
      if (strcmp(encoding, p->name) == 0)
         break;
      else
         p = p->next;
   if (p == 0) {
      p = (struct seenEncodings *)mymalloc(sizeof(struct seenEncodings));
      p->next = seenEncodings;
      seenEncodings = p;
      p->name = xstrdup(encoding);
      p->glyphs = load_enc_file(encoding);
   }
   return p->glyphs;
}
/*
 *   When partially downloading a type 1 font, sometimes the font uses
 *   the built-in encoding (which we don't know at this point) so we want
 *   to subset it by character code.  Sometimes the font uses a different
 *   encoding, so we want to subset it by glyph.  These routines manage
 *   accumulating all the glyph names for a particular font from all
 *   the various uses into a single, slash-delimited string for writet1().
 *
 *   If no glyphs have been added, extraGlyphs is null.  In all cases,
 *   the memory allocated for this is in extraGlyphSpace.
 */
static char *extraGlyphs = 0;
static char *extraGlyphSpace = 0;
static int extraGlyphSize = 0;
static int glyphSizeUsed = 0;
/*
 *   We want to make sure we pass in null or "/" but never "".  
 */
static void clearExtraGlyphList(void) {
   glyphSizeUsed = 0;
   extraGlyphs = 0;
}
/*
 *   Add the glyph name; make sure it hasn't been added already.
 *   We do this by adding it, so we can get the // around it,
 *   and *then* see if it's already there and if it is un-add it.
 */
static void addGlyph(const char *glyphName) {
   int len = strlen(glyphName);
   char *startOfAdd = 0;
   if (len + glyphSizeUsed + 3 > extraGlyphSize) {
      extraGlyphSize = 2 * (extraGlyphSize + len + 100);
      extraGlyphSpace = (char *) xrealloc(extraGlyphSpace, extraGlyphSize);
   }
   extraGlyphs = extraGlyphSpace;
   if (glyphSizeUsed == 0) {
      startOfAdd = extraGlyphs + glyphSizeUsed;
      extraGlyphs[glyphSizeUsed++] = '/'; /* leading / */
   } else {
      startOfAdd = extraGlyphs + glyphSizeUsed - 1;
   }
   strcpy(extraGlyphs + glyphSizeUsed, glyphName);
   glyphSizeUsed += len;
   extraGlyphs[glyphSizeUsed++] = '/';    /* trailing / */
   extraGlyphs[glyphSizeUsed] = 0;
   if (strstr(extraGlyphs, startOfAdd) != startOfAdd) { /* already there! */
      glyphSizeUsed = startOfAdd - extraGlyphs + 1; /* kill the second copy */
      extraGlyphs[glyphSizeUsed] = 0;
   }
}
#endif

/*
 *   Download a PostScript font, using partial font downloading if
 *   necessary.
 */
static void
downpsfont(charusetype *p, charusetype *all)
{
    static unsigned char grid[256];
    int GridCount;
    register int b;
    register halfword bit;
    register chardesctype *c;
    struct resfont *rf;
    int cc;
    int j;
    //AP--begin
    luacharmap *map;
    charusetype *all_gl = all;
    //AP--end

    curfnt = p->fd;
    rf = curfnt->resfont;
    if (rf == 0 || rf->Fontfile == NULL)
       return;
    for (; all->fd; all++)
//AP--begin
//      if (all->fd->resfont &&
//           strcmp(rf->PSname, all->fd->resfont->PSname) == 0)
//          break;
	  if (rf->otftype) {
          if (all->fd->resfont && all->fd->resfont->otftype &&
             (strcmp(rf->PSname, all->fd->resfont->PSname) == 0))
           break;
      }
      else {
          if (all->fd->resfont && (!all->fd->resfont->otftype) &&
             (strcmp(rf->PSname, all->fd->resfont->PSname) == 0))
           break;
      }
//AP--end
    if (all != p)
       return;
    if (rf->sent == 2) /* sent as header, from a PS file */
       return;
    for (j=0; downloadedpsnames[j] && j < unused_top_of_psnames; j++) {
       if (strcmp (downloadedpsnames[j], rf->PSname) == 0)
          return;
    }
    if (all->fd == 0)
       error("! internal error in downpsfont");
//AP--begin
    if (rf->otftype) {
        map = (luacharmap *)mymalloc((integer)sizeof(luacharmap));
        map->luamap_idx = rf->luamap_idx;
        map->next = NULL;
        p->map_chain = map;
        all++;
        for (; all->fd; all++) {
            if (all->fd->resfont && all->fd->resfont->otftype &&
					(strcmp(rf->PSname, all->fd->resfont->PSname) == 0)) {
                for (j = 0; j < 4096; j++)
                    p->bitmap[j] |= all->bitmap[j];
                map = (luacharmap *)mymalloc((integer)sizeof(luacharmap));
                map->luamap_idx = all->fd->resfont->luamap_idx;
                map->next = p->map_chain;
                p->map_chain = map;
            }
        }
        newline();
        if (!disablecomments)
            fprintf(bitfile, "%%%%BeginFont: %s\n", rf->PSname);
        writecid(p);
        if (!quiet) {
            if (strlen(realnameoffile) + prettycolumn > STDOUTSIZE) {
                fprintf(stderr, "\n");
                prettycolumn = 0;
            }
            fprintf(stderr, "<%s>", realnameoffile);
            prettycolumn += strlen(realnameoffile) + 2;
        }
        if (!disablecomments)
            fprintf(bitfile, "%%%%EndFont \n");
        return;
    }
    //    if (!partialdownload) {
    if (!t1_partialdownload || !rf->partialdownload) {
        /*
        infont = all->fd->resfont->PSname;
        copyfile(all->fd->resfont->Fontfile);
        infont = 0;
        */
#ifdef DOWNLOAD_USING_PDFTEX
        for (cc = 0; cc<256; cc++)
            grid[cc] = 1;
        newline();
        if (!disablecomments)
            fprintf(bitfile, "%%%%BeginFont: %s\n", rf->PSname);
        if (!t1_write_full(rf->Fontfile, grid))
            dvips_exit(1);
        //            exit(1);
        if (!quiet) {
            if (strlen(realnameoffile) + prettycolumn > STDOUTSIZE) {
                fprintf(stderr, "\n");
                prettycolumn = 0;
            }
            fprintf(stderr, "<%s>", realnameoffile);
            prettycolumn += strlen(realnameoffile) + 2;
        }
        if (!disablecomments)
            fprintf(bitfile, "%%%%EndFont \n");
#else
        infont = all->fd->resfont->PSname;
        copyfile(all->fd->resfont->Fontfile);
        infont = 0;
#endif
        return;
//AP--end
    }
    for (cc=0; cc<256; cc++)
       grid[cc] = 0;
#ifdef DOWNLOAD_USING_PDFTEX
    clearExtraGlyphList();
#endif
    for (; all->fd; all++) {
//AP--begin
//        if (all->fd->resfont == 0 ||
        if ((all->fd->resfont == 0) || all->fd->resfont->otftype ||
//AP--end
            strcmp(rf->PSname, all->fd->resfont->PSname))
           continue;
        curfnt = all->fd;
#ifdef DOWNLOAD_USING_PDFTEX
        if (curfnt->resfont->Vectfile) {
       char **glyphs = getEncoding(curfnt->resfont->Vectfile);
//AP--begin
//           c = curfnt->chardesc + 255;
           c = find_chardesc(curfnt, cc);
//AP--end
           cc = 255;
           for (b=15; b>=0; b--) {
               for (bit=1; bit; bit<<=1) {
                   if (all->bitmap[b] & bit) {
                      addGlyph(glyphs[cc]);
                   }
                   cc--;
//AP--begin
//                   c--;
                   c = find_chardesc(curfnt, cc);
//AP--end
               }
            }
        } else {
#endif
//AP--begin
//           c = curfnt->chardesc + 255;
           c = find_chardesc(curfnt, cc);
//AP--end
           cc = 255;
           for (b=15; b>=0; b--) {
               for (bit=1; bit; bit<<=1) {
                   if (all->bitmap[b] & bit) {
                       grid[cc]=1;
                   }
                   cc--;
//AP--begin
//                   c--;
                   c = find_chardesc(curfnt, cc);
//AP--end
               }
            }
#ifdef DOWNLOAD_USING_PDFTEX
        }
#endif
    }

    for (GridCount=0,cc=0; cc<256; cc++) {
/*      tmpgrid[cc]=grid[cc]; */
        if(grid[cc] ==1) {
            GridCount++;
        }
    }
    if(GridCount!=0
#ifdef DOWNLOAD_USING_PDFTEX
       || extraGlyphs
#endif
       ) {
        newline();
        if (! disablecomments)
           fprintf(bitfile, "%%%%BeginFont: %s\n",  rf->PSname);
#ifdef DOWNLOAD_USING_PDFTEX
        if (!t1_subset_2(rf->Fontfile, grid, extraGlyphs))
#else
        if(FontPart(bitfile, rf->Fontfile, rf->Vectfile) < 0)
#endif
//AP--begin
            dvips_exit(1);
//            exit(1);
//AP--end
        if (!quiet) {
           if (strlen(realnameoffile) + prettycolumn > STDOUTSIZE) {
              fprintf(stderr, "\n");
              prettycolumn = 0;
           }
           fprintf(stderr, "<%s>", realnameoffile);
           prettycolumn += strlen(realnameoffile) + 2;
        }
        if (! disablecomments)
           fprintf(bitfile, "%%%%EndFont \n");
   }
}

void
dopsfont(sectiontype *fs)
{
    charusetype *cu;
//AP--begin
    int psfont;
//AP--end

    cu = (charusetype *) (fs + 1);
//AP--begin
    psfont = 1;
//AP--end
#ifdef DOWNLOAD_USING_PDFTEX
    while (cu->fd) {
       if (cu->psfused)
          cu->fd->psflag = EXISTS;
//AP--begin
       cu->fd->psname = psfont;
       psfont++;
//AP--end
       downpsfont(cu++, (charusetype *)(fs + 1));
    }
#else
    line = getmem(BUFSIZ);
    tmpline=line;
    while (cu->fd) {
       if (cu->psfused)
          cu->fd->psflag = EXISTS;
//AP--begin
       cu->fd->psname = psfont;
       psfont++;
//AP--end
       downpsfont(cu++, (charusetype *)(fs + 1));
    }
    loadbase = ~FLG_LOAD_BASE;
    FirstCharB=UnDefineChars(FirstCharB);
    free(tmpline);
#endif
}
