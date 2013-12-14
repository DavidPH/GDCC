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

#include "AS/Parse.hpp"

#include "Core/TokenStream.hpp"

#include "IR/Arg.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Function                                                            |
//

//
// ParseArg0
//
template<typename T>
static T ParseArg0(GDCC::Core::TokenStream &in, GDCC::IR::Program &)
{
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   return T();
}

//
// ParseArgU
//
template<typename T>
static T ParseArgU(GDCC::Core::TokenStream &in, GDCC::IR::Program &prog)
{
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");
   auto exp = GDCC::AS::ParseFastU(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   return T(std::move(exp));
}

//
// ParseArg1
//
template<typename T>
static T ParseArg1(GDCC::Core::TokenStream &in, GDCC::IR::Program &prog)
{
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");
   auto exp = GDCC::AS::ParseExp(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   return T(std::move(exp));
}

//
// ParseArg2
//
template<typename T>
static T ParseArg2(GDCC::Core::TokenStream &in, GDCC::IR::Program &prog)
{
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");
   auto arg = GDCC::AS::ParseArg(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_Comma, ",");
   auto exp = GDCC::AS::ParseExp(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   return T(std::move(arg), std::move(exp));
}

//
// ParseArg3
//
template<typename T>
static T ParseArg3(GDCC::Core::TokenStream &in, GDCC::IR::Program &prog)
{
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenO, "(");
   auto arg0 = GDCC::AS::ParseArg(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_Comma, ",");
   auto arg1 = GDCC::AS::ParseArg(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_Comma, ",");
   auto exp  = GDCC::AS::ParseExp(in, prog);
   GDCC::AS::SkipToken(in, GDCC::Core::TOK_ParenC, ")");

   return T(std::move(arg0), std::move(arg1), std::move(exp));
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseArg
      //
      IR::Arg ParseArg(Core::TokenStream &in, IR::Program &prog)
      {
         ExpectToken(in, Core::TOK_Identi, "identifier");
         switch(static_cast<Core::StringIndex>(in.get().str))
         {
         case Core::STR_Gen:    return ParseArg2<IR::Arg_Gen   >(in, prog);

         case Core::STR_Cpy:    return ParseArgU<IR::Arg_Cpy   >(in, prog);
         case Core::STR_Lit:    return ParseArg1<IR::Arg_Lit   >(in, prog);
         case Core::STR_Nul:    return ParseArg0<IR::Arg_Nul   >(in, prog);
         case Core::STR_Stk:    return ParseArg0<IR::Arg_Stk   >(in, prog);

         case Core::STR_Far:    return ParseArg2<IR::Arg_Far   >(in, prog);
         case Core::STR_GblArs: return ParseArg2<IR::Arg_GblArs>(in, prog);
         case Core::STR_GblReg: return ParseArg2<IR::Arg_GblReg>(in, prog);
         case Core::STR_Loc:    return ParseArg2<IR::Arg_Loc   >(in, prog);
         case Core::STR_LocArs: return ParseArg2<IR::Arg_LocArs>(in, prog);
         case Core::STR_LocReg: return ParseArg2<IR::Arg_LocReg>(in, prog);
         case Core::STR_MapArs: return ParseArg2<IR::Arg_MapArs>(in, prog);
         case Core::STR_MapReg: return ParseArg2<IR::Arg_MapReg>(in, prog);
         case Core::STR_StrArs: return ParseArg2<IR::Arg_StrArs>(in, prog);
         case Core::STR_Vaa:    return ParseArg2<IR::Arg_Vaa   >(in, prog);
         case Core::STR_WldArs: return ParseArg2<IR::Arg_WldArs>(in, prog);
         case Core::STR_WldReg: return ParseArg2<IR::Arg_WldReg>(in, prog);

         case Core::STR_GblArr: return ParseArg3<IR::Arg_GblArr>(in, prog);
         case Core::STR_MapArr: return ParseArg3<IR::Arg_MapArr>(in, prog);
         case Core::STR_StrArr: return ParseArg3<IR::Arg_StrArr>(in, prog);
         case Core::STR_WldArr: return ParseArg3<IR::Arg_WldArr>(in, prog);

         default:
            in.unget();
            std::cerr << "ERROR: " << in.peek().pos << ": bad statement argument: '"
               << in.peek().str << "'\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

