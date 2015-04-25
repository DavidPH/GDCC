//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
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
      static void ParseAttr_address(ParserCtx const &ctx, Scope &scope,
         AST::Attribute &attr)
      {
         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // string-literal
         if(ctx.in.peek().isTokString())
            attr.addrS = ctx.in.get().str;

         // constant-expression
         else
            attr.addrI = GetExp_Cond(ctx, scope)->getIRExp();

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
      static void ParseAttr_address_Lit(ParserCtx const &ctx, Scope &scope,
         AST::Attribute &attr)
      {
         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "'", true);

         // constant-expression
         if(ctx.in.peek(Core::TOK_ParenC))
            attr.addrL = nullptr;
         else
            attr.addrL = GetExp_Cond(ctx, scope)->getIRExp();

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), "'", true);
      }

      //
      // ParseAttr_alloc_Loc
      //
      // attribute-alloc_Loc:
      //    attribute-alloc_Loc-name ( constant-expression )
      //
      // attribute-alloc_Loc-name:
      //    <alloc_Loc>
      //    <__alloc_Loc>
      //
      static void ParseAttr_alloc_Loc(ParserCtx const &ctx, Scope &scope,
         AST::Attribute &attr)
      {
         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // constant-expression
         attr.allocLoc = GetExp_Cond(ctx, scope)->getIRExp();

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
      static void ParseAttr_call(ParserCtx const &ctx, Scope &,
         AST::Attribute &attr)
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
      static void ParseAttr_deprecated(ParserCtx const &ctx, Scope &,
         AST::Attribute &attr)
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
      //
      static void ParseAttr_extern(ParserCtx const &ctx, Scope &,
         AST::Attribute &attr)
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
      static void ParseAttr_no_init(ParserCtx const &, Scope &,
         AST::Attribute &attr)
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
      static void ParseAttr_optional_args(ParserCtx const &ctx, Scope &scope,
         AST::Attribute &attr)
      {
         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // constant-expression
         attr.paramOpt = ExpToFastU(GetExp_Cond(ctx, scope));

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
      static void ParseAttr_script(ParserCtx const &ctx, Scope &,
         AST::Attribute &attr)
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
      bool IsAttrSpec(ParserCtx const &ctx, Scope &)
      {
         // [ [
         if(ctx.in.drop(Core::TOK_BrackO))
         {
            bool res = ctx.in.peek().tok == Core::TOK_BrackO;
            ctx.in.unget();
            return res;
         }

         // <__attribute__> ( (
         if(ctx.in.drop(Core::TOK_Identi, Core::STR___attribute__))
         {
            if(ctx.in.drop(Core::TOK_ParenO))
            {
               bool res = ctx.in.peek().tok == Core::TOK_ParenO;
               ctx.in.unget();
               ctx.in.unget();
               return res;
            }

            ctx.in.unget();
         }

         return false;
      }

      //
      // ParseAttr
      //
      void ParseAttr(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr)
      {
         auto name = ctx.in.peek();

         if(name.tok != Core::TOK_Identi && name.tok != Core::TOK_KeyWrd)
            return;

         ctx.in.get();
         switch(name.str)
         {
         case Core::STR_address: case Core::STR___address:
            ParseAttr_address(ctx, scope, attr); break;

         case Core::STR_address_Lit: case Core::STR___address_Lit:
            ParseAttr_address_Lit(ctx, scope, attr); break;

         case Core::STR_alloc_Loc: case Core::STR___alloc_Loc:
            ParseAttr_alloc_Loc(ctx, scope, attr); break;

         case Core::STR_call: case Core::STR___call:
            ParseAttr_call(ctx, scope, attr); break;

         case Core::STR_deprecated: case Core::STR___deprecated:
            ParseAttr_deprecated(ctx, scope, attr); break;

         case Core::STR_extern:
            ParseAttr_extern(ctx, scope, attr); break;

         case Core::STR_no_init: case Core::STR___no_init:
            ParseAttr_no_init(ctx, scope, attr); break;

         case Core::STR_optional_args: case Core::STR___optional_args:
            ParseAttr_optional_args(ctx, scope, attr); break;

         case Core::STR_script: case Core::STR___script:
            ParseAttr_script(ctx, scope, attr); break;

         default:
            // Skip unknown attribute.
            if(ctx.in.drop(Core::TOK_ParenO))
            {
               for(unsigned depth = 1; depth;)
               {
                  auto const &tok = ctx.in.get();

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
      // ParseAttrList
      //
      void ParseAttrList(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr)
      {
         // attribute-list:
         //    attribute
         //    attribute-list , attribute
         do ParseAttr(ctx, scope, attr);
         while(ctx.in.drop(Core::TOK_Comma));
      }

      //
      // ParseAttrSpec
      //
      void ParseAttrSpec(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr)
      {
         // attribute-specifier:
         //    [ [ attribute-list ] ]
         //    <__attribute__> ( ( attribute-list ) )

         // [ [ attribute-list ] ]
         if(ctx.in.drop(Core::TOK_BrackO))
         {
            // [ [
            if(!ctx.in.drop(Core::TOK_BrackO))
               throw Core::ParseExceptExpect(ctx.in.peek(), "[", true);

            // attribute-list
            ParseAttrList(ctx, scope, attr);

            // ] ]
            if(!ctx.in.drop(Core::TOK_BrackC) || !ctx.in.drop(Core::TOK_BrackC))
               throw Core::ParseExceptExpect(ctx.in.peek(), "]", true);
         }

         // <__attribute__> ( ( attribute-list ) )
         else if(ctx.in.drop(Core::TOK_Identi, Core::STR___attribute__))
         {
            // ( (
            if(!ctx.in.drop(Core::TOK_ParenO) || !ctx.in.drop(Core::TOK_ParenO))
               throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

            // attribute-list
            ParseAttrList(ctx, scope, attr);

            // ) )
            if(!ctx.in.drop(Core::TOK_ParenC) || !ctx.in.drop(Core::TOK_ParenC))
               throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
         }

         else
            throw Core::ParseExceptExpect(ctx.in.peek(), "attribute-specifier", false);
      }

      //
      // ParseAttrSpecList
      //
      void ParseAttrSpecList(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr)
      {
         do ParseAttrSpec(ctx, scope, attr);
         while(IsAttrSpec(ctx, scope));
      }
   }
}

// EOF

