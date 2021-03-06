//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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

#include "../CC/Types.hpp"

#include "../Core/Array.hpp"
#include "../Core/Token.hpp"

#include "../IR/Exp.hpp"

#include "../SR/Exp.hpp"
#include "../SR/Type.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
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

      SR::Exp::CPtr        valueExp;
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

      void genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst,
         bool skipZero) const;

      IR::Exp::CRef getIRExp() const;

      bool isEffect() const;

      bool isIRExp() const;

      bool isNoAuto() const;

      void parse(InitRaw const &raw, Parser &ctx, Scope &scope);
      std::size_t parse(InitRaw const &raw, std::size_t rawIdx,
         Parser &ctx, Scope &scope);

      Core::Origin      pos;
      SR::Exp::CPtr     value;
      SR::Type::CRef    type;
      Core::FastU const offset;


      // Creates an initializer hierarchy for a given type.
      static Ptr Create(SR::Type const *type, Core::FastU offset,
         Core::Origin pos, Factory &fact);

      static Ptr Create(InitRaw const &raw, Parser &ctx, Scope &scope,
         SR::Type const *type, Factory &fact);

      static bool IsInitString(Core::Token const &tok, SR::Type const *type);

   protected:
      Init(SR::Type const *type_, Core::FastU offset_, Core::Origin pos_,
         Factory &fact_) :
         pos{pos_}, type{type_}, offset{offset_}, fact{fact_}, parsed{false} {}

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx,
         SR::Arg const &dst, bool skipZero) const = 0;

      virtual IR::Exp::CRef v_getIRExp() const;

      virtual bool v_isEffect() const;

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const = 0;

      virtual void v_parseBlock(InitRaw const &raw, Parser &ctx, Scope &scope) = 0;

      virtual std::size_t v_parseOpen(InitRaw const &raw, std::size_t rawIdx,
         Parser &ctx, Scope &scope) = 0;

      virtual void v_parseSingle(InitRaw const &raw, Parser &ctx, Scope &scope);

      virtual void v_parseString(Core::Token const &tok);

      Factory &fact;

      bool parsed : 1;
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
      bool         anon : 1;
   };

   //
   // Init_Aggregate
   //
   class Init_Aggregate : public Init
   {
   protected:
      Init_Aggregate(SR::Type const *type_, Core::FastU offset_,
         Core::Origin pos_, Factory &fact_) : Init{type_, offset_, pos_, fact_} {}

      virtual std::size_t findSub(Core::String name);

      virtual std::size_t firstSub() const;

      virtual Init *getSub(std::size_t index) = 0;

      virtual std::size_t nextSub(std::size_t index) const;

      virtual void v_parseBlock(InitRaw const &raw, Parser &ctx, Scope &scope);

      virtual std::size_t v_parseOpen(InitRaw const &raw, std::size_t rawIdx,
         Parser &ctx, Scope &scope);

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
      Init_Array(SR::Type const *type, Core::FastU offset,
         Core::Origin pos, std::size_t width, Factory &fact);

   protected:
      virtual Init *getSub(std::size_t index);

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx,
         SR::Arg const &dst, bool skipZero) const;

      virtual IR::Exp::CRef v_getIRExp() const;

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const;

      Core::Array<std::unique_ptr<Init>> subs;
   };

   //
   // Init_Array0
   //
   class Init_Array0 : public Init_Aggregate
   {
   public:
      Init_Array0(SR::Type const *type, Core::FastU offset,
         Core::Origin pos, Factory &fact);

   protected:
      virtual Init *getSub(std::size_t index);

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx,
         SR::Arg const &dst, bool skipZero) const;

      virtual IR::Exp::CRef v_getIRExp() const;

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const;

      std::vector<std::unique_ptr<Init>> subs;

      SR::Type::CRef const subT;
      Core::FastU    const subB;
   };

   //
   // Init_Div
   //
   class Init_Div : public Init_Aggregate
   {
   public:
      Init_Div(Type_Div const *type, Core::FastU offset, Core::Origin pos,
         Factory &fact);

   protected:
      virtual std::size_t findSub(Core::String name);

      virtual Init *getSub(std::size_t index);

      virtual std::size_t nextSub(std::size_t index) const;

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx,
         SR::Arg const &dst, bool skipZero) const;

      virtual IR::Exp::CRef v_getIRExp() const;

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const;

      std::unique_ptr<Init> subs[2];
   };

   //
   // Init_Struct
   //
   class Init_Struct : public Init_Aggregate
   {
   public:
      Init_Struct(Type_Struct const *type, Core::FastU offset,
         Core::Origin pos, Factory &fact);

   protected:
      virtual std::size_t findSub(Core::String name);

      InitMem const *getMem(std::size_t index) const;

      virtual Init *getSub(std::size_t index);

      virtual std::size_t nextSub(std::size_t index) const;

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx,
         SR::Arg const &dst, bool skipZero) const;

      virtual IR::Exp::CRef v_getIRExp() const;

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const;

      Core::Array<InitMem> subs;
      std::size_t          subInit;
      std::size_t          subTotal;
   };

   //
   // Init_Union
   //
   class Init_Union : public Init_Struct
   {
   public:
      Init_Union(Type_Struct const *type, Core::FastU offset,
         Core::Origin pos, Factory &fact);

   protected:
      virtual void v_genStmnt(SR::GenStmntCtx const &ctx,
         SR::Arg const &dst, bool skipZero) const;

      virtual IR::Exp::CRef v_getIRExp() const;

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const;
   };

   //
   // Init_Value
   //
   class Init_Value : public Init
   {
   public:
      Init_Value(SR::Type const *type, Core::FastU offset, Core::Origin pos,
         Factory &fact);

   protected:
      virtual void v_genStmnt(SR::GenStmntCtx const &ctx,
         SR::Arg const &dst, bool skipZero) const;

      virtual void v_parseBlock(InitRaw const &raw, Parser &ctx, Scope &scope);

      virtual std::size_t v_parseOpen(InitRaw const &raw, std::size_t rawIdx,
         Parser &ctx, Scope &scope);

      virtual bool v_isNoAuto() const;
   };
}

#endif//GDCC__CC__Init_H__

