//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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
#include "CC/Factory.hpp"
#include "CC/Init.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Warning.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"

#include "SR/Attribute.hpp"
#include "SR/Function.hpp"
#include "SR/Object.hpp"
#include "SR/Statement.hpp"
#include "SR/Storage.hpp"
#include "SR/Type.hpp"
#include "SR/Warning.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // GetLinkageFunc
   //
   static IR::Linkage GetLinkageFunc(SR::Attribute const &attr)
   {
      if(attr.storeInt)
         return GetLinkageInt(attr.linka);
      else
         return GetLinkageExt(attr.linka);
   }

   //
   // GetLinkageObj (global)
   //
   static IR::Linkage GetLinkageObj(Scope_Global &, SR::Attribute const &attr)
   {
      if(attr.storeInt)
         return GetLinkageInt(attr.linka);
      else
         return GetLinkageExt(attr.linka);
   }

   //
   // GetLinkageObj (local)
   //
   static IR::Linkage GetLinkageObj(Scope_Local &, SR::Attribute const &attr)
   {
      if(attr.storeExt)
         return GetLinkageExt(attr.linka);
      else
         return IR::Linkage::None;
   }

   //
   // GetDeclFunc (global)
   //
   static SR::Function::Ref GetDeclFunc(Scope_Global &scope, SR::Attribute &attr)
   {
      // Determine linkage.
      attr.linka = GetLinkageFunc(attr);

      auto fn = scope.getFunction(attr);

      // Set address, if one provided.
      if(attr.addrI)
         fn->valueInt = attr.addrI;
      else if(attr.addrS)
         fn->valueStr = attr.addrS;

      if(attr.addrL)
         fn->valueLit = attr.addrL;

      return fn;
   }

   //
   // GetDeclFunc (local)
   //
   static SR::Function::Ref GetDeclFunc(Scope_Local &scope, SR::Attribute &attr)
   {
      if(!attr.storeExt && !attr.storeInt)
         Core::Error(attr.namePos, "block scope function not extern or static");

      // Determine linkage.
      attr.linka = GetLinkageFunc(attr);

      auto fn = scope.global.getFunction(attr);

      // Set address, if one provided.
      if(attr.addrI)
         fn->valueInt = attr.addrI;
      else if(attr.addrS)
         fn->valueStr = attr.addrS;

      return fn;
   }

   //
   // GetDeclObj (global)
   //
   static SR::Object::Ref GetDeclObj(Parser &ctx, Scope_Global &scope,
      SR::Attribute &attr, bool init)
   {
      if(attr.storeAuto)
         Core::Error(attr.namePos, "file scope auto");

      if(attr.storeReg)
         Core::Error(attr.namePos, "file scope register");

      // Determine linkage.
      attr.linka = GetLinkageObj(scope, attr);

      // Fetch/generate object.
      auto obj = scope.getObject(attr);

      // If not marked extern, then this is a definition. If init, wait
      // until later to mark as definition because the type might need to be
      // completed by the initializer.
      if(!attr.storeExt && !init)
      {
         // First, make sure it has a complete type.
         if(!obj->type->isTypeComplete())
            Core::Error(attr.namePos, "object with incomplete type");

         obj->defin = true;

         // Give default initializer.
         if(!attr.objNoInit)
         {
            obj->init = Exp_Init::Create(
               Init::Create(obj->type, 0, attr.namePos, ctx.fact), true);
         }
      }

      // Set address, if one provided.
      if(attr.addrI)
         obj->value = attr.addrI;

      return obj;
   }

   //
   // GetDeclObj (local)
   //
   static SR::Object::Ref GetDeclObj(Parser &ctx, Scope_Local &scope,
      SR::Attribute &attr, bool init)
   {
      // Determine linkage.
      attr.linka = GetLinkageObj(scope, attr);

      // Automatic storage objects can only have certain address spaces.
      if(!attr.storeExt && !attr.storeInt)
      {
         switch(attr.type->getQualAddr().base)
         {
         case IR::AddrBase::Aut:
         case IR::AddrBase::Gen:
         case IR::AddrBase::LocArr:
         case IR::AddrBase::LocReg:
            break;

         default:
            Core::Error(attr.namePos, "invalid address space for automatic storage object");
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
               Core::Error(attr.namePos, "object with incomplete type");

            obj->defin = true;

            // Give default initializer, if a static storage object.
            if(obj->store == SR::Storage::Static && !attr.objNoInit)
            {
               obj->init = Exp_Init::Create(
                  Init::Create(obj->type, 0, attr.namePos, ctx.fact), true);
            }
         }
      }
      // Local scope objects with linkage must not have an initializer.
      else if(init)
         Core::Error(attr.namePos, "linkage local with initializer");

      // Set address, if one provided.
      if(attr.addrI)
         obj->value = attr.addrI;

      return obj;
   }

   //
   // SetDeclObjInit (global)
   //
   static void SetDeclObjInit(Parser &, Scope_Global &,
      SR::Attribute &, std::vector<SR::Statement::CRef> &,
      SR::Object *obj)
   {
      obj->defin = true;

      // File-scope objects get initialization code generated later.
   }

   //
   // SetDeclObjInit (local)
   //
   static void SetDeclObjInit(Parser &ctx, Scope_Local &,
      SR::Attribute &attr, std::vector<SR::Statement::CRef> &inits,
      SR::Object *obj)
   {
      obj->defin = true;

      // Block-scope statics must have constant initializers, so they can be
      // handled like file-scope statics.
      if(obj->store == SR::Storage::Static)
         return;

      auto initExp = ctx.fact.expCreate_Obj(ctx.prog, obj, attr.namePos);
      initExp = Exp_Assign::Create(initExp, obj->init, obj->init->pos);

      inits.emplace_back(ctx.fact.stCreate_Exp({}, initExp));
   }

   //
   // ParseDecl_Function (global)
   //
   static void ParseDecl_Function(Parser &ctx, Scope_Global &scope,
      SR::Attribute &attr, SR::Function *fn)
   {
      if(!fn->retrn->isTypeComplete() && !fn->retrn->isTypeVoid())
         Core::Error(attr.namePos, "incomplete return");

      for(auto const &param : attr.param)
         if(!param.type || !param.type->isTypeComplete())
            Core::Error(param.namePos, "incomplete argument");

      auto &fnScope = scope.createScope(attr, fn);

      auto stmntPre  = ctx.fact.stCreate_FuncPre(ctx.in.peek().pos, fnScope);
      auto stmntBody = ctx.getStCompound(fnScope, {}, {});
      auto stmntPro  = ctx.fact.stCreate_FuncPro(ctx.in.reget().pos, fnScope);

      // Create statements for the function.
      Core::Array<SR::Statement::CRef> stmnts =
         {Core::Pack, stmntPre, stmntBody, stmntPro};

      fn->stmnt = ctx.fact.stCreate_Multi({}, attr.namePos, std::move(stmnts));
      fn->defin = true;

      fn->setAllocAut(attr.allocAut);

      if(!fn->retrn->isTypeVoid())
      {
         if(fn->stmnt->isNoReturn())
            SR::WarnReturnType(attr.namePos, "no return in non-void function");

         else if(!fn->stmnt->isReturn())
            SR::WarnReturnTypeExt(attr.namePos,
               "non-void function possibly does not return");
      }
   }

   //
   // ParseDecl_Function (local)
   //
   static void ParseDecl_Function(Parser &, Scope_Local &,
      SR::Attribute &attr, SR::Function *)
   {
      Core::Error(attr.namePos, "local function definition");
   }

   //
   // ParseDecl_typedef
   //
   static void ParseDecl_typedef(Scope &scope, SR::Attribute &attr)
   {
      // Check compatibility with existing symbol, if any.
      if(auto lookup = scope.find(attr.name))
      {
         if(lookup.res != Lookup::Type)
            Core::Error(attr.namePos, "name redefined as different kind of symbol");

         if(lookup.resType != attr.type)
            Core::Error(attr.namePos, "typedef redefined as different type");
      }

      scope.add(attr.name, attr.type);
   }

   //
   // ParseDeclBase_Function
   //
   template<typename T>
   static bool ParseDeclBase_Function(Parser &ctx, T &scope, SR::Attribute &attr)
   {
      bool defin = ctx.in.peek(Core::TOK_BraceO);

      // Check compatibility with existing symbol, if any.
      if(auto lookup = scope.find(attr.name))
      {
         if(lookup.res != Lookup::Func)
            Core::Error(attr.namePos, "name redefined as different kind of symbol");

         SR::Function::Ref fn = lookup.resFunc;

         if(fn->defin && defin)
            Core::Error(attr.namePos, "function redefined");

         if(fn->retrn != attr.type->getBaseType())
            Core::Error(attr.namePos, "function redeclared with different return type");

         // TODO: Compatible parameter types check.

         if(fn->ctype != attr.callt)
            WarnDeclCompat(attr.namePos, "function redeclared with different call type");

         if(fn->linka != GetLinkageFunc(attr))
            WarnDeclCompat(attr.namePos, "function redeclared with different linkage");
      }

      auto fn = GetDeclFunc(scope, attr);

      scope.add(attr.name, fn);

      if(defin)
         return ParseDecl_Function(ctx, scope, attr, fn), true;

      return false;
   }

   //
   // ParseDeclBase_Object
   //
   template<typename T>
   static void ParseDeclBase_Object(Parser &ctx, T &scope,
      SR::Attribute &attr, std::vector<SR::Statement::CRef> &inits)
   {
      // Check compatibility with existing symbol, if any.
      if(auto lookup = scope.find(attr.name))
      {
         if(lookup.res != Lookup::Obj)
            Core::Error(attr.namePos, "name redefined as different kind of symbol");

         SR::Object::Ref obj = lookup.resObj;

         if(obj->type != attr.type &&
            (!obj->type->isTypeArray() || !attr.type->isTypeArray() ||
               obj->type->getBaseType() != attr.type->getBaseType()))
         {
            Core::Error(attr.namePos, "object redeclared with different type");
         }

         if(obj->linka != GetLinkageObj(scope, attr))
            WarnDeclCompat(attr.namePos, "object redeclared with different linkage");
      }

      // Insert special declaration statement.
      if(inits.empty())
         inits.emplace_back(ctx.fact.stCreate_Decl(attr.namePos, scope));

      // = initializer
      if(ctx.in.drop(Core::TOK_Equal))
      {
         auto obj = GetDeclObj(ctx, scope, attr, true);

         scope.add(attr.name, obj);

         obj->init = ctx.getExp_Init(scope, obj->type);
         obj->type = obj->init->getType();

         if(obj->store == SR::Storage::Static && !obj->init->isIRExp())
            Core::Error(obj->init->pos, "non-constant initializer for static storage object");

         SetDeclObjInit(ctx, scope, attr, inits, obj);
      }
      else
         scope.add(attr.name, GetDeclObj(ctx,scope, attr, false));
   }

   //
   // GetDeclBase
   //
   template<typename T>
   static SR::Statement::CRef GetDeclBase(Parser &ctx, T &scope,
      SR::Attribute &&attrBase, Core::Array<Core::String> &&labels)
   {
      auto pos = ctx.in.peek().pos;

      // declaration:
      //    attribute-specifier-list(opt) declaration-specifiers
      //       init-declarator-list(opt) ;
      //    static_assert-declaration
      //    address-space-declaration

      // address-space-declaration
      if(ctx.isAddrDecl(scope))
         return ctx.parseAddrDecl(scope, attrBase), ctx.fact.stCreate_Empty({}, pos);

      // static_assert-declaration
      if(ctx.isStaticAssert(scope))
         return ctx.parseStaticAssert(scope), ctx.fact.stCreate_Empty({}, pos);

      std::vector<SR::Statement::CRef> inits;

      // declaration-specifiers
      ctx.parseDeclSpec(scope, attrBase);

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
         ctx.parseDeclarator(scope, attr);

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
            Core::Error(attr.namePos, "expected object type");

         ParseDeclBase_Object(ctx, scope, attr, inits);
      }
      while(ctx.in.drop(Core::TOK_Comma));

      ctx.expect(Core::TOK_Semico);

      decl_end:
      switch(inits.size())
      {
      case  0: return ctx.fact.stCreate_Empty(std::move(labels), pos);
      case  1: if(labels.empty()) return inits[0];
      default: return ctx.fact.stCreate_Multi(std::move(labels), pos,
         {inits.begin(), inits.end()});
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::getDecl
   //
   SR::Statement::CRef Parser::getDecl(Scope_Global &scope)
   {
      SR::Attribute attr;
      attr.linka = defLinkage();
      if(isAttrSpec(scope))
         parseAttrSpecList(scope, attr);

      if(in.peek(Core::TOK_Semico))
      {
         Core::Origin pos = in.get().pos;
         WarnFileSemico(pos, "extraneous file-scope semicolon");
         return fact.stCreate_Empty({}, in.reget().pos);
      }

      return GetDeclBase(*this, scope, std::move(attr), {});
   }

   //
   // Parser::getDecl
   //
   SR::Statement::CRef Parser::getDecl(Scope_Local &scope, SR::Attribute &&attr, Labels &&labels)
   {
      return GetDeclBase(*this, scope, std::move(attr), std::move(labels));
   }

   //
   // Parser::isDecl
   //
   bool Parser::isDecl(Scope &scope)
   {
      return
         isAddrDecl(scope) ||
         isStaticAssert(scope) ||
         isDeclSpec(scope);
   }
}

// EOF

