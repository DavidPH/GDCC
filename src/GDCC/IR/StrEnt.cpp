//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation string table entry handling.
//
//-----------------------------------------------------------------------------

#include "StrEnt.hpp"

#include "Glyph.hpp"
#include "IArchive.hpp"
#include "OArchive.hpp"


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static std::unordered_map<GDCC::String, GDCC::IR::StrEnt> Table;


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // IArchive::getTablesStrEnt
      //
      IArchive &IArchive::getTablesStrEnt()
      {
         for(auto count = GetIR<std::size_t>(*this); count--;)
         {
            String name;         *this >> name;
            StrEnt newStr{name}; *this >> newStr;
            auto  &oldStr = StrEnt::Get(name);

            if(!oldStr.defin)
            {
               if(newStr.defin)
                  oldStr = std::move(newStr);
            }
         }

         return *this;
      }

      //
      // OArchive::putTablesStrEnt
      //
      OArchive &OArchive::putTablesStrEnt()
      {
         *this << Table.size();
         for(auto const &itr : Table)
            *this << itr.first << itr.second;

         return *this;
      }

      //
      // StrEnt constructor
      //
      StrEnt::StrEnt(String glyph_) :
         glyph{glyph_},
         valueInt{0},
         valueStr{STRNULL},

         alias{false},
         alloc{false},
         defin{false}
      {
         Glyph::GetData(glyph).type = Type_StrEn();
      }

      //
      // StrEnt::allocValue
      //
      void StrEnt::allocValue()
      {
         // First, check for any strings to alias with.
         if(alias) for(auto const &itr : Table)
         {
            auto const &str = itr.second;

            if(&str == this || str.alloc || !str.alias) continue;
            if(str.valueStr != valueStr) continue;
            if(str.valueInt <  valueInt) continue;

            alloc    = false;
            valueInt = str.valueInt;
            return;
         }

         // Otherwise, run allocation.
         for(;; ++valueInt)
         {
            for(auto const &itr : Table)
            {
               auto const &str = itr.second;
               if(&str != this && !str.alloc && str.valueInt == valueInt)
                  goto nextValue;
            }

            break;

         nextValue:;
         }

         alloc = false;
      }

      //
      // StrEnt::FindValue
      //
      StrEnt *StrEnt::FindValue(String value)
      {
         for(auto &itr : Table)
            if(itr.second.valueStr == value && itr.second.alias && itr.second.defin)
               return &itr.second;

         return nullptr;
      }

      //
      // StrEnt::Get
      //
      StrEnt &StrEnt::Get(String glyph)
      {
         auto itr = Table.find(glyph);

         if(itr == Table.end())
            itr = Table.emplace(std::piecewise_construct,
               std::forward_as_tuple(glyph), std::forward_as_tuple(glyph)).first;

         return itr->second;
      }

      //
      // operator OArchive << StrEnt
      //
      OArchive &operator << (OArchive &out, StrEnt const &in)
      {
         return out << in.valueInt << in.valueStr << in.alias << in.alloc << in.defin;
      }

      //
      // Operator IArchive >> StrEnt
      //
      IArchive &operator >> (IArchive &in, StrEnt &out)
      {
         in >> out.valueInt >> out.valueStr;

         out.alias = GetIR<bool>(in);
         out.alloc = GetIR<bool>(in);
         out.defin = GetIR<bool>(in);

         return in;
      }

      //
      // StrEntRange
      //
      GDCC::Range<std::unordered_map<String, StrEnt>::iterator> StrEntRange()
      {
         return MakeRange(Table.begin(), Table.end());
      }
   }
}

// EOF

