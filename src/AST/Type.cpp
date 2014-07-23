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

#include "AST/Type.hpp"

#include "AST/Exp.hpp"

#include "Core/Array.hpp"

#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace AST
   {
      Type::CRef const Type::Label{GetLabel()};
      Type::CRef const Type::None{GetNone()};
      Type::CRef const Type::Size{GetSize()};
      Type::CRef const Type::StrEnt{GetStrEnt()};
      Type::CRef const Type::Void{GetVoid()};

      TypeSet::Ref const TypeSet::Head{new TypeSet(false)};
      TypeSet::Ref const TypeSet::HeadV{new TypeSet(true)};
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Type constructor
      //
      Type::Type() : quals{IR::AddrBase::Gen},
         qualNone{this}, qualNext{this}, qualPrev{this},
         arrType{nullptr}, arrType0{nullptr}, avmType{nullptr},
         avmType0{nullptr}, bitType{nullptr}, funType{nullptr},
         lvrType{nullptr}, ptrType{nullptr}, rvrType{nullptr}
      {
         // We hold a reference to this, and so must not count that reference.
         // During destruction, the reference count will be decremented one last
         // time. Because the count will already be at 0, the decrement result
         // will not be 0.
         --refCount;
      }

      //
      // Type copy constructor
      //
      Type::Type(Type const &type) : Super{type}, quals{IR::AddrBase::Gen},
         qualNone{type.qualNone},
         qualNext{type.qualNext}, qualPrev{qualNext->qualPrev},
         arrType{nullptr}, arrType0{nullptr}, avmType{nullptr},
         avmType0{nullptr}, bitType{nullptr}, funType{nullptr},
         lvrType{nullptr}, ptrType{nullptr}, rvrType{nullptr}
      {
         qualNext->qualPrev = this;
         qualPrev->qualNext = this;
      }

      //
      // Type destructor
      //
      Type::~Type()
      {
         qualNext->qualPrev = qualPrev;
         qualPrev->qualNext = qualNext;
      }

      //
      // Type::getTypeQual
      //
      Type::CRef Type::getTypeQual(TypeQual newQuals) const
      {
         if(quals == newQuals) return static_cast<CRef>(this);

         for(auto type = qualNext; type != this; type = type->qualNext)
            if(type->quals == newQuals) return static_cast<CRef>(type);

         auto type = clone();
         type->quals = newQuals;
         return type;
      }

      //
      // Type::getTypeQualAddr
      //
      Type::CRef Type::getTypeQualAddr(IR::AddrSpace addr) const
      {
         auto newQuals = quals;
         newQuals.space = addr;
         return Type::getTypeQual(newQuals);
      }

      //
      // Type::getTypeArrayQualAddr
      //
      Type::CRef Type::getTypeArrayQualAddr(IR::AddrSpace addr) const
      {
         return getTypeQualAddr(addr);
      }

      //
      // Type::getIRType
      //
      IR::Type Type::getIRType() const
      {
         throw TypeError();
      }

      //
      // Type::getSizeBytesVM
      //
      Exp::CRef Type::getSizeBytesVM() const
      {
         return ExpCreate_Size(getSizeBytes());
      }

      //
      // Type::getSizePointVM
      //
      Exp::CRef Type::getSizePointVM() const
      {
         return ExpCreate_Size(getSizePoint());
      }

      //
      // Type::getSizeWordsVM
      //
      Exp::CRef Type::getSizeWordsVM() const
      {
         return ExpCreate_Size(getSizeWords());
      }

      //
      // Type::getMember
      //
      TypeMember Type::getMember(Core::String) const
      {
         throw TypeError();
      }

      //
      // TypeSet constructor
      //
      TypeSet::TypeSet(bool varia_) : next{this}, prev{this}, tbeg{nullptr},
         tend{nullptr}, varia{varia_}
      {
      }

      //
      // TypeSet constructor
      //
      TypeSet::TypeSet(TypeSet *head, Type::CRef const *tbeg_,
         Type::CRef const *tend_, bool varia_) : next{head}, prev{head->prev},
         tbeg{tbeg_}, tend{tend_}, varia{varia_}
      {
         next->prev = this;
         prev->next = this;
      }

      //
      // TypeSet destructor
      //
      TypeSet::~TypeSet()
      {
         next->prev = prev;
         prev->next = next;

         Core::Array<Type::CRef>::Del(tbeg, tend);
      }

      //
      // TypeSet::Get
      //
      TypeSet::CRef TypeSet::Get(bool varia)
      {
         return varia ? HeadV : Head;
      }

      //
      // TypeSet::Get
      //
      TypeSet::CRef TypeSet::Get(Type::CRef const *typev, std::size_t typec, bool varia)
      {
         TypeSet *head = varia ? HeadV : Head;
         if(!typec) return static_cast<CRef>(head);

         for(auto set = head->next; set != head; set = set->next)
         {
            if(set->size() == typec && std::equal(set->begin(), set->end(), typev))
               return static_cast<CRef>(set);
         }

         auto tbeg = Core::Array<Type::CRef>::Cpy(typev, typev + typec);
         return static_cast<CRef>(new TypeSet(head, tbeg, tbeg + typec, varia));
      }
   }
}

// EOF

