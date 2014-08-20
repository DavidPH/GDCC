//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C local scope handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Scope__Local_H__
#define GDCC__CC__Scope__Local_H__

#include "../../CC/Scope.hpp"

#include "../../Core/Number.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Attribute;
      class Function;
   }

   namespace IR
   {
      enum class Linkage;

      class Program;
   }

   namespace CC
   {
      class Scope_Block;
      class Scope_Case;
      class Scope_Function;
      class Scope_Global;

      //
      // Scope_Local
      //
      // A non-global scope. In C itself, a function or block scope.
      //
      class Scope_Local : public Scope
      {
      public:
         //
         // AllocAutoInfo
         //
         class AllocAutoInfo
         {
         public:
            void setMax(AllocAutoInfo const &alloc);

            Core::FastU localArs = 0;
            Core::FastU localReg = 0;
         };


         Scope_Block &createScopeBlock();
         Scope_Block &createScopeLoop();
         Scope_Case  &createScopeCase();

         Core::String genGlyphObj(Core::String name, IR::Linkage linka);

         void genIR(IR::Program &prog);

         Core::String getLabel(Core::String name, bool define = false);
         virtual Core::String getLabelBreak();
         virtual Core::String getLabelCase(Core::Integ const &n, bool define = false);
         virtual Core::String getLabelContinue();
         virtual Core::String getLabelDefault(bool define = false);

         Core::CounterRef<AST::Object> getObject(AST::Attribute const &attr);

         Scope_Function &fn;
         Scope_Global   &global;
         Core::String    label;

      protected:
         Scope_Local(Scope_Global &global, Scope_Function &fn);
         explicit Scope_Local(Scope_Local &parent);
         virtual ~Scope_Local();

         void allocAutoObj(AllocAutoInfo &alloc, AST::Object *obj);

         LookupTable<AST::Object> localObj;

         std::vector<std::unique_ptr<Scope_Block>> subScopes;
      };
   }
}

#endif//GDCC__CC__Scope__Local_H__

