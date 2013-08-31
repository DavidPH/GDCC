//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR expression parsing.
//
//-----------------------------------------------------------------------------

#ifndef C__GetExpIR_H__
#define C__GetExpIR_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      class Exp;
   }

   template<typename T> class CounterRef;
   class TokenStream;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_Prim(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_Post(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_Unar(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_Mult(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_Addi(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_Shft(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_Rela(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_Equa(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_BAnd(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_BOrX(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_BOrI(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_LAnd(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_LOrI(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_Cond(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR_Assi(GDCC::TokenStream &in);
   GDCC::CounterRef<GDCC::IR::Exp> GetExpIR(GDCC::TokenStream &in);
}

#endif//C__GetExpIR_H__

