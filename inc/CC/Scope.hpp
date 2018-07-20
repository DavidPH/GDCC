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

#ifndef GDCC__CC__Scope_H__
#define GDCC__CC__Scope_H__

#include "../CC/Types.hpp"

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"
#include "../Core/String.hpp"

#include <unordered_map>
#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Lookup
   //
   // Stores the result of an identifier lookup.
   //
   class Lookup
   {
   public:
      using ResultEnum  = Core::Integ const *;
      using ResultFunc  = Core::CounterRef<SR::Function>;
      using ResultObj   = Core::CounterRef<SR::Object>;
      using ResultSpace = Core::CounterRef<SR::Space>;
      using ResultType  = Core::CounterRef<SR::Type const>;

      //
      // Result
      //
      enum Result
      {
         None,
         Enum,
         Func,
         Obj,
         Space,
         Type,
      };


      Lookup(std::nullptr_t = nullptr) : res{None} {}
      Lookup(Lookup const &lookup);
      explicit Lookup(Core::Integ const *e);
      explicit Lookup(SR::Function      *func);
      explicit Lookup(SR::Object        *obj);
      explicit Lookup(SR::Space         *space);
      explicit Lookup(SR::Type    const *type);
      ~Lookup();

      explicit operator bool () const {return res != None;}

      Lookup &operator = (Lookup const &lookup);


      Result res;

      union
      {
         ResultEnum  resEnum;
         ResultFunc  resFunc;
         ResultObj   resObj;
         ResultSpace resSpace;
         ResultType  resType;
      };
   };

   //
   // Scope
   //
   // Common base for all scopes, handling the most basic aspects of what a
   // scope contains. (Functions, objects, types, and address spaces.)
   //
   class Scope
   {
   public:
      Scope() = delete;
      Scope(Scope const &) = delete;
      Scope(Scope &&) = delete;

      void add(Core::String name, SR::Function   *fn);
      void add(Core::String name, SR::Object     *obj);
      void add(Core::String name, SR::Space      *space);
      void add(Core::String name, SR::Type const *type);

      void addEnum(Core::String name, Core::Integ const &e);

      void addTypeTag(Core::String name, SR::Type *type);

      // Searches this scope for the identifier.
      Lookup find(Core::String name) const;

      Core::Integ const *findEnum(Core::String name) const;

      Core::CounterPtr<SR::Function> findFunction(Core::String name) const;

      Core::CounterPtr<SR::Object> findObject(Core::String name) const;

      Core::CounterPtr<SR::Space> findSpace(Core::String name) const;

      Core::CounterPtr<SR::Type const> findType(Core::String name) const;

      // Searches this scope for a tagged type (struct, union, enum).
      Core::CounterPtr<SR::Type> findTypeTag(Core::String name) const;

      // Performs a recursive lookup.
      Lookup lookup(Core::String name) const;

      // Searches for a tagged type (struct, union, enum).
      Core::CounterPtr<SR::Type> lookupTypeTag(Core::String name) const;


      Scope  *const parent;
      Scope_Global &global;

   protected:
      template<typename T>
      using LookupTable = std::unordered_map<Core::String, Core::CounterRef<T>>;


      explicit Scope(Scope *parent);
      Scope(Scope *parent, Scope_Global &global);
      virtual ~Scope();

      std::unordered_map<Core::String, Core::Integ> tableEnum;

      LookupTable<SR::Function>   tableFunc;
      LookupTable<SR::Object>     tableObj;
      LookupTable<SR::Space>      tableSpace;
      LookupTable<SR::Type const> tableType;
      LookupTable<SR::Type>       tableTypeTag;
   };
}

#endif//GDCC__CC__Scope_H__

