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

#ifndef GDCC__CC__Scope__Global_H__
#define GDCC__CC__Scope__Global_H__

#include "../../CC/Scope.hpp"

#include "../../Core/StringGen.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Attribute;
   }

   namespace IR
   {
      enum class Linkage;

      class Program;
   }

   namespace CC
   {
      class Scope_Function;

      //
      // Scope_Global
      //
      class Scope_Global : public Scope
      {
      public:
         explicit Scope_Global(Core::String label);
         virtual ~Scope_Global();

         void allocAuto();

         Scope_Function &createScope(AST::Attribute const &attr, AST::Function *fn);

         Core::String genGlyphObj(Core::String name, IR::Linkage linka);

         void genIR(IR::Program &prog);

         // Finds/creates a function, but does not add to lookup table.
         Core::CounterRef<AST::Function> getFunction(AST::Attribute const &attr);

         Core::CounterRef<AST::Object> getObject(AST::Attribute const &attr);

         Core::CounterRef<AST::Space> getSpace(AST::Attribute const &attr);

         Core::String const label;

      protected:
         LookupTable<AST::Function> globalFunc;
         LookupTable<AST::Object>   globalObj;
         LookupTable<AST::Space>    globalSpace;

         std::vector<std::unique_ptr<Scope_Function>> subScopes;

         Core::StringGen stringGen;
      };
   }
}

#endif//GDCC__CC__Scope__Global_H__

