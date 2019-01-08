//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR expression parsing.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__GetExp_H__
#define GDCC__CPP__GetExp_H__

#include "../CPP/Types.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CPP
{
   Core::CounterRef<IR::Exp const> GetExp_Prim(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_Post(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_Unar(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_Mult(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_Addi(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_Shft(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_Rela(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_Equa(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_BAnd(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_BOrX(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_BOrI(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_LAnd(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_LOrI(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_Cond(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp_Assi(Core::TokenStream &in);
   Core::CounterRef<IR::Exp const> GetExp(Core::TokenStream &in);
}

#endif//GDCC__CPP__GetExp_H__

