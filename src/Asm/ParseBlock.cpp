//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Block parsing utilities.
//
//-----------------------------------------------------------------------------

#include "Parse.hpp"

#include "Macro.hpp"

#include "GDCC/TokenStream.hpp"

#include "GDCC/IR/Block.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // ParseBlock
   //
   void ParseBlock(GDCC::TokenStream &in, GDCC::IR::Block &block, GDCC::TokenType end)
   {
      std::vector<GDCC::IR::Arg> args;
      GDCC::IR::Code code;

      while(!in.drop(end)) switch(in.peek().tok)
      {
      case GDCC::TOK_String:
         block.addLabel(in.get().str);
         break;

      case GDCC::TOK_Identi:
         block.setOrigin(in.peek().pos);

         switch(static_cast<GDCC::StringIndex>(in.get().str))
         {
            #define GDCC_IR_CodeList(c) \
               case GDCC::STR_##c: code = GDCC::IR::Code::c; break;
            #include "GDCC/IR/CodeList.hpp"

         default:
            std::cerr << "ERROR:" << in.peek().pos << ": unknown code: '"
               << in.peek().str << "'\n";
            throw EXIT_FAILURE;
         }

         args.clear();

         while(in.peek().tok != GDCC::TOK_LnEnd)
            args.push_back(ParseArg(SkipToken(in, GDCC::TOK_Comma, "end of line")));

         block.setArgs(GDCC::Array<GDCC::IR::Arg>(GDCC::Move, args.begin(), args.end()));
         block.addStatement(code);

         break;

      case GDCC::TOK_Not:
         in.get();
         ExpectToken(in, GDCC::TOK_Identi, "identifier");
         if(auto macro = Macro::Find(in.peek().str))
         {
            block.setOrigin(in.get().pos);

            // Read arguments to macro invocation.
            args.clear();

            while(in.peek().tok != GDCC::TOK_LnEnd)
               args.push_back(ParseArg(SkipToken(in, GDCC::TOK_Comma, "end of line")));

            // Expand macro.
            macro->expand(block, args.data(), args.size());
         }
         else
         {
            std::cerr << "ERROR: " << in.peek().pos << ": expected macro name\n";
            throw EXIT_FAILURE;
         }
         break;

      case GDCC::TOK_LnEnd:
         in.get();
         break;

      default:
         std::cerr << "ERROR: " << in.peek().pos << ": expected block terminator\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

