//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2024 David Hill
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

#include "../ACC/Types.hpp"

#include "../CC/Parse.hpp"

#include <utility>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
{
   //
   // Parser
   //
   class Parser : public CC::Parser
   {
   public:
      using CC::Parser::getDecl;
      using CC::Parser::getExp_Post;
      using CC::Parser::getSt;

      Parser(Core::TokenStream &in, Factory &fact, PragmaData &prag,
         IR::Program &prog, bool importing = false);

      std::unique_ptr<Parser> clone(Core::TokenStream &in_) const
         {return static_cast<std::unique_ptr<Parser>>(cloneRaw(in_));}

      virtual StatementCRef getDecl(Scope_Global &scope);
      virtual StatementCRef getDecl(CC::Scope_Local &scope, SR::Attribute &&attr, Labels &&labels);

      virtual StatementCRef getDecl_CreateTrans(Scope_Global &scope);

      virtual StatementCRef getDecl_Enum(Scope_Global &scope);

      virtual StatementCRef getDecl_Function(Scope_Global &scope);

      virtual StatementCRef getDecl_Object(Scope_Global &scope);
      virtual StatementCRef getDecl_Object(CC::Scope_Local &scope, Labels &&labels);

      virtual StatementCRef getDecl_Print(Scope_Global &scope);

      virtual StatementCRef getDecl_Script(Scope_Global &scope);

      virtual StatementCRef getDecl_Special(Scope_Global &scope);

      virtual StatementCRef getDecl_Struct(Scope_Global &scope);

      virtual ExpCRef getExp_Post_ParenO(CC::Scope &scope, SR::Exp const *exp);

      virtual ExpCRef getExp_Unar_createtranslation(CC::Scope &scope,
         CreateTransDecl const *ctrans);
      virtual ExpCRef getExp_Unar_print(CC::Scope &scope, PrintDecl const *print);

      virtual ExpCRef getExp_Unar_Identi(CC::Scope &scope);
      virtual ExpCRef getExp_Unar_KeyWrd(CC::Scope &scope);

      virtual ExpCRef getExp_Prim(CC::Scope &scope);
      virtual ExpCRef getExp_Post(CC::Scope &scope);
      virtual ExpCRef getExp_Cast(CC::Scope &scope);

      virtual CC::InitRaw getInitRaw(CC::Scope &scope);

      virtual StatementCRef getSt(CC::Scope_Local &scope, SR::Attribute &&attr, Labels &&labels);
      virtual StatementCRef getSt_do(CC::Scope_Local &scope, SR::Attribute &&attr, Labels &&labels);
      virtual StatementCRef getSt_goto(CC::Scope_Local &scope, SR::Attribute &&attr, Labels &&labels);
      virtual StatementCRef getSt_restart(CC::Scope_Local &scope, SR::Attribute &&attr, Labels &&labels);
      virtual StatementCRef getSt_while(CC::Scope_Local &scope, SR::Attribute &&attr, Labels &&labels);
      virtual StatementCRef getSt_terminate(CC::Scope_Local &scope, SR::Attribute &&attr, Labels &&labels);

      virtual bool isAttrSpec(CC::Scope &scope);

      virtual bool isDecl(CC::Scope &scope);

      virtual bool isDeclSpec(CC::Scope &scope);

      virtual bool isDeclarator(CC::Scope &scope);

      virtual bool isTypeQual(CC::Scope &scope);

      virtual bool isTypeSpec(CC::Scope &scope);

      virtual void parseAttrSpec(CC::Scope &scope, SR::Attribute &attr);

      virtual void parseDeclSpec(CC::Scope &scope, SR::Attribute &attr);

      virtual void parseDeclarator(CC::Scope &scope, SR::Attribute &attr);
      virtual void parseDeclaratorSuffix(CC::Scope &scope, SR::Attribute &attr);

      virtual void parseTypeQual(CC::Scope &scope, SR::TypeQual &qual);

      virtual void parseTypeSpec(CC::Scope &scope, SR::Attribute &attr, CC::TypeSpec &spec);

      PragmaData &prag;

      bool const importing;

   protected:
      Parser(Parser const &ctx, Core::TokenStream &in);

      virtual Parser *cloneRaw(Core::TokenStream &in_) const
         {return new Parser(*this, in_);}

      virtual IR::Linkage defLinkage() const;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   bool ParseEscape(std::ostream &out, std::istream &in, char escape);

   void ParseFile(char const *inName, IR::Program &prog);
}

#endif//GDCC__ACC__Parse_H__

