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
// attribute-specifier-list:
//    attribute-specifier attribute-specifier-list(opt)
//
// attribute-specifier:
//    [ [ attribute-list ] ]
//    <__attribute__> ( ( attribute-list ) )
//
// attribute-list:
//    attribute(opt)
//    attribute-list , attribute(opt)
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
static void ParseAttr_address(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope, GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // (
   if(!ctx.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

   // string-literal
   if(ctx.in.peek().isTokString())
      attr.addrS = ctx.in.get().str;

   // constant-expression
   else
      attr.addrI = CC::GetExp_Cond(ctx, scope)->getIRExp();

   // )
   if(!ctx.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");
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
static void ParseAttr_alloc_Loc(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope, GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // (
   if(!ctx.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

   // constant-expression
   attr.allocLoc = CC::GetExp_Cond(ctx, scope)->getIRExp();

   // )
   if(!ctx.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");
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
static void ParseAttr_call(GDCC::CC::ParserCtx const &ctx, GDCC::CC::Scope &,
   GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // (
   if(!ctx.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

   // string-literal
   if(!ctx.in.peek().isTokString())
      throw Core::ExceptStr(ctx.in.peek().pos, "expected string-literal");

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
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");
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
static void ParseAttr_extern(GDCC::CC::ParserCtx const &ctx, GDCC::CC::Scope &,
   GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // (
   if(!ctx.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

   // string-literal
   if(!ctx.in.peek().isTokString())
      throw Core::ExceptStr(ctx.in.peek().pos, "expected string-literal");

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
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");
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
static void ParseAttr_no_init(GDCC::CC::ParserCtx const &, GDCC::CC::Scope &,
   GDCC::AST::Attribute &attr)
{
   attr.objNoInit = true;
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
static void ParseAttr_script(GDCC::CC::ParserCtx const &ctx, GDCC::CC::Scope &,
   GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // (
   if(!ctx.in.drop(Core::TOK_ParenO))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

   // string-literal
   if(!ctx.in.peek().isTokString())
      throw Core::ExceptStr(ctx.in.peek().pos, "expected string-literal");

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
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");
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

         case Core::STR_alloc_Loc: case Core::STR___alloc_Loc:
            ParseAttr_alloc_Loc(ctx, scope, attr); break;

         case Core::STR_call: case Core::STR___call:
            ParseAttr_call(ctx, scope, attr); break;

         case Core::STR_extern:
            ParseAttr_extern(ctx, scope, attr); break;

         case Core::STR_no_init: case Core::STR___no_init:
            ParseAttr_no_init(ctx, scope, attr); break;

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
         //    attribute(opt)
         //    attribute-list , attribute(opt)
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
               throw Core::ExceptStr(ctx.in.peek().pos, "expected '['");

            // attribute-list
            ParseAttrList(ctx, scope, attr);

            // ] ]
            if(!ctx.in.drop(Core::TOK_BrackC) || !ctx.in.drop(Core::TOK_BrackC))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected ']'");
         }

         // <__attribute__> ( ( attribute-list ) )
         else if(ctx.in.drop(Core::TOK_Identi, Core::STR___attribute__))
         {
            // ( (
            if(!ctx.in.drop(Core::TOK_ParenO) || !ctx.in.drop(Core::TOK_ParenO))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

            // attribute-list
            ParseAttrList(ctx, scope, attr);

            // ) )
            if(!ctx.in.drop(Core::TOK_ParenC) || !ctx.in.drop(Core::TOK_ParenC))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");
         }

         else
            throw Core::ExceptStr(ctx.in.peek().pos, "expected attribute-specifier");
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

