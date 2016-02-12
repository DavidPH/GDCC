//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Wad handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AR__Wad__Wad_H__
#define GDCC__AR__Wad__Wad_H__

#include "../../AR/Wad/Lump.hpp"

#include "../../Core/List.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AR
   {
      namespace Wad
      {
         class LumpInfo;

         //
         // Wad
         //
         class Wad
         {
         private:
            using ListUtil = Core::ListUtil<Lump, &Lump::wadPrev, &Lump::wadNext>;

         public:
            using value_type       = Lump;
            using const_value_type = value_type const;
            using const_iterator   = ListUtil::CItr;
            using iterator         = ListUtil::Itr;


            Wad();
            ~Wad();

            void addLump(Lump *lump);
            void addLump(LumpInfo const &info);

                  iterator begin()       {return static_cast<      iterator>(head.wadNext);}
            const_iterator begin() const {return static_cast<const_iterator>(head.wadNext);}

            bool empty() const {return head.wadNext == &head;}

                  iterator end()       {return static_cast<      iterator>(&head);}
            const_iterator end() const {return static_cast<const_iterator>(&head);}

            std::size_t size() const;

            std::size_t sizeData() const;

            void writeData(std::ostream &out) const;

            bool iwad;

         private:
            Lump_Empty head;
         };
      }
   }
}

#endif//GDCC__AR__Wad__Wad_H__

