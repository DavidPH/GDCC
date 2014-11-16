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

#include "CC/Type/Enum.hpp"

#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Type_Enum::Data constructor
      //
      Type_Enum::Data::Data(Core::String name_) :
         name{name_},
         type{nullptr},

         complete{false}
      {
      }

      //
      // Type_Enum constructor
      //
      Type_Enum::Type_Enum(Core::String name) : data{*new Data(name)}
      {
      }

      //
      // Type_Enum destructor
      //
      Type_Enum::~Type_Enum()
      {
         if(!getQual())
            delete &data;
      }

      //
      // Type_Enum::getBaseType
      //
      AST::Type::CRef Type_Enum::getBaseType() const
      {
         if(!data.complete) throw AST::TypeError();
         return static_cast<AST::Type::CRef>(data.type);
      }

      //
      // Type_Enum::getName
      //
      Core::String Type_Enum::getName() const
      {
         return data.name;
      }

      //
      // Type_Enum::getRankC
      //
      AST::TypeRankC Type_Enum::getRankC() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.type->getRankC();
      }

      //
      // Type_Enum::getIRType
      //
      IR::Type Type_Enum::getIRType() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.type->getIRType();
      }

      //
      // Type_Enum::getSizeAlign
      //
      Core::FastU Type_Enum::getSizeAlign() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.type->getSizeAlign();
      }

      //
      // Type_Enum::getSizeBitsF
      //
      Core::FastU Type_Enum::getSizeBitsF() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.type->getSizeBitsF();
      }

      //
      // Type_Enum::getSizeBitsI
      //
      Core::FastU Type_Enum::getSizeBitsI() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.type->getSizeBitsI();
      }

      //
      // Type_Enum::getSizeBitsS
      //
      bool Type_Enum::getSizeBitsS() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.type->getSizeBitsS();
      }

      //
      // Type_Enum::getSizeBytes
      //
      Core::FastU Type_Enum::getSizeBytes() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.type->getSizeBytes();
      }

      //
      // Type_Enum::getSizePoint
      //
      Core::FastU Type_Enum::getSizePoint() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.type->getSizePoint();
      }

      //
      // Type_Enum::getSizeShift
      //
      Core::FastU Type_Enum::getSizeShift() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.type->getSizeShift();
      }

      //
      // Type_Enum::getSizeWords
      //
      Core::FastU Type_Enum::getSizeWords() const
      {
         if(!data.complete) throw AST::TypeError();
         return data.type->getSizeWords();
      }

      //
      // Type_Enum::setComplete
      //
      void Type_Enum::setComplete(AST::Type const *type)
      {
         if(data.complete) throw AST::TypeError();

         data.type     = type;
         data.complete = true;
      }

      //
      // Type_Enum::Create
      //
      Type_Enum::Ref Type_Enum::Create(Core::String name)
      {
         return Ref(new Type_Enum(name));
      }
   }
}

// EOF

