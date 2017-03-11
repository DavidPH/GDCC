//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2017 David Hill
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

#include "../../Core/Array.hpp"

#include "../../SR/Exp.hpp"
#include "../../SR/Type.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // StructPropArg
   //
   class StructPropArg
   {
   public:
      //
      // Special
      //
      enum Special
      {
         ThisPtr,
         ThisRef,
         Value,
      };


      StructPropArg(SR::Exp const *val_) : val{val_}, spec{Value} {}
      StructPropArg(Special spec_) : val{nullptr}, spec{spec_} {}

      SR::Exp::CPtr val;
      Special       spec;
   };

   //
   // StructProp
   //
   class StructProp
   {
   public:
      SR::Exp::CPtr              func;
      Core::Array<StructPropArg> args;
   };

   //
   // Type_Div
   //
   // A special structure-like type for storing the result of __div.
   //
   class Type_Div final : public SR::Type
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Type_Div, GDCC::SR::Type);

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


      static CRef Get(SR::Type const *type);

   protected:
      Type_Div(SR::Type const *t) : type{t} {}

      SR::Type::CRef const type;
   };

   //
   // Type_Struct
   //
   class Type_Struct final : public SR::Type
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Type_Struct, GDCC::SR::Type);

   public:
      //
      // MemberData
      //
      class MemberData
      {
      public:
         MemberData(Core::String name_, SR::Type const *type_,
            Core::FastU addr_, bool anon_) :
            name{name_}, type{type_}, addr{addr_}, anon{anon_} {}

         Core::String   const name;
         SR::Type::CRef const type;
         Core::FastU    const addr;
         bool           const anon : 1;
      };

      //
      // PropData
      //
      class PropData
      {
      public:
         Core::String   const name;
         SR::Type::CRef const type;
         Core::FastU    const addr;

         StructProp const propCall;
         StructProp const propGet;
         StructProp const propSet;
      };

      //
      // Prop
      //
      class Prop
      {
      public:
         Core::FastU addr;
         Type::CRef  type;

         StructProp const *propCall;
         StructProp const *propGet;
         StructProp const *propSet;
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
         Core::Array<PropData>   prop;
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
              Prop   getProp(Core::String name) const;
              bool   hasProp(Core::String name) const;

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
      void setMembers(MemberData const *memv, std::size_t memc,
         PropData const *propv, std::size_t propc, Core::FastU sizeBytes = 0);


      static Ref Create(Core::String name, bool isUnion = false);
      static Ref CreateUnion(Core::String name);

   protected:
      Type_Struct(Core::String name, bool isUnion);
      virtual ~Type_Struct();

      Data &data;
   };
}

#endif//GDCC__CC__Type__Struct_H__

