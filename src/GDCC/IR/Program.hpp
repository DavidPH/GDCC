//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation full program data handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Program_H__
#define GDCC__IR__Program_H__

#include "Function.hpp"
#include "Glyph.hpp"
#include "Import.hpp"
#include "Object.hpp"
#include "Space.hpp"
#include "StrEnt.hpp"

#include "../Utility.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Program
      //
      class Program
      {
      public:
         template<typename T>
         using Table = std::unordered_map<String, T>;

         template<typename T>
         using TableRange = Range<MemItr<typename Table<T>::iterator>>;


         Program();
         Program(Program const &) = delete;
         Program(Program &&) = delete;
         ~Program();

         Program &operator = (Program const &) = delete;
         Program &operator = (Program &&) = delete;

         Function  *findFunction (String glyph);
         GlyphData *findGlyphData(String glyph);
         Import    *findImport   (String glyph);
         Object    *findObject   (String glyph);
         Space     *findSpace    (AddrSpace as);
         StrEnt    *findStrEnt   (String glyph);
         StrEnt    *findStrEntVal(String value);

         Function  &getFunction   (String glyph);
         GlyphData &getGlyphData  (String glyph);
         Import    &getImport     (String glyph);
         Object    &getObject     (String glyph);
         Space     &getSpace      (AddrSpace as);
         Space     &getSpaceGblArr(String glyph);
         Space     &getSpaceGblReg();
         Space     &getSpaceLocArs();
         Space     &getSpaceMapArr(String glyph);
         Space     &getSpaceMapReg();
         Space     &getSpaceWldArr(String glyph);
         Space     &getSpaceWldReg();
         StrEnt    &getStrEnt     (String glyph);

         void mergeFunction (Function  &out, Function  &&in);
         void mergeGlyphData(GlyphData &out, GlyphData &&in);
         void mergeImport   (Import    &out, Import    &&in);
         void mergeObject   (Object    &out, Object    &&in);
         void mergeSpace    (Space     &out, Space     &&in);
         void mergeStrEnt   (StrEnt    &out, StrEnt    &&in);

         TableRange<Function>  rangeFunction   ();
         TableRange<GlyphData> rangeGlyphData  ();
         TableRange<Import>    rangeImport     ();
         TableRange<Object>    rangeObject     ();
         TableRange<Space>     rangeSpaceGblArs();
         TableRange<Space>     rangeSpaceMapArs();
         TableRange<Space>     rangeSpaceWldArs();
         TableRange<StrEnt>    rangeStrEnt     ();

         std::size_t sizeFunction()    const;
         std::size_t sizeGlyphData()   const;
         std::size_t sizeImport()      const;
         std::size_t sizeObject()      const;
         std::size_t sizeSpaceGblArs() const;
         std::size_t sizeSpaceMapArs() const;
         std::size_t sizeSpaceWldArs() const;
         std::size_t sizeStrEnt()      const;


         friend OArchive &operator << (OArchive &out, Program const &in);

         friend IArchive &operator >> (IArchive &in, Program &out);

      private:
         Table<Function>  tableFunction;
         Table<GlyphData> tableGlyphData;
         Table<Import>    tableImport;
         Table<Object>    tableObject;
         Table<Space>     tableSpaceGblArs;
         Table<Space>     tableSpaceMapArs;
         Table<Space>     tableSpaceWldArs;
         Table<StrEnt>    tableStrEnt;

         Space spaceGblReg;
         Space spaceLocArs;
         Space spaceMapReg;
         Space spaceWldReg;
      };
   }
}

#endif//GDCC__IR__Program_H__

