//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR::Value dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "IRDump/Put.hpp"

#include "IR/Value.hpp"

#include "Target/Addr.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IRDump
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
   // PutValue_Array
   //
   void PutValue_Array(std::ostream &out, IR::Value_Array const &val)
   {
      PutType_Array(out << "Value ", val.vtype);

      out << " (";
      if(!val.value.empty())
      {
         auto itr = val.value.begin(), end = val.value.end();
         PutValue(out, *itr++);
         while(itr != end)
            PutValue(out << ' ', *itr++);
      }
      out << ')';
   }

   //
   // PutValue_Assoc
   //
   void PutValue_Assoc(std::ostream &out, IR::Value_Assoc const &val)
   {
      PutType_Assoc(out << "Value ", val.vtype);

      out << " (";
      if(!val.value.empty())
      {
         auto itr = val.value.begin(), end = val.value.end();
         PutValue(out, *itr++);
         while(itr != end)
            PutValue(out << ' ', *itr++);
      }
      out << ')';
   }

   //
   // PutValue_DJump
   //
   void PutValue_DJump(std::ostream &out, IR::Value_DJump const &val)
   {
      PutType_DJump(out << "Value ", val.vtype);
      out << " (" << val.value << ')';
   }

   //
   // PutValue_Empty
   //
   void PutValue_Empty(std::ostream &out, IR::Value_Empty const &val)
   {
      PutType_Empty(out << "Value ", val.vtype);
      out << " ()";
   }

   //
   // PutValue_Fixed
   //
   void PutValue_Fixed(std::ostream &out, IR::Value_Fixed const &val)
   {
      if(val.vtype.bitsF)
         Core::WriteNumber(out, Core::NumberCast<Core::Float>(val.value) >> val.vtype.bitsF);
      else
         Core::WriteNumberDec(out, val.value);

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
      Core::WriteNumber(out, val.value);

      out << '_';
      out << 'f';
      if(val.vtype.bitsS) out << 's';
      out << val.vtype.bitsI << '.' << val.vtype.bitsF;
      if(val.vtype.satur) out << 's';

   }

   //
   // PutValue_Funct
   //
   void PutValue_Funct(std::ostream &out, IR::Value_Funct const &val)
   {
      PutType_Funct(out << "Value ", val.vtype);
      out << " (" << val.value << ')';
   }

   //
   // PutValue_Point
   //
   void PutValue_Point(std::ostream &out, IR::Value_Point const &val)
   {
      PutType_Point(out << "Value ", val.vtype);

      out << " (";
      out << val.value;
      out << ' ';
      out << val.addrB << ' ';
      PutString(out, val.addrN);
      out << ')';
   }

   //
   // PutValue_StrEn
   //
   void PutValue_StrEn(std::ostream &out, IR::Value_StrEn const &val)
   {
      PutType_StrEn(out << "Value ", val.vtype);
      out << " (" << val.value << ')';
   }

   //
   // PutValue_Tuple
   //
   void PutValue_Tuple(std::ostream &out, IR::Value_Tuple const &val)
   {
      out << '[';
      if(!val.value.empty())
      {
         auto itr = val.value.begin(), end = val.value.end();
         PutValue(out, *itr++);
         while(itr != end)
            PutValue(out << ' ', *itr++);
      }
      out << ']';
   }

   //
   // PutValue_Union
   //
   void PutValue_Union(std::ostream &out, IR::Value_Union const &val)
   {
      PutType_Union(out << "Value ", val.vtype);

      out << " (";
      PutValue(out, *val.value);
      out << ')';
   }
}

// EOF

