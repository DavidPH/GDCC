//-----------------------------------------------------------------------------
//
// Copyright (C) 2022 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String-mapped integer option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__IntMap_H__
#define GDCC__Option__IntMap_H__

#include "../Option/Int.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Option
{
   //
   // IntMap
   //
   template<typename Str, typename Int>
   class IntMap : public Base
   {
   public:
      // constructor
      IntMap(Program *program, Info const &optInfo) :
         Base{program, optInfo} {}

      Int       &operator [] (Str const &s)       {return map[s];}
      Int const &operator [] (Str const &s) const {return map[s];}

      void erase(Str const &s) {map.erase(s);}

      Int *find(Str const &s)
         {auto i = map.find(s); return i != map.end() ? &i->second : nullptr;}
      Int const *find(Str const &s) const
         {auto i = map.find(s); return i != map.end() ? &i->second : nullptr;}

   protected:
      //
      // v_process
      //
      virtual std::size_t v_process(Args const &args)
      {
         if(args.optFalse)
         {
            if(args.argC < 1)
               Option::Exception::Error(args, "argument required");
         }
         else if(args.argC < 2)
            Option::Exception::Error(args, "2 arguments required");

         if(args.optFalse)
         {
            map.erase(args.argV[0]);
            return 1;
         }

         map[args.argV[0]] = Option::Int<Int>::ProcessInt(args, args.argV[1]);

         return 2;
      }

   private:
      std::unordered_map<Str, Int> map;
   };
}

#endif//GDCC__Option__IntMap_H__

