//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Function-calling option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__Function_H__
#define GDCC__Option__Function_H__

#include "../Option/Base.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Option
   {
      /// Function-calling option handler.

      /// Processes options by deferring to a function.
      ///
      class Function : public Base
      {
      public:
         using Processor = std::size_t(*)(Base *opt, Args const &args);


         /// Insertion and initializer constructor.

         /// Associates a function for option handling and invokes Base's
         /// insertion constructor.
         ///
         /// @param program Program to insert this into.
         /// @param optInfo Basic option information.
         /// @param processor Function to call for option handling.
         ///
         Function(Program *program, Info const &optInfo, Processor processor) :
            Base{program, optInfo}, fn{processor}
         {
         }

      protected:
         /// Virtual implementation of option processing.

         /// Calls processor, passing this and args, and returns the result
         /// as-is.
         ///
         /// @param args Program arguments.
         ///
         /// @return Number of consumed arguments.
         ///
         virtual std::size_t v_process(Args const &args)
         {
            return fn(this, args);
         }

      private:
         Processor const fn;
      };
   }
}

#endif//GDCC__Option__Function_H__

