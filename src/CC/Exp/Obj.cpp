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
         obj->used = true;
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
      void Exp_Obj::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         GenStmnt_Move(this, ctx, dst, getArg());
      }

      //
      // Exp_Obj::v_getArg
      //
      AST::Arg Exp_Obj::v_getArg() const
      {
         auto type = getType();

         IR::Glyph glyph{&prog, obj->glyph};
         auto addr = IR::ExpCreate_Glyph(glyph, pos);
         auto expt = type->getTypePointer();
         auto exp  = AST::ExpCreate_IRExp(addr, expt, pos);

         return AST::Arg(type, exp);
      }

      //
      // Exp_Obj::v_getIRExp
      //
      IR::Exp::CRef Exp_Obj::v_getIRExp() const
      {
         if(!obj->type)
            throw Core::ExceptStr(pos, "object has no type");

         if(!obj->type->getQualCons() || !obj->init)
            return Super::v_getIRExp();

         return obj->init->getIRExp();
      }

      //
      // Exp_Obj::v_getObject
      //
      AST::Object::Ref Exp_Obj::v_getObject() const
      {
         return obj;
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
         if(!obj->type)
            return false;

         return obj->type->getQualCons() && obj->init && obj->init->isIRExp();
      }

      //
      // Exp_Obj::v_isNoAuto
      //
      bool Exp_Obj::v_isNoAuto() const
      {
         if(!obj->type)
            return false;

         return getType()->getQualAddr().base != IR::AddrBase::Loc;
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

         // If object has explicit address space, keep it.
         if(type->getQualAddr().base != IR::AddrBase::Gen)
            return type;

         // Otherwise, select based on needing to be addressable.
         if(obj->refer)
            return type->getTypeArrayQualAddr(IR::AddrBase::Loc);
         else
            return type->getTypeArrayQualAddr(IR::AddrBase::LocReg);
      }

      //
      // ExpCreate_Obj
      //
      AST::Exp::CRef ExpCreate_Obj(IR::Program &prog, AST::Object *obj,
         Core::Origin pos)
      {
         if(obj->store == AST::Storage::Auto)
            return Exp_ObjAut::Create(prog, obj, pos);

         return Exp_Obj::Create(prog, obj, pos);
      }
   }
}

// EOF

