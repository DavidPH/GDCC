//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Core::String option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__StringOption_H__
#define GDCC__Core__StringOption_H__

#include "../Core/String.hpp"

#include "../Option/Base.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // StringOption
      //
      class StringOption : public Option::Base
      {
      public:
         StringOption(Option::Program *program, Info const &optInfo, String *ptr);

      protected:
         virtual std::size_t v_process(Option::Args const &args);

      private:
         String *dptr;
      };
   }
}

#endif//GDCC__Core__StringOption_H__

