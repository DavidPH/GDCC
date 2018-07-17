//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR expression parsing.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__GetExpIR_H__
#define GDCC__CPP__GetExpIR_H__

#include "../CPP/Types.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CPP
{
   Core::CounterRef<IR::Exp const> GetExpIR_Prim(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_Post(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_Unar(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_Mult(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_Addi(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_Shft(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_Rela(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_Equa(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_BAnd(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_BOrX(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_BOrI(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_LAnd(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_LOrI(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_Cond(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR_Assi(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExpIR(Core::TokenStream &in);
}

#endif//GDCC__CPP__GetExpIR_H__

