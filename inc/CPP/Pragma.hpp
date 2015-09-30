//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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
      class Token;
      class TokenStream;
   }

   namespace CPP
   {
      //
      // PragmaDataBase
      //
      class PragmaDataBase
      {
      public:
         virtual void drop();
         virtual void push();
      };

      //
      // PragmaData
      //
      class PragmaData : public PragmaDataBase
      {
      public:
         PragmaData();

         virtual void drop();
         virtual void push();

         std::vector<Core::String> stateLibrary;

         bool stateCXLimitedRange;
         bool stateFEnvAccess;
         bool stateFPContract;
         bool stateFixedLiteral;
         bool stateStrEntLiteral;

      protected:
         std::vector<bool> stackCXLimitedRange;
         std::vector<bool> stackFEnvAccess;
         std::vector<bool> stackFPContract;
         std::vector<bool> stackFixedLiteral;
         std::vector<bool> stackStrEntLiteral;
      };

      //
      // PragmaParserBase
      //
      class PragmaParserBase
      {
      public:
         virtual bool parse(Core::Token const *toks, std::size_t n) = 0;
      };

      //
      // PragmaParserVA
      //
      template<typename... Parsers>
      class PragmaParserVA final : public PragmaParserBase, public Parsers...
      {
      public:
         template<typename Data>
         explicit PragmaParserVA(Data &data_) : Parsers{data_}... {}

         virtual bool parse(Core::Token const *toks, std::size_t n)
            {return PI<Parsers...>::Pragma(this, toks, n);}

      private:
         // Variadic template "iteration" to implement parse function.
         template<typename P0, typename... PV> struct PI
         {
            static bool Pragma(PragmaParserVA<Parsers...> *p,
               Core::Token const *toks, std::size_t n)
               {return p->P0::parse(toks, n) || PI<PV...>::Pragma(p, toks, n);}
         };

         template<typename P0> struct PI<P0>
         {
            static bool Pragma(PragmaParserVA<Parsers...> *p,
               Core::Token const *toks, std::size_t n)
               {return p->P0::parse(toks, n);}
         };
      };

      //
      // PragmaParserACS
      //
      // Handles #pragma ACS ...
      //
      class PragmaParserACS
      {
      public:
         PragmaParserACS(PragmaData &data_) : data{data_} {}

         bool parse(Core::Token const *toks, std::size_t n);

      protected:
         PragmaData &data;
      };

      //
      // PragmaParserGDCC
      //
      // Handles #pragma GDCC ...
      //
      class PragmaParserGDCC
      {
      public:
         PragmaParserGDCC(PragmaData &data_) : data{data_} {}

         bool parse(Core::Token const *toks, std::size_t n);

      protected:
         PragmaData &data;
      };

      //
      // PragmaParserSTDC
      //
      // Handles #pragma STDC ...
      //
      class PragmaParserSTDC
      {
      public:
         PragmaParserSTDC(PragmaData &data_) : data{data_} {}

         bool parse(Core::Token const *toks, std::size_t n);

      protected:
         PragmaData &data;
      };

      using PragmaParser = PragmaParserVA<
         PragmaParserACS,
         PragmaParserGDCC,
         PragmaParserSTDC>;
   }
}

#endif//GDCC__CPP__Pragma_H__

