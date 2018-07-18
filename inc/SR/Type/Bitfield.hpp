//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation expression bitfield type handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Type__Bitfield_H__
#define GDCC__SR__Type__Bitfield_H__

#include "../../SR/Type/Wrapper.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Type_Bitfield
   //
   class Type_Bitfield final : public Type_Wrapper
   {
      GDCC_Core_CounterPreamble(
         GDCC::SR::Type_Bitfield, GDCC::SR::Type_Wrapper);

   public:
      // Type information.
      virtual IR::Type    getIRType()    const;
      virtual Core::FastU getSizeBitsF() const {return bitsF;}
      virtual Core::FastU getSizeBitsI() const {return bitsI;}
      virtual Core::FastU getSizeBitsO() const {return bitsO;}

      // Type classification: General classifications.
      virtual bool isTypeBitfield() const {return true;}


      friend Type::CRef Type::getTypeBitfield(Core::FastU bitsF,
         Core::FastU bitsI, Core::FastU bitsO) const;

   protected:
      Type_Bitfield(Type_Bitfield const &type);
      Type_Bitfield(Type const *base, Core::FastU bitsF, Core::FastU bitsI,
         Core::FastU bitsO);
      virtual ~Type_Bitfield();

      Core::FastU const bitsF;
      Core::FastU const bitsI;
      Core::FastU const bitsO;

   private:
      mutable Type_Bitfield const *bitNext, *bitPrev;
   };
}

#endif//GDCC__SR__Type__Bitfield_H__

