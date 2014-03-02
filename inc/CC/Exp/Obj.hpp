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

#ifndef GDCC__CC__Exp__Obj_H__
#define GDCC__CC__Exp__Obj_H__

#include "../../CC/Exp.hpp"

#include "../../AST/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Obj
      //
      class Exp_Obj : public AST::Exp
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_Obj, GDCC::AST::Exp);

      public:
         Core::CounterRef<AST::Object> const obj;
         IR::Program                        &prog;


         friend AST::Exp::CRef ExpCreate_Obj(IR::Program &prog,
            AST::Object *obj, Core::Origin pos);

      protected:
         Exp_Obj(IR::Program &prog, AST::Object *obj, Core::Origin pos);
         virtual ~Exp_Obj();

         virtual void v_genStmnt(IR::Block &block, AST::Function *fn,
            AST::Arg const &dst) const;

         virtual AST::Arg v_getArg() const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;

         virtual void v_setRefer() const;
      };

      //
      // Exp_ObjAut
      //
      // Special handling for automatic storage duration objects.
      //
      class Exp_ObjAut : public Exp_Obj
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_ObjAut, GDCC::CC::Exp_Obj);

      public:
         friend AST::Exp::CRef ExpCreate_Obj(IR::Program &prog,
            AST::Object *obj, Core::Origin pos);

      protected:
         Exp_ObjAut(IR::Program &prog, AST::Object *obj, Core::Origin pos);

         virtual TypeCRef v_getType() const;
      };
   }
}

#endif//GDCC__CC__Exp__Obj_H__

