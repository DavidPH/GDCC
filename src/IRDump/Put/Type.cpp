//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR::Type dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "IRDump/Put.hpp"

#include "IR/Addr.hpp"
#include "IR/CallType.hpp"
#include "IR/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//
namespace GDCC
{
   namespace IRDump
   {
      //
      // PutType
      //
      void PutType(std::ostream &out, IR::Type const &type)
      {
         switch(type.t)
         {
            #define GDCC_IR_TypeList(tp) \
               case IR::TypeBase::tp: PutType_##tp(out, type.t##tp); break;
            #include "IR/TypeList.hpp"
         }
      }

      //
      // PutType_Empty
      //
      void PutType_Empty(std::ostream &out, IR::Type_Empty const &)
      {
         out << "Empty()";
      }

      //
      // PutType_Fixed
      //
      void PutType_Fixed(std::ostream &out, IR::Type_Fixed const &type)
      {
         out << "Fixed("
            << type.bitsI << ", "
            << type.bitsF << ", "
            << type.bitsS << ", "
            << type.satur << ')';
      }

      //
      // PutType_Float
      //
      void PutType_Float(std::ostream &out, IR::Type_Float const &type)
      {
         out << "Float("
            << type.bitsI << ", "
            << type.bitsF << ", "
            << type.bitsS << ", "
            << type.satur << ')';
      }

      //
      // PutType_Funct
      //
      void PutType_Funct(std::ostream &out, IR::Type_Funct const &type)
      {
         out << "Funct(" << type.callT << ')';
      }

      //
      // PutType_Multi
      //
      void PutType_Multi(std::ostream &out, IR::Type_Multi const &type)
      {
         out << "Multi(";
         if(!type.types.empty())
         {
            auto itr = type.types.begin(), end = type.types.end();
            PutType(out, *itr++);
            while(itr != end)
            {
               out << ", ";
               PutType(out, *itr++);
            }
         }
         out << ')';
      }

      //
      // PutType_Point
      //
      void PutType_Point(std::ostream &out, IR::Type_Point const &type)
      {
         out << "Point("
            << type.reprB << ' ';

         PutString(out, type.reprN);

         out << ", "
            << type.reprS << ", "
            << type.reprW << ')';
      }

      //
      // PutType_StrEn
      //
      void PutType_StrEn(std::ostream &out, IR::Type_StrEn const &)
      {
         out << "StrEn()";
      }
   }
}

// EOF

