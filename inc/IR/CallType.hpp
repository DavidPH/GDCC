//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation function call type/convention handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__CallType_H__
#define GDCC__IR__CallType_H__

#include "../IR/Types.hpp"

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // CallType
   //
   enum class CallType
   {
      #define GDCC_IR_CallTypeList(name) name,
      #include "../IR/CallTypeList.hpp"
   };
}

namespace std
{
   //
   // hash<::GDCC::IR::CallType>
   //
   template<> struct hash<::GDCC::IR::CallType>
   {
      size_t operator () (::GDCC::IR::CallType ctype) const
         {return static_cast<size_t>(ctype);}
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   OArchive &operator << (OArchive &out, CallType in);
   std::ostream &operator << (std::ostream &out, CallType in);

   IArchive &operator >> (IArchive &in, CallType &out);

   // Maps a CallType to its IR implementation.
   CallType GetCallTypeIR(CallType ctype);
}

#endif//GDCC__IR__CallType_H__

