//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS property-based declarations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__PropDecl_H__
#define GDCC__ACC__PropDecl_H__

#include "../AST/Exp.hpp"

#include "../Core/Token.hpp"

#include <map>
#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // CreateTransPropName
      //
      class CreateTransPropName
      {
      public:
         bool operator < (CreateTransPropName const &r) const;

         Core::Token prefix;
         Core::FastU argc[4];
      };

      //
      // CreateTransProp
      //
      class CreateTransProp
      {
      public:
         AST::Exp::CPtr prop;
      };

      //
      // CreateTransDecl
      //
      class CreateTransDecl
      {
      public:
         std::map<CreateTransPropName, CreateTransProp> props;

         AST::Exp::CPtr propBegin;
         AST::Exp::CPtr propEnd;
      };

      //
      // PrintProp
      //
      class PrintProp
      {
      public:
         bool isMultiArg() const;

         // Generic print function.
         AST::Exp::CPtr prop;

         // Array print functions.
         AST::Exp::CPtr propGlobalArray, propGlobalRange;
         AST::Exp::CPtr propHubArray,    propHubRange;
         AST::Exp::CPtr propLocalArray,  propLocalRange;
         AST::Exp::CPtr propModuleArray, propModuleRange;

      private:
         bool isMultiArg(AST::Exp const *prop) const;
      };

      //
      // PrintDecl
      //
      class PrintDecl
      {
      public:
         std::unordered_map<Core::String, PrintProp> props;

         AST::Exp::CPtr propBegin;
         AST::Exp::CPtr propChar;
         AST::Exp::CPtr propEnd;
         AST::Exp::CPtr propMore;
         AST::Exp::CPtr propOpt;
         AST::Exp::CPtr propStr;
      };
   }
}

#endif//GDCC__ACC__PropDecl_H__

