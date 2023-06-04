//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2023 David Hill
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

#include "Core/Exception.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"
#include "IR/Program.hpp"

#include "SR/Attribute.hpp"
#include "SR/Function.hpp"
#include "SR/Object.hpp"
#include "SR/Space.hpp"
#include "SR/Statement.hpp"
#include "SR/Storage.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
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
      // Break potential reference-cycles in functions.
      for(auto &itr : globalFunc)
         itr.second->stmnt = nullptr;
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
   Scope_Function &Scope_Global::createScope(SR::Attribute const &attr,
      SR::Function *fn)
   {
      // Set the function's label.
      if(!fn->label)
         fn->label = fn->genLabel();

      // If there are any parameters, generate a label to prefix them with.
      Core::String ctxLabel =
         attr.param.empty() ? Core::String(Core::STRNULL) : fn->genLabel();

      // Create an SR::Object for each parameter.
      std::vector<SR::Object::Ref> params;
      params.reserve(attr.param.size());
      for(auto const &param : attr.param)
      {
         auto obj = SR::Object::Create(param.name,
            param.name ? ctxLabel + param.name : fn->genLabel());

         obj->store = SR::Storage::Auto;
         obj->type  = param.type;
         obj->defin = true;

         if(param.storeAuto) obj->refer = true;
         if(param.storeReg)  obj->noRef = true;

         params.emplace_back(obj);
      }

      // Create new scope.
      auto fnCtx = new Scope_Function(*this, fn,
         Core::Array<SR::Object::Ref>(params.begin(), params.end()));
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
         return name.getLower();

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
   SR::Function::Ref Scope_Global::getFunction(SR::Attribute const &attr)
   {
      auto glyph = genGlyphObj(attr.name, attr.linka);

      auto itr = globalFunc.find(glyph);
      if(itr == globalFunc.end())
      {
         auto fn = SR::Function::Create(attr.name, glyph);

         fn->ctype    = attr.callt;
         fn->linka    = attr.linka;
         fn->paramOpt = attr.paramOpt;
         fn->retrn    = attr.type->getBaseType();
         fn->stype    = {attr.stype.begin(), attr.stype.end()};
         fn->type     = attr.type;
         fn->warnUse  = attr.warnUse;

         fn->declAuto    = attr.declAuto;
         fn->delay       = attr.funcDelay;
         fn->noInitDelay = attr.funcNoInitDelay;

         itr = globalFunc.emplace(glyph, fn).first;
      }

      // If not yet defined, update certain properties.
      if(!itr->second->defin)
      {
         SR::Function::Ref &fn = itr->second;

         fn->stype = {attr.stype.begin(), attr.stype.end()};

         fn->delay = attr.funcDelay;

         // If previously auto-declared, replace type information.
         if(fn->declAuto)
         {
            fn->retrn = attr.type->getBaseType();
            fn->type  = attr.type;

            fn->declAuto = attr.declAuto;
         }
      }

      return itr->second;
   }

   //
   // Scope_Global::getObject
   //
   SR::Object::Ref Scope_Global::getObject(SR::Attribute const &attr)
   {
      auto glyph = genGlyphObj(attr.name, attr.linka);

      auto itr = globalObj.find(glyph);
      if(itr == globalObj.end())
      {
         auto obj = SR::Object::Create(attr.name, glyph);

         obj->linka    = attr.linka;
         obj->store    = SR::Storage::Static;
         obj->type     = attr.type;
         obj->warnUse  = attr.warnUse;

         itr = globalObj.emplace(glyph, obj).first;
      }

      // If not yet defined, update certain properties.
      if(!itr->second->defin)
      {
         SR::Object::Ref &obj = itr->second;

         // Use newer type. This allows declaring with incomplete array type
         // and then later defining it with a size.
         obj->type = attr.type;
      }

      return itr->second;
   }

   //
   // Scope_Globa::getSpace
   //
   SR::Space::Ref Scope_Global::getSpace(SR::Attribute const &attr)
   {
      auto glyph = genGlyphObj(attr.name, attr.linka);

      auto itr = globalSpace.find(glyph);
      if(itr == globalSpace.end())
      {
         auto space = SR::Space::Create(attr.name, glyph);

         space->linka = attr.linka;
         space->space = attr.space.base;

         itr = globalSpace.emplace(glyph, space).first;
      }

      return itr->second;
   }
}

// EOF

