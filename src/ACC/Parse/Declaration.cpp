//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Scope.hpp"

#include "AST/Attribute.hpp"
#include "AST/Function.hpp"
#include "AST/Object.hpp"
#include "AST/Space.hpp"
#include "AST/Statement.hpp"
#include "AST/Storage.hpp"
#include "AST/Type.hpp"
#include "AST/Warning.hpp"

#include "CC/Exp.hpp"
#include "CC/Exp/Assign.hpp"
#include "CC/Exp/Init.hpp"
#include "CC/Init.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Scope/Local.hpp"
#include "CC/Statement.hpp"
#include "CC/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/CallType.hpp"
#include "IR/Linkage.hpp"
#include "IR/ScriptType.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      static AST::Function::Ref GetDeclFunction(CC::Scope_Global &scope,
         AST::Attribute &attr);

      template<typename T>
      static void ParseDeclObject(ParserCtx const &ctx, T &scope,
         AST::Attribute &attr, std::vector<AST::Statement::CRef> &inits);

      template<typename T>
      static void ParseDeclFunction(ParserCtx const &ctx, T &scope,
         AST::Attribute &attr);

      //
      // GetDecl_Function
      //
      static AST::Statement::CRef GetDecl_Function(ParserCtx const &ctx,
         CC::Scope_Global &scope)
      {
         auto pos = ctx.in.get().pos;

         AST::Attribute attr;
         attr.linka = IR::Linkage::ExtACS;

         // declaration-specifiers
         ParseDeclSpec(ctx, scope, attr);

         // declarator
         ParseDeclarator(ctx, scope, attr);

         ParseDeclFunction(ctx, scope, attr);

         return AST::StatementCreate_Empty(pos);
      }

      //
      // GetDecl_Object
      //
      template<typename T>
      static AST::Statement::CRef GetDecl_Object(ParserCtx const &ctx, T &scope)
      {
         auto pos = ctx.in.peek().pos;

         // declaration:
         //    declaration-specifiers init-declarator-list ;

         if(!IsDeclSpec(ctx, scope))
            throw Core::ParseExceptExpect(ctx.in.peek(), "object-declaration", false);

         AST::Attribute attrBase;
         attrBase.linka = IR::Linkage::ExtACS;

         // declaration-specifiers
         ParseDeclSpec(ctx, scope, attrBase);

         std::vector<AST::Statement::CRef> inits;

         // init-declarator-list:
         //    init-declarator
         //    init-declarator-list , init-declarator
         do
         {
            // init-declarator:
            //    declarator
            //    declarator = initializer

            // declarator
            auto attr = attrBase;
            ParseDeclarator(ctx, scope, attr);

            // Must otherwise be an object type.
            if(!attr.type->isCTypeObject())
               throw Core::ExceptStr(attr.namePos, "expected object type");

            ParseDeclObject(ctx, scope, attr, inits);
         }
         while(ctx.in.drop(Core::TOK_Comma));

         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         switch(inits.size())
         {
         case  0: return AST::StatementCreate_Empty(pos);
         case  1: return inits[0];
         default: return AST::StatementCreate_Multi(pos,
            Core::Array<AST::Statement::CRef>(inits.begin(), inits.end()));
         }
      }

      //
      // GetDecl_Script
      //
      static AST::Statement::CRef GetDecl_Script(ParserCtx const &ctx,
         CC::Scope_Global &scope)
      {
         auto pos = ctx.in.get().pos;

         AST::Attribute attr;

         attr.type = AST::Type::Void;

         // script-address:
         //    integer
         //    string-literal
         attr.namePos = ctx.in.peek().pos;
         if(ctx.in.peek().isTokString())
         {
            attr.addrS = ctx.in.get().str;
            attr.callt = IR::CallType::ScriptS;
         }
         else if(ctx.in.peek(Core::TOK_NumInt))
         {
            attr.addrI = GetExp_Prim(ctx, scope)->getIRExp();
            attr.callt = IR::CallType::ScriptI;
         }
         else
         {
            throw Core::ParseExceptExpect(ctx.in.peek(), "script-address", false);
         }

         // script-parameters(opt)
         if(ctx.in.drop(Core::TOK_ParenO))
         {
            AST::TypeSet::CPtr types;
            std::tie(types, attr.param) = GetTypeList(ctx, scope);
            attr.type = attr.type->getTypeFunction(types, attr.callt);

            if(!ctx.in.drop(Core::TOK_ParenC))
               throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);
         }
         else
         {
            attr.type = attr.type->getTypeFunction(AST::TypeSet::Get(false), attr.callt);
         }

         // script-type(opt)
         if(ctx.in.peek(Core::TOK_KeyWrd)) switch(ctx.in.get().str)
         {
         case Core::STR_death:      attr.stype = IR::ScriptType::Death;      break;
         case Core::STR_disconnect: attr.stype = IR::ScriptType::Disconnect; break;
         case Core::STR_enter:      attr.stype = IR::ScriptType::Enter;      break;
         case Core::STR_lightning:  attr.stype = IR::ScriptType::Lightning;  break;
         case Core::STR_open:       attr.stype = IR::ScriptType::Open;       break;
         case Core::STR_respawn:    attr.stype = IR::ScriptType::Respawn;    break;
         case Core::STR_return:     attr.stype = IR::ScriptType::Return;     break;
         case Core::STR_unloading:  attr.stype = IR::ScriptType::Unloading;  break;

         default: ctx.in.unget(); break;
         }

         // script-flag-list(opt)
         while(ctx.in.peek(Core::TOK_KeyWrd)) switch(ctx.in.get().str)
         {
         case Core::STR_clientside: attr.sflagClS = true; break;
         case Core::STR_net:        attr.sflagNet = true; break;
         default: ctx.in.unget(); goto script_flag_done;
         }

      script_flag_done:
         if(!ctx.in.peek(Core::TOK_BraceO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "{", true);

         ParseDeclFunction(ctx, scope, attr);

         return AST::StatementCreate_Empty(pos);
      }

      //
      // GetDecl_Special
      //
      static AST::Statement::CRef GetDecl_Special(ParserCtx const &ctx,
         CC::Scope_Global &scope)
      {
         auto pos = ctx.in.get().pos;

         // special-list:
         //    special-item
         //    special-list , special-item
         do
         {
            // special-item:
            //    type-name(opt) special-address identifier
            //       ( special-parameters )

            AST::Attribute attr;
            attr.linka = IR::Linkage::ExtACS;

            // type-name(opt)
            if(IsType(ctx, scope))
            {
               attr.type = GetType(ctx, scope);
            }
            else
            {
               attr.type = CC::TypeIntegPrS;
            }

            // special-address:
            //    integer-constant :
            //    - integer-constant :
            //    { integer-constant } :
            //    { integer-constant , integer-constant } :
            if(ctx.in.peek(Core::TOK_NumInt))
            {
               attr.callt = IR::CallType::Special;
               attr.addrI = GetExp_Prim(ctx, scope)->getIRExp();
            }
            else if(ctx.in.drop(Core::TOK_Sub))
            {
               attr.callt = IR::CallType::Native;
               attr.addrI = GetExp_Prim(ctx, scope)->getIRExp();
            }
            else if(ctx.in.drop(Core::TOK_BraceO))
            {
               attr.callt = IR::CallType::AsmFunc;

               if(!ctx.in.peek(Core::TOK_NumInt))
                  throw Core::ParseExceptExpect(ctx.in.peek(), "integer-constant", false);

               attr.addrI = GetExp_Prim(ctx, scope)->getIRExp();

               if(ctx.in.drop(Core::TOK_Comma))
               {
                  if(!ctx.in.peek(Core::TOK_NumInt))
                     throw Core::ParseExceptExpect(ctx.in.peek(), "integer-constant", false);

                  // TODO: Alternate immediate mode address.
                  GetExp_Prim(ctx, scope)->getIRExp();
               }

               if(!ctx.in.drop(Core::TOK_BraceC))
                  throw Core::ParseExceptExpect(ctx.in.peek(), "}", true);
            }
            else
               throw Core::ParseExceptExpect(ctx.in.peek(), "special-address", false);

            if(!ctx.in.drop(Core::TOK_Colon))
               throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

            // identifier
            if(!ctx.in.peek(Core::TOK_Identi))
               throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

            attr.setName(ctx.in.get());

            // (
            if(!ctx.in.drop(Core::TOK_ParenO))
               throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

            // special-parameters:
            //    integer-constant
            //    integer-constant , integer-constant
            //    integer-constant , parameter-type-list
            //    parameter-type-list
            if(ctx.in.peek(Core::TOK_NumInt))
            {
               auto argMin = CC::ExpToFastU(GetExp_Prim(ctx, scope));

               AST::TypeSet::CPtr types;
               if(ctx.in.drop(Core::TOK_Comma))
               {
                  if(ctx.in.peek(Core::TOK_NumInt))
                  {
                     auto argMax = CC::ExpToFastU(GetExp_Prim(ctx, scope));

                     Core::Array<AST::Type::CRef> param{argMax, CC::TypeIntegPrS};

                     types = AST::TypeSet::Get(param.data(), param.size(), false);
                  }
                  else
                     types = std::get<0>(GetTypeList(ctx, scope));
               }
               else
               {
                  Core::Array<AST::Type::CRef> param{argMin, CC::TypeIntegPrS};

                  types = AST::TypeSet::Get(param.data(), param.size(), false);
               }

               attr.type     = attr.type->getTypeFunction(types, attr.callt);
               attr.paramOpt = types->size() - argMin;
            }
            else
            {
               auto types = std::get<0>(GetTypeList(ctx, scope));
               attr.type = attr.type->getTypeFunction(types, attr.callt);
            }

            // )
            if(!ctx.in.drop(Core::TOK_ParenC))
               throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

            // Check compatibility with existing symbol, if any.
            if(auto lookup = scope.find(attr.name))
            {
               if(lookup.res != CC::Lookup::Func)
                  throw Core::ExceptStr(attr.namePos,
                     "name redefined as different kind of symbol");

               if(lookup.resFunc->retrn != attr.type->getBaseType())
                  throw Core::ExceptStr(attr.namePos,
                     "function redeclared with different return type");
            }

            scope.add(attr.name, GetDeclFunction(scope, attr));
         }
         while(ctx.in.drop(Core::TOK_Comma));

         if(!ctx.in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(ctx.in.peek(), ";", true);

         return AST::StatementCreate_Empty(pos);
      }

      //
      // GetDeclFunction (global)
      //
      static AST::Function::Ref GetDeclFunction(CC::Scope_Global &scope,
         AST::Attribute &attr)
      {
         auto fn = scope.getFunction(attr);

         // Set address, if one provided.
         if(attr.addrI)
            fn->valueInt = attr.addrI;
         else if(attr.addrS)
            fn->valueStr = attr.addrS;

         return fn;
      }

      //
      // GetDeclObjectSpace
      //
      template<typename T>
      static void GetDeclObjectSpace(T &scope, AST::Attribute &attr,
         IR::AddrBase base)
      {
         auto spaceAttr = attr;

         spaceAttr.name      += "$arr";
         spaceAttr.space.base = base;

         auto space = scope.getSpace(spaceAttr);

         space->defin = true;
         space->value = spaceAttr.addrI;

         // Don't use the supplied address for the contained object.
         attr.addrI = nullptr;

         attr.type = attr.type->getTypeArrayQualAddr({base, space->glyph});
      }

      //
      // GetDeclObject (global)
      //
      static AST::Object::Ref GetDeclObject(ParserCtx const &ctx,
         CC::Scope_Global &scope, AST::Attribute &attr, bool init)
      {
         if(attr.storeInt)
            throw Core::ExceptStr(attr.namePos, "file scope static");

         if(attr.storeGbl)
         {
            if(attr.type->isTypeArray())
               GetDeclObjectSpace(scope, attr, IR::AddrBase::GblArr);
            else
               attr.type = attr.type->getTypeQualAddr(IR::AddrBase::GblReg);
         }
         else if(attr.storeWld)
         {
            if(attr.type->isTypeArray())
               GetDeclObjectSpace(scope, attr, IR::AddrBase::WldArr);
            else
               attr.type = attr.type->getTypeQualAddr(IR::AddrBase::WldReg);
         }
         else
         {
            if(attr.type->isTypeArray())
               GetDeclObjectSpace(scope, attr, IR::AddrBase::MapArr);
            else
               attr.type = attr.type->getTypeQualAddr(IR::AddrBase::MapReg);
         }

         // Fetch/generate object.
         auto obj = scope.getObject(attr);

         // If init, wait until later to mark as definition because the type
         // might need to be completed by the initializer.
         if(!init || ctx.importing)
         {
            // First, make sure it has a complete type.
            if(!obj->type->isTypeComplete())
            {
               // Allow global/world arrays to be incomplete for compatibility.
               if(obj->type->isTypeArray() && (attr.storeGbl || attr.storeWld))
                  obj->type = obj->type->getBaseType()->getTypeArray(1);
               else
                  throw Core::ExceptStr(attr.namePos,
                     "object with incomplete type");
            }

            obj->defin = true;

            // Give default initializer.
            obj->init = CC::Exp_Init::Create(
               CC::Init::Create(obj->type, 0, attr.namePos), true);
         }

         // Set address, if one provided.
         if(attr.addrI)
            obj->value = attr.addrI;

         return obj;
      }

      //
      // GetDeclObject (local)
      //
      static AST::Object::Ref GetDeclObject(ParserCtx const &,
         CC::Scope_Local &scope, AST::Attribute &attr, bool init)
      {
         // All block-scope declarations are definitions and all block-scope
         // definitions must have no linkage.
         attr.linka = IR::Linkage::None;

         if(attr.storeGbl)
            throw Core::ExceptStr(attr.namePos, "block scope global");

         if(attr.storeWld)
            throw Core::ExceptStr(attr.namePos, "block scope world");

         if(attr.storeInt)
         {
            if(attr.type->isTypeArray())
               GetDeclObjectSpace(scope, attr, IR::AddrBase::MapArr);
            else
               attr.type = attr.type->getTypeQualAddr(IR::AddrBase::MapReg);
         }
         else
         {
            if(attr.type->isTypeArray())
               GetDeclObjectSpace(scope, attr, IR::AddrBase::LocArr);
            else
               attr.type = attr.type->getTypeQualAddr(IR::AddrBase::LocReg);
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
                  throw Core::ExceptStr(attr.namePos,
                     "object with incomplete type");

               obj->defin = true;

               // Give default initializer.
               obj->init = CC::Exp_Init::Create(
                  CC::Init::Create(obj->type, 0, attr.namePos), true);
            }
         }

         // Set address, if one provided.
         if(attr.addrI)
            obj->value = attr.addrI;

         return obj;
      }

      //
      // ParseDeclFunction
      //
      template<typename T>
      static void ParseDeclFunction(ParserCtx const &ctx, T &scope,
         AST::Attribute &attr)
      {
         // Check compatibility with existing symbol, if any.
         if(auto lookup = scope.find(attr.name))
         {
            if(lookup.res != CC::Lookup::Func)
               throw Core::ExceptStr(attr.namePos,
                  "name redefined as different kind of symbol");

            if(lookup.resFunc->retrn != attr.type->getBaseType())
               throw Core::ExceptStr(attr.namePos,
                  "function redeclared with different return type");
         }

         if(ctx.importing)
         {
            if(attr.linka != IR::Linkage::None)
               scope.add(attr.name, GetDeclFunction(scope, attr));

            SkipBalancedToken(ctx);
            return;
         }

         auto fn = GetDeclFunction(scope, attr);

         scope.add(attr.name, fn);

         if(ctx.in.drop(Core::TOK_Semico))
            return;

         if(!fn->retrn->isTypeComplete() && !fn->retrn->isTypeVoid())
            throw Core::ExceptStr(attr.namePos, "incomplete return");

         auto &fnScope = scope.createScope(attr, fn);

         auto stmntPre  = CC::StatementCreate_FuncPre(ctx.in.peek().pos, fnScope);
         auto stmntBody = GetStatement(ctx, fnScope);
         auto stmntPro  = CC::StatementCreate_FuncPro(ctx.in.reget().pos, fnScope);

         // Create statements for the function.
         Core::Array<AST::Statement::CRef> stmnts =
            {Core::Pack, stmntPre, stmntBody, stmntPro};

         fn->stmnt = AST::StatementCreate_Multi(attr.namePos, std::move(stmnts));
         fn->defin = true;

         fn->setAllocLoc(attr.allocLoc);

         if(!fn->retrn->isTypeVoid() && fn->stmnt->isNoReturn())
            AST::WarnReturnType(attr.namePos, "no return in non-void function");
      }

      //
      // ParseDeclObject
      //
      template<typename T>
      static void ParseDeclObject(ParserCtx const &ctx, T &scope,
         AST::Attribute &attr, std::vector<AST::Statement::CRef> &inits)
      {
         AST::Type::CPtr lookupType;

         // Check compatibility with existing symbol, if any.
         if(auto lookup = scope.find(attr.name))
         {
            if(lookup.res != CC::Lookup::Obj)
               throw Core::ExceptStr(attr.namePos,
                  "name redefined as different kind of symbol");

            // Defer type compatibility check for later.
            lookupType = lookup.resObj->type;
         }

         // Insert special declaration statement.
         if(inits.empty())
            inits.emplace_back(CC::StatementCreate_Decl(attr.namePos, scope));

         // = initializer
         if(ctx.in.drop(Core::TOK_Equal))
         {
            auto obj = GetDeclObject(ctx, scope, attr, true);

            scope.add(attr.name, obj);

            obj->init = GetExp_Init(ctx, scope, obj->type);
            obj->type = obj->init->getType();

            if(obj->store == AST::Storage::Static && !obj->init->isIRExp())
               throw Core::ExceptStr(obj->init->pos,
                  "non-constant initializer for static storage object");

            SetDeclObjectInit(ctx, scope, attr, inits, obj);
         }
         else
            scope.add(attr.name, GetDeclObject(ctx, scope, attr, false));

         // Do type compatibility test here because type may have been altered.
         if(lookupType && lookupType != attr.type &&
            (!lookupType->isTypeArray() || !attr.type->isTypeArray() ||
             lookupType->getBaseType() != attr.type->getBaseType()))
         {
            throw Core::ExceptStr(attr.namePos,
               "object redeclared with different type");
         }
      }

      //
      // SetDeclObjectInit (global)
      //
      static void SetDeclObjectInit(ParserCtx const &ctx, CC::Scope_Global &,
         AST::Attribute &, std::vector<AST::Statement::CRef> &,
         AST::Object *obj)
      {
         if(ctx.importing)
            return;

         obj->defin = true;

         // File-scope objects get initialization code generated later.
      }

      //
      // SetDeclObjectInit (local)
      //
      static void SetDeclObjectInit(ParserCtx const &ctx, CC::Scope_Local &,
         AST::Attribute &attr, std::vector<AST::Statement::CRef> &inits,
         AST::Object *obj)
      {
         obj->defin = true;

         // Block-scope statics must have constant initializers, so they can be
         // handled like file-scope statics.
         if(obj->store == AST::Storage::Static)
            return;

         auto initExp = CC::ExpCreate_Obj(ctx.prog, obj, attr.namePos);
         initExp = CC::Exp_Assign::Create(initExp, obj->init, obj->init->pos);

         inits.emplace_back(AST::StatementCreate_Exp(initExp));
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetDecl
      //
      AST::Statement::CRef GetDecl(ParserCtx const &ctx, Scope_Global &scope)
      {
         if(ctx.in.peek(Core::TOK_KeyWrd, Core::STR_function))
            return GetDecl_Function(ctx, scope);

         if(ctx.in.peek(Core::TOK_Identi, Core::STR_print))
            return GetDecl_Print(ctx, scope);

         if(ctx.in.peek(Core::TOK_KeyWrd, Core::STR_script))
            return GetDecl_Script(ctx, scope);

         if(ctx.in.peek(Core::TOK_KeyWrd, Core::STR_special))
            return GetDecl_Special(ctx, scope);

         return GetDecl_Object(ctx, scope);
      }

      //
      // GetDecl
      //
      AST::Statement::CRef GetDecl(ParserCtx const &ctx, CC::Scope_Local &scope)
      {
         return GetDecl_Object(ctx, scope);
      }

      //
      // IsDecl
      //
      bool IsDecl(ParserCtx const &ctx, CC::Scope &scope)
      {
         return IsDeclSpec(ctx, scope);
      }
   }
}

// EOF

