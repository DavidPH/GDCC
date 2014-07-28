//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C initializers.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Init_H__
#define GDCC__CC__Init_H__

#include "../AST/Exp.hpp"
#include "../AST/Type.hpp"

#include "../Core/Array.hpp"
#include "../Core/Token.hpp"

#include "../IR/Exp.hpp"

#include <exception>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Arg;
      struct GenStmntCtx;
   }

   namespace Core
   {
      struct Token;
   }

   namespace CC
   {
      class ParserCtx;
      class Scope;
      class Type_Struct;

      //
      // InitRawDes
      //
      class InitRawDes
      {
      public:
         InitRawDes(Core::FastU i) : desigStr{Core::STRNULL}, desigInt{i} {}
         InitRawDes(Core::String s) : desigStr{s}, desigInt{0} {}

         Core::String desigStr;
         Core::FastU  desigInt;
      };

      //
      // InitRaw
      //
      class InitRaw
      {
      public:
         InitRaw(Core::Origin const &pos_) :
            valueTok{pos_, Core::STRNULL, Core::TOK_EOF} {}

         Core::Array<InitRawDes> desig;

         AST::Exp::CPtr       valueExp;
         Core::Token          valueTok;
         Core::Array<InitRaw> valueSub;
      };

      //
      // Init
      //
      class Init
      {
      public:
         using Ptr = std::unique_ptr<Init>;


         virtual ~Init() {}

         void genStmnt(AST::GenStmntCtx const &ctx, AST::Arg const &dst,
            bool skipZero) const;

         IR::Exp::CRef getIRExp() const;

         bool isEffect() const;

         bool isIRExp() const;

         void parse(InitRaw const &raw, ParserCtx const &ctx, Scope &scope);
         std::size_t parse(InitRaw const &raw, std::size_t rawIdx,
            ParserCtx const &ctx, Scope &scope);

         Core::Origin      pos;
         AST::Exp::CPtr    value;
         AST::Type::CRef   type;
         Core::FastU const offset;


         // Creates an initializer hierarchy for a given type.
         static Ptr Create(AST::Type const *type, Core::FastU offset, Core::Origin pos);

         static Ptr Create(InitRaw const &raw, ParserCtx const &ctx,
            Scope &scope, AST::Type const *type);

         static bool IsInitString(Core::Token const &tok, AST::Type const *type);

      protected:
         Init(AST::Type const *type_, Core::FastU offset_, Core::Origin pos_) :
            pos{pos_}, type{type_}, offset{offset_} {}

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst, bool skipZero) const = 0;

         virtual IR::Exp::CRef v_getIRExp() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;

         virtual void v_parseBlock(InitRaw const &raw, ParserCtx const &ctx,
           Scope &scope) = 0;

         virtual std::size_t v_parseOpen(InitRaw const &raw, std::size_t rawIdx,
            ParserCtx const &ctx, Scope &scope) = 0;

         virtual void v_parseSingle(InitRaw const &raw, ParserCtx const &ctx,
            Scope &scope);

         virtual void v_parseString(Core::Token const &tok);
      };

      //
      // InitMem
      //
      class InitMem
      {
      public:
         Init::Ptr    init;
         Core::String name;
         std::size_t  step;
      };

      //
      // Init_Aggregate
      //
      class Init_Aggregate : public Init
      {
      protected:
         Init_Aggregate(AST::Type const *type_, Core::FastU offset_,
            Core::Origin pos_) : Init{type_, offset_, pos_} {}

         virtual std::size_t findSub(Core::String name);

         virtual std::size_t firstSub() const;

         virtual Init *getSub(std::size_t index) = 0;

         virtual std::size_t nextSub(std::size_t index) const;

         virtual void v_parseBlock(InitRaw const &raw, ParserCtx const &ctx, Scope &scope);

         virtual std::size_t v_parseOpen(InitRaw const &raw, std::size_t rawIdx,
            ParserCtx const &ctx, Scope &scope);

         virtual void v_parseString(Core::Token const &tok);

      private:
         std::pair<std::size_t, Init *> getDes(
           Core::Array<InitRawDes> const &desig, std::size_t desigIdx = 0);
      };

      //
      // Init_Array
      //
      class Init_Array : public Init_Aggregate
      {
      public:
         Init_Array(AST::Type const *type, Core::FastU offset,
            Core::Origin pos, std::size_t width);

      protected:
         virtual Init *getSub(std::size_t index);

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst, bool skipZero) const;

         Core::Array<std::unique_ptr<Init>> subs;
      };

      //
      // Init_Array0
      //
      class Init_Array0 : public Init_Aggregate
      {
      public:
         Init_Array0(AST::Type const *type, Core::FastU offset,
            Core::Origin pos);

      protected:
         virtual Init *getSub(std::size_t index);

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst, bool skipZero) const;

         std::vector<std::unique_ptr<Init>> subs;

         AST::Type::CRef const subT;
         Core::FastU     const subB;
      };

      //
      // Init_Struct
      //
      class Init_Struct : public Init_Aggregate
      {
      public:
         Init_Struct(Type_Struct const *type, Core::FastU offset,
            Core::Origin pos);

      protected:
         virtual std::size_t findSub(Core::String name);

         virtual Init *getSub(std::size_t index);

         virtual std::size_t nextSub(std::size_t index) const;

         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst, bool skipZero) const;

         Core::Array<InitMem> subs;
         std::size_t          subInit;
      };

      //
      // Init_Value
      //
      class Init_Value : public Init
      {
      public:
         Init_Value(AST::Type const *type, Core::FastU offset, Core::Origin pos);

      protected:
         virtual void v_genStmnt(AST::GenStmntCtx const &ctx,
            AST::Arg const &dst, bool skipZero) const;

         virtual void v_parseBlock(InitRaw const &raw, ParserCtx const &ctx, Scope &scope);

         virtual std::size_t v_parseOpen(InitRaw const &raw, std::size_t rawIdx,
            ParserCtx const &ctx, Scope &scope);
      };
   }
}

#endif//GDCC__CC__Init_H__

