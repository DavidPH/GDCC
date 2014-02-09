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

#ifndef GDCC__CC__Scope_H__
#define GDCC__CC__Scope_H__

#include "../Core/Array.hpp"
#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"
#include "../Core/String.hpp"

#include <unordered_map>
#include <unordered_set>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Attribute;
      class Function;
      class Object;
      class Space;
      class Type;
   }

   namespace IR
   {
      enum class Linkage;
   }

   namespace CC
   {
      class BlockScope;
      class FunctionScope;

      //
      // Lookup
      //
      // Stores the result of an identifier lookup.
      //
      class Lookup
      {
      public:
         using ResultFunc  = Core::CounterRef<AST::Function>;
         using ResultObj   = Core::CounterRef<AST::Object>;
         using ResultSpace = Core::CounterRef<AST::Space>;
         using ResultType  = Core::CounterRef<AST::Type const>;

         //
         // Result
         //
         enum Result
         {
            None,
            Func,
            Obj,
            Space,
            Type,
         };


         Lookup() : res{None} {}
         Lookup(Lookup const &lookup);
         explicit Lookup(AST::Function   *func);
         explicit Lookup(AST::Object     *obj);
         explicit Lookup(AST::Space      *space);
         explicit Lookup(AST::Type const *type);
         ~Lookup();

         explicit operator bool () const {return res != None;}

         Lookup &operator = (Lookup const &lookup);


         Result res;

         union
         {
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
      class Scope : public Core::Counter
      {
         GDCC_Core_CounterPreambleNoClone(
            GDCC::CC::Scope, GDCC::Core::Counter);

      public:
         void add(Core::String name, AST::Function   *fn);
         void add(Core::String name, AST::Object     *obj);
         void add(Core::String name, AST::Space      *space);
         void add(Core::String name, AST::Type const *type);

         void addTypeTag(Core::String name, AST::Type *type);

         // Searches this scope for the identifier.
         Lookup find(Core::String name) const;

         Core::CounterPtr<AST::Function> findFunction(Core::String name) const;

         Core::CounterPtr<AST::Object> findObject(Core::String name) const;

         Core::CounterPtr<AST::Space> findSpace(Core::String name) const;

         Core::CounterPtr<AST::Type const> findType(Core::String name) const;

         // Searches this scope for a tagged type (struct, union, enum).
         Core::CounterPtr<AST::Type> findTypeTag(Core::String name) const;

         // Performs a recursive lookup.
         Lookup lookup(Core::String name) const;

         // Searches for a tagged type (struct, union, enum).
         Core::CounterPtr<AST::Type> lookupTypeTag(Core::String name) const;


         Ptr parent;

      protected:
         template<typename T>
         using LookupTable = std::unordered_map<Core::String, Core::CounterRef<T>>;


         explicit Scope(Scope *parent);
         virtual ~Scope();

         LookupTable<AST::Function>   tableFunc;
         LookupTable<AST::Object>     tableObj;
         LookupTable<AST::Space>      tableSpace;
         LookupTable<AST::Type const> tableType;
         LookupTable<AST::Type>       tableTypeTag;
      };

      //
      // GlobalScope
      //
      class GlobalScope : public Scope
      {
         GDCC_Core_CounterPreambleNoClone(
            GDCC::CC::GlobalScope, GDCC::CC::Scope);

      public:
         void allocAuto();

         Core::CounterRef<FunctionScope> createScope(
            AST::Attribute const &attr, AST::Function *fn);

         Core::String genGlyphObj(Core::String name, IR::Linkage linka);

         // Finds/creates a function, but does not add to lookup table.
         Core::CounterRef<AST::Function> getFunction(AST::Attribute const &attr);

         Core::CounterRef<AST::Object> getObject(AST::Attribute const &attr);

         Core::CounterRef<AST::Space> getSpace(AST::Attribute const &attr);


         friend class FunctionScope;

         static Ref Create();

      protected:
         GlobalScope();
         virtual ~GlobalScope();

         LookupTable<AST::Function> globalFunc;
         LookupTable<AST::Object>   globalObj;
         LookupTable<AST::Space>    globalSpace;

         std::unordered_set<FunctionScope *> subScopes;
      };

      //
      // LocalScope
      //
      // A non-global scope. In C itself, a function or block scope.
      //
      class LocalScope : public Scope
      {
         GDCC_Core_CounterPreambleNoClone(
            GDCC::CC::LocalScope, GDCC::CC::Scope);

      public:
         virtual Core::CounterRef<BlockScope> createScope() = 0;

         Core::CounterRef<AST::Object> getObject(AST::Attribute const &attr);

         GlobalScope::Ref global;


         friend class BlockScope;

      protected:
         LocalScope(Scope *parent, GlobalScope *global);
         virtual ~LocalScope();

         LookupTable<AST::Object> localObj;

         std::unordered_set<BlockScope *> subScopes;
      };

      //
      // FunctionScope
      //
      class FunctionScope : public LocalScope
      {
         GDCC_Core_CounterPreambleNoClone(
            GDCC::CC::FunctionScope, GDCC::CC::LocalScope);

      public:
         void allocAuto();

         virtual Core::CounterRef<BlockScope> createScope();

         Core::Array<Core::CounterRef<AST::Object>> params;
         Core::CounterRef<AST::Function>            fn;


         friend FunctionScope::Ref GlobalScope::createScope(
            AST::Attribute const &attr, AST::Function *fn);

      protected:
         FunctionScope(GlobalScope *parent, AST::Function *fn,
            Core::Array<Core::CounterRef<AST::Object>> &&params);
         virtual ~FunctionScope();
      };

      //
      // BlockScope
      //
      class BlockScope : public LocalScope
      {
         GDCC_Core_CounterPreambleNoClone(
            GDCC::CC::BlockScope, GDCC::CC::LocalScope);

      public:
         //
         // AllocAutoInfo
         //
         struct AllocAutoInfo
         {
            void setMax(AllocAutoInfo const &alloc);

            Core::FastU localArs = 0;
            Core::FastU localReg = 0;
         };

         AllocAutoInfo allocAuto(AllocAutoInfo const &base);

         virtual BlockScope::Ref createScope();

         FunctionScope::Ref fn;


         friend BlockScope::Ref FunctionScope::createScope();

      protected:
         BlockScope(LocalScope *parent, FunctionScope *fn);
         virtual ~BlockScope();
      };
   }
}

#endif//GDCC__CC__Scope_H__

