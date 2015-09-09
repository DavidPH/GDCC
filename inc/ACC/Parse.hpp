//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS parsing.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Parse_H__
#define GDCC__ACC__Parse_H__

#include "../CC/Parse.hpp"

#include <utility>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace ACC
   {
      class CreateTransDecl;
      class PragmaData;
      class PrintDecl;
      class Scope_Global;

      //
      // Parser
      //
      class Parser : public CC::Parser
      {
      public:
         using CC::Parser::getDecl;
         using CC::Parser::getExp_Post;

         Parser(Core::TokenStream &in, PragmaData &prag, IR::Program &prog,
            bool importing = false);

         std::unique_ptr<Parser> clone(Core::TokenStream &in_) const
            {return static_cast<std::unique_ptr<Parser>>(cloneRaw(in_));}

         virtual StatementCRef getDecl(Scope_Global &scope);
         virtual StatementCRef getDecl(CC::Scope_Local &scope);

         virtual StatementCRef getDecl_CreateTrans(Scope_Global &scope);

         virtual StatementCRef getDecl_Enum(Scope_Global &scope);

         virtual StatementCRef getDecl_Function(Scope_Global &scope);

         virtual StatementCRef getDecl_Object(Scope_Global &scope);
         virtual StatementCRef getDecl_Object(CC::Scope_Local &scope);

         virtual StatementCRef getDecl_Print(Scope_Global &scope);

         virtual StatementCRef getDecl_Script(Scope_Global &scope);

         virtual StatementCRef getDecl_Special(Scope_Global &scope);

         virtual StatementCRef getDecl_Struct(Scope_Global &scope);

         virtual ExpCRef getExp_Post_ParenO(CC::Scope &scope, AST::Exp const *exp);

         virtual ExpCRef getExp_Unar_createtranslation(CC::Scope &scope,
            CreateTransDecl const *ctrans);
         virtual ExpCRef getExp_Unar_print(CC::Scope &scope, PrintDecl const *print);

         virtual ExpCRef getExp_Unar_Identi(CC::Scope &scope);
         virtual ExpCRef getExp_Unar_KeyWrd(CC::Scope &scope);

         virtual ExpCRef getExp_Prim(CC::Scope &scope);
         virtual ExpCRef getExp_Post(CC::Scope &scope);
         virtual ExpCRef getExp_Cast(CC::Scope &scope);

         virtual CC::InitRaw getInitRaw(CC::Scope &scope);

         virtual StatementCRef getStatement(CC::Scope_Local &scope);

         virtual bool isAttrSpec(CC::Scope &scope);

         virtual bool isDecl(CC::Scope &scope);

         virtual bool isDeclSpec(CC::Scope &scope);

         virtual bool isDeclarator(CC::Scope &scope);

         virtual bool isTypeQual(CC::Scope &scope);

         virtual bool isTypeSpec(CC::Scope &scope);

         virtual void parseAttrSpec(CC::Scope &scope, AST::Attribute &attr);

         virtual void parseDeclSpec(CC::Scope &scope, AST::Attribute &attr);

         virtual void parseDeclarator(CC::Scope &scope, AST::Attribute &attr);
         virtual void parseDeclaratorSuffix(CC::Scope &scope, AST::Attribute &attr);

         virtual void parseTypeQual(CC::Scope &scope, AST::TypeQual &qual);

         virtual void parseTypeSpec(CC::Scope &scope, AST::Attribute &attr, CC::TypeSpec &spec);

         PragmaData &prag;

         bool const importing;

      protected:
         Parser(Parser const &ctx, Core::TokenStream &in);

         virtual Parser *cloneRaw(Core::TokenStream &in_) const
            {return new Parser(*this, in_);}
      };
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      void ParseFile(char const *inName, IR::Program &prog);
   }
}

#endif//GDCC__ACC__Parse_H__

