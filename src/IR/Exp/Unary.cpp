//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: unary base and utilities.
//
//-----------------------------------------------------------------------------

#include "IR/Exp/Unary.hpp"

#include "IR/IArchive.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      GDCC_IR_Exp_UnaryImplCreate(Inv)
      GDCC_IR_Exp_UnaryImplCreate(Neg)

      Exp::CRef ExpCreate_Cst(Type const &t, Exp const *e)
         {return static_cast<Exp::CRef>(new Exp_Cst(t, e, e->pos));}

      Exp::CRef ExpCreate_Cst(Type const &t, Exp const *e, Core::Origin pos)
         {return static_cast<Exp::CRef>(new Exp_Cst(t, e, pos));}

      Exp::CRef ExpCreate_Cst(Type &&t, Exp const *e)
         {return static_cast<Exp::CRef>(new Exp_Cst(std::move(t), e, e->pos));}

      Exp::CRef ExpCreate_Cst(Type &&t, Exp const *e, Core::Origin pos)
         {return static_cast<Exp::CRef>(new Exp_Cst(std::move(t), e, pos));}

      Exp::CRef ExpGetIR_Cst(IArchive &in)
         {return static_cast<Exp::CRef>(new Exp_Cst(in));}

      //
      // Exp_Unary constructor
      //
      Exp_Unary::Exp_Unary(IArchive &in) : Super{in}, exp{GetIR(in, exp)}
      {
      }

      //
      // Exp_Unary::v_putIR
      //
      OArchive &Exp_Unary::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << exp;
      }

      //
      // Exp_Cst constructor
      //
      Exp_Cst::Exp_Cst(IArchive &in) : Super{in}, type{in}
      {
      }

      //
      // Exp_Cst::v_getValue
      //
      Value Exp_Cst::v_getValue() const
      {
         auto e = exp->getValue();
         auto t = e.getType();

         if(t == type)
            return e;

         switch(t.t)
         {
         case TypeBase::DJump:
            switch(type.t)
            {
            case TypeBase::Fixed: return Value_Fixed(std::move(e.vDJump), type.tFixed);
            default: throw Core::ExceptStr(pos, "bad Exp_Cst from DJump");
            }

         case TypeBase::Fixed:
            switch(type.t)
            {
            case TypeBase::DJump: return Value_DJump(std::move(e.vFixed), type.tDJump);
            case TypeBase::Fixed: return Value_Fixed(std::move(e.vFixed), type.tFixed);
            case TypeBase::Float: return Value_Float(std::move(e.vFixed), type.tFloat);
            case TypeBase::Funct: return Value_Funct(std::move(e.vFixed), type.tFunct);
            case TypeBase::Point: return Value_Point(std::move(e.vFixed), type.tPoint);
            case TypeBase::StrEn: return Value_StrEn(std::move(e.vFixed), type.tStrEn);
            default: throw Core::ExceptStr(pos, "bad Exp_Cst from Fixed");
            }

         case TypeBase::Float:
            switch(type.t)
            {
            case TypeBase::Fixed: return Value_Fixed(std::move(e.vFloat), type.tFixed);
            case TypeBase::Float: return Value_Float(std::move(e.vFloat), type.tFloat);
            default: throw Core::ExceptStr(pos, "bad Exp_Cst from Float");
            }

         case TypeBase::Funct:
            switch(type.t)
            {
            case TypeBase::Fixed: return Value_Fixed(std::move(e.vFunct), type.tFixed);
            case TypeBase::Funct: return Value_Funct(std::move(e.vFunct), type.tFunct);
            default: throw Core::ExceptStr(pos, "bad Exp_Cst from Funct");
            }

         case TypeBase::Point:
            switch(type.t)
            {
            case TypeBase::Fixed: return Value_Fixed(std::move(e.vPoint), type.tFixed);
            case TypeBase::Point: return Value_Point(std::move(e.vPoint), type.tPoint);
            default: throw Core::ExceptStr(pos, "bad Exp_Cst from Point");
            }

         case TypeBase::StrEn:
            switch(type.t)
            {
            case TypeBase::Fixed: return Value_Fixed(std::move(e.vStrEn), type.tFixed);
            default: throw Core::ExceptStr(pos, "bad Exp_Cst from StrEn");
            }

         default: throw Core::ExceptStr(pos, "bad Exp_Cst");
         }
      }

      //
      // Exp_Cst::v_putIR
      //
      OArchive &Exp_Cst::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << type;
      }

      //
      // Exp_Inv::v_getValue
      //
      Value Exp_Inv::v_getValue() const
      {
         return ~exp->getValue();
      }

      //
      // Exp_Neg::v_getValue
      //
      Value Exp_Neg::v_getValue() const
      {
         return -exp->getValue();
      }
   }
}

// EOF


