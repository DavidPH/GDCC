//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Option
   {
      /// Basic integer option handler.

      /// Processes options by parsing an integer from an argument.
      ///
      template<typename T>
      class Int : public Base
      {
      public:
         /// Insertion and initializer constructor.

         /// Initializes the pointer to option state and invokes Base's
         /// insertion constructor.
         ///
         /// @param program Program to insert this into.
         /// @param optInfo Basic option information.
         /// @param ptr Pointer to object to store option state in.
         ///
         Int(Program *program, Info const &optInfo, T *ptr) :
            Base{program, optInfo}, dptr{ptr}
         {
         }

      protected:
         /// Virtual implementation of option processing.

         /// If optFalse is set, sets the associated integer to 0 and consumes
         /// no arguments. Otherwise, consumes one argument to parse a decimal
         /// integer from.
         ///
         /// @param args Program arguments.
         ///
         /// @return Number of consumed arguments.
         ///
         /// @exception Option::Exception Thrown for lack of argument or if
         ///   argument is not a decimal integer.
         ///
         virtual std::size_t v_process(Args const &args)
         {
            if(args.optFalse) {*dptr = 0; return 0;}
            if(!args.argC) Exception::Error(args, "argument required");

            *dptr = 0;
            for(auto s = args.argV[0]; *s; ++s)
            {
               if(*s < '0' || *s > '9')
                  Exception::Error(args, "integer required");

               *dptr = *dptr * 10 + (*s - '0');
            }

            return 1;
         }

      private:
         T *const dptr;
      };
   }
}

#endif//GDCC__Option__Int_H__

