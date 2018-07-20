//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C identifier scoping/lookup handling.
//
//-----------------------------------------------------------------------------

#include "CC/Scope.hpp"

#include "SR/Function.hpp"
#include "SR/Object.hpp"
#include "SR/Space.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Lookup copy constructor
   //
   Lookup::Lookup(Lookup const &lookup) : res{lookup.res}
   {
      switch(res)
      {
      case None:                                               break;
      case Enum:  new(&resEnum)  ResultEnum (lookup.resEnum);  break;
      case Func:  new(&resFunc)  ResultFunc (lookup.resFunc);  break;
      case Obj:   new(&resObj)   ResultObj  (lookup.resObj);   break;
      case Space: new(&resSpace) ResultSpace(lookup.resSpace); break;
      case Type:  new(&resType)  ResultType (lookup.resType);  break;
      }
   }

   //
   // Lookup constructor
   //
   Lookup::Lookup(Core::Integ const *e) : res{Enum}, resEnum{e}
   {
   }

   //
   // Lookup constructor
   //
   Lookup::Lookup(SR::Function *func) : res{Func}, resFunc{func}
   {
   }

   //
   // Lookup constructor
   //
   Lookup::Lookup(SR::Object *obj) : res{Obj}, resObj{obj}
   {
   }

   //
   // Lookup constructor
   //
   Lookup::Lookup(SR::Space *space) : res{Space}, resSpace{space}
   {
   }

   //
   // Lookup constructor
   //
   Lookup::Lookup(SR::Type const *type) : res{Type}, resType{type}
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
      case Enum:  resEnum .~ResultEnum (); break;
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
      case Enum:  resEnum  = lookup.resEnum;  break;
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
   Scope::Scope(Scope *parent_) :
      parent{parent_},
      global(parent_->global)
   {
   }

   //
   // Scope constructor
   //
   Scope::Scope(Scope *parent_, Scope_Global &global_) :
      parent{parent_},
      global(global_)
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
   void Scope::add(Core::String name, SR::Function *fn)
   {
      tableFunc.emplace(std::piecewise_construct,
         std::forward_as_tuple(name),
         std::forward_as_tuple(fn));
   }

   //
   // Scope::add
   //
   void Scope::add(Core::String name, SR::Object *obj)
   {
      tableObj.emplace(std::piecewise_construct,
         std::forward_as_tuple(name),
         std::forward_as_tuple(obj));
   }

   //
   // Scope::add
   //
   void Scope::add(Core::String name, SR::Space *space)
   {
      tableSpace.emplace(std::piecewise_construct,
         std::forward_as_tuple(name),
         std::forward_as_tuple(space));
   }

   //
   // Scope::add
   //
   void Scope::add(Core::String name, SR::Type const *type)
   {
      tableType.emplace(std::piecewise_construct,
         std::forward_as_tuple(name),
         std::forward_as_tuple(type));
   }

   //
   // Scope::addEnum
   //
   void Scope::addEnum(Core::String name, Core::Integ const &e)
   {
      tableEnum.emplace(std::piecewise_construct,
         std::forward_as_tuple(name),
         std::forward_as_tuple(e));
   }

   //
   // Scope::addTypeTag
   //
   void Scope::addTypeTag(Core::String name, SR::Type *type)
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

      if(auto e     = findEnum(name))     return Lookup(e);
      if(auto fn    = findFunction(name)) return Lookup(fn);
      if(auto obj   = findObject(name))   return Lookup(obj);
      if(auto space = findSpace(name))    return Lookup(space);
      if(auto type  = findType(name))     return Lookup(type);

      return Lookup();
   }

   //
   // Scope::findEnum
   //
   Core::Integ const *Scope::findEnum(Core::String name) const
   {
      auto e = tableEnum.find(name);
      return e == tableEnum.end() ? nullptr : &e->second;
   }

   //
   // Scope::findFunction
   //
   SR::Function::Ptr Scope::findFunction(Core::String name) const
   {
      auto fn = tableFunc.find(name);
      return fn == tableFunc.end() ? nullptr : &*fn->second;
   }

   //
   // Scope::findObject
   //
   SR::Object::Ptr Scope::findObject(Core::String name) const
   {
      auto obj = tableObj.find(name);
      return obj == tableObj.end() ? nullptr : &*obj->second;
   }

   //
   // Scope::findSpace
   //
   SR::Space::Ptr Scope::findSpace(Core::String name) const
   {
      auto space = tableSpace.find(name);
      return space == tableSpace.end() ? nullptr : &*space->second;
   }

   //
   // Scope::findType
   //
   SR::Type::CPtr Scope::findType(Core::String name) const
   {
      auto type = tableType.find(name);
      return type == tableType.end() ? nullptr : &*type->second;
   }

   //
   // Scope::findTypeTag
   //
   SR::Type::Ptr Scope::findTypeTag(Core::String name) const
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
   SR::Type::Ptr Scope::lookupTypeTag(Core::String name) const
   {
      if(auto res = findTypeTag(name)) return res;
      if(parent) return parent->lookupTypeTag(name);
      return nullptr;
   }
}

// EOF

