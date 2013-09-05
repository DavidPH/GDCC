//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: unary base and utilities.
//
//-----------------------------------------------------------------------------

#include "Unary.hpp"

#include "../IArchive.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      GDCC_IR_Exp_UnaryImplCreate(Add);
      GDCC_IR_Exp_UnaryImplCreate(Not);
      GDCC_IR_Exp_UnaryImplCreate(Sub);

      Exp::Ref ExpCreate_UnaryCst(Type const &t, Exp *e)
         {return static_cast<Exp::Ref>(new Exp_UnaryCst(t, e, e->pos));}

      Exp::Ref ExpCreate_UnaryCst(Type const &t, Exp *e, Origin pos)
         {return static_cast<Exp::Ref>(new Exp_UnaryCst(t, e, pos));}

      Exp::Ref ExpCreate_UnaryCst(Type &&t, Exp *e)
         {return static_cast<Exp::Ref>(new Exp_UnaryCst(std::move(t), e, e->pos));}

      Exp::Ref ExpCreate_UnaryCst(Type &&t, Exp *e, Origin pos)
         {return static_cast<Exp::Ref>(new Exp_UnaryCst(std::move(t), e, pos));}

      Exp::Ref ExpGetIR_UnaryCst(IArchive &in)
         {return static_cast<Exp::Ref>(new Exp_UnaryCst(in));}

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
      // Exp_UnaryAdd::v_getValue
      //
      Value Exp_UnaryAdd::v_getValue() const
      {
         return +exp->getValue();
      }

      //
      // Exp_UnaryCst constructor
      //
      Exp_UnaryCst::Exp_UnaryCst(IArchive &in) : Super{in}, type{in}
      {
      }

      //
      // Exp_UnaryCst::v_getValue
      //
      Value Exp_UnaryCst::v_getValue() const
      {
         auto e = exp->getValue();
         auto t = e.getType();

         switch(t.t)
         {
         case TypeBase::Fixed:
            switch(type.t)
            {
            case TypeBase::Fixed: return Value_Fixed(e.vFixed, type.tFixed);
            case TypeBase::Float: return Value_Float(e.vFixed, type.tFloat);
            default: return Value_Empty();
            }

         case TypeBase::Float:
            switch(type.t)
            {
            case TypeBase::Fixed: return Value_Fixed(e.vFloat, type.tFixed);
            case TypeBase::Float: return Value_Float(e.vFloat, type.tFloat);
            default: return Value_Empty();
            }

         default: return Value_Empty();
         }
      }

      //
      // Exp_UnaryCst::v_putIR
      //
      OArchive &Exp_UnaryCst::v_putIR(OArchive &out) const
      {
         return Super::v_putIR(out) << type;
      }

      //
      // Exp_UnaryNot::v_getValue
      //
      Value Exp_UnaryNot::v_getValue() const
      {
         return ~exp->getValue();
      }

      //
      // Exp_UnarySub::v_getValue
      //
      Value Exp_UnarySub::v_getValue() const
      {
         return -exp->getValue();
      }
   }
}

// EOF


