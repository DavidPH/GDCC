//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C function scope handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Scope__Function_H__
#define GDCC__CC__Scope__Function_H__

#include "../../CC/Scope/Local.hpp"

#include "../../Core/Array.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Exp;
   }

   namespace CC
   {
      //
      // Scope_Function
      //
      class Scope_Function : public Scope_Local
      {
      public:
         Scope_Function(Scope_Global &parent, AST::Function *fn,
            Core::Array<Core::CounterRef<AST::Object>> &&params);
         virtual ~Scope_Function();

         void allocAuto();

         Core::String getLabel(Core::String name, bool define = false);
         virtual Core::String getLabelBreak();
         virtual Core::String getLabelCase(Core::Integ const &n, bool define);
         virtual Core::String getLabelContinue();
         virtual Core::String getLabelDefault(bool define);
         Core::String getLabelLJR();

         Core::Array<Core::CounterRef<AST::Object>> params;
         Core::CounterRef<AST::Function>            fn;
         Core::String                               labelLJR;
         Core::CounterPtr<AST::Exp const>           nameObj;
      };
   }
}

#endif//GDCC__CC__Scope__Function_H__

