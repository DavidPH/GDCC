//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree expression type handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Type_H__
#define GDCC__AST__Type_H__

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"

#include "../IR/Addr.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_AST_Type_Insert
//
#define GDCC_AST_Type_Insert(name) \
   if(base->name##Type) \
   { \
      name##Next = base->name##Type; \
      name##Prev = base->name##Type->name##Prev; \
      \
      name##Next->name##Prev = this; \
      name##Prev->name##Next = this; \
   } \
   else \
   { \
      base->name##Type = this; \
      \
      name##Next = this; \
      name##Prev = this; \
   }

//
// GDCC_AST_Type_Unlink
//
#define GDCC_AST_Type_Unlink(name) \
   if(base->name##Type == this) \
   { \
      if(name##Next == this) \
      { \
         base->name##Type = nullptr; \
      } \
      else \
      { \
         base->name##Type = name##Next; \
         \
         name##Next->name##Prev = name##Prev; \
         name##Prev->name##Next = name##Next; \
      } \
   } \
   else \
   { \
      name##Next->name##Prev = name##Prev; \
      name##Prev->name##Next = name##Next; \
   } \


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class CallType;

      class Type;
   }

   namespace AST
   {
      class Exp;
      class Type_ArrVM;
      class Type_ArrVM0;
      class Type_Array;
      class Type_Array0;
      class Type_Bitfield;
      class Type_Function;
      class Type_Pointer;
      class Type_RefL;
      class Type_RefR;
      class TypeMember;
      class TypeSet;

      //
      // TypeRankC
      //
      enum class TypeRankC
      {
         None,
         IntegHH,
         IntegH,
         Integ,
         IntegL,
         IntegLL,
         FractH,
         Fract,
         FractL,
         FixedH,
         Fixed,
         FixedL,
         Float,
         FloatL,
         FloatLL,
      };

      //
      // TypeError
      //
      class TypeError : public std::exception
      {
      public:
         virtual char const *what() const noexcept {return "AST::TypeError";}
      };

      //
      // TypeQual
      //
      struct TypeQual
      {
         TypeQual() = default;
         constexpr TypeQual(bool a, bool c, bool r, bool v) :
            space{IR::AddrBase::Gen, Core::STRNULL},
            aAtom{a}, aCons{c}, aRest{r}, aVola{v} {}
         constexpr TypeQual(IR::AddrBase space_) : space{space_, Core::STRNULL},
            aAtom{false}, aCons{false}, aRest{false}, aVola{false} {}
         constexpr TypeQual(IR::AddrSpace space_) : space{space_},
            aAtom{false}, aCons{false}, aRest{false}, aVola{false} {}
         constexpr TypeQual(IR::AddrSpace as, bool a, bool c, bool r, bool v) :
            space{as}, aAtom{a}, aCons{c}, aRest{r}, aVola{v} {}

         constexpr explicit operator bool () const
            {return space.base != IR::AddrBase::Gen || aAtom || aCons || aRest || aVola;}

         //
         // operator TypeQual == TypeQual
         //
         constexpr bool operator == (TypeQual const &q) const
         {
            return space == q.space &&
               aAtom == q.aAtom && aCons == q.aCons &&
               aRest == q.aRest && aVola == q.aVola;
         }

         IR::AddrSpace space;

         bool aAtom : 1; // atomic
         bool aCons : 1; // const
         bool aRest : 1; // restrict
         bool aVola : 1; // volatile
      };

      //
      // Type
      //
      class Type : public Core::Counter
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::AST::Type, GDCC::Core::Counter);

      protected:
         using ExpCRef     = Core::CounterRef<Exp const>;
         using Member      = TypeMember;
         using RankC       = TypeRankC;
         using TypeSetCRef = Core::CounterRef<TypeSet const>;

      public:
         // Type creation.
         CRef getTypeArray() const;                 // Type/Array.cpp
         CRef getTypeArray(Core::FastU size) const; // Type/Array.cpp
         CRef getTypeArray(Exp const *size) const;  // Type/Array.cpp
         CRef getTypeBitfield(Core::FastU bitsF,    // Type/Bitfield.cpp
            Core::FastU bitsI, Core::FastU bitsO) const;
         CRef getTypeFunction(TypeSet const *param, // Type/Function.cpp
            IR::CallType ctype) const;
         CRef getTypePointer() const;               // Type/Pointer.cpp
         CRef getTypeQual() const {return qualNone;}
         CRef getTypeQual(TypeQual quals) const;
         CRef getTypeQualAddr(IR::AddrSpace addr) const;
         CRef getTypeRefL() const;                  // Type/Pointer.cpp
         CRef getTypeRefR() const;                  // Type/Pointer.cpp

         // Returns a type with the given address space qualifer applied to
         // itself and (if an array type) its base type, recursively.
         virtual CRef getTypeArrayQualAddr(IR::AddrSpace addr) const;

         // Type information.
         virtual CRef          getBaseType()    const {throw TypeError();}
         virtual IR::CallType  getCallType()    const {throw TypeError();}
         virtual Core::FastU   getCallWords()   const {throw TypeError();}
         virtual IR::Type      getIRType()      const;
         virtual Core::String  getName()        const {throw TypeError();}
         virtual TypeSetCRef   getParameters()  const {throw TypeError();}
         virtual CRef          getPrimType()    const {throw TypeError();}
                 TypeQual      getQual()        const {return quals;}
                 IR::AddrSpace getQualAddr()    const {return quals.space;}
                 bool          getQualAtom()    const {return quals.aAtom;}
                 bool          getQualCons()    const {return quals.aCons;}
                 bool          getQualRest()    const {return quals.aRest;}
                 bool          getQualVola()    const {return quals.aVola;}
         virtual RankC         getRankC()       const {throw TypeError();}
         virtual CRef          getSatuType()    const {throw TypeError();}
         virtual CRef          getSignType()    const {throw TypeError();}
         virtual Core::FastU   getSizeAlign()   const {throw TypeError();}
         virtual Core::FastU   getSizeBitsF()   const {throw TypeError();}
         virtual Core::FastU   getSizeBitsI()   const {throw TypeError();}
         virtual Core::FastU   getSizeBitsO()   const {return 0;}
         virtual bool          getSizeBitsS()   const {throw TypeError();}
         virtual Core::FastU   getSizeBytes()   const {throw TypeError();}
         virtual ExpCRef       getSizeBytesVM() const;
         virtual Core::FastU   getSizePoint()   const {throw TypeError();}
         virtual ExpCRef       getSizePointVM() const;
         virtual Core::FastU   getSizeShift()   const {throw TypeError();}
         virtual Core::FastU   getSizeWords()   const {throw TypeError();}
         virtual ExpCRef       getSizeWordsVM() const;
         virtual CRef          getUnsiType()    const {throw TypeError();}

         // Type information: Members.
         virtual Member getMember(Core::String name) const;

         // Type classification: General classifications.
         virtual bool isTypeArray()    const {return false;}
         virtual bool isTypeBitfield() const {return false;}
         virtual bool isTypeBoolean()  const {return false;}
         virtual bool isTypeComplete() const {return false;}
         virtual bool isTypeComplex()  const {return false;}
         virtual bool isTypeFunction() const {return false;}
         virtual bool isTypeImagin()   const {return false;}
         virtual bool isTypeLabel()    const {return false;}
         virtual bool isTypeNullptr()  const {return false;}
         virtual bool isTypePointer()  const {return false;}
         virtual bool isTypeRef()      const {return false;}
         virtual bool isTypeRefL()     const {return false;}
         virtual bool isTypeRefR()     const {return false;}
         virtual bool isTypeSaturate() const {return false;}
         virtual bool isTypeSizeVM()   const {return false;}
         virtual bool isTypeStrEnt()   const {return false;}
         virtual bool isTypeVM()       const {return false;}
         virtual bool isTypeVoid()     const {return false;}

         // Type classification: C/C++ classifications.
         virtual bool isCTypeAccum()    const {return false;}
         virtual bool isCTypeAccumS()   const {return false;}
         virtual bool isCTypeAccumU()   const {return false;}
         virtual bool isCTypeAggregat() const {return false;}
         virtual bool isCTypeArith()    const {return false;}
         virtual bool isCTypeChar()     const {return false;}
         virtual bool isCTypeComplex()  const {return false;}
         virtual bool isCTypeEnum()     const {return false;}
         virtual bool isCTypeEnumS()    const {return false;}
         virtual bool isCTypeEnumU()    const {return false;}
         virtual bool isCTypeFixed()    const {return false;}
         virtual bool isCTypeFixedPr()  const {return false;}
         virtual bool isCTypeFixedPrS() const {return false;}
         virtual bool isCTypeFixedPrU() const {return false;}
         virtual bool isCTypeFixedSa()  const {return false;}
         virtual bool isCTypeFixedSaS() const {return false;}
         virtual bool isCTypeFixedSaU() const {return false;}
         virtual bool isCTypeFloat()    const {return false;}
         virtual bool isCTypeFract()    const {return false;}
         virtual bool isCTypeFractS()   const {return false;}
         virtual bool isCTypeFractU()   const {return false;}
         virtual bool isCTypeFunction() const {return false;}
         virtual bool isCTypeImagin()   const {return false;}
         virtual bool isCTypeInteg()    const {return false;}
         virtual bool isCTypeIntegS()   const {return false;}
         virtual bool isCTypeIntegU()   const {return false;}
         virtual bool isCTypeMemFn()    const {return false;}
         virtual bool isCTypeMemPt()    const {return false;}
         virtual bool isCTypeObject()   const {return false;}
         virtual bool isCTypeReal()     const {return false;}
         virtual bool isCTypeRealFlt()  const {return false;}
         virtual bool isCTypeScalar()   const {return false;}
         virtual bool isCTypeStruct()   const {return false;}
         virtual bool isCTypeUnion()    const {return false;}


         friend class Type_ArrVM;
         friend class Type_ArrVM0;
         friend class Type_Array;
         friend class Type_Array0;
         friend class Type_Bitfield;
         friend class Type_Function;
         friend class Type_Pointer;
         friend class Type_RefL;
         friend class Type_RefR;

         static CRef GetLabel();
         static CRef GetNone();
         static CRef GetSize();
         static CRef GetStrEnt();
         static CRef GetVoid();

         static CRef const Label;
         static CRef const None;
         static CRef const Size;
         static CRef const StrEnt;
         static CRef const Void;

      protected:
         Type();
         // Derived types must implement copy construction to be suitable for
         // the creation of qualified types.
         Type(Type const &type);
         virtual ~Type();

      private:
         // Qualified types.
         TypeQual quals;
         CRef const qualNone;
         mutable Type const *qualNext, *qualPrev;

         // Array types. (Type/Array.cpp)
         mutable Type_Array const *arrType;
         mutable Type       const *arrType0;
         mutable Type_ArrVM const *avmType;
         mutable Type       const *avmType0;

         // Bitfield types. (Type/Bitfield.cpp)
         mutable Type_Bitfield const *bitType;

         // Function types. (Type/Function.cpp)
         mutable Type_Function const *funType;

         // Pointer types. (Type/Pointer.cpp)
         mutable Type const *lvrType; // lvalue reference
         mutable Type const *ptrType; // pointer
         mutable Type const *rvrType; // rvalue reference
      };

      //
      // TypeMember
      //
      class TypeMember
      {
      public:
         Core::FastU addr;
         Type::CRef  type;
      };

      //
      // TypeSet
      //
      class TypeSet final : public Core::PlainCounter
      {
         GDCC_Core_CounterPreambleNoVirtual(
            GDCC::AST::TypeSet, GDCC::Core::PlainCounter);

      public:
         Type::CRef const &operator[] (std::size_t i) const {return tbeg[i];}

         Type::CRef const *begin() const {return tbeg;}

         Type::CRef const *end() const {return tend;}

         std::size_t size() const {return tend - tbeg;}

         bool variadic() const {return varia;}


         static CRef Get(bool varia);
         static CRef Get(Type::CRef const *typev, std::size_t typec, bool varia);

      private:
         TypeSet(TypeSet const &) = delete;
         explicit TypeSet(bool varia);
         TypeSet(TypeSet *head, Type::CRef const *tbeg, Type::CRef const *tend,
            bool varia);
         ~TypeSet();

         TypeSet *next, *prev;

         Type::CRef const *const tbeg;
         Type::CRef const *const tend;
         bool              const varia;


         static Ref const Head, HeadV;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace AST
   {
      constexpr TypeQual QualNone{false, false, false, false};
      constexpr TypeQual QualAtom{true,  false, false, false};
      constexpr TypeQual QualCons{false, true,  false, false};
      constexpr TypeQual QualRest{false, false, true,  false};
      constexpr TypeQual QualVola{false, false, false, true };
   }
}

#endif//GDCC__AST__Type_H__

