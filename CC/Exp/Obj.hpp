//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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

#include "../../SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Exp_Obj
   //
   class Exp_Obj : public SR::Exp
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Exp_Obj, GDCC::SR::Exp);

   public:
      ObjectRef const obj;
      IR::Program    &prog;


      // Create
      static CRef Create(IR::Program &prog, SR::Object *obj, Core::Origin pos)
         {return CRef(new This(prog, obj, pos));}

   protected:
      Exp_Obj(IR::Program &prog, SR::Object *obj, Core::Origin pos);
      virtual ~Exp_Obj();

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual SR::Arg v_getArg() const;

      virtual IRExpCRef v_getIRExp() const;

      virtual ObjectRef v_getObject() const;

      virtual TypeCRef v_getType() const;

      virtual bool v_isEffect() const;

      virtual bool v_isIRExp() const;

      virtual bool v_isObject() const {return true;}

      virtual bool v_isNoAuto() const;
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
      // Create
      static CRef Create(IR::Program &prog, SR::Object *obj, Core::Origin pos)
         {return CRef(new This(prog, obj, pos));}

   protected:
      Exp_ObjAut(IR::Program &prog, SR::Object *obj, Core::Origin pos);

      virtual TypeCRef v_getType() const;
   };
}

#endif//GDCC__CC__Exp__Obj_H__

