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

#include "IStream.hpp"

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
   void ParseBlock(IStream &in, GDCC::IR::Block &block, GDCC::TokenType end)
   {
      std::vector<GDCC::IR::Arg> args;
      GDCC::IR::Code code;

      for(GDCC::Token tok; in >> tok;) switch(tok.tok)
      {
      case GDCC::TOK_String:
         block.addLabel(tok.str);
         break;

      case GDCC::TOK_Identifier:
         switch(static_cast<GDCC::StringIndex>(tok.str))
         {
            #define GDCC_IR_CodeList(c) \
               case GDCC::STR_##c: code = GDCC::IR::Code::c; break;
            #include "GDCC/IR/CodeList.hpp"

         default:
            std::cerr << "ERROR:" << tok.pos << ": unknown code: '" << tok.str << "'\n";
            throw EXIT_FAILURE;
         }

         args.clear();

         while(in >> tok && tok.tok != GDCC::TOK_EOL)
         {
            if(tok.tok != GDCC::TOK_Comma)
            {
               std::cerr << "ERROR: " << tok.pos << ": expected end of line\n";
               throw EXIT_FAILURE;
            }

            args.push_back(ParseArg(in));
         }

         block.setArgs(GDCC::Array<GDCC::IR::Arg>(GDCC::Move, args.begin(), args.end()));
         block.addStatement(code);

         break;

      case GDCC::TOK_EOL:
         break;

      default:
         if(tok.tok == end) return;

         std::cerr << "ERROR: " << tok.pos << ": expected block terminator\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

