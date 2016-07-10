//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ." and "operator ->" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Mem.hpp"

#include "CC/Exp/Add.hpp"

#include "SR/Arg.hpp"
#include "SR/Type.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Mem::v_genStmnt
      //
      void Exp_Mem::v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const
      {
         SR::GenStmnt_Move(this, ctx, dst, getArg());
      }

      //
      // Exp_Mem::v_getArg
      //
      SR::Arg Exp_Mem::v_getArg() const
      {
         try
         {
            // Lookup member.
            auto mem = expL->getType()->getMember(expR);

            // Calculate pointer offset.
            auto off  = mem.addr / mem.type->getSizeShift();
            auto expO = ExpCreate_LitInt(SR::Type::Size, off, pos);

            // Create pointer expression.
            auto type = mem.type->getTypePointer();
            auto addr = ExpConvert_Pointer(type, expL->getArg().data, pos);
                 addr = Exp_AddPtrRaw::Create(type, addr, expO, pos);

            // Convert pointer into an arg.
            return {mem.type, addr};
         }
         catch(SR::TypeError const &)
         {
            throw Core::ExceptStr(pos, "invalid member");
         }
      }

      //
      // Exp_Mem::v_getType
      //
      SR::Type::CRef Exp_Mem::v_getType() const
      {
         try
         {
            return expL->getType()->getMember(expR).type;
         }
         catch(SR::TypeError const &)
         {
            throw Core::ExceptStr(pos, "invalid member");
         }
      }

      //
      // Exp_Mem::v_isEffect
      //
      bool Exp_Mem::v_isEffect() const
      {
         return expL->isEffect();
      }

      //
      // Exp_Mem::v_isNoAuto
      //
      bool Exp_Mem::v_isNoAuto() const
      {
         return expL->isNoAuto();
      }

      //
      // Exp_Mem::v_setRefer
      //
      void Exp_Mem::v_setRefer() const
      {
         expL->setRefer();
      }

      //
      // ExpCreate_Mem
      //
      SR::Exp::CRef ExpCreate_Mem(SR::Exp const *l, Core::String r, Core::Origin pos)
      {
         return Exp_Mem::Create(l, r, pos);
      }

      //
      // ExpCreate_MemPt
      //
      SR::Exp::CRef ExpCreate_MemPt(SR::Exp const *l, Core::String r, Core::Origin pos)
      {
         return Exp_Mem::Create(ExpCreate_Deref(l, pos), r, pos);
      }
   }
}

// EOF

