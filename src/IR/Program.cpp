//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation full program data handling.
//
//-----------------------------------------------------------------------------

#include "IR/Program.hpp"

#include "IR/IArchive.hpp"
#include "IR/Linkage.hpp"
#include "IR/OArchive.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// FindTable
//
template<typename T>
static T *FindTable(GDCC::IR::Program::Table<T> &table, GDCC::Core::String glyph)
{
   auto itr = table.find(glyph);
   return itr == table.end() ? nullptr : &itr->second;
}

//
// GetTable
//
template<typename T, typename... Args>
static T &GetTable(GDCC::IR::Program::Table<T> &table, GDCC::Core::String glyph,
   Args &&...args)
{
   auto itr = table.find(glyph);

   if(itr == table.end())
   {
      itr = table.emplace(std::piecewise_construct,
         std::forward_as_tuple(glyph),
         std::forward_as_tuple(std::forward<Args>(args)...)
         ).first;
   }

   return itr->second;
}

//
// RangeTable
//
template<typename T>
static GDCC::IR::Program::TableRange<T>
RangeTable(GDCC::IR::Program::Table<T> &table)
{
   return {table.begin(), table.end()};
}

//
// RangeTable
//
template<typename T>
static GDCC::IR::Program::TableCRange<T>
RangeTable(GDCC::IR::Program::Table<T> const &table)
{
   return {table.begin(), table.end()};
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Program constructor
      //
      Program::Program() :
         spaceGblReg{AddrSpace(AddrBase::GblReg, Core::STR_)},
         spaceMapReg{AddrSpace(AddrBase::MapReg, Core::STR_)},
         spaceSta   {AddrSpace(AddrBase::Sta,    Core::STR_)},
         spaceWldReg{AddrSpace(AddrBase::WldReg, Core::STR_)}
      {
      }

      //
      // Program destructor
      //
      Program::~Program()
      {
      }

      //
      // Program::findFunction
      //
      Function *Program::findFunction(Core::String glyph)
      {
         return FindTable(tableFunction, glyph);
      }

      //
      // Program::findGlyphData
      //
      GlyphData *Program::findGlyphData(Core::String glyph)
      {
         return FindTable(tableGlyphData, glyph);
      }

      //
      // Program::findImport
      //
      Import *Program::findImport(Core::String glyph)
      {
         return FindTable(tableImport, glyph);
      }

      //
      // Program::findObject
      //
      Object *Program::findObject(Core::String glyph)
      {
         return FindTable(tableObject, glyph);
      }

      //
      // Program::findSpace
      //
      Space *Program::findSpace(AddrSpace as)
      {
         switch(as.base)
         {
         case AddrBase::GblArr:
         case AddrBase::GblArs: return findSpaceGblArr(as.name);
         case AddrBase::GblReg: return &getSpaceGblReg();
         case AddrBase::LocArr: return findSpaceLocArr(as.name);
         case AddrBase::MapArr:
         case AddrBase::MapArs: return findSpaceMapArr(as.name);
         case AddrBase::MapReg: return &getSpaceMapReg();
         case AddrBase::Sta:    return &getSpaceSta();
         case AddrBase::WldArr:
         case AddrBase::WldArs: return findSpaceWldArr(as.name);
         case AddrBase::WldReg: return &getSpaceWldReg();

         default:
            return nullptr;
         }
      }

      //
      // Program::findSpaceGblArr
      //
      Space *Program::findSpaceGblArr(Core::String glyph)
      {
         return FindTable(tableSpaceGblArs, glyph);
      }

      //
      // Program::findSpaceLocArr
      //
      Space *Program::findSpaceLocArr(Core::String glyph)
      {
         return FindTable(tableSpaceLocArs, glyph);
      }

      //
      // Program::findSpaceMapArr
      //
      Space *Program::findSpaceMapArr(Core::String glyph)
      {
         return FindTable(tableSpaceMapArs, glyph);
      }

      //
      // Program::findSpaceWldArr
      //
      Space *Program::findSpaceWldArr(Core::String glyph)
      {
         return FindTable(tableSpaceWldArs, glyph);
      }

      //
      // Program::findStrEnt
      //
      StrEnt *Program::findStrEnt(Core::String glyph)
      {
         return FindTable(tableStrEnt, glyph);
      }

      //
      // Program::findStrEntVal
      //
      StrEnt *Program::findStrEntVal(Core::String value)
      {
         for(auto &itr : tableStrEnt)
         {
            if(itr.second.valueStr == value && itr.second.alias && itr.second.defin)
               return &itr.second;
         }

         return nullptr;
      }

      //
      // Program::getFunction
      //
      Function &Program::getFunction(Core::String glyph)
      {
         return GetTable(tableFunction, glyph, glyph);
      }

      //
      // Program::getGlyphData
      //
      GlyphData &Program::getGlyphData(Core::String glyph)
      {
         return GetTable(tableGlyphData, glyph, glyph);
      }

      //
      // Program::getImport
      //
      Import &Program::getImport(Core::String glyph)
      {
         return GetTable(tableImport, glyph, glyph);
      }

      //
      // Program::getObject
      //
      Object &Program::getObject(Core::String glyph)
      {
         return GetTable(tableObject, glyph, glyph);
      }

      //
      // Program::getSpace
      //
      Space &Program::getSpace(AddrSpace as)
      {
         switch(as.base)
         {
         case AddrBase::GblArr:
         case AddrBase::GblArs: return getSpaceGblArr(as.name);
         case AddrBase::GblReg: return getSpaceGblReg();
         case AddrBase::LocArr: return getSpaceLocArr(as.name);
         case AddrBase::MapArr:
         case AddrBase::MapArs: return getSpaceMapArr(as.name);
         case AddrBase::MapReg: return getSpaceMapReg();
         case AddrBase::Sta:    return getSpaceSta();
         case AddrBase::WldArr:
         case AddrBase::WldArs: return getSpaceWldArr(as.name);
         case AddrBase::WldReg: return getSpaceWldReg();

         default:
            std::cerr << "GDCC::IR::Program::getSpace: bad AddrBase\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // Program::getSpaceGblArr
      //
      Space &Program::getSpaceGblArr(Core::String glyph)
      {
         return GetTable(tableSpaceGblArs, glyph, AddrSpace(AddrBase::GblArr, glyph));
      }

      //
      // Program::getSpaceGblReg
      //
      Space &Program::getSpaceGblReg()
      {
         return spaceGblReg;
      }

      //
      // Program::getSpaceLocArr
      //
      Space &Program::getSpaceLocArr(Core::String glyph)
      {
         return GetTable(tableSpaceLocArs, glyph, AddrSpace(AddrBase::LocArr, glyph));
      }

      //
      // Program::getSpaceMapArr
      //
      Space &Program::getSpaceMapArr(Core::String glyph)
      {
         return GetTable(tableSpaceMapArs, glyph, AddrSpace(AddrBase::MapArr, glyph));
      }

      //
      // Program::getSpaceMapReg
      //
      Space &Program::getSpaceMapReg()
      {
         return spaceMapReg;
      }

      //
      // Program::getSpaceSta
      //
      Space &Program::getSpaceSta()
      {
         return spaceSta;
      }

      //
      // Program::getSpaceWldArr
      //
      Space &Program::getSpaceWldArr(Core::String glyph)
      {
         return GetTable(tableSpaceWldArs, glyph, AddrSpace(AddrBase::WldArr, glyph));
      }

      //
      // Program::getSpaceWldReg
      //
      Space &Program::getSpaceWldReg()
      {
         return spaceWldReg;
      }

      //
      // Program::getStrEnt
      //
      StrEnt &Program::getStrEnt(Core::String glyph)
      {
         return GetTable(tableStrEnt, glyph, glyph);
      }

      //
      // Program::mergeFunction
      //
      void Program::mergeFunction(Function &out, Function &&in)
      {
         if(!out.defin)
            out = std::move(in);
      }

      //
      // Program::mergeGlyphData
      //
      void Program::mergeGlyphData(GlyphData &out, GlyphData &&in)
      {
         if(in.type.t != TypeBase::Empty)
         {
            if(out.type.t == TypeBase::Empty)
               out.type = in.type;
         }

         if(in.value)
         {
            if(!out.value)
               out.value = in.value;
         }
      }

      //
      // Program::mergeImport
      //
      void Program::mergeImport(Import &, Import &&)
      {
      }

      //
      // Program::mergeObject
      //
      void Program::mergeObject(Object &out, Object &&in)
      {
         if(!out.defin)
         {
            if(in.defin)
            {
               out.initi = in.initi;
               out.linka = in.linka;
               out.space = in.space;
               out.value = in.value;
               out.words = in.words;

               out.alias = in.alias;
               out.alloc = in.alloc;
               out.defin = in.defin;
            }
            else
            {
               if(out.space.base == AddrBase::Cpy) out.space = in.space;

               if(!out.initi) out.initi = in.initi;
               if(!out.value) out.value = in.value;
               if(!out.words) out.words = in.words;

               if(!out.alias) out.alias = in.alias;
               if(!out.alloc) out.alloc = in.alloc;

               if(out.linka == Linkage::None) out.linka = in.linka;
            }
         }
      }

      //
      // Program::mergeSpace
      //
      void Program::mergeSpace(Space &out, Space &&in)
      {
         if(!out.defin)
         {
            if(in.defin)
            {
               out.linka = in.linka;
               out.value = in.value;
               out.words = in.words;

               out.alloc = in.alloc;
               out.defin = in.defin;
            }
            else
            {
               if(!out.value) out.value = in.value;
               if(!out.words) out.words = in.words;

               if(!out.alloc) out.alloc = in.alloc;

               if(out.linka == Linkage::None) out.linka = in.linka;
            }
         }
      }

      //
      // Program::mergeStrEnt
      //
      void Program::mergeStrEnt(StrEnt &out, StrEnt &&in)
      {
         if(!out.defin)
            out = std::move(in);
      }

      //
      // Program::rangeFunction
      //
      auto Program::rangeFunction() -> TableRange<Function>
      {
         return RangeTable(tableFunction);
      }

      //
      // Program::rangeFunction
      //
      auto Program::rangeFunction() const -> TableCRange<Function>
      {
         return RangeTable(tableFunction);
      }

      //
      // Program::rangeGlyphData
      //
      auto Program::rangeGlyphData() -> TableRange<GlyphData>
      {
         return RangeTable(tableGlyphData);
      }

      //
      // Program::rangeGlyphData
      //
      auto Program::rangeGlyphData() const -> TableCRange<GlyphData>
      {
         return RangeTable(tableGlyphData);
      }

      //
      // Program::rangeImport
      //
      auto Program::rangeImport() -> TableRange<Import>
      {
         return RangeTable(tableImport);
      }

      //
      // Program::rangeImport
      //
      auto Program::rangeImport() const -> TableCRange<Import>
      {
         return RangeTable(tableImport);
      }

      //
      // Program::rangeObject
      //
      auto Program::rangeObject() -> TableRange<Object>
      {
         return RangeTable(tableObject);
      }

      //
      // Program::rangeObject
      //
      auto Program::rangeObject() const -> TableCRange<Object>
      {
         return RangeTable(tableObject);
      }

      //
      // Program::rangeSpaceGblArs
      //
      auto Program::rangeSpaceGblArs() -> TableRange<Space>
      {
         return RangeTable(tableSpaceGblArs);
      }

      //
      // Program::rangeSpaceGblArs
      //
      auto Program::rangeSpaceGblArs() const -> TableCRange<Space>
      {
         return RangeTable(tableSpaceGblArs);
      }

      //
      // Program::rangeSpaceLocArs
      //
      auto Program::rangeSpaceLocArs() -> TableRange<Space>
      {
         return RangeTable(tableSpaceLocArs);
      }

      //
      // Program::rangeSpaceLocArs
      //
      auto Program::rangeSpaceLocArs() const -> TableCRange<Space>
      {
         return RangeTable(tableSpaceLocArs);
      }

      //
      // Program::rangeSpaceMapArs
      //
      auto Program::rangeSpaceMapArs() -> TableRange<Space>
      {
         return RangeTable(tableSpaceMapArs);
      }

      //
      // Program::rangeSpaceMapArs
      //
      auto Program::rangeSpaceMapArs() const -> TableCRange<Space>
      {
         return RangeTable(tableSpaceMapArs);
      }

      //
      // Program::rangeSpaceWldArs
      //
      auto Program::rangeSpaceWldArs() -> TableRange<Space>
      {
         return RangeTable(tableSpaceWldArs);
      }

      //
      // Program::rangeSpaceWldArs
      //
      auto Program::rangeSpaceWldArs() const -> TableCRange<Space>
      {
         return RangeTable(tableSpaceWldArs);
      }

      //
      // Program::rangeStrEnt
      //
      auto Program::rangeStrEnt() -> TableRange<StrEnt>
      {
         return RangeTable(tableStrEnt);
      }

      //
      // Program::rangeStrEnt
      //
      auto Program::rangeStrEnt() const -> TableCRange<StrEnt>
      {
         return RangeTable(tableStrEnt);
      }

      //
      // Program::sizeFunction
      //
      std::size_t Program::sizeFunction() const
      {
         return tableFunction.size();
      }

      //
      // Program::sizeGlyphData
      //
      std::size_t Program::sizeGlyphData() const
      {
         return tableGlyphData.size();
      }

      //
      // Program::sizeImport
      //
      std::size_t Program::sizeImport() const
      {
         return tableImport.size();
      }

      //
      // Program::sizeObject
      //
      std::size_t Program::sizeObject() const
      {
         return tableObject.size();
      }

      //
      // Program::sizeSpaceGblArs
      //
      std::size_t Program::sizeSpaceGblArs() const
      {
         return tableSpaceGblArs.size();
      }

      //
      // Program::sizeSpaceLocArs
      //
      std::size_t Program::sizeSpaceLocArs() const
      {
         return tableSpaceLocArs.size();
      }

      //
      // Program::sizeSpaceMapArs
      //
      std::size_t Program::sizeSpaceMapArs() const
      {
         return tableSpaceMapArs.size();
      }

      //
      // Program::sizeSpaceWldArs
      //
      std::size_t Program::sizeSpaceWldArs() const
      {
         return tableSpaceWldArs.size();
      }

      //
      // Program::sizeStrEnt
      //
      std::size_t Program::sizeStrEnt() const
      {
         return tableStrEnt.size();
      }

      //
      // operator OArchive << Program
      //
      OArchive &operator << (OArchive &out, Program const &in)
      {
         out
            << in.tableFunction
            << in.tableGlyphData
            << in.tableImport
            << in.tableSpaceGblArs
            << in.tableSpaceMapArs
            << in.tableSpaceWldArs
            << in.tableStrEnt
            << in.tableObject
         ;

         return out;
      }

      //
      // operator IArchive >> Program
      //
      IArchive &operator >> (IArchive &in, Program &out)
      {
         in.prog = &out;

         // tableFunction
         for(auto count = GetIR<Program::Table<Function>::size_type>(in); count--;)
         {
            Core::String name;          in >> name;
            Function     newFunc{name}; in >> newFunc;

            out.mergeFunction(out.getFunction(name), std::move(newFunc));
         }

         // tableGlyphData
         for(auto count = GetIR<Program::Table<GlyphData>::size_type>(in); count--;)
         {
            Core::String name;          in >> name;
            GlyphData    newData{name}; in >> newData;

            out.mergeGlyphData(out.getGlyphData(name), std::move(newData));
         }

         // tableImport
         for(auto count = GetIR<Program::Table<Import>::size_type>(in); count--;)
         {
            Core::String name;         in >> name;
            Import       newImp{name}; in >> newImp;

            out.mergeImport(out.getImport(name), std::move(newImp));
         }

         // tableSpace*
         auto getSpace = [&](AddrBase base, Space &(Program::*getter)(Core::String))
         {
            for(auto count = GetIR<Program::Table<Space>::size_type>(in); count--;)
            {
               Core::String name;                            in >> name;
               Space        newSpace{AddrSpace(base, name)}; in >> newSpace;

               out.mergeSpace((out.*getter)(name), std::move(newSpace));
            }
         };

         getSpace(AddrBase::GblArr, &Program::getSpaceGblArr);
         getSpace(AddrBase::MapArr, &Program::getSpaceMapArr);
         getSpace(AddrBase::WldArr, &Program::getSpaceWldArr);

         // tableStrEnt
         for(auto count = GetIR<Program::Table<StrEnt>::size_type>(in); count--;)
         {
            Core::String name;         in >> name;
            StrEnt       newStr{name}; in >> newStr;

            out.mergeStrEnt(out.getStrEnt(name), std::move(newStr));
         }

         // tableObject
         for(auto count = GetIR<Program::Table<Object>::size_type>(in); count--;)
         {
            Core::String name;         in >> name;
            Object       newObj{name}; in >> newObj;

            out.mergeObject(out.getObject(name), std::move(newObj));
         }

         in.prog = nullptr;

         return in;
      }
   }
}

// EOF

