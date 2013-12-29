//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C parsing.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Parse_H__
#define GDCC__CC__Parse_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Attribute;
      struct TypeQual;
   }

   namespace Core
   {
      class TokenStream;
   }

   namespace CPP
   {
      class Pragma;
   }

   namespace CC
   {
      class Scope;

      //
      // ParserData
      //
      class ParserData
      {
      public:
         ParserData(Core::TokenStream &in_, CPP::Pragma &prag_) :
            in(in_),
            prag(prag_)
         {
         }

         Core::TokenStream &in;
         CPP::Pragma       &prag;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      bool IsDeclSpec(ParserData &in, Scope *ctx);

      bool IsTypeQual(ParserData &in, Scope *ctx);

      void ParseDeclSpec(ParserData &in, Scope *ctx, AST::Attribute &attr);

      AST::TypeQual ParseTypeQual(ParserData &in, Scope *ctx);
   }
}

#endif//GDCC__CC__Parse_H__

