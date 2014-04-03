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

   namespace IR
   {
      class Program;
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

            // GDCC types.
            BaseLabl,
            BaseStrE,
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
      Core::CounterRef<AST::Statement const> GetDecl(ParserCtx const &in,
         Scope_Global &ctx);
      Core::CounterRef<AST::Statement const> GetDecl(ParserCtx const &in,
         Scope_Local &ctx);

      Core::CounterRef<AST::Exp const> GetExp_CLit(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_CLit(ParserCtx const &in, Scope &ctx,
         AST::Type const *type);

      Core::CounterRef<AST::Exp const> GetExp_Prim(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_Post(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_Post(ParserCtx const &in, Scope &ctx,
         AST::Exp const *exp);
      Core::CounterRef<AST::Exp const> GetExp_Unar(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_Cast(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_Mult(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_Addi(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_Shft(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_Rela(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_Equa(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_BAnd(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_BOrX(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_BOrI(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_LAnd(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_LOrI(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_Cond(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp_Assi(ParserCtx const &in, Scope &ctx);
      Core::CounterRef<AST::Exp const> GetExp(ParserCtx const &in, Scope &ctx);

      Core::CounterRef<AST::Statement const> GetStatement(ParserCtx const &in,
         Scope_Local &ctx);

      Core::CounterRef<AST::Type const> GetType(ParserCtx const &in, Scope &ctx);

      bool IsAttrSpec(ParserCtx const &in, Scope &ctx);

      bool IsDecl(ParserCtx const &in, Scope &ctx);

      bool IsDeclSpec(ParserCtx const &in, Scope &ctx);

      bool IsDeclarator(ParserCtx const &in, Scope &ctx);

      bool IsExp_Cast(ParserCtx const &in, Scope &ctx);

      bool IsSpecQual(ParserCtx const &in, Scope &ctx);

      bool IsStaticAssert(ParserCtx const &in, Scope &ctx);

      bool IsType(ParserCtx const &in, Scope &ctx);

      bool IsTypeQual(ParserCtx const &in, Scope &ctx);

      bool IsTypeSpec(ParserCtx const &in, Scope &ctx);

      void ParseAttr(ParserCtx const &in, Scope &ctx, AST::Attribute &attr);
      void ParseAttrSpec(ParserCtx const &in, Scope &ctx, AST::Attribute &attr);

      void ParseDeclSpec(ParserCtx const &in, Scope &ctx, AST::Attribute &attr);

      void ParseDeclarator(ParserCtx const &in, Scope &ctx, AST::Attribute &attr);
      void ParseDeclaratorSuffix(ParserCtx const &in, Scope &ctx,
         AST::Attribute &attr);

      void ParseSpecQual(ParserCtx const &in, Scope &ctx, AST::Attribute &attr);

      void ParseStaticAssert(ParserCtx const &in, Scope &ctx);

      void ParseTypeQual(ParserCtx const &in, Scope &ctx, AST::TypeQual &qual);

      void ParseTypeSpec(ParserCtx const &in, Scope &ctx, AST::Attribute &attr,
         TypeSpec &spec);
   }
}

#endif//GDCC__CC__Parse_H__

