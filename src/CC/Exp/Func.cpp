//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C function expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Func.hpp"

#include "CC/Factory.hpp"

#include "Core/Exception.hpp"

#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"

#include "SR/Arg.hpp"
#include "SR/Function.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_Func constructor
   //
   Exp_Func::Exp_Func(IR::Program &prog_, SR::Function *fn_, Core::Origin pos_) :
      Super{pos_},
      fn{fn_},
      prog(prog_)
   {
      fn->used = true;
   }

   //
   // Exp_Func destructor
   //
   Exp_Func::~Exp_Func()
   {
   }

   //
   // Exp_Func::v_genStmnt
   //
   void Exp_Func::v_genStmnt(SR::GenStmntCtx const &, SR::Arg const &) const
   {
      Core::Error(pos, "genStmnt on function");
   }

   //
   // Exp_Func::v_getArg
   //
   SR::Arg Exp_Func::v_getArg() const
   {
      if(!fn->type)
         Core::Error(pos, "function has no type");

      IR::Glyph glyph{&prog, fn->glyph};
      auto addr = IR::ExpCreate_Glyph(glyph, pos);
      auto type = fn->type->getTypePointer();
      auto exp  = SR::ExpCreate_IRExp(addr, type, pos);

      return SR::Arg(fn->type, exp);
   }

   //
   // Exp_Func::v_getFunction
   //
   SR::Function::Ref Exp_Func::v_getFunction() const
   {
      return fn;
   }

   //
   // Exp_Func::v_getType
   //
   SR::Type::CRef Exp_Func::v_getType() const
   {
      if(!fn->type)
         Core::Error(pos, "function has no type");

      return static_cast<SR::Type::CRef>(fn->type);
   }

   //
   // Exp_Func::v_isEffect
   //
   bool Exp_Func::v_isEffect() const
   {
      return false;
   }

   //
   // Exp_Func::v_isIRExp
   //
   bool Exp_Func::v_isIRExp() const
   {
      return false;
   }

   //
   // Factory::expCreate_Func
   //
   SR::Exp::CRef Factory::expCreate_Func(IR::Program &prog, SR::Function *fn,
      Core::Origin pos)
   {
      return Exp_Func::Create(prog, fn, pos);
   }
}

// EOF

