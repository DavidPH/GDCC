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

#include "CC/Type/Struct.hpp"

#include "IR/Type.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Type_Struct::Data constructor
      //
      Type_Struct::Data::Data(Core::String name_, bool isUnion_) :
         memb{},
         name{name_},

         sizeAlign{0},
         sizeBytes{0},
         sizePoint{0},
         sizeShift{0},
         sizeWords{0},

         complete{false},
         isStruct{!isUnion_},
         isUnion { isUnion_}
      {
      }

      //
      // Type_Struct constructor
      //
      Type_Struct::Type_Struct(Core::String name, bool isUnion) :
         data(*new Data(name, isUnion))
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
         if(!data.complete) throw AST::TypeError();

         if(data.isStruct)
         {
            return IR::Type_Assoc(
               {data.memb.begin(), data.memb.end(),
                  [](MemberData const &m) -> IR::TypeAssoc
                     {return {m.type->getIRType(), m.name, m.addr};}});
         }
         else
         {
            return IR::Type_Union(
               {data.memb.begin(), data.memb.end(),
                  [](MemberData const &m) {return m.type->getIRType();}});
         }
      }

      //
      // Type_Struct::getMember
      //
      Type_Struct::Member Type_Struct::getMember(Core::String name) const
      {
         if(!data.complete) throw AST::TypeError();

         // getMemQual
         auto getMemQual = [&](AST::Type const *t)
         {
            auto q = getQual();

            q.aAtom |= t->getQual().aAtom;
            q.aCons |= t->getQual().aCons;
            q.aRest |= t->getQual().aRest;
            q.aVola |= t->getQual().aVola;

            return t->getTypeQual(q);
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
            catch(AST::TypeError const &) {}
         }

         throw AST::TypeError();
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
         if(!data.complete) throw AST::TypeError();
         return data.sizeAlign;
      }

      //
      // Type_Struct::getSizeBytes
      //
      Core::FastU Type_Struct::getSizeBytes() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.sizeBytes;
      }

      //
      // Type_Struct::getSizePoint
      //
      Core::FastU Type_Struct::getSizePoint() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.sizePoint;
      }

      //
      // Type_Struct::getSizeShift
      //
      Core::FastU Type_Struct::getSizeShift() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.sizeShift;
      }

      //
      // Type_Struct::getSizeWords
      //
      Core::FastU Type_Struct::getSizeWords() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.sizeWords;
      }

      //
      // Type_Struct::setMembers
      //
      void Type_Struct::setMembers(MemberData const *memv, std::size_t memc,
         Core::FastU sizeBytes)
      {
         if(data.complete) throw AST::TypeError();

         for(auto const &mem : data.memb = Core::Array<MemberData>(memv, memv + memc))
         {
            if(mem.type->isTypeComplete())
               sizeBytes = std::max(sizeBytes, mem.addr + mem.type->getSizeBytes());
         }

         // TODO: Should be able to generate sub-word structures for targets
         // that do not need special sub-word pointers.

         Core::FastU wordBytes = Platform::GetWordBytes();

         data.sizeAlign = Platform::GetWordAlign();
         data.sizeWords = (sizeBytes + wordBytes - 1) / wordBytes;
         data.sizeBytes = data.sizeWords * wordBytes;
         data.sizePoint = Platform::GetWordPoint() * data.sizeWords;
         data.sizeShift = Platform::GetWordShift();

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
}

// EOF

