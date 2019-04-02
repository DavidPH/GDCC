//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

namespace GDCC::CC
{
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

         std::unordered_map<Core::FastU, Core::FastU> spaceMap;

         Core::FastU localArr = 0;
         Core::FastU localAut = 0;
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

      Core::CounterRef<SR::Object> getObject(SR::Attribute const &attr);

      Scope_Block &getScopeFirst();

      Core::CounterRef<SR::Space> getSpace(SR::Attribute const &attr);

      Scope_Function &fn;
      Scope_Global   &global;
      Core::String    label;

   protected:
      Scope_Local(Scope_Global &global, Scope_Function &fn);
      explicit Scope_Local(Scope_Local &parent);
      virtual ~Scope_Local();

      void allocAutoObj(AllocAutoInfo &alloc, SR::Object *obj);

      void allocAutoSpace(AllocAutoInfo &alloc, SR::Space *space);

      Core::FastU getSpaceValue(Core::String name) const;

      LookupTable<SR::Object> localObj;
      LookupTable<SR::Space>  localSpace;

      std::vector<std::unique_ptr<Scope_Block>> subScopes;
   };
}

#endif//GDCC__CC__Scope__Local_H__

