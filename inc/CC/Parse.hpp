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
      class TypeQual;
   }

   namespace Core
   {
      template<typename T> class Array;
      template<typename T> class CounterRef;
      class Origin;
      class String;
      class Token;
      class TokenStream;
   }

   namespace CPP
   {
      class Pragma;
   }

   namespace IR
   {
      class Program;
      class Value;
   }

   namespace CC
   {
      class Scope;
      class Scope_Global;
      class Scope_Local;

      //
      // ParserCtx
      //
      class ParserCtx
      {
      public:
         ParserCtx(Core::TokenStream &in_, CPP::Pragma &prag_,
            IR::Program &prog_) :
            in(in_),
            prag(prag_),
            prog(prog_)
         {
         }

         ParserCtx(ParserCtx const &data, Core::TokenStream &in_) :
            in(in_),
            prag(data.prag),
            prog(data.prog)
         {
         }

         Core::TokenStream &in;
         CPP::Pragma       &prag;
         IR::Program       &prog;
      };

      //
      // TypeSpec
      //
      class TypeSpec
      {
      public:
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

            // GDCC types.
            BaseLabl,
            BaseStrE,
            BaseVaLi,
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
      Core::CounterRef<AST::Statement const> GetDecl(ParserCtx const &ctx,
         Scope_Global &scope);
      Core::CounterRef<AST::Statement const> GetDecl(ParserCtx const &ctx,
         Scope_Local &scope);

      Core::CounterRef<AST::Exp const> GetExp_CLit(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_CLit(ParserCtx const &ctx, Scope &scope,
         AST::Type const *type);

      Core::CounterRef<AST::Exp const> GetExp_Init(ParserCtx const &ctx, Scope &scope,
         AST::Type const *type);

      Core::CounterRef<AST::Exp const> GetExp_Prim(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Post(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Post(ParserCtx const &ctx, Scope &scope,
         AST::Exp const *exp);
      Core::CounterRef<AST::Exp const> GetExp_Unar(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Cast(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Mult(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Addi(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Shft(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Rela(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Equa(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_BAnd(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_BOrX(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_BOrI(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_LAnd(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_LOrI(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Cond(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Assi(ParserCtx const &ctx, Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp(ParserCtx const &ctx, Scope &scope);

      Core::CounterRef<AST::Statement const> GetStatement(ParserCtx const &ctx,
         Scope_Local &scope);

      Core::Array<IR::Value> GetStrU08(Core::String str);
      Core::Array<IR::Value> GetStrU16(Core::String str);
      Core::Array<IR::Value> GetStrU32(Core::String str);
      Core::Array<IR::Value> GetString(Core::String str);
      Core::Array<IR::Value> GetString(Core::Token const &tok);

      Core::CounterRef<AST::Type const> GetType(ParserCtx const &ctx, Scope &scope);

      bool IsAddrDecl(ParserCtx const &ctx, Scope &scope);

      bool IsAttrSpec(ParserCtx const &ctx, Scope &scope);

      bool IsDecl(ParserCtx const &ctx, Scope &scope);

      bool IsDeclSpec(ParserCtx const &ctx, Scope &scope);

      bool IsDeclarator(ParserCtx const &ctx, Scope &scope);

      bool IsExp_Cast(ParserCtx const &ctx, Scope &scope);

      bool IsSpecQual(ParserCtx const &ctx, Scope &scope);

      bool IsStaticAssert(ParserCtx const &ctx, Scope &scope);

      bool IsType(ParserCtx const &ctx, Scope &scope);

      bool IsTypeQual(ParserCtx const &ctx, Scope &scope);

      bool IsTypeSpec(ParserCtx const &ctx, Scope &scope);

      void ParseAddrDecl(ParserCtx const &ctx, Scope &scope);

      void ParseAttr(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr);
      void ParseAttrSpec(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr);
      void ParseAttrSpecList(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr);

      void ParseDeclSpec(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr);

      void ParseDeclarator(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr);
      void ParseDeclaratorSuffix(ParserCtx const &ctx, Scope &scope,
         AST::Attribute &attr);

      void ParseSpecQual(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr);

      void ParseStaticAssert(ParserCtx const &ctx, Scope &scope);

      void ParseTypeQual(ParserCtx const &ctx, Scope &scope, AST::TypeQual &qual);

      void ParseTypeSpec(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr,
         TypeSpec &spec);
   }
}

#endif//GDCC__CC__Parse_H__

