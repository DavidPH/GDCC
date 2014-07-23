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

#include "CC/Init.hpp"

#include "CC/Exp.hpp"
#include "CC/Exp/Add.hpp"
#include "CC/Parse.hpp"
#include "CC/Type.hpp"
#include "CC/Type/Struct.hpp"

#include "AST/Arg.hpp"
#include "AST/Type/Array.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static std::size_t SubCount(GDCC::CC::Type_Struct::MemberData const *itr,
   GDCC::CC::Type_Struct::MemberData const *end);

//
// SkipInit
//
static void SkipInit(GDCC::CC::ParserCtx const &ctx, GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   if(ctx.in.drop(Core::TOK_BraceO))
   {
      do SkipInit(ctx, scope);
      while(ctx.in.drop(Core::TOK_Comma) && !ctx.in.peek(Core::TOK_BraceC));
   }
   else
      CC::GetExp_Assi(ctx, scope);
}

//
// SubCount
//
static std::size_t SubCount(GDCC::CC::Type_Struct const *type)
{
   return SubCount(type->getData().memb.begin(), type->getData().memb.end());
}

//
// SubCount
//
static std::size_t SubCount(GDCC::CC::Type_Struct::MemberData const *itr,
   GDCC::CC::Type_Struct::MemberData const *end)
{
   using namespace GDCC;

   std::size_t n = 0;

   for(; itr != end; ++itr)
   {
      if(itr->name)
      {
         ++n;
      }
      else if(itr->anon)
      {
         if(auto memT = dynamic_cast<CC::Type_Struct const *>(&*itr->type))
            n += SubCount(memT);
      }
   }

   return n;
}

