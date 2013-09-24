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

#include "Wrapper.hpp"


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
         GDCC_CounterPreamble(GDCC::AST::Type_Bitfield, GDCC::AST::Type_Wrapper);

      public:
         // Type information.
         virtual IR::Type getIRType()    const;
         virtual FastU    getSizeBitsF() const {return bitsF;}
         virtual FastU    getSizeBitsI() const {return bitsI;}
         virtual FastU    getSizeBitsO() const {return bitsO;}

         // Type classification: General classifications.
         virtual bool isTypeBitfield() const {return base->isTypeBitfield();}


         friend Type::CRef Type::getTypeBitfield(FastU bitsF, FastU bitsI, FastU bitsO) const;

      protected:
         Type_Bitfield(Type_Bitfield const &type);
         Type_Bitfield(Type const *base, FastU bitsF, FastU bitsI, FastU bitsO);
         virtual ~Type_Bitfield();

         FastU const bitsF;
         FastU const bitsI;
         FastU const bitsO;

      private:
         mutable Type_Bitfield const *bitNext, *bitPrev;
      };
   }
}

#endif//GDCC__AST__Type__Bitfield_H__

