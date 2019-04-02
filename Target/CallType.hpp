//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Function call type/convention handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Target__CallType_H__
#define GDCC__Target__CallType_H__

#include "../Target/Types.hpp"

#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Target
{
   //
   // CallType
   //
   enum class CallType
   {
      #define GDCC_Target_CallTypeList(name) name,
      #include "../Target/CallTypeList.hpp"
   };
}

namespace std
{
   //
   // hash<::GDCC::Target::CallType>
   //
   template<> struct hash<::GDCC::Target::CallType>
   {
      size_t operator () (::GDCC::Target::CallType ctype) const
         {return static_cast<size_t>(ctype);}
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Target
{
   std::ostream &operator << (std::ostream &out, CallType in);

   // Maps a CallType to its IR implementation.
   CallType GetCallTypeIR(CallType ctype);
}

#endif//GDCC__Target__CallType_H__

