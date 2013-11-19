//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Pragma parsing and handling.
//
//-----------------------------------------------------------------------------

#ifndef C__Pragma_H__
#define C__Pragma_H__

#include "GDCC/String.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   class TokenStream;
}

namespace C
{
   //
   // Pragma
   //
   // Allows classes to accept a PragmaVA without being a template.
   //
   class Pragma
   {
   public:
      virtual bool pragma(GDCC::TokenStream &in) = 0;
   };

   //
   // PragmaVA
   //
   template<typename... Pragmas>
   class PragmaVA final : public Pragma, Pragmas...
   {
   public:
      //
      // pragma
      //
      virtual bool pragma(GDCC::TokenStream &in)
      {
         return pragmaVA<Pragmas...>::Pragma(this, in);
      }

   private:
      // Variadic template "iteration" to implement pragma function.
      template<typename Pragma0, typename... PragmaV> struct pragmaVA
      {
         static bool Pragma(PragmaVA<Pragmas...> *prag, GDCC::TokenStream &in)
            {return prag->Pragma0::pragma(in) || pragmaVA<PragmaV...>::Pragma(prag, in);}
      };
      template<typename Pragma0> struct pragmaVA<Pragma0>
      {
         static bool Pragma(PragmaVA<Pragmas...> *prag, GDCC::TokenStream &in)
            {return prag->Pragma0::pragma(in);}
      };
   };

   //
   // PragmaACS
   //
   // Handles #pragma ACS ...
   //
   class PragmaACS
   {
   public:
      bool pragma(GDCC::TokenStream &in);

      std::vector<GDCC::String> pragmaACS_library;
   };

   //
   // PragmaSTDC
   //
   // Handles #pragma STDC ...
   //
   class PragmaSTDC
   {
   public:
      PragmaSTDC() :
         pragmaSTDC_CXLimitedRange{false},
         pragmaSTDC_FEnvAccess{false},
         pragmaSTDC_FPContract{false}
         {}

      bool pragma(GDCC::TokenStream &in);

      bool pragmaSTDC_CXLimitedRange;
      bool pragmaSTDC_FEnvAccess;
      bool pragmaSTDC_FPContract;
   };

   //
   // PragmaTest
   //
   // Handles pragmas used for testing the compiler.
   //
   class PragmaTest
   {
   public:
      bool pragma(GDCC::TokenStream &in);
   };

   using PragmaLangC = PragmaVA<PragmaACS, PragmaSTDC, PragmaTest>;
}

#endif//C__Pragma_H__

