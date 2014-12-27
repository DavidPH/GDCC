//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "../IR/Function.hpp"
#include "../IR/Glyph.hpp"
#include "../IR/Import.hpp"
#include "../IR/Object.hpp"
#include "../IR/Space.hpp"
#include "../IR/StrEnt.hpp"

#include "../Core/MemItr.hpp"
#include "../Core/Range.hpp"

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
         using Table = std::unordered_map<Core::String, T>;

         template<typename T>
         using TableRange = Core::Range<Core::MemItr<typename Table<T>::iterator>>;

         template<typename T>
         using TableCRange =
            Core::Range<Core::MemItr<typename Table<T>::const_iterator, T const>>;


         Program();
         Program(Program const &) = delete;
         Program(Program &&) = delete;
         ~Program();

         Program &operator = (Program const &) = delete;
         Program &operator = (Program &&) = delete;

         Function  *findFunction   (Core::String glyph);
         GlyphData *findGlyphData  (Core::String glyph);
         Import    *findImport     (Core::String glyph);
         Object    *findObject     (Core::String glyph);
         Space     *findSpace      (AddrSpace    as);
         Space     *findSpaceGblArr(Core::String glyph);
         Space     *findSpaceMapArr(Core::String glyph);
         Space     *findSpaceWldArr(Core::String glyph);
         StrEnt    *findStrEnt     (Core::String glyph);
         StrEnt    *findStrEntVal  (Core::String value);

         Function  &getFunction   (Core::String glyph);
         GlyphData &getGlyphData  (Core::String glyph);
         Import    &getImport     (Core::String glyph);
         Object    &getObject     (Core::String glyph);
         Space     &getSpace      (AddrSpace    as);
         Space     &getSpaceGblArr(Core::String glyph);
         Space     &getSpaceGblReg();
         Space     &getSpaceMapArr(Core::String glyph);
         Space     &getSpaceMapReg();
         Space     &getSpaceSta();
         Space     &getSpaceWldArr(Core::String glyph);
         Space     &getSpaceWldReg();
         StrEnt    &getStrEnt     (Core::String glyph);

         void mergeFunction (Function  &out, Function  &&in);
         void mergeGlyphData(GlyphData &out, GlyphData &&in);
         void mergeImport   (Import    &out, Import    &&in);
         void mergeObject   (Object    &out, Object    &&in);
         void mergeSpace    (Space     &out, Space     &&in);
         void mergeStrEnt   (StrEnt    &out, StrEnt    &&in);

         TableRange <Function>  rangeFunction   ();
         TableCRange<Function>  rangeFunction   () const;
         TableRange <GlyphData> rangeGlyphData  ();
         TableCRange<GlyphData> rangeGlyphData  () const;
         TableRange <Import>    rangeImport     ();
         TableCRange<Import>    rangeImport     () const;
         TableRange <Object>    rangeObject     ();
         TableCRange<Object>    rangeObject     () const;
         TableRange <Space>     rangeSpaceGblArs();
         TableCRange<Space>     rangeSpaceGblArs() const;
         TableRange <Space>     rangeSpaceMapArs();
         TableCRange<Space>     rangeSpaceMapArs() const;
         TableRange <Space>     rangeSpaceWldArs();
         TableCRange<Space>     rangeSpaceWldArs() const;
         TableRange <StrEnt>    rangeStrEnt     ();
         TableCRange<StrEnt>    rangeStrEnt     () const;

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
         Space spaceMapReg;
         Space spaceSta;
         Space spaceWldReg;
      };
   }
}

#endif//GDCC__IR__Program_H__

