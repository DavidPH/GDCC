//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
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

#include "../ACC/Types.hpp"

#include "../Core/Token.hpp"

#include "../SR/Exp.hpp"

#include <map>
#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
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
      SR::Exp::CPtr prop;
   };

   //
   // CreateTransDecl
   //
   class CreateTransDecl
   {
   public:
      std::map<CreateTransPropName, CreateTransProp> props;

      SR::Exp::CPtr propBegin;
      SR::Exp::CPtr propEnd;
   };

   //
   // PrintProp
   //
   class PrintProp
   {
   public:
      bool isMultiArg() const;

      // Generic print function.
      SR::Exp::CPtr prop;

      // Array print functions.
      SR::Exp::CPtr propGlobalArray, propGlobalRange;
      SR::Exp::CPtr propHubArray,    propHubRange;
      SR::Exp::CPtr propLocalArray,  propLocalRange;
      SR::Exp::CPtr propModuleArray, propModuleRange;

   private:
      bool isMultiArg(SR::Exp const *prop) const;
   };

   //
   // PrintDecl
   //
   class PrintDecl
   {
   public:
      std::unordered_map<Core::String, PrintProp> props;

      SR::Exp::CPtr propBegin;
      SR::Exp::CPtr propChar;
      SR::Exp::CPtr propEnd;
      SR::Exp::CPtr propMore;
      SR::Exp::CPtr propOpt;
      SR::Exp::CPtr propStr;
   };
}

#endif//GDCC__ACC__PropDecl_H__

