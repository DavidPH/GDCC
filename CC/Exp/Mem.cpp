//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ." and "operator ->" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Mem.hpp"

#include "CC/Exp/Add.hpp"
#include "CC/Factory.hpp"
#include "CC/Type/Struct.hpp"

#include "SR/Arg.hpp"
#include "SR/Type.hpp"

#include "Core/Exception.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_Mem::v_genStmnt
   //
   void Exp_Mem::v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const
   {
      SR::GenStmnt_Move(this, ctx, dst, getArg());
   }

   //
   // Exp_Mem::v_getArg
   //
   SR::Arg Exp_Mem::v_getArg() const
   {
      try
      {
         // Lookup member.
         auto mem = expL->getType()->getMember(expR);

         // Calculate pointer offset.
         auto off  = mem.addr / mem.type->getSizeShift();
         auto expO = fact.expCreate_LitInt(SR::Type::Size,
            Core::NumberCast<Core::Integ>(off), pos);

         // Create pointer expression.
         auto type = mem.type->getTypePointer();
         auto addr = fact.expConvert_Pointer(type, expL->getArg().data, pos);
              addr = Exp_AddPtrRaw::Create(type, addr, expO, pos);

         // Convert pointer into an arg.
         return {mem.type, addr};
      }
      catch(SR::TypeError const &)
      {
         Core::Error(pos, "invalid member");
      }
   }

   //
   // Exp_Mem::v_getType
   //
   SR::Type::CRef Exp_Mem::v_getType() const
   {
      try
      {
         return expL->getType()->getMember(expR).type;
      }
      catch(SR::TypeError const &)
      {
         Core::Error(pos, "invalid member");
      }
   }

   //
   // Exp_Mem::v_isEffect
   //
   bool Exp_Mem::v_isEffect() const
   {
      return expL->isEffect();
   }

   //
   // Exp_Mem::v_isNoAuto
   //
   bool Exp_Mem::v_isNoAuto() const
   {
      return expL->isNoAuto();
   }

   //
   // Exp_Mem::v_setRefer
   //
   void Exp_Mem::v_setRefer() const
   {
      expL->setRefer();
   }

   //
   // Exp_MemProp::createExp
   //
   SR::Exp::CRef Exp_MemProp::createExp(Type_Struct::Prop const &,
      StructProp const *func, SR::Exp::CRef const *argv, std::size_t argc) const
   {
      std::vector<SR::Exp::CRef> args;

      // Populate arguments vector.
      auto argi = argv, arge = argi + argc;
      for(auto const &arg : func->args)
      {
         switch(arg.spec)
         {
         case StructPropArg::Member:
            // TODO: Avoid multiple evaluation.
            args.emplace_back(fact.expCreate_Mem(expL, arg.name, pos, scope));
            break;

         case StructPropArg::ThisPtr:
            // TODO: Avoid multiple evaluation.
            args.emplace_back(fact.expCreate_Refer(expL, pos));
            break;

         case StructPropArg::ThisRef:
            // TODO: Avoid multiple evaluation.
            args.emplace_back(expL);
            break;

         case StructPropArg::Value:
            if(!arg.val)
            {
               if(argi == arge)
                  Core::Error(pos, "insufficient property args");

               args.emplace_back(*argi++);
            }
            else
               args.emplace_back(arg.val);
            break;
         }
      }

      while(argi != arge)
         args.emplace_back(*argi++);

      // TODO: If this not referenced, evaluate it.

      return fact.expCreate_Call(func->func, {args.begin(), args.end()}, scope, pos);
   }

   //
   // Exp_MemProp::createExp
   //
   SR::Exp::CRef Exp_MemProp::createExp() const
   {
      auto prop = type->getProp(expR);

      if(!prop.prop)
         Core::Error(pos, "structure property has no default");

      return createExp(prop, prop.prop, nullptr, 0);
   }

   //
   // Exp_MemProp::createExp_Paren
   //
   SR::Exp::CRef Exp_MemProp::createExp_Paren(Core::Array<SR::Exp::CRef> &&args) const
   {
      auto prop = type->getProp(expR);

      // If no call property defined, fall back to get property.
      if(!prop.propParen)
         return fact.expCreate_Call(createExp(), std::move(args), scope, pos);

      return createExp(prop, prop.propParen, args.data(), args.size());
   }

   //
   // Exp_MemProp::createExpEq
   //
   SR::Exp::CRef Exp_MemProp::createExpEq(SR::Exp::CRef const &arg,
      StructProp const *Type_Struct::Prop::*mem, char const *op) const
   {
      auto prop = type->getProp(expR);

      if(!(prop.*mem))
         Core::Error(pos, "structure property has no operator", op);

      return createExp(prop, prop.*mem, &arg, 1);
   }

   //
   // Exp_MemProp::createExpEq
   //
   SR::Exp::CRef Exp_MemProp::createExpEq(
      StructProp const *Type_Struct::Prop::*mem, char const *op) const
   {
      auto prop = type->getProp(expR);

      if(!(prop.*mem))
         Core::Error(pos, "structure property has no operator", op);

      return createExp(prop, prop.*mem, nullptr, 0);
   }

   //
   // Exp_MemProp::v_getArg
   //
   SR::Arg Exp_MemProp::v_getArg() const
   {
      Core::Error(pos, "structure property cannot be evaulated");
   }

   //
   // Exp_MemProp::v_getType
   //
   SR::Type::CRef Exp_MemProp::v_getType() const
   {
      Core::Error(pos, "structure property has no type");
   }

   //
   // Factory::expCreate_Mem
   //
   SR::Exp::CRef Factory::expCreate_Mem(SR::Exp const *l, Core::String r,
      Core::Origin pos, Scope &scope)
   {
      // Special check for structure property.
      if(Type_Struct::CPtr type = dynamic_cast<Type_Struct const *>(&*l->getType()))
      {
         if(type->hasProp(r))
            return Exp_MemProp::Create(l, r, pos, *this, type, scope);
      }

      // Special check for structure property.
      if(auto expL = dynamic_cast<Exp_MemProp const *>(l))
         return Exp_Mem::Create(expL->createExp(), r, pos, *this);

      // Otherwise, normal member.
      return Exp_Mem::Create(l, r, pos, *this);
   }

   //
   // Factory::expCreate_MemPt
   //
   SR::Exp::CRef Factory::expCreate_MemPt(SR::Exp const *l, Core::String r,
      Core::Origin pos, Scope &scope)
   {
      return expCreate_Mem(expCreate_Deref(l, pos), r, pos, scope);
   }
}

// EOF

