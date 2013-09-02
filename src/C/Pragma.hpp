//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Pragma parsing and handling.
//
//-----------------------------------------------------------------------------

#ifndef C__Pragma_H__
#define C__Pragma_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   class TokenStream;
}

namespace C
{
   using PragmaFunc = void(*)(GDCC::TokenStream &);
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace C
{
   extern bool Pragma_STDC_CXLimitedRange;
   extern bool Pragma_STDC_FEnvAccess;
   extern bool Pragma_STDC_FPContract;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   void Pragma(GDCC::TokenStream &in);
}

#endif//C__Pragma_H__

