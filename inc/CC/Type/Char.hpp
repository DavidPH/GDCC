//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C char type.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Type__Char_H__
#define GDCC__CC__Type__Char_H__

#include "../../CC/Type.hpp"

#include "../../SR/Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Type_Char
      //
      class Type_Char final : public SR::Type
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Type_Char, GDCC::SR::Type);

      public:
         // Type information.
         virtual IR::Type      getIRType()      const;
         virtual RankC         getRankC()       const;
         virtual Core::FastU   getSizeAlign()   const {return 1;}
         virtual Core::FastU   getSizeBitsF()   const {return 0;}
         virtual Core::FastU   getSizeBitsI()   const;
         virtual bool          getSizeBitsS()   const;
         virtual Core::FastU   getSizeBytes()   const {return 1;}
         virtual Core::FastU   getSizePoint()   const {return 1;}
         virtual Core::FastU   getSizeShift()   const {return 1;}
         virtual Core::FastU   getSizeWords()   const {return 1;}

         // Type classification: General classifications.
         virtual bool isTypeComplete() const {return true;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeArith()    const {return true;}
         virtual bool isCTypeChar()     const {return true;}
         virtual bool isCTypeInteg()    const {return true;}
         virtual bool isCTypeObject()   const {return true;}
         virtual bool isCTypeReal()     const {return true;}
         virtual bool isCTypeScalar()   const {return true;}


         friend SR::Type::CRef GetTypeChar();

      protected:
         Type_Char() = default;
      };
   }
}

#endif//GDCC__CC__Type__Char_H__

