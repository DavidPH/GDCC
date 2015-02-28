//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
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
      //
      // CStrV constructor
      //
      CStrV::CStrV(Program *program, Info const &optInfo, std::size_t argMax) :
         Base{program, optInfo},

         strF{nullptr},
         strV{nullptr},
         strA{0},
         strC{0},
         argM{argMax}
      {
      }

      //
      // CStr destructor
      //
      CStrV::~CStrV()
      {
         while(strC) if(strF[--strC]) delete[] strV[strC];

         delete[] strF;
         delete[] strV;
      }

      //
      // CStrV::pop
      //
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

      //
      // CStrV::push
      //
      void CStrV::push(char const *const *argV, std::size_t argC,
         bool optKeepA, bool optFinal)
      {
         std::size_t newC = strC + argC;

         // Possibly allocate more storage.
         if(newC > strA)
         {
            // If these arguments are final, don't overallocate.
            std::size_t newA = optFinal ? newC : strA * 2 + argC;

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
         if(optKeepA) for(std::size_t i = 0; i != argC; ++i, ++strC)
         {
            strV[strC] = argV[i];
            strF[strC] = false;
         }
         else for(std::size_t i = 0; i != argC; ++i, ++strC)
         {
            strV[strC] = StrDup(argV[i]).release();
            strF[strC] = true;
         }
      }

      //
      // CStrV::v_process
      //
      std::size_t CStrV::v_process(Args const &args)
      {
         if(argM && !args.argC)
            Exception::Error(args, "argument required");

         // Calculate actual number of arguments to take.
         std::size_t argC = args.argC;
         if(argM && argC > argM)
            argC = argM;

         push(args.argV, argC, args.optKeepA, args.optFinal);

         return argC;
      }
   }
}

// EOF

