//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ." and "operator ->" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Add.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_Mem
      //
      AST::Exp::CRef ExpCreate_Mem(AST::Exp const *l,
         Core::String r, Core::Origin pos)
      {
         AST::Exp::CRef expL{l};

         auto argL  = expL->getArg();
         auto typeL = expL->getType();

         try
         {
            // Lookup member.
            auto mem = typeL->getMember(r);

            // Calculate pointer offset.
            auto off  = mem.addr / mem.type->getSizeShift();
            auto expR = ExpCreate_LitInt(AST::Type::Size, off, pos);

            // Create pointer expression.
            typeL = mem.type->getTypePointer();
            expL  = ExpConvert_Pointer(typeL, argL.data, pos);
            expL  = Exp_AddPtrRaw::Create(typeL, expL, expR, pos);

            // Convert pointer into an arg.
            return AST::ExpCreate_Arg(AST::Arg(mem.type, expL), pos);
         }
         catch(AST::TypeError const &)
         {
            throw Core::ExceptStr(pos, "invalid member");
         }
      }

      //
      // ExpCreate_MemPt
      //
      AST::Exp::CRef ExpCreate_MemPt(AST::Exp const *l,
         Core::String r, Core::Origin pos)
      {
         return ExpCreate_Mem(ExpCreate_Deref(l, pos), r, pos);
      }
   }
}

// EOF

