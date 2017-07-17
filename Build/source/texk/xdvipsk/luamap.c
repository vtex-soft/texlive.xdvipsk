/*
*   Loads a opentype font charcodes mapping file from Luatex.
*/
#include "dvips.h" /* The copyright notice in that file is included too! */
#ifdef KPATHSEA
#include <kpathsea/c-pathmx.h>
#include <kpathsea/concatn.h>
#include <kpathsea/variable.h>
#else
/* Be sure we have `isascii'.  */
#ifndef WIN32
#if !(defined(HAVE_DECL_ISASCII) && HAVE_DECL_ISASCII)
#define isascii(c) (((c) & ~0x7f) == 0)
#endif
#endif
#endif
/*
*   The external declarations:
*/
#include "protos.h"
#include "mem.h"
#include "error.h"
#include "dpxfile.h"

#define LUAMAP_CACHE_ALLOC_SIZE 16u
#define LUAMAP_DEBUG_STR "LuaMap"

struct LuaMap {
	char   *name;
	luamaptype *luamap;
};

struct LuaMap_cache {
  int    num;
  int    max;
  struct LuaMap **luamaps;
};

static struct LuaMap_cache *__cache = NULL;

#define CHECK_ID(n) do {\
                        if (! __cache)\
                           ERROR("%s: LuaMap cache not initialized.", LUAMAP_DEBUG_STR);\
                        if ((n) < 0 || (n) >= __cache->num)\
                           ERROR("Invalid LuaMap ID %d", (n));\
                    } while (0)

static const long hextable[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

static long hexdec(unsigned const char *hex) {
	long ret = 0;
	while (*hex && ret >= 0) {
		ret = (ret << 4) | hextable[*hex++];
	}
	return ret;
}

static struct LuaMap *LuaMap_new()
{
	struct LuaMap *map = NEW(1, struct LuaMap);
	map->name = NULL;
	map->luamap = NULL;
	return map;
}

static int LuaMap_parse(const char *name, struct LuaMap *map, FILE *fp)
{
	luamaptype *lmp;
	int k, m, n, charcode, cid;
	unsigned short gid;
	char strbuf[512], str[5];
	unsigned int *tounicode;
	char seps[] = ",";
	char *token;

	map->name = NEW(strlen(name) + 1, char);
	strcpy(map->name, name);

	while (fgets(strbuf, 512, fp) != NULL) {
		token = strtok(strbuf, seps);
		m = 0;
		while (token != NULL) {
			switch (m) {
			case 0:
				charcode = atoi(token);
				break;
			case 1:
				gid = atoi(token);
				break;
			case 2:
				if (token[strlen(token) - 1] == '\n')
					token[strlen(token) - 1] = 0;
				if (strcmp(token, "nil") == 0) {
					n = 0;
					tounicode = NEW(1, unsigned int);
				}
				else {
					n = strlen(token) / 4;
					if (strlen(token) % 4)
						n++;
					tounicode = NEW(n, unsigned int);
					for (k = 0; k < n; k++) {
						memset(str, 0, 5);
						strncpy(str, &token[k * 4], 4);
						tounicode[k] = hexdec(str);
					}
				}
				lmp = NEW(1, luamaptype);
				lmp->charcode = charcode;
				lmp->gid = gid;
				lmp->tu_count = n;
				lmp->tounicode = tounicode;
				HASH_ADD_INT(map->luamap, gid, lmp);
				break;
			default:
				continue;
			}
			token = strtok(NULL, seps);
			m++;
		}
	}

	return 0;
}

static void LuaMap_release(struct LuaMap *map)
{
	luamaptype *current, *tmp;

	if (map->name)
		RELEASE(map->name);
	HASH_ITER(hh, map->luamap, current, tmp) {
		RELEASE(current->tounicode);
		HASH_DEL(map->luamap, current);  /* delete it (users advances to next) */
		RELEASE(current);            /* free it */
	}
	RELEASE(map);
}


void
LuaMap_cache_init (void)
{
  static unsigned char range_min[2] = {0x00, 0x00};
  static unsigned char range_max[2] = {0xff, 0xff};

  if (__cache)
    ERROR("%s: Already initialized.", LUAMAP_DEBUG_STR);

  __cache = NEW(1, struct LuaMap_cache);

  __cache->max   = LUAMAP_CACHE_ALLOC_SIZE;
  __cache->luamaps = NEW(__cache->max, struct LuaMap *);
  __cache->num   = 0;
}

luamaptype *
LuaMap_cache_get (int id)
{
  CHECK_ID(id);
  return __cache->luamaps[id]->luamap;
}

int
LuaMap_cache_find (const char *luamap_name)
{
  int   n, id = 0;
  char strbuf[500], *p;
  FILE *fp = NULL;

  if (!__cache)
    LuaMap_cache_init();
  ASSERT(__cache);

  for (id = 0; id < __cache->num; id++) {
    char *name = NULL;
    /* CMapName may be undefined when processing usecmap. */
    name = __cache->luamaps[id]->name;
    if (name && strcmp(luamap_name, name) == 0) {
      return id;
    }
  }

  if ((fulliname == 0) || (strlen(fulliname) == 0))
	  return -1;
  strcpy(strbuf, fulliname);
  p = strbuf + strlen(strbuf) - 1;
  while (p > strbuf) {
	  if ((*p == '\\') || (*p == '/')) {
		  *(p + 1) = 0;
		  break;
	  }
	  p--;
  }
  if (p == strbuf) 
	  *p = 0;
  strcat(strbuf, OTFMAPFILEDIR);
  strcat(strbuf, luamap_name);
  strcat(strbuf, OTFENCDFILEEXT);
  fp = DPXFOPEN(strbuf, DPX_RES_TYPE_TEXT);
  if (!fp)
    return -1;

  if (__cache->num >= __cache->max) {
    __cache->max   += LUAMAP_CACHE_ALLOC_SIZE;
    __cache->luamaps = RENEW(__cache->luamaps, __cache->max, struct LuaMap *);
  }
  id = __cache->num;
  (__cache->num)++;
  __cache->luamaps[id] = LuaMap_new();

  if (LuaMap_parse(luamap_name, __cache->luamaps[id], fp) < 0)
    ERROR("%s: Parsing CMap file failed.", LUAMAP_DEBUG_STR);

  DPXFCLOSE(fp);

  return id;
}

void
LuaMap_cache_close (void)
{
  if (__cache) {
    int id;
    for (id = 0; id < __cache->num; id++) {
      LuaMap_release(__cache->luamaps[id]);
    }
    RELEASE(__cache->luamaps);
    RELEASE(__cache);
    __cache = NULL;
  }
}
