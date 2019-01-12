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

#include "CC/Init.hpp"

#include "CC/Exp.hpp"
#include "CC/Exp/Add.hpp"
#include "CC/Factory.hpp"
#include "CC/Parse.hpp"
#include "CC/Type.hpp"
#include "CC/Type/Struct.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Block.hpp"

#include "SR/Arg.hpp"
#include "SR/Type/Array.hpp"
#include "SR/Warning.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   static std::size_t SubCount(Type_Struct::MemberData const *itr,
      Type_Struct::MemberData const *end);

   //
   // SubCount
   //
   static std::size_t SubCount(Type_Struct const *type)
   {
      return SubCount(type->getData().memb.begin(), type->getData().memb.end());
   }

   //
   // SubCount
   //
   static std::size_t SubCount(Type_Struct::MemberData const *itr,
      Type_Struct::MemberData const *end)
   {
      std::size_t n = 0;

      for(; itr != end; ++itr)
      {
         if(itr->name || itr->anon)
            ++n;
      }

      return n;
   }

   //
   // SubInit
   //
   static void SubInit(Type_Struct const *type, InitMem *&sub,
      Core::FastU offset, Core::Origin pos, Factory &fact)
   {
      auto const &data = type->getData();

      for(auto itr = data.memb.begin(), end = data.memb.end(); itr != end; ++itr)
      {
         if(itr->name || itr->anon)
         {
            sub->name = itr->name;
            sub->init = Init::Create(itr->type, offset + itr->addr, pos, fact);
            sub->step = data.isUnion ? SubCount(itr, end) : 1;
            sub->anon = itr->anon;
            ++sub;
         }
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Init::genStmnt
   //
   void Init::genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst,
      bool skipZero) const
   {
      if(value)
      {
         SR::Arg tmp = dst;

         if(tmp.data)
         {
            // Convert dst's type to this's type and offset to this's offset.
            // This codegen assumes a lack of side effects in dst.data.

            auto offsetType = type->getTypeQual(tmp.type->getQual());
            auto offsetPtrT = offsetType->getTypePointer();

            auto offsetRaw = offset / type->getSizeShift();
            auto offsetExp = fact.expCreate_LitInt(SR::Type::Size,
               Core::NumberCast<Core::Integ>(offsetRaw), pos);
            auto offsetPtr = fact.expConvert_Pointer(offsetPtrT, dst.data, pos);
            offsetPtr = Exp_AddPtrRaw::Create(
               offsetPtrT, offsetPtr, offsetExp, pos);

            tmp = {offsetType, offsetPtr};
         }

         if(type->isTypeBitfield())
         {
            auto bits = type->getSizeBitsF() + type->getSizeBitsI() + type->getSizeBitsS();
            auto offs = type->getSizeBitsO();

            if(!tmp.isIRArg())
               Core::Error(pos, "bitfield init must be IR arg");

            value->genStmntStk(ctx);
            ctx.block.setArgSize().addStmnt(IR::Code::Bset,
               tmp.getIRArg(ctx.prog), IR::Arg_Stk(type->getSizeBytes()), bits, offs);
         }
         else
            value->genStmnt(ctx, tmp);
      }
      else
         v_genStmnt(ctx, dst, skipZero);
   }

   //
   // Init::getIRExp
   //
   IR::Exp::CRef Init::getIRExp() const
   {
      return value ? value->getIRExp() : v_getIRExp();
   }

   //
   // Init::isEffect
   //
   bool Init::isEffect() const
   {
      return value ? value->isEffect() : v_isEffect();
   }

   //
   // Init::isIRExp
   //
   bool Init::isIRExp() const
   {
      return value ? value->isIRExp() : v_isIRExp();
   }

   //
   // Init::isNoAuto
   //
   bool Init::isNoAuto() const
   {
      return value ? value->isNoAuto() : v_isNoAuto();
   }

   //
   // Init::parse
   //
   void Init::parse(InitRaw const &raw, Parser &ctx, Scope &scope)
   {
      // If already parsed, warn about overriding initializer.
      if(parsed)
         SR::WarnUnusedInit(raw.valueTok.pos,
            "overriding initializer from ", pos);

      pos = raw.valueTok.pos;

      if(raw.valueExp || raw.valueTok.str)
         v_parseSingle(raw, ctx, scope);
      else
         v_parseBlock(raw, ctx, scope);

      if(value)
         value = fact.expPromo_Assign(type, value, value->pos);

      parsed = true;
   }

   //
   // Init::parse
   //
   std::size_t Init::parse(InitRaw const &raw, std::size_t rawIdx,
      Parser &ctx, Scope &scope)
   {
      auto const &rawRef = raw.valueSub[rawIdx];

      // If already parsed, warn about overriding initializer.
      if(parsed)
         SR::WarnUnusedInit(rawRef.valueTok.pos,
            "overriding initializer from ", pos);

      pos = rawRef.valueTok.pos;

      if(rawRef.valueExp || rawRef.valueTok.str)
         rawIdx = v_parseOpen(raw, rawIdx, ctx, scope);
      else
         ++rawIdx, v_parseBlock(rawRef, ctx, scope);

      if(value)
         value = fact.expPromo_Assign(type, value, value->pos);

      parsed = true;

      return rawIdx;
   }

   //
   // Init::v_getIRExp
   //
   IR::Exp::CRef Init::v_getIRExp() const
   {
      Core::Error(pos, "expected constant initializer");
   }

   //
   // Init::v_isEffect
   //
   bool Init::v_isEffect() const
   {
      return false;
   }

   //
   // Init::v_isIRExp
   //
   bool Init::v_isIRExp() const
   {
      return false;
   }

   //
   // Init::v_parseSingle
   //
   void Init::v_parseSingle(InitRaw const &raw, Parser &ctx, Scope &scope)
   {
      if(raw.valueExp)
      {
         value = raw.valueExp;
      }
      else if(raw.valueTok.str)
      {
         // String initializer?
         if(IsInitString(raw.valueTok, type))
            return v_parseString(raw.valueTok);

         // If not, reprocess the token as a primary-expression.
         Core::ArrayTBuf   buf{&raw.valueTok, 1};
         Core::TokenStream str{&buf};

         value = ctx.clone(str)->getExp_Prim(scope);
      }
      else
         Core::Error(raw.valueTok.pos, "expected expression");
   }

   //
   // Init::v_parseString
   //
   void Init::v_parseString(Core::Token const &tok)
   {
      Core::Error(tok.pos, "Init::v_parseString");
   }

   //
   // Init::Create
   //
   Init::Ptr Init::Create(SR::Type const *type, Core::FastU offset,
      Core::Origin pos, Factory &fact)
   {
      if(type->isTypeArray())
      {
         if(dynamic_cast<SR::Type_Array0 const *>(type))
            return Ptr(new Init_Array0(type, offset, pos, fact));

         if(auto t = dynamic_cast<SR::Type_Array const *>(type))
            return Ptr(new Init_Array(t, offset, pos, t->getSizeWidth(), fact));

         Core::Error(pos, "invalid array type for initializer");
      }

      if(type->isCTypeStruct())
      {
         if(auto t = dynamic_cast<Type_Struct const *>(type))
            return Ptr(new Init_Struct(t, offset, pos, fact));

         if(auto t = dynamic_cast<Type_Div const *>(type))
            return Ptr(new Init_Div(t, offset, pos, fact));

         Core::Error(pos, "invalid structure type for initializer");
      }

      if(type->isCTypeUnion())
      {
         if(auto t = dynamic_cast<Type_Struct const *>(type))
            return Ptr(new Init_Union(t, offset, pos, fact));

         Core::Error(pos, "invalid union type for initializer");
      }

      return Ptr(new Init_Value(type, offset, pos, fact));
   }

   //
   // Init::Create
   //
   Init::Ptr Init::Create(InitRaw const &raw, Parser &ctx, Scope &scope,
      SR::Type const *type, Factory &fact)
   {
      auto init = Create(type, 0, raw.valueTok.pos, fact);
      init->parse(raw, ctx, scope);
      return init;
   }

   //
   // Init::IsInitString
   //
   bool Init::IsInitString(Core::Token const &tok, SR::Type const *type)
   {
      if(!type->isTypeArray()) return false;

      auto base = type->getBaseType()->getTypeQual();

      if(tok.tok == Core::TOK_String && base->isCTypeChar()) return true;
      if(tok.tok == Core::TOK_StrU08 && base->isCTypeChar()) return true;

      if(tok.tok == Core::TOK_StrWid && base == TypeIntegPrU)  return true;
      if(tok.tok == Core::TOK_StrU16 && base == TypeIntegPrUH) return true;
      if(tok.tok == Core::TOK_StrU32 && base == TypeIntegPrU)  return true;

      return false;
   }

   //
   // Init_Aggregate::findSub
   //
   std::size_t Init_Aggregate::findSub(Core::String)
   {
      return -1;
   }

   //
   // Init_Aggregate::firstSub
   //
   std::size_t Init_Aggregate::firstSub() const
   {
      return 0;
   }

   //
   // Init_Aggregate::getDes
   //
   std::pair<std::size_t, Init *>
   Init_Aggregate::getDes(Core::Array<InitRawDes> const &desig,
      std::size_t desigIdx)
   {
      std::size_t index;

      // Member designator.
      if(desig[desigIdx].desigStr)
      {
         if(!type->isCTypeStruct() && !type->isCTypeUnion())
            Core::Error(pos, "expected structure type");

         index = findSub(desig[desigIdx].desigStr);
      }

      // Element designator.
      else
      {
         if(!type->isTypeArray())
            Core::Error(pos, "expected array type");

         index = desig[desigIdx].desigInt;
      }

      // Check validity of index.
      auto sub = getSub(index);
      if(!sub) Core::Error(pos, "out of bounds designator");

      // Designator list terminated by =.
      if(desigIdx + 1 == desig.size())
         return {index, sub};

      // Otherwise, expect nested designator.
      auto subAgg = dynamic_cast<Init_Aggregate *>(sub);

      if(!subAgg)
         Core::Error(pos, "invalid nested designator");

      return {index, subAgg->getDes(desig, desigIdx + 1).second};
   }

   //
   // Init_Aggregate::nextSub
   //
   std::size_t Init_Aggregate::nextSub(std::size_t index) const
   {
      return index + 1;
   }

   //
   // Init_Aggregate::v_parseBlock
   //
   void Init_Aggregate::v_parseBlock(InitRaw const &raw, Parser &ctx, Scope &scope)
   {
      // Bracketed aggregate initializer.

      auto rawPtr = raw.valueSub.begin();
      auto rawItr = rawPtr;
      auto rawEnd = raw.valueSub.end();

      // Single initializer?
      if(raw.valueSub.size() == 1)
      {
         // String initializer?
         if(IsInitString(rawItr->valueTok, type))
            return v_parseString(rawItr->valueTok);

         // Initializing from aggregate?
         if(rawItr->valueExp &&
            rawItr->valueExp->getType()->getTypeQual() == type->getTypeQual())
         {
            value = rawItr->valueExp;
            return;
         }
      }

      Init *sub;
      auto  index = firstSub();

      for(; rawItr != rawEnd; index = nextSub(index))
      {
         // Designator.
         if(!rawItr->desig.empty())
         {
            std::tie(index, sub) = getDes(rawItr->desig);

            rawItr = rawPtr + sub->parse(raw, rawItr - rawPtr, ctx, scope);
         }
         else if((sub = getSub(index)))
            rawItr = rawPtr + sub->parse(raw, rawItr - rawPtr, ctx, scope);
         else
         {
            SR::WarnUnusedInit(rawItr->valueTok.pos, "excess initializer");
            ++rawItr;
         }
      }
   }

   //
   // Init_Aggregate::v_parseOpen
   //
   std::size_t Init_Aggregate::v_parseOpen(InitRaw const &raw,
      std::size_t rawIdx, Parser &ctx, Scope &scope)
   {
      // Unbracketed aggregate initializer.

      auto rawPtr = raw.valueSub.begin();
      auto rawItr = rawPtr + rawIdx;
      auto rawEnd = raw.valueSub.end();

      // String initializer?
      if(IsInitString(rawItr->valueTok, type))
         return v_parseString(rawItr->valueTok), rawIdx + 1;

      // Initializing from aggregate?
      if(rawItr->valueExp &&
         rawItr->valueExp->getType()->getTypeQual() == type->getTypeQual())
      {
         value = rawItr->valueExp;
         return rawIdx + 1;
      }

      Init *sub;
      auto  index = firstSub();

      for(; rawItr != rawEnd; index = nextSub(index))
      {
         // Designator.
         if(!rawItr->desig.empty())
         {
            break;
         }
         else if((sub = getSub(index)))
            rawItr = rawPtr + sub->parse(raw, rawItr - rawPtr, ctx, scope);
         else
            break;
      }

      return rawItr - rawPtr;
   }

   //
   // Init_Aggregate::v_parseString
   //
   void Init_Aggregate::v_parseString(Core::Token const &tok)
   {
      auto index = firstSub();
      for(auto &c : GetString(tok))
      {
         if(auto sub = getSub(index))
         {
            auto cIR = IR::ExpCreate_Value(std::move(c), pos);
            sub->value = SR::ExpCreate_IRExp(cIR, sub->type, pos);
         }

         index = nextSub(index);
      }
   }

   //
   // Init_Array constructor
   //
   Init_Array::Init_Array(SR::Type const *type_, Core::FastU offset_,
      Core::Origin pos_, std::size_t width, Factory &fact_) :
      Init_Aggregate{type_, offset_, pos_, fact_}, subs{width}
   {
      auto subT = type->getBaseType();
      auto subB = subT->getSizeBytes();

      std::size_t subC = 0;

      for(auto &mem : subs)
         mem = Create(subT, offset + subC++ * subB, pos, fact);
   }

   //
   // Init_Array::getSub
   //
   Init *Init_Array::getSub(std::size_t index)
   {
      return index < subs.size() ? subs[index].get() : nullptr;
   }

   //
   // Init_Array::v_genStmnt
   //
   void Init_Array::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &arg, bool skipZero) const
   {
      for(auto const &sub : subs) sub->genStmnt(ctx, arg, skipZero);
   }

   //
   // Init_Array::v_getIRExp
   //
   IR::Exp::CRef Init_Array::v_getIRExp() const
   {
      return IR::ExpCreate_Array(type->getBaseType()->getIRType(),
         {subs.begin(), subs.end(), [](std::unique_ptr<Init> const &ini)
            {return ini->getIRExp();}}, pos);
   }

   //
   // Init_Array::v_isIRExp
   //
   bool Init_Array::v_isIRExp() const
   {
      for(auto const &sub : subs) if(!sub->isIRExp()) return false;
      return true;
   }

   //
   // Init_Array::v_isNoAuto
   //
   bool Init_Array::v_isNoAuto() const
   {
      for(auto const &sub : subs) if(!sub->isNoAuto()) return false;
      return true;
   }

   //
   // Init_Array0 constructor
   //
   Init_Array0::Init_Array0(SR::Type const *type_, Core::FastU offset_,
      Core::Origin pos_, Factory &fact_) :
      Init_Aggregate{type_, offset_, pos_, fact_},
      subT{type->getBaseType()},
      subB{subT->getSizeBytes()}
   {
   }

   //
   // Init_Array0::getSub
   //
   Init *Init_Array0::getSub(std::size_t index)
   {
      if(index >= subs.size())
      {
         do
            subs.emplace_back(Create(subT, offset + subs.size() * subB, pos, fact));
         while(index >= subs.size());

         type = type->getBaseType()->getTypeArray(subs.size())
            ->getTypeQual(type->getQual());
      }

      return subs[index].get();
   }

   //
   // Init_Array0::v_genStmnt
   //
   void Init_Array0::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &arg, bool skipZero) const
   {
      for(auto const &sub : subs) sub->genStmnt(ctx, arg, skipZero);
   }

   //
   // Init_Array0::v_getIRExp
   //
   IR::Exp::CRef Init_Array0::v_getIRExp() const
   {
      return IR::ExpCreate_Array(subT->getIRType(),
         {subs.begin(), subs.end(), [](std::unique_ptr<Init> const &ini)
            {return ini->getIRExp();}}, pos);
   }

   //
   // Init_Array0::v_isIRExp
   //
   bool Init_Array0::v_isIRExp() const
   {
      for(auto const &sub : subs) if(!sub->isIRExp()) return false;
      return true;
   }

   //
   // Init_Array0::v_isNoAuto
   //
   bool Init_Array0::v_isNoAuto() const
   {
      for(auto const &sub : subs) if(!sub->isNoAuto()) return false;
      return true;
   }

   //
   // Init_Div constructor
   //
   Init_Div::Init_Div(Type_Div const *typeD, Core::FastU offset_,
      Core::Origin pos_, Factory &fact_) :
      Init_Aggregate{typeD, offset_, pos_, fact_},
      subs   {}
   {
      auto mem = typeD->getMember(Core::STR_quot);
      subs[0] = Create(mem.type, mem.addr, pos, fact);
      mem = typeD->getMember(Core::STR_rem);
      subs[1] = Create(mem.type, mem.addr, pos, fact);
   }

   //
   // Init_Div::findSub
   //
   std::size_t Init_Div::findSub(Core::String name)
   {
      if(name == Core::STR_quot) return 0;
      if(name == Core::STR_rem)  return 1;

      return -1;
   }

   //
   // Init_Div::getSub
   //
   Init *Init_Div::getSub(std::size_t index)
   {
      return index < 2 ? subs[index].get() : nullptr;
   }

   //
   // Init_Div::nextSub
   //
   std::size_t Init_Div::nextSub(std::size_t index) const
   {
      return index + 1;
   }

   //
   // Init_Div::v_genStmnt
   //
   void Init_Div::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &arg, bool skipZero) const
   {
      subs[0]->genStmnt(ctx, arg, skipZero);
      subs[1]->genStmnt(ctx, arg, skipZero);
   }

   //
   // Init_Div::v_getIRExp
   //
   IR::Exp::CRef Init_Div::v_getIRExp() const
   {
      return IR::ExpCreate_Assoc(type->getIRType().tAssoc,
         {Core::Pack, subs[0]->getIRExp(), subs[1]->getIRExp()}, pos);
   }

   //
   // Init_Div::v_isIRExp
   //
   bool Init_Div::v_isIRExp() const
   {
      return subs[0]->isIRExp() && subs[1]->isIRExp();
   }

   //
   // Init_Div::v_isNoAuto
   //
   bool Init_Div::v_isNoAuto() const
   {
      return subs[0]->isNoAuto() && subs[1]->isNoAuto();
   }

   //
   // Init_Struct constructor
   //
   Init_Struct::Init_Struct(Type_Struct const *typeS, Core::FastU offset_,
      Core::Origin pos_, Factory &fact_) :
      Init_Aggregate{typeS, offset_, pos_, fact_},
      subs    {SubCount(typeS)},
      subInit {0},
      subTotal{0}
   {
      auto subItr = subs.begin();
      SubInit(typeS, subItr, offset, pos, fact);

      subTotal = subs.size();

      for(auto const &sub : subs)
      {
         if(sub.anon)
         {
            if(auto init = dynamic_cast<Init_Struct *>(&*sub.init))
               subTotal += init->subTotal;
         }
      }
   }

   //
   // Init_Struct::findSub
   //
   std::size_t Init_Struct::findSub(Core::String name)
   {
      std::size_t index = 0;

      for(auto const &sub : subs)
      {
         if(sub.name == name)
            return subInit = index;

         ++index;

         if(sub.anon)
         {
            if(auto init = dynamic_cast<Init_Struct *>(&*sub.init))
            {
               auto subIdx = init->findSub(name);
               if(subIdx == static_cast<std::size_t>(-1))
                  index += init->subTotal;
               else
                  return index + subIdx;
            }
         }
      }

      return -1;
   }

   //
   // Init_Struct::getMem
   //
   InitMem const *Init_Struct::getMem(std::size_t index) const
   {
      for(auto &sub : subs)
      {
         if(!index--)
            return &sub;

         if(sub.anon)
         {
            if(auto init = dynamic_cast<Init_Struct *>(&*sub.init))
            {
               if(index < init->subTotal)
                  return init->getMem(index);
               else
                  index -= init->subTotal;
            }
         }
      }

      return nullptr;
   }

   //
   // Init_Struct::getSub
   //
   Init *Init_Struct::getSub(std::size_t index)
   {
      auto mem = getMem(index);
      return mem ? mem->init.get() : nullptr;
   }

   //
   // Init_Struct::nextSub
   //
   std::size_t Init_Struct::nextSub(std::size_t index) const
   {
      if(auto mem = getMem(index))
      {
         Init_Struct *init;
         if(mem->anon && (init = dynamic_cast<Init_Struct *>(&*mem->init)))
            index += init->subTotal + 1;
         else
            index += mem->step;
      }
      else
         ++index;

      return index;
   }

   //
   // Init_Struct::v_genStmnt
   //
   void Init_Struct::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &arg, bool skipZero) const
   {
      for(auto const &sub : subs) sub.init->genStmnt(ctx, arg, skipZero);
   }

   //
   // Init_Struct::v_getIRExp
   //
   IR::Exp::CRef Init_Struct::v_getIRExp() const
   {
      return IR::ExpCreate_Assoc(type->getIRType().tAssoc,
         {subs.begin(), subs.end(), [](InitMem const &ini)
            {return ini.init->getIRExp();}}, pos);
   }

   //
   // Init_Struct::v_isIRExp
   //
   bool Init_Struct::v_isIRExp() const
   {
      for(auto const &sub : subs) if(!sub.init->isIRExp()) return false;
      return true;
   }

   //
   // Init_Struct::v_isNoAuto
   //
   bool Init_Struct::v_isNoAuto() const
   {
      for(auto const &sub : subs) if(!sub.init->isNoAuto()) return false;
      return true;
   }

   //
   // Init_Union constructor
   //
   Init_Union::Init_Union(Type_Struct const *typeS, Core::FastU offset_,
      Core::Origin pos_, Factory &fact_) :
      Init_Struct{typeS, offset_, pos_, fact_}
   {
   }

   //
   // Init_Union::v_genStmnt
   //
   void Init_Union::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &arg, bool skipZero) const
   {
      getMem(subInit)->init->genStmnt(ctx, arg, skipZero);
   }

   //
   // Init_Union::v_getIRExp
   //
   IR::Exp::CRef Init_Union::v_getIRExp() const
   {
      return IR::ExpCreate_Union(type->getIRType().tUnion,
         getMem(subInit)->init->getIRExp(), pos);
   }

   //
   // Init_Union::v_isIRExp
   //
   bool Init_Union::v_isIRExp() const
   {
      return getMem(subInit)->init->isIRExp();
   }

   //
   // Init_Union::v_isNoAuto
   //
   bool Init_Union::v_isNoAuto() const
   {
      return getMem(subInit)->init->isNoAuto();
   }

   //
   // Init_Value constructor
   //
   Init_Value::Init_Value(SR::Type const *type_, Core::FastU offset_,
      Core::Origin pos_, Factory &fact_) :
      Init{type_, offset_, pos_, fact_}
   {
      value = fact.expPromo_Assign(type,
         fact.expCreate_LitInt(TypeIntegPrS, 0, pos), pos);
   }

   //
   // Init_Value::v_genStmnt
   //
   void Init_Value::v_genStmnt(SR::GenStmntCtx const &,
      SR::Arg const &, bool) const
   {
      // This shouldn't be called.
      Core::Error(pos, "Init_Value::v_genStmnt");
   }

   //
   // Init_Value::v_isNoAuto
   //
   bool Init_Value::v_isNoAuto() const
   {
      return true;
   }

   //
   // Init_Value::v_parseBlock
   //
   void Init_Value::v_parseBlock(InitRaw const &raw, Parser &ctx, Scope &scope)
   {
      v_parseSingle(raw.valueSub[0], ctx, scope);
   }

   //
   // Init_Value::v_parseOpen
   //
   std::size_t Init_Value::v_parseOpen(InitRaw const &raw,
      std::size_t rawIdx, Parser &ctx, Scope &scope)
   {
      v_parseSingle(raw.valueSub[rawIdx], ctx, scope);
      return rawIdx + 1;
   }
}

// EOF

