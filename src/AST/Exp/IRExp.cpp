//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree IR expression values.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/IRExp.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_IRExp constructor
      //
      Exp_IRExp::Exp_IRExp(IR::Exp const *exp_, Type const *type_,
         Core::Origin pos_) :
         Super{pos_}, exp{exp_}, type{type_}
      {
      }

      //
      // Exp_IRExp destructor
      //
      Exp_IRExp::~Exp_IRExp()
      {
      }

      //
      // Exp_IRExp::v_genStmnt
      //
      void Exp_IRExp::v_genStmnt(IR::Block &block, Function *fn, Arg const &dst) const
      {
         genStmntMove(block, fn, dst, getArg());
      }

      //
      // Exp_IRExp::v_getIRExp
      //
      IR::Exp::CRef Exp_IRExp::v_getIRExp() const
      {
         return exp;
      }

      //
      // Exp_IRExp::v_getType
      //
      Type::CRef Exp_IRExp::v_getType() const
      {
         return type;
      }

      //
      // ExpCreate_IRExp
      //
      Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type)
      {
         return static_cast<Exp::CRef>(new Exp_IRExp(exp, type, exp->pos));
      }

      //
      // ExpCreate_IRExp
      //
      Exp::CRef ExpCreate_IRExp(IR::Exp const *exp, Type const *type,
         Core::Origin pos)
      {
         return static_cast<Exp::CRef>(new Exp_IRExp(exp, type, pos));
      }

      //
      // ExpCreate_Size
      //
      Exp::CRef ExpCreate_Size(Core::FastU size)
      {
         return ExpCreate_IRExp(
            IR::ExpCreate_ValueRoot(
               IR::Value_Fixed(size, Type::Size->getIRType().tFixed),
               Core::Origin(Core::STR_, 0)),
            Type::Size);
      }
   }
}

// EOF

