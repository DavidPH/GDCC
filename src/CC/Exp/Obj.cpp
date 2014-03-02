//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C object expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Obj.hpp"

#include "AST/Arg.hpp"
#include "AST/Object.hpp"
#include "AST/Storage.hpp"
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
      // Exp_Obj constructor
      //
      Exp_Obj::Exp_Obj(IR::Program &prog_, AST::Object *obj_,
         Core::Origin pos_) :
         Super{pos_},
         obj{obj_},
         prog(prog_)
      {
      }

      //
      // Exp_Obj destructor
      //
      Exp_Obj::~Exp_Obj()
      {
      }

      //
      // Exp_Obj::v_genStmnt
      //
      void Exp_Obj::v_genStmnt(IR::Block &block, AST::Function *fn,
         AST::Arg const &dst) const
      {
         genStmntMove(block, fn, dst, getArg());
      }

      //
      // Exp_Obj::v_getArg
      //
      AST::Arg Exp_Obj::v_getArg() const
      {
         auto type = getType();

         IR::Glyph glyph{&prog, obj->glyph};
         auto addr = IR::ExpCreate_ValueGlyph(glyph, pos);
         auto expt = type->getTypePointer();
         auto exp  = AST::ExpCreate_ValueIRExp(addr, expt, pos);

         return AST::Arg(type, exp);
      }

      //
      // Exp_Obj::v_getType
      //
      AST::Type::CRef Exp_Obj::v_getType() const
      {
         if(!obj->type)
            throw Core::ExceptStr(pos, "object has no type");

         return static_cast<AST::Type::CRef>(obj->type);
      }

      //
      // Exp_Obj::v_isEffect
      //
      bool Exp_Obj::v_isEffect() const
      {
         if(!obj->type)
            return false;

         return obj->type->getQualVola();
      }

      //
      // Exp_Obj::v_isIRExp
      //
      bool Exp_Obj::v_isIRExp() const
      {
         return false;
      }

      //
      // Exp_Obj::v_setRefer
      //
      void Exp_Obj::v_setRefer() const
      {
         obj->point = true;
      }

      //
      // Exp_ObjAut constructor
      //
      Exp_ObjAut::Exp_ObjAut(IR::Program &prog_, AST::Object *obj_,
         Core::Origin pos_) :
         Super{prog_, obj_, pos_}
      {
      }

      //
      // Exp_ObjAut::v_getType
      //
      AST::Type::CRef Exp_ObjAut::v_getType() const
      {
         auto type = Super::v_getType();
         auto qual = type->getQual();

         // If object has explicit address space, keep it.
         if(qual.space.base != IR::AddrBase::Gen)
            return type;

         // Otherwise, select based on needing to be addressable.
         if(obj->point)
            qual.space.base = IR::AddrBase::Loc;
         else
            qual.space.base = IR::AddrBase::LocReg;

         return type->getTypeQual(qual);
      }

      //
      // ExpCreate_Obj
      //
      AST::Exp::CRef ExpCreate_Obj(IR::Program &prog, AST::Object *obj,
         Core::Origin pos)
      {
         if(obj->store == AST::Storage::Auto)
            return static_cast<AST::Exp::CRef>(new Exp_ObjAut(prog, obj, pos));

         return static_cast<AST::Exp::CRef>(new Exp_Obj(prog, obj, pos));
      }
   }
}

// EOF

