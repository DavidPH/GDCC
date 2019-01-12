//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ()" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Call.hpp"

#include "CC/Exp/Init.hpp"
#include "CC/Exp/Mem.hpp"
#include "CC/Factory.hpp"
#include "CC/Init.hpp"
#include "CC/Scope/Local.hpp"

#include "Core/Exception.hpp"

#include "SR/Function.hpp"
#include "SR/Type.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // IsCallLit
   //
   static bool IsCallLit(SR::Exp const *exp,
      Core::Array<SR::Exp::CRef> const &args)
   {
      auto type = exp->getType()->getBaseType();

      switch(Target::GetCallTypeIR(type->getCallType()))
      {
      case IR::CallType::AsmFunc:
         if(!exp->isIRExp() || !exp->isFunction() || !exp->getFunction()->valueLit)
            return false;
         break;

      case IR::CallType::Special:
         if(!exp->isIRExp())
            return false;
         break;

      default:
         return false;
      }

      for(auto const &arg : args)
         if(!arg->isIRExp()) return false;

      return true;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_Call constructor
   //
   Exp_Call::Exp_Call(SR::Exp const *e, Core::Origin pos_,
      Core::Array<SR::Exp::CRef> const &args_) :
      Super{e->getType()->getBaseType()->getBaseType(), e, pos_},
      args{args_},
      func{e->getType()->getBaseType()}
   {
   }

   //
   // Exp_Call constructor
   //
   Exp_Call::Exp_Call(SR::Exp const *e, Core::Origin pos_,
      Core::Array<SR::Exp::CRef> &&args_) :
      Super{e->getType()->getBaseType()->getBaseType(), e, pos_},
      args{std::move(args_)},
      func{e->getType()->getBaseType()}
   {
   }

   //
   // Exp_Call::v_isEffect
   //
   bool Exp_Call::v_isEffect() const
   {
      return true;
   }

   //
   // Exp_Call::v_isIRExp
   //
   bool Exp_Call::v_isIRExp() const
   {
      return false;
   }

   //
   // Exp_Call::v_isNoAuto
   //
   bool Exp_Call::v_isNoAuto() const
   {
      switch(Target::GetCallTypeIR(func->getCallType()))
      {
      case IR::CallType::SScriptI:
      case IR::CallType::SScriptS:
      case IR::CallType::StdCall:
         return false;

      default:
         break;
      }

      for(auto const &arg : args)
         if(!arg->isNoAuto()) return false;

      return true;
   }

   //
   // Factory::expCreate_Call
   //
   SR::Exp::CRef Factory::expCreate_Call(SR::Exp const *e,
      Core::Array<SR::Exp::CRef> &&args, Scope &scope, Core::Origin pos)
   {
      // Special check for structure property.
      if(auto exp = dynamic_cast<Exp_MemProp const *>(e))
         return exp->createExp_call(std::move(args));

      auto exp  = expPromo_LValue(e, pos);
      auto type = exp->getType();

      if(!type->isTypePointer() || !(type = type->getBaseType())->isCTypeFunction())
         Core::Error(pos, "expected pointer-to-function");

      // Check argument count.
      auto param = type->getParameters();
      if(args.size() < param->size())
      {
         SR::Function::Ptr fn;
         if(exp->isFunction())
            fn = exp->getFunction();

         // Possible optional args?
         if(!fn || param->size() - args.size() > fn->paramOpt)
            Core::Error(pos, "insufficient arguments");

         switch(Target::GetCallTypeIR(type->getCallType()))
         {
         case IR::CallType::Native:
         case IR::CallType::Special:
            // Natives and specials can simply have fewer arguments.
            break;

         default:
            std::vector<SR::Exp::CRef> argsNew;
            argsNew.reserve(param->size());
            auto paramItr = param->begin(), paramEnd = param->end();
            auto argsItr  = args.begin(),   argsEnd  = args.end();

            for(; argsItr != argsEnd; ++argsItr, ++paramItr)
               argsNew.emplace_back(*argsItr);
            for(; paramItr != paramEnd; ++paramItr)
               argsNew.emplace_back(Exp_Init::Create(
                  Init::Create(*paramItr, 0, pos, *this), false, pos));

            args = {Core::Move, argsNew.begin(), argsNew.end()};

            break;
         }
      }

      if(args.size() > param->size() && !param->variadic())
         Core::Error(pos, "too many arguments");

      // Promote/convert args.
      auto paramItr = param->begin(), paramEnd = param->end();
      auto argsItr  = args.begin(),   argsEnd  = args.end();

      for(; paramItr != paramEnd && argsItr != argsEnd; ++paramItr, ++argsItr)
         *argsItr = expPromo_Assign(*paramItr, *argsItr);
      for(; argsItr != argsEnd; ++argsItr)
         *argsItr = expPromo_Arg(*argsItr, pos);

      // Check for constant parameters.
      if(IsCallLit(exp, args))
         return Exp_CallLit::Create(exp, pos, std::move(args));

      if(auto scopeLocal = dynamic_cast<Scope_Local *>(&scope))
         return Exp_CallStk::Create(exp, pos, std::move(args), *scopeLocal);
      else
         Core::Error(pos, "invalid scope for call");
   }
}

// EOF

