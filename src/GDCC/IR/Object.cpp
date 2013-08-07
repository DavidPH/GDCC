//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation object handling.
//
//-----------------------------------------------------------------------------

#include "Object.hpp"

#include "Exp.hpp"
#include "IArchive.hpp"
#include "Linkage.hpp"
#include "OArchive.hpp"

#include <iostream>
#include <unordered_map>


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace IR
   {
      Space Space::GblReg{AddrSpace(AddrBase::GblReg, STR_)};
      Space Space::LocArs{AddrSpace(AddrBase::LocArs, STR_)};
      Space Space::MapReg{AddrSpace(AddrBase::MapReg, STR_)};
      Space Space::WldReg{AddrSpace(AddrBase::WldReg, STR_)};

      Space::SpaceMap Space::GblArs;
      Space::SpaceMap Space::MapArs;
      Space::SpaceMap Space::WldArs;
   }
}

// This must destruct before the Spaces.
static std::unordered_map<GDCC::String, GDCC::IR::Object> ObjMap;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // IArchive::getTablesObject
      //
      IArchive &IArchive::getTablesObject()
      {
         for(auto count = GetIR<std::size_t>(*this); count--;)
         {
            Space *space;                 *this >> space;
            String name;                  *this >> name;
            Object newObj{name, nullptr}; *this >> newObj;
            auto  &oldObj = Object::Get(name, space);

            if(oldObj.exdef)
            {
               if(!newObj.exdef)
               {
                  oldObj.initi = newObj.initi;
                  oldObj.value = newObj.value;
                  oldObj.words = newObj.words;

                  oldObj.alias = newObj.alias;
                  oldObj.alloc = newObj.alloc;
                  oldObj.exdef = newObj.exdef;
               }
            }
         }

         return *this;
      }

      //
      // IArchive::getTablesSpaces
      //
      IArchive &IArchive::getTablesSpaces()
      {
         for(auto count = GetIR<std::size_t>(*this); count--;)
         {
            AddrSpace space;           *this >> space;
            Space     newSpace{space}; *this >> newSpace;
            Space    &oldSpace = Space::Get(space);

            if(oldSpace.exdef)
            {
               if(!newSpace.exdef)
               {
                  oldSpace.value = newSpace.value;
                  oldSpace.words = newSpace.words;

                  oldSpace.alloc = newSpace.alloc;
                  oldSpace.exdef = newSpace.exdef;
               }
            }
         }

         return *this;
      }

      //
      // OArchive::putTablesObject
      //
      OArchive &OArchive::putTablesObject()
      {
         *this << ObjMap.size();
         for(auto const &i : ObjMap)
            *this << i.second.space << i.first << i.second;

         return *this;
      }

      //
      // OArchive::putTablesSpaces
      //
      OArchive &OArchive::putTablesSpaces()
      {
         auto putTable = [&](Space::SpaceMap const &map)
         {
            for(auto const &i : map)
               *this << AddrSpace(i.second.space, i.second.glyph) << i.second;
         };

         *this << (Space::GblArs.size() + Space::MapArs.size() + Space::WldArs.size());

         putTable(Space::GblArs);
         putTable(Space::MapArs);
         putTable(Space::WldArs);

         return *this;
      }

      //
      // Object constructor
      //
      Object::Object(String name, Space *space_) :
         glyph{name},
         initi{},
         linka{Linkage::None},
         space{space_},
         value{0},
         words{0},

         alias{false},
         alloc{false},
         exdef{true}
      {
         if(space)
            space->obset.emplace(this);
      }

      //
      // Object destructor
      //
      Object::~Object()
      {
         if(space)
            space->obset.erase(this);
      }

      //
      // Object::allocValue
      //
      void Object::allocValue()
      {
         if(!space) return;

         for(;; ++value)
         {
            auto lo = value;
            auto hi = words + lo;

            for(auto const &obj : space->obset)
            {
               if(obj->alloc || obj == this)
                  continue;

               auto objLo = obj->value;
               auto objHi = obj->words + objLo;

               if((objLo <= lo && lo < objHi) || (objLo < hi && hi < objHi))
                  goto nextValue;

               if((lo <= objLo && objLo < hi) || (lo < objHi && objHi < hi))
                  goto nextValue;
            }

            break;

         nextValue:;
         }

         alloc = false;
      }

      //
      // Object::Get
      //
      Object &Object::Get(String name, Space *space)
      {
         auto itr = ObjMap.find(name);

         if(itr == ObjMap.end())
            itr = ObjMap.emplace(std::piecewise_construct,
               std::tie(name), std::tie(name, space)).first;

         return itr->second;
      }

      //
      // Space constructor
      //
      Space::Space(AddrSpace as) :
         glyph{as.name},
         linka{Linkage::None},
         obset{},
         space{as.base},
         value{0},
         words{0},

         alloc{false},
         exdef{true}
      {
         switch(space)
         {
         case AddrBase::GblArs:
            space = AddrBase::GblArr;
         case AddrBase::GblArr:
            spmap = &GblArs;
            break;

         case AddrBase::MapArs:
            space = AddrBase::MapArr;
         case AddrBase::MapArr:
            spmap = &MapArs;
            break;

         case AddrBase::WldArs:
            space = AddrBase::WldArr;
         case AddrBase::WldArr:
            spmap = &WldArs;
            break;

         default:
            spmap = nullptr;
            break;
         }
      }

      //
      // Space::allocValue
      //
      void Space::allocValue()
      {
         if(!spmap) return;

         for(;; ++value)
         {
            for(auto const &itr : *spmap)
            {
               auto const &sp = itr.second;
               if(!sp.alloc && &sp != this && sp.value == value)
                  goto nextValue;
            }

            break;

         nextValue:;
         }

         alloc = false;
      }

      //
      // Space::allocWords
      //
      void Space::allocWords()
      {
         for(auto const &obj : obset)
         {
            auto w = obj->value + obj->words;
            if(words < w) words = w;
         }
      }

      //
      // Space::Find
      //
      Space *Space::Find(AddrSpace as)
      {
         SpaceMap::iterator itr;

         switch(as.base)
         {
         case AddrBase::GblReg: return &GblReg;
         case AddrBase::LocArs: return &LocArs;
         case AddrBase::MapReg: return &MapReg;
         case AddrBase::WldReg: return &WldReg;

         case AddrBase::GblArr:
         case AddrBase::GblArs:
            itr = GblArs.find(as.name);
            return itr == GblArs.end() ? nullptr : &itr->second;

         case AddrBase::MapArr:
         case AddrBase::MapArs:
            itr = MapArs.find(as.name);
            return itr == MapArs.end() ? nullptr : &itr->second;

         case AddrBase::WldArr:
         case AddrBase::WldArs:
            itr = WldArs.find(as.name);
            return itr == WldArs.end() ? nullptr : &itr->second;

         default: return nullptr;
         }
      }

      //
      // Space::Get
      //
      Space &Space::Get(AddrSpace as)
      {
         SpaceMap::iterator itr;

         switch(as.base)
         {
         case AddrBase::GblReg: return GblReg;
         case AddrBase::LocArs: return LocArs;
         case AddrBase::MapReg: return MapReg;
         case AddrBase::WldReg: return WldReg;

         case AddrBase::GblArr:
         case AddrBase::GblArs:
            itr = GblArs.find(as.name);
            if(itr == GblArs.end())
               itr = GblArs.emplace(std::piecewise_construct,
                  std::tie(as.name), std::tie(as)).first;

            return itr->second;

         case AddrBase::MapArr:
         case AddrBase::MapArs:
            itr = MapArs.find(as.name);
            if(itr == MapArs.end())
               itr = MapArs.emplace(std::piecewise_construct,
                  std::tie(as.name), std::tie(as)).first;

            return itr->second;

         case AddrBase::WldArr:
         case AddrBase::WldArs:
            itr = WldArs.find(as.name);
            if(itr == WldArs.end())
               itr = WldArs.emplace(std::piecewise_construct,
                  std::tie(as.name), std::tie(as)).first;

            return itr->second;

         default:
            std::cerr << "bad AddrSpace for Space\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // operator OArchive << Object
      //
      OArchive &operator << (OArchive &out, Object const &in)
      {
         return out << in.initi << in.linka << in.value << in.words << in.alias
            << in.alloc << in.exdef;
      }

      //
      // operator OArchive << Space
      //
      OArchive &operator << (OArchive &out, Space const &in)
      {
         return out << in.linka << in.value << in.words << in.alloc << in.exdef;
      }

      //
      // operator OArchive << Space *
      //
      OArchive &operator << (OArchive &out, Space const *in)
      {
         if(in)
            return out << AddrSpace(in->space, in->glyph);
         else
            return out << AddrSpace(AddrBase::Cpy, STRNULL);
      }

      //
      // operator IArchive >> Object
      //
      IArchive &operator >> (IArchive &in, Object &out)
      {
         in >> out.initi >> out.linka >> out.value >> out.words;

         out.alias = GetIR<bool>(in);
         out.alloc = GetIR<bool>(in);
         out.exdef = GetIR<bool>(in);

         return in;
      }

      //
      // operator IArchive >> Space
      //
      IArchive &operator >> (IArchive &in, Space &out)
      {
         in >> out.linka >> out.value >> out.words;

         out.alloc = GetIR<bool>(in);
         out.exdef = GetIR<bool>(in);

         return in;
      }

      //
      // operator IArchive >> Space *
      //
      IArchive &operator >> (IArchive &in, Space *&out)
      {
         out = Space::Find(GetIR<AddrSpace>(in));
         return in;
      }
   }
}

// EOF

