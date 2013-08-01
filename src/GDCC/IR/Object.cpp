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


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace IR
   {
      Space Space::LocArs{AddrSpace(AddrBase::LocArs, STRNULL)};
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
            Space *space;               *this >> space;
            String name;                *this >> name;
            Object newObj{name, space}; *this >> newObj;
            auto  &oldObj = Object::Get(name, space);

            if(oldObj.exdef)
            {
               if(!newObj.exdef)
                  oldObj = std::move(newObj);
            }
         }

         return *this;
      }

      //
      // IArchive::getTablesSpaces
      //
      IArchive &IArchive::getTablesSpaces()
      {
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

         exdef{true}
      {
      }

      //
      // Space::Find
      //
      Space *Space::Find(AddrSpace as)
      {
         switch(as.base)
         {
         case AddrBase::LocArs: return &LocArs;

         default: return nullptr;
         }
      }

      //
      // operator OArchive << Object
      //
      OArchive &operator << (OArchive &out, Object const &in)
      {
         return out << in.initi << in.linka << in.value << in.words << in.alias << in.exdef;
      }

      //
      // operator OArchive << Space *
      //
      OArchive &operator << (OArchive &out, Space const *in)
      {
         return in ? out << in->space << in->glyph : out << AddrBase::Cpy << STRNULL;
      }

      //
      // operator IArchive >> Object
      //
      IArchive &operator >> (IArchive &in, Object &out)
      {
         in >> out.initi >> out.linka >> out.value >> out.words;

         out.alias = GetIR<bool>(in);
         out.exdef = GetIR<bool>(in);

         return in;
      }

      //
      // operator IArchive >> Space *
      //
      IArchive &operator >> (IArchive &in, Space *&out)
      {
         AddrBase base; in >> base;
         String   name; in >> name;

         out = Space::Find(AddrSpace(base, name));
         return in;
      }
   }
}

// EOF

