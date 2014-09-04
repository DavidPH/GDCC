//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

namespace GDCC
{
   namespace Option
   {
      /// Basic boolean option handler.

      /// Processes options by setting an associated boolean.
      ///
      class Bool : public Base
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
         Bool(Program *program, Info const &optInfo, bool *ptr) :
            Base{program, optInfo}, dptr{ptr}
         {
         }

      protected:
         /// Virtual implementation of option processing.

         /// If optFalse is set, sets the associated boolean to false.
         /// Otherwise, sets it to true. No arguments are consumed.
         ///
         /// @param args Program arguments.
         ///
         /// @return Number of consumed arguments.
         ///
         virtual std::size_t v_process(Args const &args)
         {
            *dptr = !args.optFalse;
            return 0;
         }

      private:
         bool *const dptr;
      };
   }
}

#endif//GDCC__Option__Bool_H__

