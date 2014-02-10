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

#include "AST/Function.hpp"
#include "AST/Object.hpp"
#include "AST/Space.hpp"
#include "AST/Type.hpp"


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
   }
}

// EOF

