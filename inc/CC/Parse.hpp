//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C parsing.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Parse_H__
#define GDCC__CC__Parse_H__

#include "../CC/Types.hpp"

#include "../Core/Token.hpp"

#include <memory>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Parser
   //
   class Parser
   {
   protected:
      using ExpCRef       = Core::CounterRef<SR::Exp       const>;
      using Labels        = Core::Array<Core::String>;
      using StatementCRef = Core::CounterRef<SR::Statement const>;
      using TypeCRef      = Core::CounterRef<SR::Type      const>;
      using TypeSetCRef   = Core::CounterRef<SR::TypeSet   const>;

   public:
      Parser(Core::TokenStream &in, CPP::PragmaData &prag, IR::Program &prog);

      virtual ~Parser() {}

      std::unique_ptr<Parser> clone(Core::TokenStream &in_) const
         {return static_cast<std::unique_ptr<Parser>>(cloneRaw(in_));}

      Core::Token const &expect(Core::TokenType tok);
      Core::Token const &expect(Core::TokenType tok, Core::String str);
      Core::Token const &expectIdenti(bool orKeyWrd = false);
      Core::Token const &expectString();

      virtual StatementCRef getDecl(Scope_Global &scope);
      StatementCRef getDecl(Scope_Local &scope);
      virtual StatementCRef getDecl(Scope_Local &scope, Labels &&labels);

      virtual ExpCRef getExp_CLit(Scope &scope);
      virtual ExpCRef getExp_CLit(Scope &scope, SR::Type const *type);

      virtual ExpCRef getExp_Post_ParenO(Scope &scope, SR::Exp const *exp);

      virtual ExpCRef getExp_Init(Scope &scope, SR::Type const *type);

      virtual ExpCRef getExp_Unar_Identi(Scope &scope);
      virtual ExpCRef getExp_Unar_KeyWrd(Scope &scope);

      virtual ExpCRef getExp_Prim(Scope &scope);
      virtual ExpCRef getExp_Post(Scope &scope);
      virtual ExpCRef getExp_Post(Scope &scope, SR::Exp const *exp);
      virtual ExpCRef getExp_Unar(Scope &scope);
      virtual ExpCRef getExp_Cast(Scope &scope);
      virtual ExpCRef getExp_Mult(Scope &scope);
      virtual ExpCRef getExp_Addi(Scope &scope);
      virtual ExpCRef getExp_Shft(Scope &scope);
      virtual ExpCRef getExp_Rela(Scope &scope);
      virtual ExpCRef getExp_Equa(Scope &scope);
      virtual ExpCRef getExp_BAnd(Scope &scope);
      virtual ExpCRef getExp_BOrX(Scope &scope);
      virtual ExpCRef getExp_BOrI(Scope &scope);
      virtual ExpCRef getExp_LAnd(Scope &scope);
      virtual ExpCRef getExp_LOrI(Scope &scope);
      virtual ExpCRef getExp_Cond(Scope &scope);
      virtual ExpCRef getExp_Assi(Scope &scope);
      virtual ExpCRef getExp(Scope &scope);

      virtual Core::Array<ExpCRef> getExpList(Scope &scope);

      virtual InitRaw getInitRaw(Scope &scope);

      virtual Labels getLabels(Scope_Local &scope);

      StatementCRef getStatement(Scope_Local &scope);
      virtual StatementCRef getStatement(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatement_asm(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatement_break(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatement_continue(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatement_do(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatement_for(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatement_goto(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatement_if(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatement_return(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatement_switch(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatement_while(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatement_with(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatementCompound(Scope_Local &scope, Labels &&labels);
      virtual StatementCRef getStatementExp(Scope_Local &scope, Labels &&labels);

      virtual ExpCRef getStatementCond(Scope &scope);
      virtual ExpCRef getStatementCondExp(Scope &scope);

      virtual TypeCRef getType(Scope &scope);

      virtual std::pair<TypeSetCRef, Core::Array<SR::Attribute>>
      getTypeList(CC::Scope &scope);

      virtual bool isAddrDecl(Scope &scope);

      virtual bool isAttrSpec(Scope &scope);

      virtual bool isDecl(Scope &scope);

      virtual bool isDeclSpec(Scope &scope);

      virtual bool isDeclarator(Scope &scope);

      virtual bool isExp_Cast(Scope &scope);

      virtual bool isSpecQual(Scope &scope);

      virtual bool isStaticAssert(Scope &scope);

      virtual bool isType(Scope &scope);

      virtual bool isTypeQual(Scope &scope);

      virtual bool isTypeSpec(Scope &scope);
      virtual bool isTypeSpec_atomic(Scope &scope);

      virtual void parseAddrDecl(Scope_Global &scope, SR::Attribute &attr);
      virtual void parseAddrDecl(Scope_Local &scope, SR::Attribute &attr);

      virtual void parseAttr(Scope &scope, SR::Attribute &attr);
      virtual void parseAttrList(Scope &scope, SR::Attribute &attr);
      virtual void parseAttrSpec(Scope &scope, SR::Attribute &attr);
      virtual void parseAttrSpecList(Scope &scope, SR::Attribute &attr);

      virtual void parseDeclSpec(Scope &scope, SR::Attribute &attr);

      virtual void parseDeclarator(Scope &scope, SR::Attribute &attr);
      virtual void parseDeclaratorSuffix(Scope &scope, SR::Attribute &attr);

      virtual void parseSpecQual(Scope &scope, SR::Attribute &attr);

      virtual void parseStaticAssert(Scope &scope);

      virtual void parseTypeQual(Scope &scope, SR::TypeQual &qual);

      virtual void parseTypeSpec(Scope &scope, SR::Attribute &attr, TypeSpec &spec);
      virtual void parseTypeSpec_atomic(Scope &scope, SR::Attribute &attr, TypeSpec &spec);
      virtual void parseTypeSpec_enum(Scope &scope, SR::Attribute &attr, TypeSpec &spec);
      virtual void parseTypeSpec_struct(Scope &scope, SR::Attribute &attr, TypeSpec &spec);
      virtual void parseTypeSpec_union(Scope &scope, SR::Attribute &attr, TypeSpec &spec);

      virtual void skipBalancedToken();

      Core::TokenStream &in;
      CPP::PragmaData   &prag;
      IR::Program       &prog;

   protected:
      Parser(Parser const &ctx, Core::TokenStream &in);

      virtual Parser *cloneRaw(Core::TokenStream &in_) const
         {return new Parser(*this, in_);}
   };

   //
   // TypeSpec
   //
   class TypeSpec
   {
   public:
      //
      // SpecBase
      //
      enum SpecBase
      {
         BaseNone,
         BaseName,

         // C types.
         BaseBool,
         BaseChar,
         BaseDoub,
         BaseFloa,
         BaseInte,
         BaseVoid,

         // Embedded C types.
         BaseAccu,
         BaseFrac,

         // ACS types.
         BaseStri,

         // GDCC types.
         BaseDivT,
         BaseLabl,
         BaseStrE,
         BaseVaLi,
      };

      void finish(SR::Attribute &attr, SR::TypeQual qual,
         Core::Origin pos) const;

      SpecBase specBase = BaseNone;
      unsigned specCplx = 0;
      unsigned specImag = 0;
      unsigned specLong = 0;
      unsigned specSatu = 0;
      unsigned specShrt = 0;
      unsigned specSign = 0;
      unsigned specUnsi = 0;
   };
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   Core::String GetGlobalLabel(std::size_t hash);

   Core::Array<IR::Value> GetStrU08(Core::String str);
   Core::Array<IR::Value> GetStrU16(Core::String str);
   Core::Array<IR::Value> GetStrU32(Core::String str);
   Core::Array<IR::Value> GetString(Core::String str);
   Core::Array<IR::Value> GetString(Core::Token const &tok);

   void ParseFile(char const *inName, IR::Program &prog);
}

#endif//GDCC__CC__Parse_H__

