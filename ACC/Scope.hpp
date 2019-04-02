//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS identifier scoping/lookup handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Scope_H__
#define GDCC__ACC__Scope_H__

#include "../ACC/PropDecl.hpp"

#include "../CC/Scope/Global.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
{
   //
   // Scope_Global
   //
   class Scope_Global : public CC::Scope_Global
   {
   public:
      explicit Scope_Global(Core::String label);

      void addCreateTrans(Core::String name, CreateTransDecl &&decl);
      void addPrint(Core::String name, PrintDecl &&decl);

      CreateTransDecl *findCreateTrans(Core::String name);
      PrintDecl *findPrint(Core::String name);

   protected:
      std::unordered_map<Core::String, CreateTransDecl> globalCreateTrans;
      std::unordered_map<Core::String, PrintDecl> globalPrint;
   };
}

#endif//GDCC__ACC__Scope_H__

