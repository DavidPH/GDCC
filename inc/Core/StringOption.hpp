//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
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

namespace GDCC::Core
{
   //
   // StringOption
   //
   class StringOption : public Option::Base
   {
   public:
      StringOption(Option::Program *program, Info const &optInfo);
      StringOption(Option::Program *program, Info const &optInfo, String s);

      explicit operator bool () const {return static_cast<bool>(str);}
      operator String () const {return str;}

      StringOption &operator = (String s) {str = s; return *this;}

      String       &data()       {return str;}
      String const &data() const {return str;}

   protected:
      virtual std::size_t v_process(Option::Args const &args);

   private:
      String str;
   };
}

#endif//GDCC__Core__StringOption_H__

