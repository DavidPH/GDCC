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
         CStr(Program *program, Info const &optInfo);
         CStr(Program *program, Info const &optInfo, char const *s, bool copy);

         virtual ~CStr();

         char const *data() const {return str;}

         std::size_t size() const {return len;}

      protected:
         virtual std::size_t v_process(Args const &args);

      private:
         void reset();
         void reset(char const *s, bool copy);

         char const *str;
         std::size_t len;

         bool alloc;
      };
   }
}

#endif//GDCC__Option__CStr_H__

