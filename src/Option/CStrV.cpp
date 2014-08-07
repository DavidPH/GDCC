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
      //
      // CStrV constructor
      //
      CStrV::CStrV(Program *list_, Info const &info_, std::size_t argMax_) :
         Base{list_, info_},

         strC  {0},
         strV  {nullptr},
         strF  {nullptr},
         strA  {0},
         argMax{argMax_}
      {
      }

      //
      // CStrV destructor
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
      // CStrV::v_process
      //
      std::size_t CStrV::v_process(Args const &args)
      {
         if(argMax && !args.argC)
            Exception::Error(args, "argument required");

         // Calculate actual number of argumetns to take.
         std::size_t argC = args.argC;
         if(argMax && argC > argMax)
            argC = argMax;

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

