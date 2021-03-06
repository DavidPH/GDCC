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

#include "CC/Type/Struct.hpp"

#include "IR/Type.hpp"

#include "Target/Info.hpp"

#include <map>
#include <vector>


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::CC
{
   Type_Struct::Data Type_Struct::Data::Head{};
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Type_Div::getIRType
   //
   IR::Type Type_Div::getIRType() const
   {
      auto typeIR = type->getIRType();
      return IR::Type_Assoc({
         {typeIR, Core::STR_quot, 0},
         {typeIR, Core::STR_rem, type->getSizeBytes()}});
   }

   //
   // Type_Div::getName
   //
   Core::String Type_Div::getName() const
   {
      return nullptr;
   }

   //
   // Type_Div::getSizeAlign
   //
   Core::FastU Type_Div::getSizeAlign() const
   {
      return type->getSizeAlign();
   }

   //
   // Type_Div::getSizeBytes
   //
   Core::FastU Type_Div::getSizeBytes() const
   {
      return type->getSizeBytes() * 2;
   }

   //
   // Type_Div::getSizePoint
   //
   Core::FastU Type_Div::getSizePoint() const
   {
      return type->getSizePoint() * 2;
   }

   //
   // Type_Div::getSizeShift
   //
   Core::FastU Type_Div::getSizeShift() const
   {
      return type->getSizeShift();
   }

   //
   // Type_Div::getSizeWords
   //
   Core::FastU Type_Div::getSizeWords() const
   {
      return type->getSizeWords() * 2;
   }

   //
   // Type_Div::getMember
   //
   SR::Type::Member Type_Div::getMember(Core::String name) const
   {
      switch(name)
      {
      case Core::STR_quot:
         return {0, type->getTypeQual(getQual())};

      case Core::STR_rem:
         return {type->getSizeBytes(), type->getTypeQual(getQual())};

      default:
         throw SR::TypeError();
      }
   }

   //
   // Type_Div::Get
   //
   Type_Div::CRef Type_Div::Get(SR::Type const *type)
   {
      static std::map<SR::Type::CRef, Type_Div::CRef> divs;

      auto itr = divs.find(type->getTypeQual());

      if(itr == divs.end())
      {
         itr = divs.emplace(std::piecewise_construct,
            std::forward_as_tuple(type),
            std::forward_as_tuple(new Type_Div(type))
            ).first;
      }

      return itr->second;
   }

   //
   // Type_Struct::Data constructor
   //
   Type_Struct::Data::Data() :
      memb{},
      name{nullptr},

      sizeAlign{0},
      sizeBytes{0},
      sizePoint{0},
      sizeShift{0},
      sizeWords{0},

      complete{false},
      isStruct{false},
      isUnion {false},

      next{this},
      prev{this},
      type{nullptr}
   {
   }

   //
   // Type_Struct::Data constructor
   //
   Type_Struct::Data::Data(Core::String name_, bool isUnion_, Type_Struct *type_) :
      memb{},
      name{name_},

      sizeAlign{0},
      sizeBytes{0},
      sizePoint{0},
      sizeShift{0},
      sizeWords{0},

      complete{false},
      isStruct{!isUnion_},
      isUnion { isUnion_},

      next{&Head},
      prev{Head.prev},
      type{type_}
   {
      next->prev = this;
      prev->next = this;
   }

   //
   // Type_Struct::Data destructor
   //
   Type_Struct::Data::~Data()
   {
      if(!type) Cleanup();

      next->prev = prev;
      prev->next = next;
   }

   //
   // Type_Struct::Data::Cleanup
   //
   void Type_Struct::Data::Cleanup()
   {
      // Hold a reference to all the types, so the list doesn't change while
      // clearing.
      std::vector<Type_Struct::CRef> types;
      for(Data *itr = Head.next; itr != &Head; itr = itr->next)
         types.emplace_back(itr->type);

      for(Data *itr = Head.next; itr != &Head; itr = itr->next)
         itr->memb = {};
   }

   //
   // Type_Struct::PropData::getProp
   //
   Type_Struct::Prop Type_Struct::PropData::getProp() const
   {
      return
      {
         prop.func ? &prop : nullptr,

         propAdd2 .func ? &propAdd2  : nullptr,
         propAdd3 .func ? &propAdd3  : nullptr,
         propAddEq.func ? &propAddEq : nullptr,
         propAndEq.func ? &propAndEq : nullptr,
         propDivEq.func ? &propDivEq : nullptr,
         propEqual.func ? &propEqual : nullptr,
         propModEq.func ? &propModEq : nullptr,
         propMulEq.func ? &propMulEq : nullptr,
         propOrIEq.func ? &propOrIEq : nullptr,
         propOrXEq.func ? &propOrXEq : nullptr,
         propParen.func ? &propParen : nullptr,
         propShLEq.func ? &propShLEq : nullptr,
         propShREq.func ? &propShREq : nullptr,
         propSub2 .func ? &propSub2  : nullptr,
         propSub3 .func ? &propSub3  : nullptr,
         propSubEq.func ? &propSubEq : nullptr,
      };
   }

   //
   // Type_Struct constructor
   //
   Type_Struct::Type_Struct(Core::String name, bool isUnion) :
      data(*new Data(name, isUnion, this))
   {
   }

   //
   // Type_Struct destructor
   //
   Type_Struct::~Type_Struct()
   {
      if(!getQual())
         delete &data;
   }

   //
   // Type_Struct::getIRType
   //
   IR::Type Type_Struct::getIRType() const
   {
      if(!data.complete) throw SR::TypeError();

      auto membBegin = data.memb.begin();
      auto membEnd   = data.memb.end();

      // Strip trailing flexible array member, if any.
      if(membBegin != membEnd && (membEnd - 1)->type->isTypeArray() &&
         !(membEnd - 1)->type->isTypeComplete())
      {
         --membEnd;
      }

      if(data.isStruct)
      {
         return IR::Type_Assoc(
            {membBegin, membEnd,
               [](MemberData const &m) -> IR::TypeAssoc
                  {return {m.type->getIRType(), m.name, m.addr};}});
      }
      else
      {
         return IR::Type_Union(
            {membBegin, membEnd,
               [](MemberData const &m) {return m.type->getIRType();}});
      }
   }

   //
   // Type_Struct::getMember
   //
   SR::Type::Member Type_Struct::getMember(Core::String name) const
   {
      if(!data.complete) throw SR::TypeError();

      // getMemQual
      auto getMemQual = [&](SR::Type const *t)
      {
         auto q = getQual();

         q.aAtom |= t->getQual().aAtom;
         q.aCons |= t->getQual().aCons;
         q.aRest |= t->getQual().aRest;
         q.aVola |= t->getQual().aVola;

         return t->getTypeQual(q)->getTypeArrayQualAddr(q.space);
      };

      // Linear search for matching member.
      // This should probably be a hashed lookup at some point.
      for(auto const &mem : data.memb)
      {
         // Directly contained member?
         if(mem.name == name) return {mem.addr, getMemQual(mem.type)};

         // Anonymous struct/union contained member?
         if(mem.anon) try
         {
            auto m = mem.type->getMember(name);
            return {mem.addr + m.addr, getMemQual(m.type)};
         }
         catch(SR::TypeError const &) {}
      }

      throw SR::TypeError();
   }

   //
   // Type_Struct::getProp
   //
   auto Type_Struct::getProp(Core::String name) const -> Prop
   {
      if(!data.complete) throw SR::TypeError{};

      for(auto const &prop : data.prop)
      {
         // Directly contained property?
         if(prop.name == name)
            return prop.getProp();
      }

      throw SR::TypeError{};
   }

   //
   // Type_Struct::getName
   //
   Core::String Type_Struct::getName() const
   {
      return data.name;
   }

   //
   // Type_Struct::getSizeAlign
   //
   Core::FastU Type_Struct::getSizeAlign() const
   {
      if(!data.complete) throw SR::TypeError();
      return data.sizeAlign;
   }

   //
   // Type_Struct::getSizeBytes
   //
   Core::FastU Type_Struct::getSizeBytes() const
   {
      if(!data.complete) throw SR::TypeError();
      return data.sizeBytes;
   }

   //
   // Type_Struct::getSizePoint
   //
   Core::FastU Type_Struct::getSizePoint() const
   {
      if(!data.complete) throw SR::TypeError();
      return data.sizePoint;
   }

   //
   // Type_Struct::getSizeShift
   //
   Core::FastU Type_Struct::getSizeShift() const
   {
      if(!data.complete) throw SR::TypeError();
      return data.sizeShift;
   }

   //
   // Type_Struct::getSizeWords
   //
   Core::FastU Type_Struct::getSizeWords() const
   {
      if(!data.complete) throw SR::TypeError();
      return data.sizeWords;
   }

   //
   // Type_Struct::hasProp
   //
   bool Type_Struct::hasProp(Core::String name) const
   {
      if(!data.complete) throw SR::TypeError();

      try
      {
         return getProp(name), true;
      }
      catch(SR::TypeError const &)
      {
         return false;
      }
   }

   //
   // Type_Struct::setMembers
   //
   void Type_Struct::setMembers(MemberData const *memv, std::size_t memc,
      Core::FastU sizeBytes)
   {
      setMembers(memv, memc, nullptr, 0, sizeBytes);
   }

   //
   // Type_Struct::setMembers
   //
   void Type_Struct::setMembers(MemberData const *memv, std::size_t memc,
      PropData const *propv, std::size_t propc, Core::FastU sizeBytes)
   {
      if(data.complete) throw SR::TypeError();

      for(auto const &mem : data.memb = {memv, memv + memc})
      {
         if(mem.type->isTypeComplete())
            sizeBytes = std::max(sizeBytes, mem.addr + mem.type->getSizeBytes());
      }

      data.prop = {propv, propv + propc};

      // TODO: Should be able to generate sub-word structures for targets
      // that do not need special sub-word pointers.

      Core::FastU wordBytes = Target::GetWordBytes();

      data.sizeAlign = Target::GetWordAlign();
      data.sizeWords = (sizeBytes + wordBytes - 1) / wordBytes;
      data.sizeBytes = data.sizeWords * wordBytes;
      data.sizePoint = Target::GetWordPoint() * data.sizeWords;
      data.sizeShift = Target::GetWordShift();

      data.complete = true;
   }

   //
   // Type_Struct::Create
   //
   Type_Struct::Ref Type_Struct::Create(Core::String name, bool isUnion)
   {
      return static_cast<Ref>(new Type_Struct(name, isUnion));
   }

   //
   // Type_Struct::CreateUnion
   //
   Type_Struct::Ref Type_Struct::CreateUnion(Core::String name)
   {
      return static_cast<Ref>(new Type_Struct(name, true));
   }
}

// EOF

