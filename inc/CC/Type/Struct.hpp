//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
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


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Type_Div
      //
      // A special structure-like type for storing the result of __div.
      //
      class Type_Div final : public AST::Type
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Type_Div, GDCC::AST::Type);

      public:
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
         virtual bool isTypeComplete() const {return true;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeAggregat() const {return true;}
         virtual bool isCTypeObject()   const {return true;}
         virtual bool isCTypeStruct()   const {return true;}


         static CRef Get(AST::Type const *type);

      protected:
         Type_Div(AST::Type const *t) : type{t} {}

         AST::Type::CRef const type;
      };

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
         class MemberData
         {
         public:
            MemberData(Core::String name_, AST::Type const *type_,
               Core::FastU addr_, bool anon_) :
               name{name_}, type{type_}, addr{addr_}, anon{anon_} {}

            Core::String    const name;
            AST::Type::CRef const type;
            Core::FastU     const addr;
            bool            const anon : 1;
         };

         //
         // Data
         //
         class Data
         {
         public:
            Data(Core::String name, bool isUnion, Type_Struct *type);
            ~Data();

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

         private:
            Data();

            Data *next, *prev;
            Type_Struct *type;


            static void Cleanup();

            static Data Head;
         };


         // Type information.
                 Data  const &getData()      const {return data;}
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
         Type_Struct(Core::String name, bool isUnion);
         virtual ~Type_Struct();

         Data &data;
      };
   }
}

#endif//GDCC__CC__Type__Struct_H__

