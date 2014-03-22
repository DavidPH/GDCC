//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C function expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Func.hpp"

#include "AST/Arg.hpp"
#include "AST/Function.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Func constructor
      //
      Exp_Func::Exp_Func(IR::Program &prog_, AST::Function *fn_,
         Core::Origin pos_) :
         Super{pos_},
         fn{fn_},
         prog{prog_}
      {
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
      void Exp_Func::v_genStmnt(AST::GenStmntCtx const &,
         AST::Arg const &) const
      {
         throw Core::ExceptStr(pos, "genStmnt on function");
      }

      //
      // Exp_Func::v_getArg
      //
      AST::Arg Exp_Func::v_getArg() const
      {
         if(!fn->type)
            throw Core::ExceptStr(pos, "function has no type");

         IR::Glyph glyph{&prog, fn->glyph};
         auto addr = IR::ExpCreate_ValueGlyph(glyph, pos);
         auto type = fn->type->getTypePointer();
         auto exp  = AST::ExpCreate_IRExp(addr, type, pos);

         return AST::Arg(fn->type, exp);
      }

      //
      // Exp_Func::v_getType
      //
      AST::Type::CRef Exp_Func::v_getType() const
      {
         if(!fn->type)
            throw Core::ExceptStr(pos, "function has no type");

         return static_cast<AST::Type::CRef>(fn->type);
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
      // ExpCreate_Func
      //
      AST::Exp::CRef ExpCreate_Func(IR::Program &prog, AST::Function *fn,
         Core::Origin pos)
      {
         return static_cast<AST::Exp::CRef>(new Exp_Func(prog, fn, pos));
      }
   }
}

// EOF

