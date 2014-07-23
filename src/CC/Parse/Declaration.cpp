//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C declaration parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp/Assign.hpp"
#include "CC/Exp/Init.hpp"
#include "CC/Init.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Statement.hpp"

#include "AST/Attribute.hpp"
#include "AST/Function.hpp"
#include "AST/Object.hpp"
#include "AST/Statement.hpp"
#include "AST/Storage.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Linkage.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// GetLinkageExt
//
static GDCC::IR::Linkage GetLinkageExt(GDCC::IR::Linkage base)
{
   // If an explicit linkage was specified with attributes, keep it.
   if(base == GDCC::IR::Linkage::None)
      return GDCC::IR::Linkage::ExtC;

   return base;
}

//
// GetLinkageInt
//
static GDCC::IR::Linkage GetLinkageInt(GDCC::IR::Linkage base)
{
   // Convert language linkage to internal variant.
   switch(base)
   {
   case GDCC::IR::Linkage::None: return GDCC::IR::Linkage::IntC;

   case GDCC::IR::Linkage::ExtACS: return GDCC::IR::Linkage::IntC;
   case GDCC::IR::Linkage::ExtC:   return GDCC::IR::Linkage::IntC;
   case GDCC::IR::Linkage::ExtCXX: return GDCC::IR::Linkage::IntCXX;
   case GDCC::IR::Linkage::ExtDS:  return GDCC::IR::Linkage::IntCXX;

   default: return base;
   }
}

//
// GetDeclFunc (global)
//
static GDCC::AST::Function::Ref GetDeclFunc(GDCC::CC::Scope_Global &scope,
   GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // Determine linkage.
   if(attr.storeInt)
      attr.linka = GetLinkageInt(attr.linka);
   else
      attr.linka = GetLinkageExt(attr.linka);

   return scope.getFunction(attr);
}

//
// GetDeclFunc (local)
//
static GDCC::AST::Function::Ref GetDeclFunc(GDCC::CC::Scope_Local &scope,
   GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // Determine linkage.
   if(attr.storeExt)
      attr.linka = GetLinkageExt(attr.linka);
   else if(attr.storeInt)
      attr.linka = GetLinkageInt(attr.linka);
   else
      throw Core::ExceptStr(attr.namePos,
         "local scope function not extern or static");

   return scope.global.getFunction(attr);
}

//
// GetDeclObj (global)
//
static GDCC::AST::Object::Ref GetDeclObj(GDCC::CC::Scope_Global &scope,
   GDCC::AST::Attribute &attr, bool init)
{
   using namespace GDCC;

   if(attr.storeAuto)
      throw Core::ExceptStr(attr.namePos, "file scope auto");

   if(attr.storeReg)
      throw Core::ExceptStr(attr.namePos, "file scope register");

   // Determine linkage.
   if(attr.storeInt)
      attr.linka = GetLinkageInt(attr.linka);
   else
      attr.linka = GetLinkageExt(attr.linka);

   // Fetch/generate object.
   auto obj = scope.getObject(attr);

   // File scope objects are always static storage duration.
   obj->store = AST::Storage::Static;

   // If not marked extern, then this is a definition. (Do not mark if init.)
   if(!attr.storeExt && !init)
   {
      // First, make sure it has a complete type.
      if(!obj->type->isTypeComplete())
         throw Core::ExceptStr(attr.namePos, "object with incomplete type");

      obj->defin = true;
   }

   return obj;
}

//
// GetDeclObj (local)
//
static GDCC::AST::Object::Ref GetDeclObj(GDCC::CC::Scope_Local &scope,
   GDCC::AST::Attribute &attr, bool init)
{
   using namespace GDCC;

   // Determine linkage.
   if(attr.storeExt)
      attr.linka = GetLinkageExt(attr.linka);
   else
      attr.linka = IR::Linkage::None;

   // Automatic storage objects can only have certain address sapce qualifiers.
   if(!attr.storeExt && !attr.storeInt)
   {
      switch(attr.type->getQualAddr().base)
      {
      case IR::AddrBase::Gen:
      case IR::AddrBase::Loc:
      case IR::AddrBase::LocReg:
         break;

      default:
         throw Core::ExceptStr(attr.namePos,
            "invalid address space for automatic storage object");
      }
   }

   // Fetch/generate object.
   auto obj = scope.getObject(attr);

   // No-linkage declarations must be definitions.
   if(attr.linka == IR::Linkage::None)
   {
      // But if it has an initializer, do not set defined yet.
      if(!init)
      {
         // First, make sure it has a complete type.
         if(!obj->type->isTypeComplete())
            throw Core::ExceptStr(attr.namePos, "object with incomplete type");

         obj->defin = true;
      }
   }
   // Local scope objects with linkage must not have an initializer.
   else if(init)
      throw Core::ExceptStr(attr.namePos, "linkage local with initializer");

   return obj;
}

