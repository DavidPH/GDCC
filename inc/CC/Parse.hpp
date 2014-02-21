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
      class Statement;
      class Type;
      struct TypeQual;
   }

   namespace Core
   {
      template<typename T> class CounterRef;
      struct Origin;
      class TokenStream;
   }

   namespace CPP
   {
      class Pragma;
   }

   namespace CC
   {
      class GlobalScope;
      class LocalScope;
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

      //
      // TypeSpec
      //
      struct TypeSpec
      {
         //
         // SpecBase
         //
         enum SpecBase
         {
            BaseNone,
            BaseName,

            // C types.
            BaseBool,
            BaseChar,
            BaseDoub,
            BaseFloa,
            BaseInte,
            BaseVoid,

            // Embedded C types.
            BaseAccu,
            BaseFrac,

            // ACS types.
            BaseStri,
         };

         void finish(AST::Attribute &attr, AST::TypeQual qual,
            Core::Origin pos) const;

         SpecBase specBase = BaseNone;
         unsigned specCplx = 0;
         unsigned specImag = 0;
         unsigned specLong = 0;
         unsigned specSatu = 0;
         unsigned specShrt = 0;
         unsigned specSign = 0;
         unsigned specUnsi = 0;
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
      Core::CounterRef<AST::Statement const> GetDecl(ParserData &in,
         GlobalScope *ctx);
      Core::CounterRef<AST::Statement const> GetDecl(ParserData &in,
         LocalScope *ctx);

      Core::CounterRef<AST::Exp const> GetExp_CLit(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_CLit(ParserData &in, Scope *ctx,
         AST::Type const *type);

      Core::CounterRef<AST::Exp const> GetExp_Prim(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Post(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Post(ParserData &in, Scope *ctx,
         AST::Exp const *exp);
      Core::CounterRef<AST::Exp const> GetExp_Unar(ParserData &in, Scope *ctx);
      Core::CounterRef<AST::Exp const> GetExp_Cast(ParserData &in, Scope *ctx);
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

      Core::CounterRef<AST::Statement const> GetStatement(ParserData &in,
         LocalScope *ctx);

      Core::CounterRef<AST::Type const> GetType(ParserData &in, Scope *ctx);

      bool IsAttrSpec(ParserData &in, Scope *ctx);

      bool IsDecl(ParserData &in, Scope *ctx);

      bool IsDeclSpec(ParserData &in, Scope *ctx);

      bool IsDeclarator(ParserData &in, Scope *ctx);

      bool IsExp_Cast(ParserData &in, Scope *ctx);

      bool IsSpecQual(ParserData &in, Scope *ctx);

      bool IsStaticAssert(ParserData &in, Scope *ctx);

      bool IsType(ParserData &in, Scope *ctx);

      bool IsTypeQual(ParserData &in, Scope *ctx);

      bool IsTypeSpec(ParserData &in, Scope *ctx);

      void ParseAttr(ParserData &in, Scope *ctx, AST::Attribute &attr);
      void ParseAttrSpec(ParserData &in, Scope *ctx, AST::Attribute &attr);

      void ParseDeclSpec(ParserData &in, Scope *ctx, AST::Attribute &attr);

      void ParseDeclarator(ParserData &in, Scope *ctx, AST::Attribute &attr);
      void ParseDeclaratorSuffix(ParserData &in, Scope *ctx,
         AST::Attribute &attr);

      void ParseSpecQual(ParserData &in, Scope *ctx, AST::Attribute &attr);

      void ParseStaticAssert(ParserData &in, Scope *ctx);

      void ParseTypeQual(ParserData &in, Scope *ctx, AST::TypeQual &qual);

      void ParseTypeSpec(ParserData &in, Scope *ctx, AST::Attribute &attr,
         TypeSpec &spec);
   }
}

#endif//GDCC__CC__Parse_H__

