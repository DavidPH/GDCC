//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree storage arguments.
//
//-----------------------------------------------------------------------------

#include "AST/Arg.hpp"

#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Arg.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Arg copy constructor
      //
      Arg::Arg(Arg const &arg) : type{arg.type}, data{arg.data}
      {
      }

      //
      // Arg constructor
      //
      Arg::Arg(Type const *type_) : type{type_}, data{nullptr}
      {
      }

      //
      // Arg constructor
      //
      Arg::Arg(Type const *type_, Exp const *data_) : type{type_}, data{data_}
      {
      }

      //
      // Arg constructor
      //
      Arg::Arg(Type const *type_, IR::AddrBase base, Exp const *data_) :
         type{type_->getTypeQual({{base, Core::STR_}})},
         data{data_}
      {
      }

      //
      // Arg destructor
      //
      Arg::~Arg()
      {
      }

      //
      // Arg::getIRArg
      //
      IR::Arg Arg::getIRArg() const
      {
         switch(type->getQualAddr().base)
         {
         case IR::AddrBase::Lit:
            return IR::Arg_Lit(data->getIRExp());

         case IR::AddrBase::LocReg:
            return IR::Arg_LocReg(IR::Arg_Lit(data->getIRExp()));

         case IR::AddrBase::Stk:
            return IR::Arg_Stk();

         default:
            break;
         }

         // Callers should always check isIRArg, so this should never get
         // invoked. But if it does, any possible source position could help.
         if(data)
            throw Core::ExceptStr(data->pos, "bad Arg::getIRArg");
         else
            throw Core::ExceptStr({nullptr, 0}, "bad Arg::getIRArg");
      }

      //
      // Arg::isIRArg
      //
      bool Arg::isIRArg() const
      {
         // Using an IR Arg may bypass strict interpretation.
         if(type->getQualVola())
            return false;

         switch(type->getQualAddr().base)
         {
         case IR::AddrBase::Lit:
         case IR::AddrBase::LocReg:
            return data->isIRExp();

         case IR::AddrBase::Stk:
            return true;

         default:
            return false;
         }
      }
   }
}

// EOF

