//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C structure/union types.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Type__Struct_H__
#define GDCC__CC__Type__Struct_H__

#include "../../CC/Type.hpp"

#include "../../AST/Type.hpp"

#include "../../Core/Array.hpp"
#include "../../Core/Utility.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Type_Struct
      //
      class Type_Struct final : public AST::Type
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Type_Struct, GDCC::AST::Type);

      public:
         //
         // MemberData
         //
         struct MemberData
         {
            MemberData(Core::String name_, AST::Type const *type_,
               Core::FastU addr_, bool anon_) :
               name{name_}, type{type_}, addr{addr_}, anon{anon_} {}

            Core::String    const name;
            AST::Type::CRef const type;
            Core::FastU     const addr;
            bool            const anon : 1;
         };


         // Type information.
         virtual IR::Type     getIRType()    const;
         virtual Core::String getName()      const;
         virtual Core::FastU  getSizeAlign() const;
         virtual Core::FastU  getSizeBytes() const;
         virtual Core::FastU  getSizePoint() const;
         virtual Core::FastU  getSizeShift() const;
         virtual Core::FastU  getSizeWords() const;

         // Type information: Members.
         virtual Member getMember(Core::String name) const;

         // Type classification: General classifications.
         virtual bool isTypeComplete() const {return data.complete;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeAggregat() const {return data.isStruct;}
         virtual bool isCTypeObject()   const {return true;}
         virtual bool isCTypeStruct()   const {return data.isStruct;}
         virtual bool isCTypeUnion()    const {return data.isUnion;}

         // Type alteration.
         void setMembers(MemberData const *memv, std::size_t memc,
            Core::FastU sizeBytes = 0);


         static Ref Create(Core::String name, bool isUnion = false);
         static Ref CreateUnion(Core::String name);

      protected:
         //
         // Data
         //
         struct Data
         {
            Data(Core::String name, bool isUnion);

            Core::Array<MemberData> memb;
            Core::String      const name;

            Core::FastU sizeAlign;
            Core::FastU sizeBytes;
            Core::FastU sizePoint;
            Core::FastU sizeShift;
            Core::FastU sizeWords;

            bool       complete : 1;
            bool const isStruct : 1;
            bool const isUnion  : 1;
         };


         Type_Struct(Core::String name, bool isUnion);
         virtual ~Type_Struct();

         Data &data;
      };
   }
}

#endif//GDCC__CC__Type__Struct_H__

