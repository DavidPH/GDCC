//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression bitfield type handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Type__Bitfield_H__
#define GDCC__AST__Type__Bitfield_H__

#include "../../AST/Type/Wrapper.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Type_Bitfield
      //
      class Type_Bitfield final : public Type_Wrapper
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Type_Bitfield, GDCC::AST::Type_Wrapper);

      public:
         // Type information.
         virtual IR::Type    getIRType()    const;
         virtual Core::FastU getSizeBitsF() const {return bitsF;}
         virtual Core::FastU getSizeBitsI() const {return bitsI;}
         virtual Core::FastU getSizeBitsO() const {return bitsO;}

         // Type classification: General classifications.
         virtual bool isTypeBitfield() const {return base->isTypeBitfield();}


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
}

#endif//GDCC__AST__Type__Bitfield_H__

