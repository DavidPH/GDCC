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

#include "CC/Scope.hpp"
#include "CC/Statement.hpp"

#include "AST/Attribute.hpp"
#include "AST/Function.hpp"
#include "AST/Object.hpp"
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
static GDCC::AST::Function::Ref GetDeclFunc(GDCC::CC::GlobalScope *ctx,
   GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // Determine linkage.
   if(attr.storeInt)
      attr.linka = GetLinkageInt(attr.linka);
   else
      attr.linka = GetLinkageExt(attr.linka);

   return ctx->getFunction(attr);
}

//
// GetDeclFunc (local)
//
static GDCC::AST::Function::Ref GetDeclFunc(GDCC::CC::LocalScope *ctx,
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

   return ctx->global->getFunction(attr);
}

//
// GetDeclObj (global)
//
static GDCC::AST::Object::Ref GetDeclObj(GDCC::CC::GlobalScope *ctx,
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
   auto obj = ctx->getObject(attr);

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
static GDCC::AST::Object::Ref GetDeclObj(GDCC::CC::LocalScope *ctx,
   GDCC::AST::Attribute &attr, bool init)
{
   using namespace GDCC;

   // Determine linkage.
   if(attr.storeExt)
      attr.linka = GetLinkageExt(attr.linka);
   else
      attr.linka = IR::Linkage::None;

   // Fetch/generate object.
   auto obj = ctx->getObject(attr);

   // With extern or static, static storage duration.
   if(attr.storeExt || attr.storeInt)
      obj->store = AST::Storage::Static;
   // Otherwise, automatic storage duration.
   else
      obj->store = AST::Storage::Auto;

   // If declaration is explicitly auto, always make it addressable.
   if(attr.storeAuto)
      obj->point = true;

   // If register is used, do not allow taking address.
   if(attr.storeReg)
      obj->noPtr = true;

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
// ParseDecl_Function (global)
//
static void ParseDecl_Function(GDCC::CC::ParserData &in,
   GDCC::CC::GlobalScope *ctx, GDCC::AST::Attribute &attr,
   GDCC::AST::Function *fn)
{
   using namespace GDCC;

   auto fnCtx = ctx->createScope(attr, fn);

   // Create statements for the function.
   Core::Array<AST::Statement::CRef> stmnts =
   {
      Core::Pack,

      CC::StatementCreate_FuncPre(attr.namePos, fnCtx),
      CC::GetStatement(in, fnCtx),
      CC::StatementCreate_Return(attr.namePos, fn),
   };

   fn->stmnt = AST::StatementCreate_Multi(attr.namePos, std::move(stmnts));
   fn->defin = true;
}

//
// ParseDecl_Function (local)
//
static void ParseDecl_Function(GDCC::CC::ParserData &, GDCC::CC::LocalScope *,
   GDCC::AST::Attribute &attr, GDCC::AST::Function *)
{
   throw GDCC::Core::ExceptStr(attr.namePos, "local function definition");
}

//
// ParseDecl_typedef
//
static void ParseDecl_typedef(GDCC::CC::Scope *ctx, GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // Check compatibility with existing symbol, if any.
   if(auto lookup = ctx->find(attr.name))
   {
      if(lookup.res != CC::Lookup::Type)
         throw Core::ExceptStr(attr.namePos,
            "name redefined as different kind of symbol");

      if(lookup.resType != attr.type)
         throw Core::ExceptStr(attr.namePos,
            "typedef redefined as different type");
   }

   ctx->add(attr.name, attr.type);
}

//
// ParseDeclBase_Function
//
template<typename T>
static void ParseDeclBase_Function(GDCC::CC::ParserData &in, T *ctx,
   GDCC::AST::Attribute &attr)
{
   using namespace GDCC;

   // Check compatibility with existing symbol, if any.
   if(auto lookup = ctx->find(attr.name))
   {
      if(lookup.res != CC::Lookup::Func)
         throw Core::ExceptStr(attr.namePos,
            "name redefined as different kind of symbol");

      if(lookup.resFunc->retrn != attr.type->getBaseType())
         throw Core::ExceptStr(attr.namePos,
            "function redeclared with different return type");

      // TODO: Compatible parameter types check.
   }

   auto fn = GetDeclFunc(ctx, attr);

   ctx->add(attr.name, fn);

   if(in.in.peek().tok == Core::TOK_BraceO)
      ParseDecl_Function(in, ctx, attr, fn);
}

//
// ParseDeclBase_Object
//
template<typename T>
static void ParseDeclBase_Object(GDCC::CC::ParserData &in, T *ctx,
   GDCC::AST::Attribute &attr, std::vector<GDCC::AST::Statement::CRef> &inits)
{
   using namespace GDCC;

   // Check compatibility with existing symbol, if any.
   if(auto lookup = ctx->find(attr.name))
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
      inits.emplace_back(CC::StatementCreate_Decl(attr.namePos, ctx));

   // = initializer
   if(in.in.drop(Core::TOK_Equal))
   {
      auto obj = GetDeclObj(ctx, attr, true);

      throw Core::ExceptStr(in.in.reget().pos, "initializer stub");
   }
   else
      ctx->add(attr.name, GetDeclObj(ctx, attr, false));
}

//
// GetDeclBase
//
template<typename T>
static GDCC::AST::Statement::CRef GetDeclBase(GDCC::CC::ParserData &in, T *ctx)
{
   using namespace GDCC;

   auto pos = in.in.peek().pos;

   // declaration:
   //    declaration-specifiers init-declarator-list(opt) ;
   //    static_assert-declaration

   // static_assert-declaration
   if(CC::IsStaticAssert(in, ctx))
      return CC::ParseStaticAssert(in, ctx), AST::StatementCreate_Empty(pos);

   std::vector<AST::Statement::CRef> inits;

   // declaration-specifiers
   AST::Attribute attrBase;
   CC::ParseDeclSpec(in, ctx, attrBase);

   // init-declarator-list:
   //    init-declarator
   //    init-declarator-list , init-declarator
   if(in.in.peek().tok != Core::TOK_Semico) do
   {
      // init-declarator:
      //    declarator
      //    declarator = initializer

      // declarator
      auto attr = attrBase;
      CC::ParseDeclarator(in, ctx, attr);

      // Special handling for typedef.
      if(attr.isTypedef)
         {ParseDecl_typedef(ctx, attr); continue;}

      // Special handling for function types.
      if(attr.type->isCTypeFunction())
         {ParseDeclBase_Function(in, ctx, attr); continue;}

      // Must otherwise be an object type.
      if(!attr.type->isCTypeObject())
         throw Core::ExceptStr(attr.namePos, "expected object type");

      ParseDeclBase_Object(in, ctx, attr, inits);
   }
   while(in.in.drop(Core::TOK_Comma));

   if(!in.in.drop(Core::TOK_Semico))
      throw Core::ExceptStr(in.in.peek().pos, "expected ';'");

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
      AST::Statement::CRef GetDecl(ParserData &in, GlobalScope *ctx)
      {
         return GetDeclBase(in, ctx);
      }

      //
      // GetDecl
      //
      AST::Statement::CRef GetDecl(ParserData &in, LocalScope *ctx)
      {
         return GetDeclBase(in, ctx);
      }

      //
      // IsDecl
      //
      bool IsDecl(ParserData &in, Scope *ctx)
      {
         return IsStaticAssert(in, ctx) || IsDeclSpec(in, ctx);
      }
   }
}

// EOF

