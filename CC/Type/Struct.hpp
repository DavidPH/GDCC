//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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
         Member,
         ThisPtr,
         ThisRef,
         Value,
      };


      StructPropArg(Core::String name_) : name{name_}, val{nullptr}, spec{Member} {}
      StructPropArg(SR::Exp const *val_) : name{nullptr}, val{val_}, spec{Value} {}
      StructPropArg(Special spec_) : name{nullptr}, val{nullptr}, spec{spec_} {}

      Core::String  name;
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

         Core::String   name;
         SR::Type::CRef type;
         Core::FastU    addr;
         bool           anon : 1;
      };

      //
      // Prop
      //
      class Prop
      {
      public:
         StructProp const *prop;
         StructProp const *propAdd2;
         StructProp const *propAdd3;
         StructProp const *propAddEq;
         StructProp const *propAndEq;
         StructProp const *propDivEq;
         StructProp const *propEqual;
         StructProp const *propModEq;
         StructProp const *propMulEq;
         StructProp const *propOrIEq;
         StructProp const *propOrXEq;
         StructProp const *propParen;
         StructProp const *propShLEq;
         StructProp const *propShREq;
         StructProp const *propSub2;
         StructProp const *propSub3;
         StructProp const *propSubEq;
      };

      //
      // PropData
      //
      class PropData
      {
      public:
         Prop getProp() const;

         Core::String name;

         StructProp prop;
         StructProp propAdd2;
         StructProp propAdd3;
         StructProp propAddEq;
         StructProp propAndEq;
         StructProp propDivEq;
         StructProp propEqual;
         StructProp propModEq;
         StructProp propMulEq;
         StructProp propOrIEq;
         StructProp propOrXEq;
         StructProp propParen;
         StructProp propShLEq;
         StructProp propShREq;
         StructProp propSub2;
         StructProp propSub3;
         StructProp propSubEq;
      };

      //
      // Data
      //
      class Data
      {
      public:
         Data(Core::String name, bool isUnion, Type_Struct *type);
         ~Data();

         Core::Array<MemberData const> memb;
         Core::Array<PropData   const> prop;
         Core::String           const  name;

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

