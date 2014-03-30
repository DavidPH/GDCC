//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String generator.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__StringGen_H__
#define GDCC__Core__StringGen_H__

#include "../Core/String.hpp"

#include <climits>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // StringGen
      //
      class StringGen
      {
      public:
         StringGen() {reset();}
         StringGen(StringGen const &gen);
         StringGen(StringGen &&gen);
         explicit StringGen(String str) {reset(str);}
         StringGen(String str, char const *suf) {reset(str, suf);}

         String operator () () {return (*this)(++number);}
         String operator () (std::size_t i);

         StringGen &operator = (StringGen const &gen);
         StringGen &operator = (StringGen &&gen) {swap(gen); return *this;}

         void reset();
         void reset(String str);
         void reset(String str, char const *suf);

         void swap(StringGen  &gen);
         void swap(StringGen &&gen) {swap(gen);}

      private:
         std::unique_ptr<char[]> buffer;
         char                   *suffix;
         std::size_t             baseHash;
         std::size_t             baseLen;
         std::size_t             number;


         static constexpr std::size_t NumLen =
            (sizeof(std::size_t) * CHAR_BIT + 2) / 3;
      };
   }
}

#endif//GDCC__Core__StringGen_H__

