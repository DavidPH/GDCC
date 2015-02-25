//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS print declarations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Print_H__
#define GDCC__ACC__Print_H__

#include "../AST/Exp.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // PrintProp
      //
      class PrintProp
      {
      public:
         // Generic print function.
         AST::Exp::CPtr prop;

         // Array print functions.
         AST::Exp::CPtr propGlobalArray, propGlobalRange;
         AST::Exp::CPtr propLocalArray,  propLocalRange;
         AST::Exp::CPtr propMapArray,    propMapRange;
         AST::Exp::CPtr propWorldArray,  propWorldRange;
      };

      //
      // PrintDecl
      //
      class PrintDecl
      {
      public:
         std::unordered_map<Core::String, PrintProp> props;

         AST::Exp::CPtr propBegin;
         AST::Exp::CPtr propEnd;
         AST::Exp::CPtr propMore;
         AST::Exp::CPtr propOpt;
      };
   }
}

#endif//GDCC__ACC__Print_H__

