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
      /// Basic C-string option handler.

      /// Processes options by taking an argument string as-is.
      ///
      class CStr : public Base
      {
      public:
         /// Insertion constructor.
         CStr(Program *program, Info const &optInfo);

         /// Insertion and initializer constructor.
         CStr(Program *program, Info const &optInfo, char const *s, bool copy);

         /// Destructor.
         virtual ~CStr();

         /// Returns the current string.
         char const *data() const {return str;}

         /// Returns the length of the current string.
         std::size_t size() const {return len;}

      protected:
         /// Virtual implementation of option processing.
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

