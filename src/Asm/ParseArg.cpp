//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Arg parsing utilities.
//
//-----------------------------------------------------------------------------

#include "Parse.hpp"

#include "GDCC/TokenStream.hpp"

#include "GDCC/IR/Arg.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Function                                                            |
//

//
// ParseArg0
//
template<typename T>
static T ParseArg0(GDCC::TokenStream &in)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return T();
}

//
// ParseArg1
//
template<typename T>
static T ParseArg1(GDCC::TokenStream &in)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   auto exp = Asm::ParseExp(in);
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return T(std::move(exp));
}

//
// ParseArg2
//
template<typename T>
static T ParseArg2(GDCC::TokenStream &in)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   auto arg = Asm::ParseArg(in);
   Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   auto exp = Asm::ParseExp(in);
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return T(std::move(arg), std::move(exp));
}

//
// ParseArg3
//
template<typename T>
static T ParseArg3(GDCC::TokenStream &in)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   auto arg0 = Asm::ParseArg(in);
   Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   auto arg1 = Asm::ParseArg(in);
   Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   auto exp = Asm::ParseExp(in);
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return T(std::move(arg0), std::move(arg1), std::move(exp));
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // ParseArg
   //
   GDCC::IR::Arg ParseArg(GDCC::TokenStream &in)
   {
      ExpectToken(in, GDCC::TOK_Identi, "identifier");
      switch(static_cast<GDCC::StringIndex>(in.get().str))
      {
      case GDCC::STR_Gen:    return ParseArg2<GDCC::IR::Arg_Gen   >(in);

      case GDCC::STR_Cpy:    return ParseArg0<GDCC::IR::Arg_Cpy   >(in);
      case GDCC::STR_Lit:    return ParseArg1<GDCC::IR::Arg_Lit   >(in);
      case GDCC::STR_Nul:    return ParseArg0<GDCC::IR::Arg_Nul   >(in);
      case GDCC::STR_Stk:    return ParseArg0<GDCC::IR::Arg_Stk   >(in);

      case GDCC::STR_Far:    return ParseArg2<GDCC::IR::Arg_Far   >(in);
      case GDCC::STR_GblArs: return ParseArg2<GDCC::IR::Arg_GblArs>(in);
      case GDCC::STR_GblReg: return ParseArg2<GDCC::IR::Arg_GblReg>(in);
      case GDCC::STR_Loc:    return ParseArg2<GDCC::IR::Arg_Loc   >(in);
      case GDCC::STR_LocArs: return ParseArg2<GDCC::IR::Arg_LocArs>(in);
      case GDCC::STR_LocReg: return ParseArg2<GDCC::IR::Arg_LocReg>(in);
      case GDCC::STR_MapArs: return ParseArg2<GDCC::IR::Arg_MapArs>(in);
      case GDCC::STR_MapReg: return ParseArg2<GDCC::IR::Arg_MapReg>(in);
      case GDCC::STR_StrArs: return ParseArg2<GDCC::IR::Arg_StrArs>(in);
      case GDCC::STR_Vaa:    return ParseArg2<GDCC::IR::Arg_Vaa   >(in);
      case GDCC::STR_WldArs: return ParseArg2<GDCC::IR::Arg_WldArs>(in);
      case GDCC::STR_WldReg: return ParseArg2<GDCC::IR::Arg_WldReg>(in);

      case GDCC::STR_GblArr: return ParseArg3<GDCC::IR::Arg_GblArr>(in);
      case GDCC::STR_MapArr: return ParseArg3<GDCC::IR::Arg_MapArr>(in);
      case GDCC::STR_StrArr: return ParseArg3<GDCC::IR::Arg_StrArr>(in);
      case GDCC::STR_WldArr: return ParseArg3<GDCC::IR::Arg_WldArr>(in);

      default:
         in.unget();
         std::cerr << "ERROR: " << in.peek().pos << ": bad statement argument: '"
            << in.peek().str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

