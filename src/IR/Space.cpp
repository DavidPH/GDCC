//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation address space handling.
//
//-----------------------------------------------------------------------------

#include "IR/Space.hpp"

#include "IR/IArchive.hpp"
#include "IR/Linkage.hpp"
#include "IR/OArchive.hpp"
#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Space constructor
      //
      Space::Space(AddrSpace as) :
         glyph{as.name},
         linka{Linkage::None},
         space{as},
         value{0},
         words{0},

         alloc{false},
         defin{false}
      {
      }

      //
      // Space::allocValue
      //
      void Space::allocValue(Program &prog)
      {
         Program::TableRange<Space> range;

         switch(space.base)
         {
         case AddrBase::GblArs:
         case AddrBase::GblArr: range = prog.rangeSpaceGblArs(); break;
         case AddrBase::MapArs:
         case AddrBase::MapArr: range = prog.rangeSpaceMapArs(); break;
         case AddrBase::WldArs:
         case AddrBase::WldArr: range = prog.rangeSpaceWldArs(); break;

         default:
            return;
         }

         for(;; ++value)
         {
            for(auto const &itr : range)
            {
               if(!itr.alloc && &itr != this && itr.value == value)
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
      void Space::allocWords(Program &prog)
      {
         for(auto const &itr : prog.rangeObject())
         {
            if(itr.space != space) continue;
            auto w = itr.value + itr.words;
            if(words < w) words = w;
         }
      }

      //
      // operator OArchive << Space
      //
      OArchive &operator << (OArchive &out, Space const &in)
      {
         return out
            << in.linka
            << in.value
            << in.words
            << in.alloc
            << in.defin
         ;
      }

      //
      // operator IArchive >> Space
      //
      IArchive &operator >> (IArchive &in, Space &out)
      {
         in
            >> out.linka
            >> out.value
            >> out.words
         ;

         out.alloc = GetIR<bool>(in);
         out.defin = GetIR<bool>(in);

         return in;
      }
   }
}

// EOF

