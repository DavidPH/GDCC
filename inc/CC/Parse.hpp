//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
      class Exp;
      struct TypeQual;
   }

   namespace Core
   {
      template<typename T> class CounterRef;
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
      Core::CounterRef<AST::Exp const> GetExp_Prim(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Post(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Unar(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Mult(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Addi(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Shft(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Rela(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Equa(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_BAnd(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_BOrX(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_BOrI(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_LAnd(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_LOrI(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Cond(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Assi(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp(ParserData &in, Scope *ctx);

      AST::TypeQual GetTypeQual(ParserData &in, Scope *ctx);

      bool IsAttrSpec(ParserData &in, Scope *ctx);

      bool IsDeclSpec(ParserData &in, Scope *ctx);

      bool IsDeclarator(ParserData &in, Scope *ctx);

      bool IsTypeQual(ParserData &in, Scope *ctx);

      void ParseAttr(ParserData &in, Scope *ctx, AST::Attribute &attr);
      void ParseAttrSpec(ParserData &in, Scope *ctx, AST::Attribute &attr);

      void ParseDeclSpec(ParserData &in, Scope *ctx, AST::Attribute &attr);

      void ParseDeclarator(ParserData &in, Scope *ctx, AST::Attribute &attr);
      void ParseDeclaratorSuffix(ParserData &in, Scope *ctx,
         AST::Attribute &attr);
   }
}

#endif//GDCC__CC__Parse_H__

