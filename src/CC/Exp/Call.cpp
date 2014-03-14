//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ()" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Call.hpp"

#include "AST/Type.hpp"

#include "Core/Exception.hpp"


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
            throw Core::ExceptStr(pos, "insufficient arguments");

         if(args.size() > param->size() && !param->variadic())
            throw Core::ExceptStr(pos, "too many arguments");

         // Promote/convert args.
         auto paramItr = param->begin(), paramEnd = param->end();
         auto argsItr  = args.begin(),   argsEnd  = args.end();

         for(; paramItr != paramEnd; ++paramItr, ++argsItr)
            *argsItr = ExpPromo_Assign(*paramItr, *argsItr, pos);
         for(; argsItr != argsEnd; ++argsItr)
            *argsItr = ExpPromo_Arg(*argsItr, pos);

         return static_cast<AST::Exp::CRef>(
            new Exp_CallStk(exp, pos, std::move(args)));
      }
   }
}

// EOF

