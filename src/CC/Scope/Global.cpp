//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C file/global scope handling.
//
//-----------------------------------------------------------------------------

#include "CC/Scope/Global.hpp"

#include "CC/Scope/Function.hpp"

#include "AST/Attribute.hpp"
#include "AST/Function.hpp"
#include "AST/Object.hpp"
#include "AST/Space.hpp"
#include "AST/Statement.hpp"
#include "AST/Storage.hpp"
#include "AST/Type.hpp"

#include "Bytecode/Platform.hpp"

#include "Core/Exception.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"
#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GlobalScope constructor
      //
      GlobalScope::GlobalScope() : Scope{nullptr}
      {
      }

      //
      // GlobalScope destructor
      //
      GlobalScope::~GlobalScope()
      {
         for(auto &ctx : subScopes)
            delete ctx;
      }

      //
      // GlobalScope::allocAuto
      //
      void GlobalScope::allocAuto()
      {
         for(auto &ctx : subScopes)
            ctx->allocAuto();
      }

      //
      // GlobalScope::createScope
      //
      FunctionScope *GlobalScope::createScope(AST::Attribute const &attr,
         AST::Function *fn)
      {
         // Set the function's label.
         if(!fn->label)
            fn->label = fn->genLabel();

         // If there are any parameters, generate a label to prefix them with.
         Core::String ctxLabel =
            attr.param.empty() ? Core::String(Core::STRNULL) : fn->genLabel();

         // Create an AST::Object for each parameter.
         std::vector<AST::Object::Ref> params;
         params.reserve(attr.param.size());
         for(auto const &param : attr.param)
         {
            auto obj = AST::Object::Create(param.name,
               param.name ? ctxLabel + param.name : fn->genLabel());

            obj->store = AST::Storage::Auto;
            obj->type  = param.type;
            obj->defin = true;

            if(param.storeAuto) obj->refer = true;
            if(param.storeReg)  obj->noRef = true;

            params.emplace_back(obj);
         }

         // Create new scope.
         auto fnCtx = new FunctionScope(this, fn,
            Core::Array<AST::Object::Ref>(params.begin(), params.end()));
         fnCtx->label = ctxLabel;

         subScopes.emplace_back(fnCtx);

         return fnCtx;
      }

      //
      // GlobalScope::genGlyphObj
      //
      Core::String GlobalScope::genGlyphObj(Core::String name, IR::Linkage linka)
      {
         switch(linka)
         {
         case IR::Linkage::None:
            break; // TODO

         case IR::Linkage::ExtACS:
         case IR::Linkage::ExtASM:
            return name;

         case IR::Linkage::ExtC:
            return Core::STR__ + name;

         case IR::Linkage::ExtCXX:
         case IR::Linkage::ExtDS:
            break; // TODO

         case IR::Linkage::IntC:
            break; // TODO

         case IR::Linkage::IntCXX:
            break; // TODO
         }

         return name;
      }

      //
      // GlobalScope::genIR
      //
      void GlobalScope::genIR(IR::Program &prog)
      {
         for(auto &ctx : subScopes)
            ctx->genIR(prog);

         for(auto &itr : globalFunc)
            itr.second->getIRFunction(prog);

         // Generate statements as separate pass.
         for(auto &ctx : subScopes)
         {
            if(auto fn = prog.findFunction(ctx->fn->glyph))
            {
               if(ctx->fn->stmnt)
                  ctx->fn->stmnt->genStmnt({fn->block, ctx->fn, prog});
            }
         }
      }

      //
      // GlobalScope::getFunction
      //
      AST::Function::Ref GlobalScope::getFunction(AST::Attribute const &attr)
      {
         auto glyph = genGlyphObj(attr.name, attr.linka);

         auto itr = globalFunc.find(glyph);
         if(itr == globalFunc.end())
         {
            auto fn = AST::Function::Create(attr.name, glyph);

            fn->ctype = attr.callt;
            fn->linka = attr.linka;
            fn->retrn = attr.type->getBaseType();
            fn->type  = attr.type;

            itr = globalFunc.emplace(glyph, fn).first;
         }

         return itr->second;
      }

      //
      // GlobalScope::getObject
      //
      AST::Object::Ref GlobalScope::getObject(AST::Attribute const &attr)
      {
         throw Core::ExceptStr(attr.namePos, "getObject stub");
      }
   }
}

//EOF

