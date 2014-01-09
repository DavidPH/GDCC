//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C-string option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__CStr_H__
#define GDCC__Option__CStr_H__

#include "../Option/Base.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Option
   {
      //
      // CStr
      //
      class CStr : public Base
      {
      public:
         CStr(Program *list, Info const &info);
         CStr(Program *list, Info const &info, char const *str, bool copy);
         virtual ~CStr();

         void reset();
         void reset(char const *str, bool copy);

         char const *str;
         std::size_t len;

      protected:
         virtual std::size_t v_process(Args const &args);

      private:
         bool alloc;
      };
   }
}

#endif//GDCC__Option__CStr_H__

