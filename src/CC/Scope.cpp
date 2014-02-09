//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C identifier scoping/lookup handling.
//
//-----------------------------------------------------------------------------

#include "CC/Scope.hpp"

#include "AST/Attribute.hpp"
#include "AST/Function.hpp"
#include "AST/Object.hpp"
#include "AST/Space.hpp"
#include "AST/Storage.hpp"
#include "AST/Type.hpp"

#include "Bytecode/Platform.hpp"

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
      // Lookup copy constructor
      //
      Lookup::Lookup(Lookup const &lookup) : res{lookup.res}
      {
         switch(res)
         {
         case None:                                               break;
         case Func:  new(&resFunc)  ResultFunc (lookup.resFunc);  break;
         case Obj:   new(&resObj)   ResultObj  (lookup.resObj);   break;
         case Space: new(&resSpace) ResultSpace(lookup.resSpace); break;
         case Type:  new(&resType)  ResultType (lookup.resType);  break;
         }
      }

      //
      // Lookup constructor
      //
      Lookup::Lookup(AST::Function *func) : res{Func}, resFunc{func}
      {
      }

      //
      // Lookup constructor
      //
      Lookup::Lookup(AST::Object *obj) : res{Obj}, resObj{obj}
      {
      }

      //
      // Lookup constructor
      //
      Lookup::Lookup(AST::Space *space) : res{Space}, resSpace{space}
      {
      }

      //
      // Lookup constructor
      //
      Lookup::Lookup(AST::Type const *type) : res{Type}, resType{type}
      {
      }

      //
      // Lookup destructor
      //
      Lookup::~Lookup()
      {
         switch(res)
         {
         case None:                           break;
         case Func:  resFunc .~ResultFunc (); break;
         case Obj:   resObj  .~ResultObj  (); break;
         case Space: resSpace.~ResultSpace(); break;
         case Type:  resType .~ResultType (); break;
         }
      }

      //
      // Lookup::operator = Lookup
      //
      Lookup &Lookup::operator = (Lookup const &lookup)
      {
         if(res == lookup.res) switch(res)
         {
         case None:                              break;
         case Func:  resFunc  = lookup.resFunc;  break;
         case Obj:   resObj   = lookup.resObj;   break;
         case Space: resSpace = lookup.resSpace; break;
         case Type:  resType  = lookup.resType;  break;
         }
         else
         {
            this->~Lookup();
            new(this) Lookup(lookup);
         }

         return *this;
      }

      //
      // Scope constructor
      //
      Scope::Scope(Scope *parent_) : parent{parent_}
      {
      }

      //
      // Scope destructor
      //
      Scope::~Scope()
      {
      }

      //
      // Scope::add
      //
      void Scope::add(Core::String name, AST::Function *fn)
      {
         tableFunc.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(fn));
      }

      //
      // Scope::add
      //
      void Scope::add(Core::String name, AST::Object *obj)
      {
         tableObj.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(obj));
      }

      //
      // Scope::add
      //
      void Scope::add(Core::String name, AST::Space *space)
      {
         tableSpace.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(space));
      }

      //
      // Scope::add
      //
      void Scope::add(Core::String name, AST::Type const *type)
      {
         tableType.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(type));
      }

      //
      // Scope::addTypeTag
      //
      void Scope::addTypeTag(Core::String name, AST::Type *type)
      {
         tableTypeTag.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(type));
      }

      //
      // Scope::find
      //
      Lookup Scope::find(Core::String name) const
      {
         // The order of lookup here is unimportant, duplicate names produce
         // behavior beyond the specification of C.

         if(auto fn    = findFunction(name)) return Lookup(fn);
         if(auto obj   = findObject(name))   return Lookup(obj);
         if(auto space = findSpace(name))    return Lookup(space);
         if(auto type  = findType(name))     return Lookup(type);

         return Lookup();
      }

      //
      // Scope::findFunction
      //
      AST::Function::Ptr Scope::findFunction(Core::String name) const
      {
         auto fn = tableFunc.find(name);
         return fn == tableFunc.end() ? nullptr : &*fn->second;
      }

      //
      // Scope::findObject
      //
      AST::Object::Ptr Scope::findObject(Core::String name) const
      {
         auto obj = tableObj.find(name);
         return obj == tableObj.end() ? nullptr : &*obj->second;
      }

      //
      // Scope::findSpace
      //
      AST::Space::Ptr Scope::findSpace(Core::String name) const
      {
         auto space = tableSpace.find(name);
         return space == tableSpace.end() ? nullptr : &*space->second;
      }

      //
      // Scope::findType
      //
      AST::Type::CPtr Scope::findType(Core::String name) const
      {
         auto type = tableType.find(name);
         return type == tableType.end() ? nullptr : &*type->second;
      }

      //
      // Scope::findTypeTag
      //
      AST::Type::Ptr Scope::findTypeTag(Core::String name) const
      {
         auto type = tableTypeTag.find(name);
         return type == tableTypeTag.end() ? nullptr : &*type->second;
      }

      //
      // Scope::lookup
      //
      Lookup Scope::lookup(Core::String name) const
      {
         if(auto res = find(name)) return res;
         if(parent) return parent->lookup(name);
         return Lookup();
      }

      //
      // Scope::lookupTypeTag
      //
      AST::Type::Ptr Scope::lookupTypeTag(Core::String name) const
      {
         if(auto res = findTypeTag(name)) return res;
         if(parent) return parent->lookupTypeTag(name);
         return nullptr;
      }

      //
      // GlobalScope constructor
      //
      GlobalScope::GlobalScope() : Super{nullptr}
      {
      }

      //
      // GlobalScope destructor
      //
      GlobalScope::~GlobalScope()
      {
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
      FunctionScope::Ref GlobalScope::createScope(AST::Attribute const &attr,
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

         FunctionScope::Ref fnCtx{new FunctionScope(this, fn,
            Core::Array<AST::Object::Ref>(params.begin(), params.end()))};

         return fnCtx;
      }

      //
      // GlobalScope::Create
      //
      GlobalScope::Ref GlobalScope::Create()
      {
         return static_cast<Ref>(new GlobalScope);
      }

      //
      // LocalScope constructor
      //
      LocalScope::LocalScope(Scope *parent_, GlobalScope *global_) :
         Super{parent_}, global{global_}
      {
      }

      //
      // FunctionScope constructor
      //
      FunctionScope::FunctionScope(GlobalScope *parent_, AST::Function *fn_,
         Core::Array<AST::Object::Ref> &&params_) :
         Super{parent_, parent_}, params{std::move(params_)}, fn{fn_}
      {
         global->subScopes.insert(this);

         for(auto const &param : params)
            if(param->name) add(param->name, param);
      }

      //
      // FunctionScope destructor
      //
      FunctionScope::~FunctionScope()
      {
         global->subScopes.erase(this);
      }

      //
      // FunctionScope::allocAuto
      //
      void FunctionScope::allocAuto()
      {
         IR::Type_Fixed idxType{Bytecode::GetWordBits(), 0, 0, 0};

         // Allocate parameter objects.
         BlockScope::AllocAutoInfo alloc;
         for(auto &obj : params)
         {
            auto &idx = obj->point ? alloc.localArs : alloc.localReg;
            obj->value = IR::ExpCreate_ValueRoot(
               IR::Value_Fixed(idx, idxType), Core::Origin(Core::STRNULL, 0));
            idx += obj->type->getSizeWords();
         }

         // Allocate sub-scopes.
         auto allocSub = alloc;
         for(auto &ctx : subScopes)
            allocSub.setMax(ctx->allocAuto(alloc));

         // Set function's local counts.
         fn->localArs = allocSub.localArs;
         fn->localReg = allocSub.localReg;
      }

      //
      // FunctionScope::createScope
      //
      BlockScope::Ref FunctionScope::createScope()
      {
         return static_cast<BlockScope::Ref>(new BlockScope(this, this));
      }

      //
      // BlockScope::AllocAutoInfo::setMax
      //
      void BlockScope::AllocAutoInfo::setMax(AllocAutoInfo const &alloc)
      {
         localArs = std::max(localArs, alloc.localArs);
         localReg = std::max(localReg, alloc.localReg);
      }

      //
      // BlockScope constructor
      //
      BlockScope::BlockScope(LocalScope *parent_, FunctionScope *fn_) :
         Super{parent_, fn_->global}, fn{fn_}
      {
         static_cast<LocalScope *>(&*parent)->subScopes.insert(this);
      }

      //
      // BlockScope destructor
      //
      BlockScope::~BlockScope()
      {
         static_cast<LocalScope *>(&*parent)->subScopes.erase(this);
      }

      //
      // BlockScope::allocAuto
      //
      BlockScope::AllocAutoInfo BlockScope::allocAuto(AllocAutoInfo const &base)
      {
         IR::Type_Fixed idxType{Bytecode::GetWordBits(), 0, 0, 0};

         // Allocate local objects.
         auto alloc = base;
         for(auto &itr : localObj)
         {
            auto &obj = itr.second;

            auto &idx = obj->point ? alloc.localArs : alloc.localReg;
            obj->value = IR::ExpCreate_ValueRoot(
               IR::Value_Fixed(idx, idxType), Core::Origin(Core::STRNULL, 0));
            idx += obj->type->getSizeWords();
         }

         // Allocate sub-scopes.
         auto allocSub = alloc;
         for(auto &ctx : subScopes)
            allocSub.setMax(ctx->allocAuto(alloc));

         return allocSub;
      }

      //
      // BlockScope::createScope
      //
      BlockScope::Ref BlockScope::createScope()
      {
         return static_cast<Ref>(new BlockScope(this, fn));
      }
   }
}

// EOF

