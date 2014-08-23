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
      // Scope_Global constructor
      //
      Scope_Global::Scope_Global(Core::String label_) :
         Scope{nullptr, *this},

         label    {label_},
         stringGen{label_, "$L$"}
      {
      }

      //
      // Scope_Global destructor
      //
      Scope_Global::~Scope_Global()
      {
      }

      //
      // Scope_Global::allocAuto
      //
      void Scope_Global::allocAuto()
      {
         for(auto &ctx : subScopes)
            ctx->allocAuto();
      }

      //
      // Scope_Global::createScope
      //
      Scope_Function &Scope_Global::createScope(AST::Attribute const &attr,
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
         auto fnCtx = new Scope_Function(*this, fn,
            Core::Array<AST::Object::Ref>(params.begin(), params.end()));
         fnCtx->label = ctxLabel;

         subScopes.emplace_back(fnCtx);

         return *fnCtx;
      }

      //
      // Scope_Global::genGlyphObj
      //
      Core::String Scope_Global::genGlyphObj(Core::String name, IR::Linkage linka)
      {
         switch(linka)
         {
         case IR::Linkage::None:
            if(!name) return stringGen();
            return label + name;

         case IR::Linkage::ExtACS:
         case IR::Linkage::ExtASM:
            return name;

         case IR::Linkage::ExtAXX:
            break; // TODO

         case IR::Linkage::ExtC:
            return Core::STR__ + name;

         case IR::Linkage::ExtCXX:
         case IR::Linkage::ExtDS:
            break; // TODO

         case IR::Linkage::IntC:
            return label + name;

         case IR::Linkage::IntCXX:
            break; // TODO
         }

         return name;
      }

      //
      // Scope_Global::genIR
      //
      void Scope_Global::genIR(IR::Program &prog)
      {
         for(auto &ctx : subScopes)
            ctx->genIR(prog);

         for(auto &itr : globalFunc)
            itr.second->genFunctionDecl(prog);

         for(auto itr : globalObj)
            itr.second->genObject(prog);

         for(auto &itr : globalFunc)
            itr.second->genFunctionDefn(prog);

         for(auto &itr : globalSpace)
            itr.second->genSpace(prog);
      }

      //
      // Scope_Global::getFunction
      //
      AST::Function::Ref Scope_Global::getFunction(AST::Attribute const &attr)
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
      // Scope_Global::getObject
      //
      AST::Object::Ref Scope_Global::getObject(AST::Attribute const &attr)
      {
         auto glyph = genGlyphObj(attr.name, attr.linka);

         auto itr = globalObj.find(glyph);
         if(itr == globalObj.end())
         {
            auto obj = AST::Object::Create(attr.name, glyph);

            obj->linka = attr.linka;
            obj->store = AST::Storage::Static;
            obj->type  = attr.type;

            itr = globalObj.emplace(glyph, obj).first;
         }

         return itr->second;
      }

      //
      // Scope_Globa::getSpace
      //
      AST::Space::Ref Scope_Global::getSpace(AST::Attribute const &attr)
      {
         auto glyph = genGlyphObj(attr.name, attr.linka);

         auto itr = globalSpace.find(glyph);
         if(itr == globalSpace.end())
         {
            auto space = AST::Space::Create(attr.name, glyph);

            space->linka = attr.linka;
            space->space = attr.space.base;

            itr = globalSpace.emplace(glyph, space).first;
         }

         return itr->second;
      }
   }
}

//EOF

