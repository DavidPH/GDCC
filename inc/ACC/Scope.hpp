//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
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

#include "../CC/Scope/Global.hpp"

#include "../ACC/PropDecl.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace ACC
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
}

#endif//GDCC__ACC__Scope_H__