//
// SubInit
//
static void SubInit(GDCC::CC::Type_Struct const *type, GDCC::CC::InitMem *&sub,
   GDCC::Core::FastU offset, GDCC::Core::Origin pos)
{
   using namespace GDCC;

   auto const &data = type->getData();

   for(auto itr = data.memb.begin(), end = data.memb.end(); itr != end; ++itr)
   {
      if(itr->name)
      {
         sub->name = itr->name;
         sub->init = CC::Init::Create(itr->type, offset + itr->addr, pos);
         sub->step = data.isUnion ? SubCount(itr, end) : 1;
         ++sub;
      }
      else if(itr->anon)
      {
         if(auto memT = dynamic_cast<CC::Type_Struct const *>(&*itr->type))
         {
            auto subOld = sub;

            SubInit(memT, sub, offset + itr->addr, pos);

            // If this is a union, the last member added must point to the end
            // of this union. (If any members were added.)
            if(data.isUnion && sub != subOld)
               sub[-1].step = SubCount(itr + 1, end) + 1;
         }
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Init::genStmnt
      //
      void Init::genStmnt(AST::GenStmntCtx const &ctx, AST::Arg const &dst,
         bool skipZero) const
      {
         if(value)
         {
            if(!dst.data)
               return value->genStmnt(ctx, dst);

            // Convert dst's type to this's type and offset to this's offset.
            // This codegen assumes a lack of side effects in dst.data.

            auto offsetType = type->getTypeQual(dst.type->getQual());
            auto offsetPtrT = offsetType->getTypePointer();

            auto offsetRaw = offset / type->getSizeShift();
            auto offsetExp = ExpCreate_LitInt(AST::Type::Size, offsetRaw, pos);
            auto offsetPtr = ExpConvert_Pointer(offsetPtrT, dst.data, pos);
            offsetPtr = Exp_AddPtrRaw::Create(
               offsetPtrT, offsetPtr, offsetExp, pos);

            value->genStmnt(ctx, {offsetType, offsetPtr});
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
      // Init::parse
      //
      void Init::parse(ParserCtx const &ctx, Scope &scope)
      {
         pos = ctx.in.peek().pos;

         // Bracketed initializer.
         if(ctx.in.drop(Core::TOK_BraceO))
         {
            v_parseB(ctx, scope);

            if(!ctx.in.drop(Core::TOK_BraceC))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected '}'");
         }

         // Unbracketed initializer.
         else
            v_parseO(ctx, scope);

         if(value)
            value = ExpPromo_Assign(type, value, value->pos);
      }

      //
      // Init::v_getIRExp
      //
      IR::Exp::CRef Init::v_getIRExp() const
      {
         throw Core::ExceptStr(pos, "expected constant initializer");
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
      // Init::v_parseO
      //
      void Init::v_parseO(ParserCtx const &ctx, Scope &scope)
      {
         v_parseB(ctx, scope);
      }

      //
      // Init::width
      //
      Core::FastU Init::width() const
      {
         return 1;
      }

      //
      // Init::Create
      //
      Init::Ptr Init::Create(AST::Type const *type,
         Core::FastU offset, Core::Origin pos)
      {
         if(type->isTypeArray())
         {
            if(dynamic_cast<AST::Type_Array0 const *>(type))
               return Ptr(new Init_Array0(type, offset, pos));

            if(auto t = dynamic_cast<AST::Type_Array const *>(type))
               return Ptr(new Init_Array(t, offset, pos, t->getSizeWidth()));

            throw Core::ExceptStr(pos, "invalid array type for initializer");
         }

         if(type->isCTypeStruct() || type->isCTypeUnion())
         {
            if(auto t = dynamic_cast<Type_Struct const *>(type))
               return Ptr(new Init_Struct(t, offset, pos));

            throw Core::ExceptStr(pos, "invalid structure type for initializer");
         }

         return Ptr(new Init_Value(type, offset, pos));
      }

      //
      // Init::Get
      //
      Init::Ptr Init::Get(ParserCtx const &ctx, Scope &scope, AST::Type const *type)
      {
         auto init = Create(type, ctx.in.peek().pos);
         init->parse(ctx, scope);
         return init;
      }

      //
      // Init_Aggregate::findSub
      //
      std::size_t Init_Aggregate::findSub(Core::String) const
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
      Init_Aggregate::getDes(ParserCtx const &ctx, Scope &scope)
      {
         auto        pos = ctx.in.peek().pos;
         std::size_t index;

         // Element designator.
         if(ctx.in.drop(Core::TOK_BrackO))
         {
            if(!type->isTypeArray())
               throw Core::ExceptStr(pos, "expected array type");

            index = ExpToFastU(GetExp(ctx, scope));

            if(!ctx.in.drop(Core::TOK_BrackC))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected ']'");
         }

         // Member designator.
         else if(ctx.in.drop(Core::TOK_Dot))
         {
            if(!type->isCTypeStruct() && !type->isCTypeUnion())
               throw Core::ExceptStr(pos, "expected structure type");

            if(!ctx.in.peek(Core::TOK_Identi))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected identifier");

            index = findSub(ctx.in.get().str);
         }

         // Not designator.
         else
            throw Core::ExceptStr(pos, "expected designator");

         // Check validity of index.
         auto sub = getSub(index);
         if(!sub) throw Core::ExceptStr(pos, "out of bounds designator");

         // Designator list terminated by =.
         if(ctx.in.drop(Core::TOK_Equal))
            return {index, sub};

         // Otherwise, expect nested designator.
         auto subAgg = dynamic_cast<Init_Aggregate *>(sub);

         if(!subAgg)
            throw Core::ExceptStr(pos, "invalid nested designator");

         return {index, subAgg->getDes(ctx, scope).second};
      }

      //
      // Init_Aggregate::nextSub
      //
      std::size_t Init_Aggregate::nextSub(std::size_t index) const
      {
         return index + 1;
      }

      //
      // Init_Aggregate::v_parseB
      //
      void Init_Aggregate::v_parseB(ParserCtx const &ctx, Scope &scope)
      {
         // Bracketed aggregate initializer.

         Init       *sub;
         std::size_t index = firstSub();

         do
         {
            // Designator.
            if(ctx.in.peek(Core::TOK_BrackO) || ctx.in.peek(Core::TOK_Dot))
            {
               std::tie(index, sub) = getDes(ctx, scope);

               sub->parse(ctx, scope);
            }
            else if((sub = getSub(index)))
               sub->parse(ctx, scope);
            else
               SkipInit(ctx, scope);

            index = nextSub(index);
         }
         while(ctx.in.drop(Core::TOK_Comma) && !ctx.in.peek(Core::TOK_BraceC));
      }

      //
      // Init_Aggregate::v_parseO
      //
      void Init_Aggregate::v_parseO(ParserCtx const &ctx, Scope &scope)
      {
         // Unbracketed aggregate initializer.

         std::size_t index = firstSub();

         if(auto sub = getSub(index)) do
         {
            sub->parse(ctx, scope);

            if(!(sub = getSub(index = nextSub(index)))) break;
         }
         while(ctx.in.drop(Core::TOK_Comma) && !ctx.in.peek(Core::TOK_BraceC));
      }

      //
      // Init_Array constructor
      //
      Init_Array::Init_Array(AST::Type const *type_, Core::FastU offset_,
         Core::Origin pos_, std::size_t width) :
         Init_Aggregate{type_, offset_, pos_}, subs{width}
      {
         auto subT = type->getBaseType();
         auto subB = subT->getSizeBytes();

         std::size_t subC = 0;

         for(auto &mem : subs)
            mem = Create(subT, offset + subC++ * subB, pos);
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
      void Init_Array::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &arg, bool skipZero) const
      {
         for(auto const &sub : subs) sub->genStmnt(ctx, arg, skipZero);
      }

      //
      // Init_Array0 constructor
      //
      Init_Array0::Init_Array0(AST::Type const *type_, Core::FastU offset_,
         Core::Origin pos_) :
         Init_Aggregate{type_, offset_, pos_},
         subT{type->getBaseType()},
         subB{subT->getSizeBytes()}
      {
      }

      //
      // Init_Array0::getSub
      //
      Init *Init_Array0::getSub(std::size_t index)
      {
         while(index >= subs.size())
            subs.emplace_back(Create(subT, offset + subs.size() * subB, pos));

         return subs[index].get();
      }

      //
      // Init_Array0::v_genStmnt
      //
      void Init_Array0::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &arg, bool skipZero) const
      {
         for(auto const &sub : subs) sub->genStmnt(ctx, arg, skipZero);
      }

      //
      // Init_Array0::width
      //
      Core::FastU Init_Array0::width() const
      {
         return subs.size();
      }

      //
      // Init_Struct constructor
      //
      Init_Struct::Init_Struct(Type_Struct const *typeS, Core::FastU offset_,
         Core::Origin pos_) :
         Init_Aggregate{typeS, offset_, pos_},
         subs{SubCount(typeS)}
      {
         auto sub = subs.begin();
         SubInit(typeS, sub, offset, pos);
      }

      //
      // Init_Struct::findSub
      //
      std::size_t Init_Struct::findSub(Core::String name) const
      {
         for(std::size_t itr = 0, end = subs.size(); itr != end; ++itr)
            if(subs[itr].name == name) return itr;

         return -1;
      }

      //
      // Init_Struct::getSub
      //
      Init *Init_Struct::getSub(std::size_t index)
      {
         return index < subs.size() ? subs[index].init.get() : nullptr;
      }

      //
      // Init_Struct::nextSub
      //
      std::size_t Init_Struct::nextSub(std::size_t index) const
      {
         return index + (index < subs.size() ? subs[index].step : 1);
      }

      //
      // Init_Struct::v_genStmnt
      //
      void Init_Struct::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &arg, bool skipZero) const
      {
         for(auto index = firstSub(); index < subs.size(); index = nextSub(index))
            subs[index].init->genStmnt(ctx, arg, skipZero);
      }

      //
      // Init_Value constructor
      //
      Init_Value::Init_Value(AST::Type const *type_, Core::FastU offset_,
         Core::Origin pos_) :
         Init{type_, offset_, pos_}
      {
         value = ExpPromo_Assign(type, ExpCreate_LitInt(TypeIntegPrS, 0, pos), pos);
      }

      //
      // Init_Value::parseB
      //
      void Init_Value::v_parseB(ParserCtx const &ctx, Scope &scope)
      {
         value = GetExp_Assi(ctx, scope);
      }

      //
      // Init_Value::v_genStmnt
      //
      void Init_Value::v_genStmnt(AST::GenStmntCtx const &,
         AST::Arg const &, bool) const
      {
         // This shouldn't be called.
         throw Core::ExceptStr(pos, "Init_Value::v_genStmnt");
      }
   }
}

// EOF

