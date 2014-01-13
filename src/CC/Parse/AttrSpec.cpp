//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C attribute-specifier parsing.
//
// Since C has no attribute syntax, one based on C++11's is used instead.
//
// attribute-specifier:
//    [ [ attribute-list ] ]
//
// attribute-list:
//    attribute(opt)
//    attribute-list , attribute(opt)
//
// attribute:
//    identifier attribute-argument-clause(opt)
//
// attribute-argument-clause:
//    ( balanced-token-seq )
//
// balanced-token-seq:
//    balanced-token
//    balanced-token-seq balanced-token
//
// balanced-token:
//    ( balanced-token-seq )
//    [ balanced-token-seq ]
//    { balanced-token-seq }
//    any token other than a parenthesis, a bracket, or a brace
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "AST/Attribute.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// ParseAttr_call
//
// attribute-call:
//    attribute-call-name ( string-literal )
//
// attribute-call-name:
//    <call>
//    <__call>
//
static void ParseAttr_call(GDCC::CC::ParserData &in, GDCC::CC::Scope *,
   GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   if(!in.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(in.in.peek().pos, "expected '('");

   if(in.in.peek().tok != Core::TOK_String)
      throw Core::ExceptStr(in.in.peek().pos, "expected string");

   switch(in.in.get().str)
   {
   case Core::STR_None:    attr.callt = IR::CallType::None;    break;
   case Core::STR_Action:  attr.callt = IR::CallType::Action;  break;
   case Core::STR_AsmFunc: attr.callt = IR::CallType::AsmFunc; break;
   case Core::STR_LangACS: attr.callt = IR::CallType::LangACS; break;
   case Core::STR_LangASM: attr.callt = IR::CallType::LangASM; break;
   case Core::STR_LangAXX: attr.callt = IR::CallType::LangAXX; break;
   case Core::STR_LangC:   attr.callt = IR::CallType::LangC;   break;
   case Core::STR_LangCXX: attr.callt = IR::CallType::LangCXX; break;
   case Core::STR_LangDS:  attr.callt = IR::CallType::LangDS;  break;
   case Core::STR_Native:  attr.callt = IR::CallType::Native;  break;
   case Core::STR_Script:  attr.callt = IR::CallType::Script;  break;
   case Core::STR_ScriptI: attr.callt = IR::CallType::ScriptI; break;
   case Core::STR_ScriptS: attr.callt = IR::CallType::ScriptS; break;
   case Core::STR_Special: attr.callt = IR::CallType::Special; break;

   default:
      throw Core::ExceptStr(in.in.reget().pos, "invalid call");
   }

   if(!in.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ')'");
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // IsAttrSpec
      //
      // Checks if the next token sequence begins an attribute. This is an
      // unusual check in the realm of C for two reasons. Most obviously
      // because it's a multi-token check. This specific function is why
      // CPP::TStream needs a multi-token unget. The other oddity is the need
      // for certainty. Because this syntax is a language extension, it
      // absolutely must not ever alter the interpretation of strictly
      // conforming programs.
      //
      bool IsAttrSpec(ParserData &in, Scope *)
      {
         // [ [
         if(in.in.drop(Core::TOK_BrackO))
         {
            bool res = in.in.peek().tok == Core::TOK_BrackO;
            in.in.unget();
            return res;
         }

         return false;
      }

      //
      // ParseAttr
      //
      void ParseAttr(ParserData &in, Scope *ctx, AST::Attribute &attr)
      {
         auto name = in.in.peek();

         if(name.tok != Core::TOK_Identi && name.tok != Core::TOK_KeyWrd)
            return;

         in.in.get();
         switch(name.str)
         {
         case Core::STR_call: case Core::STR___call:
            ParseAttr_call(in, ctx, attr); break;

         default:
            // Skip unknown attribute.
            if(in.in.drop(Core::TOK_ParenO))
            {
               for(unsigned depth = 1; depth;)
               {
                  auto const &tok = in.in.get();

                       if(tok.tok == Core::TOK_ParenO) ++depth;
                  else if(tok.tok == Core::TOK_ParenC) --depth;
                  else if(tok.tok == Core::TOK_EOF)
                     throw Core::ExceptStr(tok.pos, "unexpected end of file");
               }
            }
            break;
         }
      }

      //
      // ParseAttrSpec
      //
      void ParseAttrSpec(ParserData &in, Scope *ctx, AST::Attribute &attr)
      {
         // attribute-specifier:
         //    [ [ attribute-list ] ]
         if(in.in.drop(Core::TOK_BrackO))
         {
            if(!in.in.drop(Core::TOK_BrackO))
               throw Core::ExceptStr(in.in.peek().pos, "expected '['");

            // attribute-list:
            //    attribute(opt)
            //    attribute-list , attribute(opt)
            do ParseAttr(in, ctx, attr);
            while(in.in.drop(Core::TOK_Comma));

            // ] ]
            if(!in.in.drop(Core::TOK_BrackC) || !in.in.drop(Core::TOK_BrackC))
               throw Core::ExceptStr(in.in.peek().pos, "expected ']'");
         }
      }
   }
}

// EOF

