//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Integer option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__Int_H__
#define GDCC__Option__Int_H__

#include "../Option/Base.hpp"

#include "../Option/Exception.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Option
{
   //
   // Int
   //
   template<typename T>
   class Int : public Base
   {
   public:
      // constructor
      Int(Program *program, Info const &optInfo) :
         Base{program, optInfo}, i{0} {}
      Int(Program *program, Info const &optInfo, T i_) :
         Base{program, optInfo}, i{i_} {}

      explicit operator bool () const {return static_cast<bool>(i);}
      operator T const & () const {return i;}

      Int &operator = (T const &i_) {i = i_; return *this;}

      T       &data()       {return i;}
      T const &data() const {return i;}

   protected:
      //
      // v_process
      //
      virtual std::size_t v_process(Args const &args)
      {
         if(args.optFalse) {i = 0; return 0;}
         if(!args.argC) Exception::Error(args, "argument required");

         i = 0;
         for(auto s = args.argV[0]; *s; ++s)
         {
            if(*s < '0' || *s > '9')
               Exception::Error(args, "integer required");

            i = i * 10 + (*s - '0');
         }

         return 1;
      }

   private:
      T i;
   };
}

#endif//GDCC__Option__Int_H__

