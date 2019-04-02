//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Boolean/flag option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__Bool_H__
#define GDCC__Option__Bool_H__

#include "../Option/Base.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Option
{
   //
   // Bool
   //
   class Bool : public Base
   {
   public:
      // constructor
      Bool(Program *program, Info const &optInfo, bool b_ = false) :
         Base{program, optInfo}, b{b_} {}

      operator bool () const {return b;}

      Bool &operator = (bool b_) {b = b_; return *this;}

      bool       &data()       {return b;}
      bool const &data() const {return b;}

   protected:
      //
      // v_process
      //
      virtual std::size_t v_process(Args const &args)
      {
         b = !args.optFalse;
         return 0;
      }

   private:
      bool b;
   };
}

#endif//GDCC__Option__Bool_H__