//
// SetDeclObjInit (global)
//
static void SetDeclObjInit(
   GDCC::CC::ParserCtx const               &,
   GDCC::CC::Scope_Global                  &,
   GDCC::AST::Attribute                    &,
   std::vector<GDCC::AST::Statement::CRef> &,
   GDCC::AST::Object                       *)
{
   // File-scope objects get initialized code generated later.
}

//
// SetDeclObjInit (local)
//
static void SetDeclObjInit(
   GDCC::CC::ParserCtx const               &ctx,
   GDCC::CC::Scope_Local                   &,
   GDCC::AST::Attribute                    &attr,
   std::vector<GDCC::AST::Statement::CRef> &inits,
   GDCC::AST::Object                       *obj)
{
   using namespace GDCC;

   // Block-scope statics must have constant initializers, so they can be
   // handled like file-scope statics.
   if(obj->store == AST::Storage::Static)
      return;

   auto initExp = CC::ExpCreate_Obj(ctx.prog, obj, attr.namePos);
   initExp = CC::Exp_Assign::Create(initExp, obj->init, obj->init->pos);

   inits.emplace_back(AST::StatementCreate_Exp(initExp));
}

//
// ParseDecl_Function (global)
//
static void ParseDecl_Function(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope_Global &scope, GDCC::AST::Attribute &attr,
   GDCC::AST::Function *fn)
{
   using namespace GDCC;

   auto &fnScope = scope.createScope(attr, fn);

   // Create statements for the function.
   Core::Array<AST::Statement::CRef> stmnts =
   {
      Core::Pack,

      CC::StatementCreate_FuncPre(ctx.in.peek().pos, fnScope),
      CC::GetStatement(ctx, fnScope),
      CC::StatementCreate_FuncPro(ctx.in.reget().pos, fnScope),
   };

   fn->stmnt = AST::StatementCreate_Multi(attr.namePos, std::move(stmnts));
   fn->defin = true;
}

//
// ParseDecl_Function (local)
//
static void ParseDecl_Function(GDCC::CC::ParserCtx const &,
   GDCC::CC::Scope_Local &, GDCC::AST::Attribute &attr, GDCC::AST::Function *)
{
   throw GDCC::Core::ExceptStr(attr.namePos, "local function definition");
}

//
// ParseDecl_typedef
//
static void ParseDecl_typedef(GDCC::CC::Scope &scope, GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // Check compatibility with existing symbol, if any.
   if(auto lookup = scope.find(attr.name))
   {
      if(lookup.res != CC::Lookup::Type)
         throw Core::ExceptStr(attr.namePos,
            "name redefined as different kind of symbol");

      if(lookup.resType != attr.type)
         throw Core::ExceptStr(attr.namePos,
            "typedef redefined as different type");
   }

   scope.add(attr.name, attr.type);
}

//
// ParseDeclBase_Function
//
template<typename T>
static bool ParseDeclBase_Function(GDCC::CC::ParserCtx const &ctx, T &scope,
   GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // Check compatibility with existing symbol, if any.
   if(auto lookup = scope.find(attr.name))
   {
      if(lookup.res != CC::Lookup::Func)
         throw Core::ExceptStr(attr.namePos,
            "name redefined as different kind of symbol");

      if(lookup.resFunc->retrn != attr.type->getBaseType())
         throw Core::ExceptStr(attr.namePos,
            "function redeclared with different return type");

      // TODO: Compatible parameter types check.
   }

   auto fn = GetDeclFunc(scope, attr);

   scope.add(attr.name, fn);

   if(ctx.in.peek().tok == Core::TOK_BraceO)
      return ParseDecl_Function(ctx, scope, attr, fn), true;

   return false;
}

