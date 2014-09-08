//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Option lists.
//
//-----------------------------------------------------------------------------

#include "Option/Program.hpp"

#include "Option/Base.hpp"
#include "Option/Exception.hpp"
#include "Option/StrUtil.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// OptCmp
//
// Defines the sorting for option lists.
//
static int OptCmp(GDCC::Option::Base *l, GDCC::Option::Base *r)
{
   // Group sorting.
   if(int res = GDCC::Option::StrCmp(l->info.group, r->info.group)) return res;

   // Long name sorting.
   if(int res = GDCC::Option::StrCmp(l->info.nameL, r->info.nameL)) return res;

   // Short name sorting.
   return l->info.nameS - r->info.nameS;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Option
   {
      //
      // Program destructor
      //
      Program::~Program()
      {
         // If there are any options still in the list, remove them and null
         // their pointers to this so they do not cause undefined behavior
         // during destruction.
         while(listHead)
            remove(listHead);

         delete[] hashChainV;
         delete[] shrtChainV;
      }

      //
      // Program::buildHashChain
      //
      void Program::buildHashChain()
      {
         if(!listHead) return;

         // Generate chains.
         hashChainV = new Base *[hashChainC = listSizeL + 1];
         for(auto i = hashChainV, e = i + hashChainC; i != e; ++i)
            *i = nullptr;

         // Populate chains.
         Base *itr = listHead;
         do if(itr->info.nameL) insertHash(itr);
         while((itr = itr->listNext) != listHead);
      }

      //
      // Program::buildShrtChain
      //
      void Program::buildShrtChain()
      {
         if(!listHead) return;

         // Generate chains.
         shrtChainV = new Base *[shrtChainC = listSizeS + 1];
         for(auto i = shrtChainV, e = i + shrtChainC; i != e; ++i)
            *i = nullptr;

         // Populate chains.
         Base *itr = listHead;
         do if(itr->info.nameS) insertShrt(itr);
         while((itr = itr->listNext) != listHead);
      }

      //
      // Program::find
      //
      Base *Program::find(Args const &args)
      {
         if(args.nameL) return find(args.nameL);
         if(args.nameS) return find(args.nameS);
         return nullptr;
      }

      //
      // Program::find
      //
      Base *Program::find(char const *nameL)
      {
         // Early fail, and allows us to make assumptions later.
         if(!listSizeL) return nullptr;

         // Calculate length here, since it's needed for both searches.
         auto len = std::strlen(nameL);

         // Try a hash lookup, if there are enough options to justify a table.
         if(listSize > 8)
         {
            if(!hashChainV) buildHashChain();

            auto hash = StrHash(nameL);

            // Walk the chain.
            auto chain = hashChainV[hash % hashChainC], opt = chain;
            if(chain) do
            {
               if(opt->hash == hash && opt->lenL == len &&
                  !std::memcmp(opt->info.nameL, nameL, len))
                  return opt;
            }
            while((opt = opt->hashNext) != chain);

            // If no exact match, try linear which can partial match.
         }

         // Linear search through all options.

         // Shorthand/partial match.
         Base    *sopt = nullptr;
         unsigned scnt = 0;

         Base *opt = listHead;
         do
         {
            // Must have a long name to match.
            if(!opt->info.nameL) continue;

            // Can't match fewer characters.
            if(opt->lenL < len) continue;

            // Exact match?
            if(opt->lenL == len)
            {
               if(!std::memcmp(opt->info.nameL, nameL, len))
                  return opt;
            }

            // Partial match?
            else
            {
               if(!std::memcmp(opt->info.nameL, nameL, len))
                  sopt = opt, ++scnt;
            }
         }
         while((opt = opt->listNext) != listHead);

         // If an unambiguous partial match, return that.
         if(scnt == 1) return sopt;

         // Otherwise, no match.
         return nullptr;
      }

      //
      // Program::find
      //
      Base *Program::find(char nameS)
      {
         // Early fail, and allows us to make assumptions later.
         if(!listSizeS) return nullptr;

         // Try a hash lookup, if there are enough options to justify a table.
         if(listSize > 8)
         {
            if(!shrtChainV) buildShrtChain();

            // Walk the chain.
            auto chain = getShrtChain(nameS), opt = chain;
            if(chain) do
            {
               if(opt->info.nameS == nameS)
                  return opt;
            }
            while((opt = opt->shrtNext) != chain);
         }

         // Linear search.
         else
         {
            Base *opt = listHead;
            do if(opt->info.nameS == nameS) return opt;
            while((opt = opt->listNext) != listHead);
         }

         return nullptr;
      }

      //
      // Program::get
      //
      Base &Program::get(Args const &args)
      {
         if(auto opt = find(args)) return *opt;
         Exception::Error(args, "unrecognized option");
      }

      //
      // Program::insert
      //
      void Program::insert(Base *opt)
      {
         opt->prog = this;

                              ++listSize;                 insertList(opt);
         if(opt->info.nameL) {++listSizeL; if(hashChainV) insertHash(opt);}
         if(opt->info.nameS) {++listSizeS; if(shrtChainV) insertShrt(opt);}
      }

      //
      // Program::insertHash
      //
      void Program::insertHash(Base *opt)
      {
         auto &chain = hashChainV[opt->hash % hashChainC];

         // First in chain?
         if(!chain)
         {
            chain = opt;
            opt->hashNext = opt->hashPrev = opt;
         }
         else
         {
            (opt->hashPrev = chain->hashPrev)->hashNext = opt;
            (opt->hashNext = chain          )->hashPrev = opt;
         }
      }

      //
      // Program::insertList
      //
      void Program::insertList(Base *opt)
      {
         // First option?
         if(!listHead)
         {
            listHead = opt;
            opt->listNext = opt->listPrev = opt;
         }
         else
         {
            // Find option that sorts after opt.
            // Would it be worthwhile to make this a binary search?
            Base *itr = listHead;
            do if(OptCmp(opt, itr) < 0) break;
            while((itr = itr->listNext) != listHead);

            (opt->listPrev = itr->listPrev)->listNext = opt;
            (opt->listNext = itr          )->listPrev = opt;
         }
      }

      //
      // Program::insertShrt
      //
      void Program::insertShrt(Base *opt)
      {
         auto &chain = getShrtChain(opt->info.nameS);

         // First in chain?
         if(!chain)
         {
            chain = opt;
            opt->shrtNext = opt->shrtPrev = opt;
         }
         else
         {
            (opt->shrtPrev = chain->shrtPrev)->shrtNext = opt;
            (opt->shrtNext = chain          )->shrtPrev = opt;
         }
      }

      //
      // Program::remove
      //
      void Program::remove(Base *opt)
      {
                              --listSize;                 removeList(opt);
         if(opt->info.nameL) {--listSizeL; if(hashChainV) removeHash(opt);}
         if(opt->info.nameS) {--listSizeS; if(shrtChainV) removeShrt(opt);}

         opt->prog = nullptr;
      }

      //
      // Program::removeHash
      //
      void Program::removeHash(Base *opt)
      {
         // Last in chain?
         if(opt->hashNext == opt)
         {
            auto &chain = hashChainV[opt->hash % hashChainC];

            chain = nullptr;
            opt->hashNext = opt->hashPrev = opt;
         }
         else
         {
            opt->hashNext->hashPrev = opt->hashPrev;
            opt->hashPrev->hashNext = opt->hashNext;

            opt->hashNext = opt->hashPrev = opt;
         }
      }

      //
      // Program::removeList
      //
      void Program::removeList(Base *opt)
      {
         // Last option?
         if(opt->listNext == opt)
         {
            listHead = nullptr;
            opt->listNext = opt->listPrev = opt;
         }
         else
         {
            opt->listNext->listPrev = opt->listPrev;
            opt->listPrev->listNext = opt->listNext;

            opt->listNext = opt->listPrev = opt;
         }
      }

      //
      // Program::removeShrt
      //
      void Program::removeShrt(Base *opt)
      {
         // Last in chain?
         if(opt->hashNext == opt)
         {
            auto &chain = getShrtChain(opt->info.nameS);

            chain = nullptr;
            opt->shrtNext = opt->shrtPrev = opt;
         }
         else
         {
            opt->shrtNext->shrtPrev = opt->shrtPrev;
            opt->shrtPrev->shrtNext = opt->shrtNext;

            opt->shrtNext = opt->shrtPrev = opt;
         }
      }
   }
}

// EOF

