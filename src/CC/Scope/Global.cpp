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
#include "AST/Storage.hpp"
#include "AST/Type.hpp"

#include "Bytecode/Platform.hpp"

#include "Core/Exception.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"


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
         std::vector<AST::Object::Ref> params;
         params.reserve(attr.param.size());

         for(auto const &param : attr.param)
         {
            auto obj = AST::Object::Create(param.name,
               genGlyphObj(param.name, IR::Linkage::None));

            obj->store = AST::Storage::Auto;
            obj->type  = param.type;
            obj->defin = true;

            if(param.storeAuto) obj->point = true;
            if(param.storeReg)  obj->noPtr = true;

            params.emplace_back(obj);
         }

         auto fnCtx = new FunctionScope(this, fn,
            Core::Array<AST::Object::Ref>(params.begin(), params.end()));

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
         case IR::Linkage::ExtACS:
         case IR::Linkage::ExtASM:
            return name;

         case IR::Linkage::ExtC:
            return Core::STR__ + name;
         }

         return name;
      }

      //
      // GlobalScope::getFunction
      //
      AST::Function::Ref GlobalScope::getFunction(AST::Attribute const &attr)
      {
         throw Core::ExceptStr(attr.namePos, "getFunction stub");
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

