//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati target information and handling class.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Arg.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace DGE
      {
         //
         // Info::CheckArg
         //
         void Info::CheckArg(IR::Arg const &arg, Core::Origin const &pos)
         {
            switch(arg.a)
            {
            case IR::ArgBase::Lit: break;
            case IR::ArgBase::Nul: break;
            case IR::ArgBase::Stk: break;

            case IR::ArgBase::LocReg:
               CheckArgB(*arg.aLocReg.idx, IR::ArgBase::Lit, pos);
               break;

            case IR::ArgBase::Sta:
               CheckArg(*arg.aSta.idx, pos);
               break;

            default:
               std::cerr << "ERROR: " << pos << ": DGE cannot use Arg: "
                  << arg.a << '\n';
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::GetWord
         //
         Core::FastU Info::GetWord(IR::Arg_Lit const &arg, Core::FastU w)
         {
            return GetWord(arg.value, arg.off + w);
         }

         //
         // Info::GetWord
         //
         Core::FastU Info::GetWord(IR::Exp const *exp, Core::FastU w)
         {
            auto val = exp->getValue();

            switch(val.v)
            {
            case IR::ValueBase::DJump: return w ? 0 : val.vDJump.value;
            case IR::ValueBase::Fixed: return GetWord_Fixed(val.vFixed, w);
            case IR::ValueBase::Funct: return w ? 0 : val.vFunct.value;
            case IR::ValueBase::Point: return w ? 0 : val.vPoint.value;
            case IR::ValueBase::StrEn: return w ? 0 : val.vStrEn.value;

            case IR::ValueBase::Empty:
               throw Core::ExceptStr(exp->pos, "bad GetWord Value: Empty");
            case IR::ValueBase::Array:
               throw Core::ExceptStr(exp->pos, "bad GetWord Value: Array");
            case IR::ValueBase::Assoc:
               throw Core::ExceptStr(exp->pos, "bad GetWord Value: Assoc");
            case IR::ValueBase::Float:
               throw Core::ExceptStr(exp->pos, "bad GetWord Value: Float");
            case IR::ValueBase::Tuple:
               throw Core::ExceptStr(exp->pos, "bad GetWord Value: Tuple");
            case IR::ValueBase::Union:
               throw Core::ExceptStr(exp->pos, "bad GetWord Value: Union");
            }

            throw Core::ExceptStr(exp->pos, "bad GetWord Value");
         }

         //
         // GetWord_Fixed
         //
         Core::FastU Info::GetWord_Fixed(IR::Value_Fixed const &val, Core::FastU w)
         {
            Core::FastU valI;

            if(w)
            {
               if(val.vtype.bitsS)
                  valI = Core::NumberCast<Core::FastI>(val.value >> (w * 32));
               else
                  valI = Core::NumberCast<Core::FastU>(val.value >> (w * 32));
            }
            else
            {
               if(val.vtype.bitsS)
                  valI = Core::NumberCast<Core::FastI>(val.value);
               else
                  valI = Core::NumberCast<Core::FastU>(val.value);
            }

            return valI & 0xFFFFFFFF;
         }
      }
   }
}

// EOF

