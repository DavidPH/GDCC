//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C attribute-specifier parsing.
//
// Since C has no attribute syntax, one based on C++11's is used instead.
//
// attribute-specifier-list:
//    attribute-specifier
//    attribute-specifier-list attribute-specifier
//
// attribute-specifier:
//    [ [ attribute-list ] ]
//    <__attribute__> ( ( attribute-list ) )
//
// attribute-list:
//    attribute
//    attribute-list , attribute
//
// attribute:
//    attribute-name attribute-argument-clause(opt)
//
// attribute-name:
//    identifier
//    keyword
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

#include "CC/Exp.hpp"

#include "AST/Attribute.hpp"
#include "AST/Exp.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/CallType.hpp"
#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"
#include "IR/ScriptType.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ParseAttr_address
      //
      // attribute-address:
      //    attribute-address-name ( constant-expression )
      //    attribute-address-name ( string-literal )
      //
      // attribute-address-name:
      //    <address>
      //    <__address>
      //
      static void ParseAttr_address(Parser &ctx, Scope &scope, AST::Attribute &attr)
      {
         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // string-literal
         if(ctx.in.peek().isTokString())
            attr.addrS = ctx.in.get().str;

         // constant-expression
         else
            attr.addrI = ctx.getExp_Cond(scope)->getIRExp();

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
      }

      //
      // ParseAttr_address_Lit
      //
      // attribute-address_Lit:
      //    attribute-address_lit-name ( constant-expression )
      //
      // attribute-address_Lit-name:
      //    <address_Lit>
      //    <__address_Lit>
      //
      static void ParseAttr_address_Lit(Parser &ctx, Scope &scope, AST::Attribute &attr)
      {
         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "'", true);

         // constant-expression
         if(ctx.in.peek(Core::TOK_ParenC))
            attr.addrL = nullptr;
         else
            attr.addrL = ctx.getExp_Cond(scope)->getIRExp();

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), "'", true);
      }

      //
      // ParseAttr_alloc_Aut
      //
      // attribute-alloc_Aut:
      //    attribute-alloc_Aut-name ( constant-expression )
      //
      // attribute-alloc_Aut-name:
      //    <alloc_Aut>
      //    <__alloc_Aut>
      //
      static void ParseAttr_alloc_Aut(Parser &ctx, Scope &scope, AST::Attribute &attr)
      {
         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // constant-expression
         attr.allocAut = ctx.getExp_Cond(scope)->getIRExp();

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
      }

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
      static void ParseAttr_call(Parser &ctx, Scope &, AST::Attribute &attr)
      {
         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // string-literal
         if(!ctx.in.peek().isTokString())
            throw Core::ParseExceptExpect(ctx.in.peek(), "string-literal", false);

         switch(ctx.in.get().str)
         {
            #define GDCC_IR_CallTypeList(name) \
               case Core::STR_##name: attr.callt = IR::CallType::name; break;
            #include "IR/CallTypeList.hpp"

         default:
            throw Core::ExceptStr(ctx.in.reget().pos, "invalid call");
         }

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
      }

      //
      // ParseAttr_deprecated
      //
      // attribute-deprecated:
      //    attribute-deprecated-name
      //    attribute-deprecated-name ( string-literal )
      //
      // attribute-deprecated-name:
      //    <deprecated>
      //    <__deprecated>
      //
      static void ParseAttr_deprecated(Parser &ctx, Scope &, AST::Attribute &attr)
      {
         // ( string-literal )
         if(ctx.in.drop(Core::TOK_ParenO))
         {
            // string-literal
            if(!ctx.in.peek().isTokString())
               throw Core::ParseExceptExpect(ctx.in.peek(), "string-literal", false);

            attr.warnUse = ctx.in.get().str;

            // )
            if(!ctx.in.drop(Core::TOK_ParenC))
               throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
         }
         else
            attr.warnUse = Core::STR_;
      }

      //
      // ParseAttr_extern
      //
      // attribute-extern:
      //    attribute-extern-name ( string-literal )
      //
      // attribute-extern-name:
      //    <extern>
      //    <__extern>
      //
      static void ParseAttr_extern(Parser &ctx, Scope &, AST::Attribute &attr)
      {
         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // string-literal
         if(!ctx.in.peek().isTokString())
            throw Core::ParseExceptExpect(ctx.in.peek(), "string-literal", false);

         switch(ctx.in.get().str)
         {
         case Core::STR_ACS:       attr.linka = IR::Linkage::ExtACS; break;
         case Core::STR_C:         attr.linka = IR::Linkage::ExtC;   break;
         case Core::STR_DS:        attr.linka = IR::Linkage::ExtDS;  break;
         case Core::STR_asm:       attr.linka = IR::Linkage::ExtASM; break;
         case Core::STR_NAM_ACSXX: attr.linka = IR::Linkage::ExtAXX; break;
         case Core::STR_NAM_CXX:   attr.linka = IR::Linkage::ExtCXX; break;

         default:
            throw Core::ExceptStr(ctx.in.reget().pos, "invalid linkage");
         }

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
      }

      //
      // ParseAttr_no_init
      //
      // attribute-no_init:
      //    attribute-no_init-name
      //
      // attribute-no_init-name:
      //    <no_init>
      //    <__no_init>
      //
      static void ParseAttr_no_init(Parser &, Scope &, AST::Attribute &attr)
      {
         attr.objNoInit = true;
      }

      //
      // ParseAttr_optional_args
      //
      // attribute-optional_args:
      //    attribute-optional_args-name ( constant-expression )
      //
      // attribute-optional_args-name:
      //    <optional_args>
      //    <__optional_args>
      //
      static void ParseAttr_optional_args(Parser &ctx, Scope &scope, AST::Attribute &attr)
      {
         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // constant-expression
         attr.paramOpt = ExpToFastU(ctx.getExp_Cond(scope));

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
      }

      //
      // ParserAttr_script
      //
      // attribute-script:
      //    attribute-script-name ( string-literal )
      //
      // attribute-script-name:
      //    <script>
      //    <__script>
      //
      static void ParseAttr_script(Parser &ctx, Scope &, AST::Attribute &attr)
      {
         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // string-literal
         if(!ctx.in.peek().isTokString())
            throw Core::ParseExceptExpect(ctx.in.peek(), "string-literal", false);

         switch(ctx.in.get().str)
         {
            // Script types.
            #define GDCC_IR_ScriptTypeList(name) \
               case Core::STR_##name: attr.stype = IR::ScriptType::name; break;
            #include "IR/ScriptTypeList.hpp"

            // Script flags.
         case Core::STR_Clientside: attr.sflagClS = true; break;
         case Core::STR_Net:        attr.sflagNet = true; break;

         default:
            throw Core::ExceptStr(ctx.in.reget().pos, "invalid script type/flag");
         }

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
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
      bool Parser::isAttrSpec(Scope &)
      {
         // [ [
         if(in.drop(Core::TOK_BrackO))
         {
            bool res = in.peek().tok == Core::TOK_BrackO;
            in.unget();
            return res;
         }

         // <__attribute__> ( (
         if(in.drop(Core::TOK_Identi, Core::STR___attribute__))
         {
            if(in.drop(Core::TOK_ParenO))
            {
               bool res = in.peek().tok == Core::TOK_ParenO;
               in.unget();
               in.unget();
               return res;
            }

            in.unget();
         }

         return false;
      }

      //
      // Parser::parseAttr
      //
      void Parser::parseAttr(Scope &scope, AST::Attribute &attr)
      {
         auto name = in.peek();

         if(name.tok != Core::TOK_Identi && name.tok != Core::TOK_KeyWrd)
            return;

         in.get();
         switch(name.str)
         {
         case Core::STR_address: case Core::STR___address:
            ParseAttr_address(*this, scope, attr); break;

         case Core::STR_address_Lit: case Core::STR___address_Lit:
            ParseAttr_address_Lit(*this, scope, attr); break;

         case Core::STR_alloc_Aut: case Core::STR___alloc_Aut:
            ParseAttr_alloc_Aut(*this, scope, attr); break;

         case Core::STR_call: case Core::STR___call:
            ParseAttr_call(*this, scope, attr); break;

         case Core::STR_deprecated: case Core::STR___deprecated:
            ParseAttr_deprecated(*this, scope, attr); break;

         case Core::STR_extern: case Core::STR___extern:
            ParseAttr_extern(*this, scope, attr); break;

         case Core::STR_no_init: case Core::STR___no_init:
            ParseAttr_no_init(*this, scope, attr); break;

         case Core::STR_optional_args: case Core::STR___optional_args:
            ParseAttr_optional_args(*this, scope, attr); break;

         case Core::STR_script: case Core::STR___script:
            ParseAttr_script(*this, scope, attr); break;

         default:
            // Skip unknown attribute's arguments, if any.
            if(in.peek(Core::TOK_ParenO))
               skipBalancedToken();
            break;
         }
      }

      //
      // Parser::parseAttrList
      //
      void Parser::parseAttrList(Scope &scope, AST::Attribute &attr)
      {
         // attribute-list:
         //    attribute
         //    attribute-list , attribute
         do parseAttr(scope, attr);
         while(in.drop(Core::TOK_Comma));
      }

      //
      // Parser::parseAttrSpec
      //
      void Parser::parseAttrSpec(Scope &scope, AST::Attribute &attr)
      {
         // attribute-specifier:
         //    [ [ attribute-list ] ]
         //    <__attribute__> ( ( attribute-list ) )

         // [ [ attribute-list ] ]
         if(in.drop(Core::TOK_BrackO))
         {
            // [ [
            if(!in.drop(Core::TOK_BrackO))
               throw Core::ParseExceptExpect(in.peek(), "[", true);

            // attribute-list
            parseAttrList(scope, attr);

            // ] ]
            if(!in.drop(Core::TOK_BrackC) || !in.drop(Core::TOK_BrackC))
               throw Core::ParseExceptExpect(in.peek(), "]", true);
         }

         // <__attribute__> ( ( attribute-list ) )
         else if(in.drop(Core::TOK_Identi, Core::STR___attribute__))
         {
            // ( (
            if(!in.drop(Core::TOK_ParenO) || !in.drop(Core::TOK_ParenO))
               throw Core::ParseExceptExpect(in.peek(), "(", true);

            // attribute-list
            parseAttrList(scope, attr);

            // ) )
            if(!in.drop(Core::TOK_ParenC) || !in.drop(Core::TOK_ParenC))
               throw Core::ParseExceptExpect(in.peek(), ")", true);
         }

         else
            throw Core::ParseExceptExpect(in.peek(), "attribute-specifier", false);
      }

      //
      // Parser::parseAttrSpecList
      //
      void Parser::parseAttrSpecList(Scope &scope, AST::Attribute &attr)
      {
         do parseAttrSpec(scope, attr);
         while(isAttrSpec(scope));
      }
   }
}

// EOF

