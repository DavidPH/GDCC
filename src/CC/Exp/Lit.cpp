//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C literal expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/CallType.hpp"
#include "IR/Exp.hpp"
#include "IR/Value.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_LitInt
      //
      AST::Exp::CRef ExpCreate_LitInt(AST::Type const *t, Core::Integ const &i,
         Core::Origin pos)
      {
         auto val = IR::Value_Fixed(i, t->getIRType().tFixed);
         auto exp = IR::ExpCreate_Value(std::move(val), pos);
         return AST::ExpCreate_IRExp(exp, t, pos);
      }

      //
      // ExpCreate_LitInt
      //
      AST::Exp::CRef ExpCreate_LitInt(AST::Type const *t, Core::Integ &&i,
         Core::Origin pos)
      {
         auto val = IR::Value_Fixed(std::move(i), t->getIRType().tFixed);
         auto exp = IR::ExpCreate_Value(std::move(val), pos);
         return AST::ExpCreate_IRExp(exp, t, pos);
      }

      //
      // ExpCreate_LitNul
      //
      AST::Exp::CRef ExpCreate_LitNul(AST::Type const *type, Core::Origin pos)
      {
         // Pointer.
         if(type->isTypePointer())
         {
            auto base = type->getBaseType();

            // Pointer to function.
            if(base->isCTypeFunction())
            {
               IR::Value_Funct val;
               val.vtype.callT = IR::GetCallTypeIR(base->getCallType());

               if(Platform::IsZeroNull_Funct(val.vtype.callT))
                  val.value = 0;
               else
                  val.value = static_cast<Core::FastU>(-1);

               auto exp = IR::ExpCreate_Value(std::move(val), pos);
               return AST::ExpCreate_IRExp(exp, type, pos);
            }

            // Pointer to object.
            if(base->isCTypeObject())
            {
               IR::Value_Point val;
               val.vtype = type->getIRType().tPoint;
               val.addrB = base->getQualAddr().base;
               val.addrN = base->getQualAddr().name;

               if(Platform::IsZeroNull_Point(val.addrB))
                  val.value = 0;
               else
                  val.value = static_cast<Core::FastU>(-1);

               auto exp = IR::ExpCreate_Value(std::move(val), pos);
               return AST::ExpCreate_IRExp(exp, type, pos);
            }

            // Pointer to string entry.
            if(base->isTypeStrEnt())
            {
               IR::Value_StrEn val;
               val.vtype = {};

               if(Platform::IsZeroNull_StrEn())
                  val.value = 0;
               else
                  val.value = static_cast<Core::FastU>(-1);

               auto exp = IR::ExpCreate_Value(std::move(val), pos);
               return AST::ExpCreate_IRExp(exp, type, pos);
            }

            // Pointer to dynamic jump target.
            if(base->isTypeLabel())
            {
               IR::Value_DJump val;
               val.vtype = {};

               if(Platform::IsZeroNull_DJump())
                  val.value = 0;
               else
                  val.value = static_cast<Core::FastU>(-1);

               auto exp = IR::ExpCreate_Value(std::move(val), pos);
               return AST::ExpCreate_IRExp(exp, type, pos);
            }
         }

         throw Core::ExceptStr(pos, "invalid LitNul type");
      }
   }
}

// EOF

