//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C file/global scope handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Scope__Global_H__
#define GDCC__CC__Scope__Global_H__

#include "../../CC/Scope.hpp"

#include "../../Core/StringGen.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Scope_Global
   //
   class Scope_Global : public Scope
   {
   public:
      explicit Scope_Global(Core::String label);
      virtual ~Scope_Global();

      void allocAuto();

      Scope_Function &createScope(SR::Attribute const &attr, SR::Function *fn);

      Core::String genGlyphObj(Core::String name, IR::Linkage linka);

      void genIR(IR::Program &prog);

      // Finds/creates a function, but does not add to lookup table.
      Core::CounterRef<SR::Function> getFunction(SR::Attribute const &attr);

      Core::CounterRef<SR::Object> getObject(SR::Attribute const &attr);

      Core::CounterRef<SR::Space> getSpace(SR::Attribute const &attr);

      Core::String const label;

   protected:
      LookupTable<SR::Function> globalFunc;
      LookupTable<SR::Object>   globalObj;
      LookupTable<SR::Space>    globalSpace;

      std::vector<std::unique_ptr<Scope_Function>> subScopes;

      Core::StringGen stringGen;
   };
}

#endif//GDCC__CC__Scope__Global_H__

