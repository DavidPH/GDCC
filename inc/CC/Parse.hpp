//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015-2016 David Hill
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

#include <memory>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Attribute;
      class Exp;
      class Statement;
      class Type;
      class TypeQual;
      class TypeSet;
   }

   namespace Core
   {
      template<typename T> class Array;
      template<typename T> class CounterRef;
      class Origin;
      class String;
      class Token;
      class TokenStream;
   }

   namespace CPP
   {
      class PragmaData;
   }

   namespace IR
   {
      class Program;
      class Value;
   }

   namespace CC
   {
      class InitRaw;
      class Scope;
      class Scope_Global;
      class Scope_Local;
      class TypeSpec;

      //
      // Parser
      //
      class Parser
      {
      protected:
         using ExpCRef       = Core::CounterRef<AST::Exp       const>;
         using Labels        = Core::Array<Core::String>;
         using StatementCRef = Core::CounterRef<AST::Statement const>;
         using TypeCRef      = Core::CounterRef<AST::Type      const>;
         using TypeSetCRef   = Core::CounterRef<AST::TypeSet   const>;

      public:
         Parser(Core::TokenStream &in, CPP::PragmaData &prag, IR::Program &prog);

         virtual ~Parser() {}

         std::unique_ptr<Parser> clone(Core::TokenStream &in_) const
            {return static_cast<std::unique_ptr<Parser>>(cloneRaw(in_));}

         virtual StatementCRef getDecl(Scope_Global &scope);
         StatementCRef getDecl(Scope_Local &scope);
         virtual StatementCRef getDecl(Scope_Local &scope, Labels &&labels);

         virtual ExpCRef getExp_CLit(Scope &scope);
         virtual ExpCRef getExp_CLit(Scope &scope, AST::Type const *type);

         virtual ExpCRef getExp_Post_ParenO(Scope &scope, AST::Exp const *exp);

         virtual ExpCRef getExp_Init(Scope &scope, AST::Type const *type);

         virtual ExpCRef getExp_Unar_Identi(Scope &scope);
         virtual ExpCRef getExp_Unar_KeyWrd(Scope &scope);

         virtual ExpCRef getExp_Prim(Scope &scope);
         virtual ExpCRef getExp_Post(Scope &scope);
         virtual ExpCRef getExp_Post(Scope &scope, AST::Exp const *exp);
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
         virtual StatementCRef getStatementCompound(Scope_Local &scope, Labels &&labels);
         virtual StatementCRef getStatementExp(Scope_Local &scope, Labels &&labels);

         virtual ExpCRef getStatementCond(Scope &scope);
         virtual ExpCRef getStatementCondExp(Scope &scope);

         virtual TypeCRef getType(Scope &scope);

         virtual std::pair<TypeSetCRef, Core::Array<AST::Attribute>>
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

         virtual void parseAddrDecl(Scope_Global &scope, AST::Attribute &attr);
         virtual void parseAddrDecl(Scope_Local &scope, AST::Attribute &attr);

         virtual void parseAttr(Scope &scope, AST::Attribute &attr);
         virtual void parseAttrList(Scope &scope, AST::Attribute &attr);
         virtual void parseAttrSpec(Scope &scope, AST::Attribute &attr);
         virtual void parseAttrSpecList(Scope &scope, AST::Attribute &attr);

         virtual void parseDeclSpec(Scope &scope, AST::Attribute &attr);

         virtual void parseDeclarator(Scope &scope, AST::Attribute &attr);
         virtual void parseDeclaratorSuffix(Scope &scope, AST::Attribute &attr);

         virtual void parseSpecQual(Scope &scope, AST::Attribute &attr);

         virtual void parseStaticAssert(Scope &scope);

         virtual void parseTypeQual(Scope &scope, AST::TypeQual &qual);

         virtual void parseTypeSpec(Scope &scope, AST::Attribute &attr, TypeSpec &spec);
         virtual void parseTypeSpec_atomic(Scope &scope, AST::Attribute &attr, TypeSpec &spec);
         virtual void parseTypeSpec_enum(Scope &scope, AST::Attribute &attr, TypeSpec &spec);
         virtual void parseTypeSpec_struct(Scope &scope, AST::Attribute &attr, TypeSpec &spec);
         virtual void parseTypeSpec_union(Scope &scope, AST::Attribute &attr, TypeSpec &spec);

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

         void finish(AST::Attribute &attr, AST::TypeQual qual,
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
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      Core::String GetGlobalLabel(std::size_t hash);

      Core::Array<IR::Value> GetStrU08(Core::String str);
      Core::Array<IR::Value> GetStrU16(Core::String str);
      Core::Array<IR::Value> GetStrU32(Core::String str);
      Core::Array<IR::Value> GetString(Core::String str);
      Core::Array<IR::Value> GetString(Core::Token const &tok);

      void ParseFile(char const *inName, IR::Program &prog);
   }
}

#endif//GDCC__CC__Parse_H__

