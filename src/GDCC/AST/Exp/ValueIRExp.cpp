//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree IR expression values.
//
//-----------------------------------------------------------------------------

#include "ValueIRExp.hpp"

#include "../Arg.hpp"
#include "../Type.hpp"

#include "GDCC/IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_ValueIRExp constructor
      //
      Exp_ValueIRExp::Exp_ValueIRExp(IR::Exp const *exp_, Type const *type_,
         Origin pos_) :
         Super{pos_}, exp{exp_}, type{type_}
      {
      }

      //
      // Exp_ValueIRExp destructor
      //
      Exp_ValueIRExp::~Exp_ValueIRExp()
      {
      }

      //
      // Exp_ValueIRExp::v_genStmnt
      //
      void Exp_ValueIRExp::v_genStmnt(IR::Block &block, Function *fn, Arg const &dst) const
      {
         genStmntMove(block, fn, dst, getArg());
      }

      //
      // Exp_ValueIRExp::v_getIRExp
      //
      IR::Exp::CRef Exp_ValueIRExp::v_getIRExp() const
      {
         return exp;
      }

      //
      // Exp_ValueIRExp::v_getType
      //
      Type::CRef Exp_ValueIRExp::v_getType() const
      {
         return type;
      }

      //
      // ExpCreate_ValueIRExp
      //
      Exp::CRef ExpCreate_ValueIRExp(IR::Exp const *exp, Type const *type)
      {
         return static_cast<Exp::CRef>(new Exp_ValueIRExp(exp, type, exp->pos));
      }

      //
      // ExpCreate_ValueIRExp
      //
      Exp::CRef ExpCreate_ValueIRExp(IR::Exp const *exp, Type const *type, Origin pos)
      {
         return static_cast<Exp::CRef>(new Exp_ValueIRExp(exp, type, pos));
      }

      //
      // ExpCreate_ValueSize
      //
      Exp::CRef ExpCreate_ValueSize(FastU size)
      {
         return ExpCreate_ValueIRExp(
            IR::ExpCreate_ValueRoot(
               IR::Value_Fixed(size, Type::Size->getIRType().tFixed),
               Origin(STR_, 0)),
            Type::Size);
      }
   }
}

// EOF

