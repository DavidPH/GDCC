//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C function preamble and prologue statements.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Statement__FuncPre_H__
#define GDCC__CC__Statement__FuncPre_H__

#include "../../CC/Types.hpp"

#include "../../SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Statement_FuncPre
   //
   class Statement_FuncPre : public SR::Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Statement_FuncPre, GDCC::SR::Statement);

   public:
      Scope_Function &scope;


      // Create
      static CRef Create(Core::Origin pos, Scope_Function &scope)
         {return CRef(new This(pos, scope));}

   protected:
      Statement_FuncPre(Core::Origin pos_, Scope_Function &scope_) :
         Super{pos_}, scope(scope_) {}

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx) const;

      virtual bool v_isEffect() const {return true;}
      virtual bool v_isNoAuto() const {return true;}
   };

   //
   // Statement_FuncPro
   //
   class Statement_FuncPro : public SR::Statement
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Statement_FuncPro, GDCC::SR::Statement);

   public:
      Scope_Function &scope;


      // Create
      static CRef Create(Core::Origin pos, Scope_Function &scope)
         {return CRef(new This(pos, scope));}

   protected:
      Statement_FuncPro(Core::Origin pos_, Scope_Function &scope_) :
         Super{pos_}, scope(scope_) {}

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx) const;

      virtual bool v_isEffect() const {return true;}
      virtual bool v_isNoAuto() const {return true;}
   };
}

#endif//GDCC__CC__Statement__FuncPre_H__

