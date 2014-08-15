//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR::Value dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "IRDump/Put.hpp"

#include "IR/Addr.hpp"
#include "IR/Value.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//
namespace GDCC
{
   namespace IRDump
   {
      //
      // PutValue
      //
      void PutValue(std::ostream &out, IR::Value const &val)
      {
         switch(val.v)
         {
            #define GDCC_IR_TypeList(tp) \
               case IR::ValueBase::tp: PutValue_##tp(out, val.v##tp); break;
            #include "IR/TypeList.hpp"
         }
      }

      //
      // PutValue_Empty
      //
      void PutValue_Empty(std::ostream &out, IR::Value_Empty const &val)
      {
         PutType(out << "Value ", val.vtype);
         out << " ()";
      }

      //
      // PutValue_Fixed
      //
      void PutValue_Fixed(std::ostream &out, IR::Value_Fixed const &val)
      {
         if(val.vtype.bitsF)
            out << (static_cast<Core::Float>(val.value) >> val.vtype.bitsF);
         else
            out << val.value;

         out << '_';
         if(val.vtype.bitsS) out << 's';
         out << val.vtype.bitsI << '.' << val.vtype.bitsF;
         if(val.vtype.satur) out << 's';

      }

      //
      // PutValue_Float
      //
      void PutValue_Float(std::ostream &out, IR::Value_Float const &val)
      {
         out << val.value;

         out << '_';
         if(val.vtype.bitsS) out << 's';
         out << val.vtype.bitsI << '.' << val.vtype.bitsF;
         if(val.vtype.satur) out << 's';

      }

      //
      // PutValue_Funct
      //
      void PutValue_Funct(std::ostream &out, IR::Value_Funct const &val)
      {
         PutType(out << "Value ", val.vtype);
         out << " (" << val.value << ')';
      }

      //
      // PutValue_Multi
      //
      void PutValue_Multi(std::ostream &out, IR::Value_Multi const &val)
      {
         out << '[';
         if(!val.value.empty())
         {
            auto itr = val.value.begin(), end = val.value.end();
            PutValue(out, *itr++);
            while(itr != end)
            {
               out << ", ";
               PutValue(out, *itr++);
            }
         }
         out << ']';
      }

      //
      // PutValue_Point
      //
      void PutValue_Point(std::ostream &out, IR::Value_Point const &val)
      {
         PutType(out << "Value ", val.vtype);

         out << " (";
         out << val.value;
         out << ", ";
         out << val.addrB;
         PutString(out, val.addrN);
         out << ')';
      }

      //
      // PutValue_StrEn
      //
      void PutValue_StrEn(std::ostream &out, IR::Value_StrEn const &val)
      {
         PutType(out << "Value ", val.vtype);
         out << " (" << val.value << ')';
      }
   }
}

// EOF

