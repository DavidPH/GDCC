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
static T ParseArg0(GDCC::TokenStream &in, GDCC::IR::Program &)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return T();
}

//
// ParseArgU
//
template<typename T>
static T ParseArgU(GDCC::TokenStream &in, GDCC::IR::Program &prog)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   auto exp = Asm::ParseFastU(in, prog);
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return T(std::move(exp));
}

//
// ParseArg1
//
template<typename T>
static T ParseArg1(GDCC::TokenStream &in, GDCC::IR::Program &prog)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   auto exp = Asm::ParseExp(in, prog);
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return T(std::move(exp));
}

//
// ParseArg2
//
template<typename T>
static T ParseArg2(GDCC::TokenStream &in, GDCC::IR::Program &prog)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   auto arg = Asm::ParseArg(in, prog);
   Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   auto exp = Asm::ParseExp(in, prog);
   Asm::SkipToken(in, GDCC::TOK_ParenC, ")");

   return T(std::move(arg), std::move(exp));
}

//
// ParseArg3
//
template<typename T>
static T ParseArg3(GDCC::TokenStream &in, GDCC::IR::Program &prog)
{
   Asm::SkipToken(in, GDCC::TOK_ParenO, "(");
   auto arg0 = Asm::ParseArg(in, prog);
   Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   auto arg1 = Asm::ParseArg(in, prog);
   Asm::SkipToken(in, GDCC::TOK_Comma, ",");
   auto exp  = Asm::ParseExp(in, prog);
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
   GDCC::IR::Arg ParseArg(GDCC::TokenStream &in, GDCC::IR::Program &prog)
   {
      ExpectToken(in, GDCC::TOK_Identi, "identifier");
      switch(static_cast<GDCC::StringIndex>(in.get().str))
      {
      case GDCC::STR_Gen:    return ParseArg2<GDCC::IR::Arg_Gen   >(in, prog);

      case GDCC::STR_Cpy:    return ParseArgU<GDCC::IR::Arg_Cpy   >(in, prog);
      case GDCC::STR_Lit:    return ParseArg1<GDCC::IR::Arg_Lit   >(in, prog);
      case GDCC::STR_Nul:    return ParseArg0<GDCC::IR::Arg_Nul   >(in, prog);
      case GDCC::STR_Stk:    return ParseArg0<GDCC::IR::Arg_Stk   >(in, prog);

      case GDCC::STR_Far:    return ParseArg2<GDCC::IR::Arg_Far   >(in, prog);
      case GDCC::STR_GblArs: return ParseArg2<GDCC::IR::Arg_GblArs>(in, prog);
      case GDCC::STR_GblReg: return ParseArg2<GDCC::IR::Arg_GblReg>(in, prog);
      case GDCC::STR_Loc:    return ParseArg2<GDCC::IR::Arg_Loc   >(in, prog);
      case GDCC::STR_LocArs: return ParseArg2<GDCC::IR::Arg_LocArs>(in, prog);
      case GDCC::STR_LocReg: return ParseArg2<GDCC::IR::Arg_LocReg>(in, prog);
      case GDCC::STR_MapArs: return ParseArg2<GDCC::IR::Arg_MapArs>(in, prog);
      case GDCC::STR_MapReg: return ParseArg2<GDCC::IR::Arg_MapReg>(in, prog);
      case GDCC::STR_StrArs: return ParseArg2<GDCC::IR::Arg_StrArs>(in, prog);
      case GDCC::STR_Vaa:    return ParseArg2<GDCC::IR::Arg_Vaa   >(in, prog);
      case GDCC::STR_WldArs: return ParseArg2<GDCC::IR::Arg_WldArs>(in, prog);
      case GDCC::STR_WldReg: return ParseArg2<GDCC::IR::Arg_WldReg>(in, prog);

      case GDCC::STR_GblArr: return ParseArg3<GDCC::IR::Arg_GblArr>(in, prog);
      case GDCC::STR_MapArr: return ParseArg3<GDCC::IR::Arg_MapArr>(in, prog);
      case GDCC::STR_StrArr: return ParseArg3<GDCC::IR::Arg_StrArr>(in, prog);
      case GDCC::STR_WldArr: return ParseArg3<GDCC::IR::Arg_WldArr>(in, prog);

      default:
         in.unget();
         std::cerr << "ERROR: " << in.peek().pos << ": bad statement argument: '"
            << in.peek().str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

