//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
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
#include "CC/Init.hpp"

#include "AST/Function.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/CallType.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // IsCallLit
      //
      static bool IsCallLit(AST::Exp const *exp,
         Core::Array<AST::Exp::CRef> const &args)
      {
         auto type = exp->getType()->getBaseType();

         switch(IR::GetCallTypeIR(type->getCallType()))
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
         {
            if(!arg->isIRExp()) return false;
            if(arg->getType()->getSizeWords() != 1) return false;
         }

         return true;
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
      // Exp_Call constructor
      //
      Exp_Call::Exp_Call(AST::Exp const *e, Core::Origin pos_,
         Core::Array<AST::Exp::CRef> const &args_) :
         Super{e->getType()->getBaseType()->getBaseType(), e, pos_},
         args{args_},
         func{e->getType()->getBaseType()}
      {
      }

      //
      // Exp_Call constructor
      //
      Exp_Call::Exp_Call(AST::Exp const *e, Core::Origin pos_,
         Core::Array<AST::Exp::CRef> &&args_) :
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
         switch(IR::GetCallTypeIR(func->getCallType()))
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
      // ExpCreate_Call
      //
      AST::Exp::CRef ExpCreate_Call(AST::Exp const *e,
         Core::Array<AST::Exp::CRef> const &args_, Core::Origin pos)
      {
         auto args = args_;
         return ExpCreate_Call(e, std::move(args), pos);
      }

      //
      // ExpCreate_Call
      //
      AST::Exp::CRef ExpCreate_Call(AST::Exp const *e,
         Core::Array<AST::Exp::CRef> &&args, Core::Origin pos)
      {
         auto exp  = ExpPromo_LValue(e, pos);
         auto type = exp->getType();

         if(!type->isTypePointer() || !(type = type->getBaseType())->isCTypeFunction())
            throw Core::ExceptStr(pos, "expected pointer-to-function");

         // Check argument count.
         auto param = type->getParameters();
         if(args.size() < param->size())
         {
            AST::Function::Ptr fn;
            if(exp->isFunction())
               fn = exp->getFunction();

            // Possible optional args?
            if(!fn || param->size() - args.size() > fn->paramOpt)
               throw Core::ExceptStr(pos, "insufficient arguments");

            switch(IR::GetCallTypeIR(type->getCallType()))
            {
            case IR::CallType::Native:
            case IR::CallType::Special:
               // Natives and specials can simply have fewer arguments.
               break;

            default:
               std::vector<AST::Exp::CRef> argsNew;
               argsNew.reserve(param->size());
               auto paramItr = param->begin(), paramEnd = param->end();
               auto argsItr  = args.begin(),   argsEnd  = args.end();

               for(; argsItr != argsEnd; ++argsItr, ++paramItr)
                  argsNew.emplace_back(*argsItr);
               for(; paramItr != paramEnd; ++paramItr)
                  argsNew.emplace_back(Exp_Init::Create(
                     Init::Create(*paramItr, 0, pos), false, pos));

               args = {Core::Move, argsNew.begin(), argsNew.end()};

               break;
            }
         }

         if(args.size() > param->size() && !param->variadic())
            throw Core::ExceptStr(pos, "too many arguments");

         // Promote/convert args.
         auto paramItr = param->begin(), paramEnd = param->end();
         auto argsItr  = args.begin(),   argsEnd  = args.end();

         for(; paramItr != paramEnd && argsItr != argsEnd; ++paramItr, ++argsItr)
            *argsItr = ExpPromo_Assign(*paramItr, *argsItr, pos);
         for(; argsItr != argsEnd; ++argsItr)
            *argsItr = ExpPromo_Arg(*argsItr, pos);

         // Check for constant parameters.
         if(IsCallLit(exp, args))
            return Exp_CallLit::Create(exp, pos, std::move(args));

         return Exp_CallStk::Create(exp, pos, std::move(args));
      }
   }
}

// EOF

