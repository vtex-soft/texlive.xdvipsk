--
--  This is file `luafonts.lua',
--
--  Copyright (C) 2015 by Sigitas Tolusis <sigitas@vtex.lt>
--
--  This work is under the CC0 license.
--

module('luafonts', package.seeall)

luafonts.module = {
    name          = "luafonts",
    version       =  2.1,
    date          = "2024/02/14",
    description   = "Lua tex support package.",
    author        = "Sigitas Tolusis",
    copyright     = "Sigitas Tolusis",
    license       = "CC0",
}

luatexbase.provides_module(luafonts.module)

local format = string.format

luafonts.log = luafonts.log or function(...)
  luatexbase.module_log('luafonts', format(...))
end

luafonts.warning = luafonts.warning or function(...)
  luatexbase.module_warning('luafonts', format(...))
end

luafonts.error = luafonts.error or function(...)
  luatexbase.module_error('luafonts', format(...))
end

config = config or { }
config.lualibs = config.lualibs or { }
config.lualibs.load_extended = false

require "lualibs"

config.luafonts = {}
config.luafonts.debug = false

local vtx_fonts_info = vtx_fonts_info or { }
vtx_fonts_info.otf_fonts = vtx_fonts_info.otf_fonts or { }
vtx_fonts_info.tfm_fonts = vtx_fonts_info.tfm_fonts or { }
vtx_fonts_info.encodings = vtx_fonts_info.encodings or { }

function createfontsmap()
   local options = config.luafonts
   local output_dir = options.output_dir or ".xdvipsk"
   if not lfs.isdir(output_dir) then
      lfs.mkdir(output_dir)
   end
   local selfautoparent = kpse.expand_var('$SELFAUTOPARENT')
   local selfautograndparent = kpse.expand_var('$SELFAUTOGRANDPARENT')
   for i,v in table.sortedhash(fonts.hashes.identifiers) do
      local tfmname = v.name:gsub( "%s$", "")
      if v['format'] ~= 'unknown' then
         local filename = v.filename:gsub("^harfloaded:", "")
         filename = filename:gsub("^" .. selfautoparent, "$SELFAUTOPARENT"):gsub("^" .. selfautograndparent, "$SELFAUTOGRANDPARENT"):gsub( "%s$", "")
         local fontname = v.fullname:gsub('[<>:"/\\|%?%*]', '@')
         local psname = v.psname:gsub( "%s$", "")
         local subfont = v.shared and v.shared.rawdata and v.shared.rawdata.subfont
         if vtx_fonts_info.otf_fonts[tfmname] == nil then 
            vtx_fonts_info.otf_fonts[tfmname] = tfmname .. '\t' .. '\t' .. psname .. '\t' .. fontname .. '\t>' .. filename
            if subfont then
               vtx_fonts_info.otf_fonts[tfmname] = vtx_fonts_info.otf_fonts[tfmname] .. "(" .. tostring(subfont) .. ")"
            end
         end
         if vtx_fonts_info.encodings[fontname] == nil then
            vtx_fonts_info.encodings[fontname] = { }
            local temp_table_var = vtx_fonts_info.encodings[fontname]
            for key,value in table.sortedhash(v["characters"]) do
               temp_table_var[#temp_table_var+1] = string.format("%s,%s,%s,%s,%s", key, value.index, value.tounicode, value.width, value.height)
            end
            vtx_fonts_info.encodings[fontname] = temp_table_var
         end
         if options.debug then
            io.savedata(output_dir .. "/" .. fontname .. '.font.dump', table.serialize(v))
         end
      else
         if vtx_fonts_info.tfm_fonts[tfmname] == nil then
             vtx_fonts_info.tfm_fonts[tfmname] = v["properties"]
         end
      end
   end
   local fd = io.open(output_dir .. "/" .. tex.jobname .. '.opentype.map', 'w')
   for key,value in table.sortedhash(vtx_fonts_info.otf_fonts) do
      fd:write(value .. "\n")
   end
   fd:close()
   for key,value in table.sortedhash(vtx_fonts_info.encodings) do
      local fd = io.open(string.format("%s/%s.encodings.map", output_dir, key), 'w')
      for _,item in ipairs(value) do
          fd:write(item .. "\n")
      end
   end
end

function buildpage_filter_callback(extrainfo)
   if extrainfo == "end" then
      if fonts then
         createfontsmap()
      end
   end
end

