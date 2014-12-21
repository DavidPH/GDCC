//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C enumeration types.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Type__Enum_H__
#define GDCC__CC__Type__Enum_H__

#include "../../CC/Type.hpp"

#include "../../AST/Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Type_Enum
      //
      class Type_Enum final : public AST::Type
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Type_Enum, GDCC::AST::Type);

      public:
         //
         // Data
         //
         class Data
         {
         public:
            Data(Core::String name);

            Core::String const name;
            AST::Type::CPtr    type;

            bool complete : 1;
         };


         // Type information.
         virtual Type::CRef   getBaseType()  const;
                 Data  const &getData()      const {return data;}
         virtual IR::Type     getIRType()    const;
         virtual Core::String getName()      const;
         virtual RankC        getRankC()     const;
         virtual Core::FastU  getSizeAlign() const;
         virtual Core::FastU  getSizeBitsF() const;
         virtual Core::FastU  getSizeBitsI() const;
         virtual bool         getSizeBitsS() const;
         virtual Core::FastU  getSizeBytes() const;
         virtual Core::FastU  getSizePoint() const;
         virtual Core::FastU  getSizeShift() const;
         virtual Core::FastU  getSizeWords() const;

         // Type classification: General classifications.
         virtual bool isTypeComplete() const {return data.complete;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeArith()  const {return true;}
         virtual bool isCTypeEnum()   const {return true;}
         virtual bool isCTypeInteg()  const {return true;}
         virtual bool isCTypeObject() const {return true;}
         virtual bool isCTypeReal()   const {return true;}
         virtual bool isCTypeScalar() const {return true;}

         // Type alteration.
         void setComplete(AST::Type const *type);


         static Ref Create(Core::String name);

      protected:
         Type_Enum(Core::String name);
         virtual ~Type_Enum();

         Data &data;
      };
   }
}

#endif//GDCC__CC__Type__Enum_H__