//
// ParseDeclBase_Object
//
template<typename T>
static void ParseDeclBase_Object(GDCC::CC::ParserCtx const &ctx, T &scope,
   GDCC::AST::Attribute &attr, std::vector<GDCC::AST::Statement::CRef> &inits)
{
   using namespace GDCC;

   // Check compatibility with existing symbol, if any.
   if(auto lookup = scope.find(attr.name))
   {
      if(lookup.res != CC::Lookup::Obj)
         throw Core::ExceptStr(attr.namePos,
            "name redefined as different kind of symbol");

      if(lookup.resObj->type != attr.type)
         throw Core::ExceptStr(attr.namePos,
            "object redeclared with different type");
   }

   // Insert special declaration statement.
   if(inits.empty())
      inits.emplace_back(CC::StatementCreate_Decl(attr.namePos, scope));

   // = initializer
   if(ctx.in.drop(Core::TOK_Equal))
   {
      auto obj = GetDeclObj(scope, attr, true);

      scope.add(attr.name, obj);

      auto init = CC::Init::Get(ctx, scope, obj->type);

      // If object is an array with indeterminate length, set the length.
      if(obj->type->isTypeArray() && !obj->type->isTypeComplete())
      {
         obj->type = obj->type->getBaseType()
            ->getTypeArray(init->width())
            ->getTypeQual(obj->type->getQual());
      }

      obj->init = CC::Exp_Init::Create(std::move(init), false);
      SetDeclObjInit(ctx, scope, attr, inits, obj);
   }
   else
      scope.add(attr.name, GetDeclObj(scope, attr, false));
}

//
// GetDeclBase
//
template<typename T>
static GDCC::AST::Statement::CRef GetDeclBase(GDCC::CC::ParserCtx const &ctx,
   T &scope)
{
   using namespace GDCC;

   auto pos = ctx.in.peek().pos;

   // declaration:
   //    declaration-specifiers init-declarator-list(opt) ;
   //    static_assert-declaration

   // static_assert-declaration
   if(CC::IsStaticAssert(ctx, scope))
      return CC::ParseStaticAssert(ctx, scope), AST::StatementCreate_Empty(pos);

   std::vector<AST::Statement::CRef> inits;

   // declaration-specifiers
   AST::Attribute attrBase;
   CC::ParseDeclSpec(ctx, scope, attrBase);

   // init-declarator-list:
   //    init-declarator
   //    init-declarator-list , init-declarator
   if(ctx.in.peek().tok != Core::TOK_Semico) do
   {
      // init-declarator:
      //    declarator
      //    declarator = initializer

      // declarator
      auto attr = attrBase;
      CC::ParseDeclarator(ctx, scope, attr);

      // Special handling for typedef.
      if(attr.isTypedef)
         {ParseDecl_typedef(scope, attr); continue;}

      // Special handling for function types.
      if(attr.type->isCTypeFunction())
      {
         if(ParseDeclBase_Function(ctx, scope, attr))
            goto decl_end;

         continue;
      }

      // Must otherwise be an object type.
      if(!attr.type->isCTypeObject())
         throw Core::ExceptStr(attr.namePos, "expected object type");

      ParseDeclBase_Object(ctx, scope, attr, inits);
   }
   while(ctx.in.drop(Core::TOK_Comma));

   if(!ctx.in.drop(Core::TOK_Semico))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");

   decl_end:
   switch(inits.size())
   {
   case  0: return AST::StatementCreate_Empty(pos);
   case  1: return inits[0];
   default: return AST::StatementCreate_Multi(pos,
      Core::Array<AST::Statement::CRef>(inits.begin(), inits.end()));
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
      // GetDecl
      //
      AST::Statement::CRef GetDecl(ParserCtx const &ctx, Scope_Global &scope)
      {
         return GetDeclBase(ctx, scope);
      }

      //
      // GetDecl
      //
      AST::Statement::CRef GetDecl(ParserCtx const &ctx, Scope_Local &scope)
      {
         return GetDeclBase(ctx, scope);
      }

      //
      // IsDecl
      //
      bool IsDecl(ParserCtx const &ctx, Scope &scope)
      {
         return IsStaticAssert(ctx, scope) || IsDeclSpec(ctx, scope);
      }
   }
}

// EOF

