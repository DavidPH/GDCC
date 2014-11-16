//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Pragma parsing and handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__Pragma_H__
#define GDCC__CPP__Pragma_H__

#include "../Core/String.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      class TokenStream;
   }

   namespace CPP
   {
      //
      // Pragma
      //
      // Allows classes to accept a PragmaVA without being a template.
      //
      class Pragma
      {
      public:
         // Processes a pragma directive.
         virtual bool pragma(Core::TokenStream &in) = 0;

         // Drops a scope layer.
         virtual void pragmaDrop() = 0;

         // Adds a scope layer.
         virtual void pragmaPush() = 0;
      };

      //
      // PragmaVA
      //
      template<typename... Pragmas>
      class PragmaVA final : public Pragma, public Pragmas...
      {
      public:
         // pragma
         virtual bool pragma(Core::TokenStream &in)
            {return PI<Pragmas...>::Pragma(this, in);}

         // pragmaDrop
         virtual void pragmaDrop()
            {PI<Pragmas...>::PragmaDrop(this);}

         // pragmaPush
         virtual void pragmaPush()
            {PI<Pragmas...>::PragmaPush(this);}

      private:
         // Variadic template "iteration" to implement pragma functions.
         template<typename P0, typename... PV> struct PI
         {
            static bool Pragma(PragmaVA<Pragmas...> *p, Core::TokenStream &in)
               {return p->P0::pragma(in) || PI<PV...>::Pragma(p, in);}

            static void PragmaDrop(PragmaVA<Pragmas...> *p)
               {p->P0::pragmaDrop(); PI<PV...>::PragmaDrop(p);}

            static void PragmaPush(PragmaVA<Pragmas...> *p)
               {p->P0::pragmaPush(); PI<PV...>::PragmaPush(p);}
         };

         template<typename P0> struct PI<P0>
         {
            static bool Pragma(PragmaVA<Pragmas...> *p, Core::TokenStream &in)
               {return p->P0::pragma(in);}

            static void PragmaDrop(PragmaVA<Pragmas...> *p)
               {p->P0::pragmaDrop();}

            static void PragmaPush(PragmaVA<Pragmas...> *p)
               {p->P0::pragmaPush();}
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
         bool pragma(Core::TokenStream &in);

         void pragmaDrop() {}
         void pragmaPush() {}

         std::vector<Core::String> pragmaACS_library;
      };

      //
      // PragmaGDCC
      //
      class PragmaGDCC
      {
      public:
         PragmaGDCC() :
            pragmaGDCC_FixedLiteral{false},
            pragmaGDCC_StrEntLiteral{false}
         {
         }

         bool pragma(Core::TokenStream &in);

         void pragmaDrop();
         void pragmaPush();

         bool pragmaGDCC_FixedLiteral;
         bool pragmaGDCC_StrEntLiteral;

      private:
         std::vector<bool> pragmaGDCC_FixedLiteral_Stack;
         std::vector<bool> pragmaGDCC_StrEntLiteral_Stack;
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

         bool pragma(Core::TokenStream &in);

         void pragmaDrop();
         void pragmaPush();

         bool pragmaSTDC_CXLimitedRange;
         bool pragmaSTDC_FEnvAccess;
         bool pragmaSTDC_FPContract;

      private:
         std::vector<bool> pragmaSTDC_CXLimitedRange_Stack;
         std::vector<bool> pragmaSTDC_FEnvAccess_Stack;
         std::vector<bool> pragmaSTDC_FPContract_Stack;
      };

      //
      // PragmaTest
      //
      // Handles pragmas used for testing the compiler.
      //
      class PragmaTest
      {
      public:
         bool pragma(Core::TokenStream &in);

         void pragmaDrop() {}
         void pragmaPush() {}
      };

      using PragmaLangC = PragmaVA<
         PragmaACS,
         PragmaGDCC,
         PragmaSTDC,
         PragmaTest>;
   }
}

#endif//GDCC__CPP__Pragma_H__

