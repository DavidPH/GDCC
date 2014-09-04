//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C-string vector option handling.
//
//-----------------------------------------------------------------------------

#include "Option/CStrV.hpp"

#include "Option/Exception.hpp"
#include "Option/StrUtil.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Option
   {
      ///
      /// Initializes array to empty and invokes Base's insertion constructor.
      /// If argMax is 0, option processing will consume all passed arguments
      /// and silently do nothing if no arguments passed. Otherwise, it will
      /// require at least one argument and consume no more than argMax
      /// arguments.
      ///
      /// @param program Program to insert this into.
      /// @param optInfo Basic option information.
      /// @param argMax Maximum arguments to consume during processing.
      ///
      CStrV::CStrV(Program *program, Info const &optInfo, std::size_t argMax) :
         Base{program, optInfo},

         strF{nullptr},
         strV{nullptr},
         strA{0},
         strC{0},
         argM{argMax}
      {
      }

      ///
      /// Frees the array and any self-allocated strings.
      ///
      CStrV::~CStrV()
      {
         while(strC) if(strF[--strC]) delete[] strV[strC];

         delete[] strF;
         delete[] strV;
      }

      ///
      /// If opt is not null, its process function is called with the newly
      /// removed string as a program argument using args as a base.
      ///
      /// @param opt Option to handle popped string.
      /// @param args Base program argument data to pass to opt.
      ///
      void CStrV::pop(Base *opt, Args args)
      {
         if(!strC) return;

         // Pop string.
         --strC;

         // Pass to option, if any.
         if(opt)
         {
            args.optKeepA = !strF[strC];
            args.setArgs(strV + strC, 1);

            opt->process(args);
         }

         // Free string.
         if(strF[strC])
            delete[] strV[strC];
      }

      ///
      /// Consumes a number of arguments as described in the insertion
      /// constructor. If optKeepA is set, argument strings are kept instead of
      /// duplicated.
      ///
      /// @param args Program arguments.
      ///
      /// @return Number of consumed arguments.
      ///
      /// @exception Option::Exception Thrown for lack of argument.
      ///
      std::size_t CStrV::v_process(Args const &args)
      {
         if(argM && !args.argC)
            Exception::Error(args, "argument required");

         // Calculate actual number of argumetns to take.
         std::size_t argC = args.argC;
         if(argM && argC > argM)
            argC = argM;

         std::size_t newC = strC + argC;

         // Possibly allocate more storage.
         if(newC > strA)
         {
            // If these arguments are final, don't overallocate.
            std::size_t newA = args.optFinal ? newC : strA * 2 + argC;

            auto newF = new bool        [newA];
            auto newV = new char const *[newA];

            std::memcpy(newF, strF, sizeof(*strF) * strC);
            std::memcpy(newV, strV, sizeof(*strV) * strC);

            delete[] strF;
            delete[] strV;

            strA = newA;
            strF = newF;
            strV = newV;
         }

         // Take args.
         if(args.optKeepA) for(std::size_t i = 0; i != argC; ++i, ++strC)
         {
            strV[strC] = args.argV[i];
            strF[strC] = false;
         }
         else for(std::size_t i = 0; i != argC; ++i, ++strC)
         {
            strV[strC] = StrDup(args.argV[i]).release();
            strF[strC] = true;
         }

         return argC;
      }
   }
}

// EOF

