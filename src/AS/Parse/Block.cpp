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

#include "AS/Parse.hpp"

#include "AS/Macro.hpp"

#include "Core/TokenStream.hpp"

#include "IR/Block.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseBlock
      //
      void ParseBlock(Core::TokenStream &in, IR::Program &prog,
         IR::Block &block, Core::TokenType end)
      {
         std::vector<IR::Arg> args;
         IR::Code code;

         while(!in.drop(end)) switch(in.peek().tok)
         {
         case Core::TOK_String:
            block.addLabel(in.get().str);
            break;

         case Core::TOK_Identi:
            block.setOrigin(in.peek().pos);

            switch(static_cast<Core::StringIndex>(in.get().str))
            {
               #define GDCC_IR_CodeList(c) \
                  case Core::STR_##c: code = IR::Code::c; break;
               #include "IR/CodeList.hpp"

            default:
               std::cerr << "ERROR:" << in.peek().pos << ": unknown code: '"
                  << in.peek().str << "'\n";
               throw EXIT_FAILURE;
            }

            args.clear();

            while(in.peek().tok != Core::TOK_LnEnd)
               args.push_back(ParseArg(SkipToken(in, Core::TOK_Comma, "end of line"), prog));

            block.setArgs(Core::Array<IR::Arg>(Core::Move, args.begin(), args.end()));
            block.addStatement(code);

            break;

         case Core::TOK_Not:
            in.get();
            ExpectToken(in, Core::TOK_Identi, "identifier");
            if(auto macro = Macro::Find(in.peek().str))
            {
               block.setOrigin(in.get().pos);

               // Read arguments to macro invocation.
               args.clear();

               while(in.peek().tok != Core::TOK_LnEnd)
                  args.push_back(ParseArg(SkipToken(in, Core::TOK_Comma, "end of line"), prog));

               // Expand macro.
               macro->expand(block, args.data(), args.size());
            }
            else
            {
               std::cerr << "ERROR: " << in.peek().pos << ": expected macro name\n";
               throw EXIT_FAILURE;
            }
            break;

         case Core::TOK_LnEnd:
            in.get();
            break;

         default:
            std::cerr << "ERROR: " << in.peek().pos << ": expected block terminator\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

