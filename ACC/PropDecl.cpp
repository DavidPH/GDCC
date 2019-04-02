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

#include "ACC/PropDecl.hpp"

#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // CreateTransPropName::operator < CreateTransPropName
   //
   bool CreateTransPropName::operator < (CreateTransPropName const &r) const
   {
      if(prefix.tok != r.prefix.tok) return prefix.tok < r.prefix.tok;
      if(prefix.str != r.prefix.str) return prefix.str < r.prefix.str;

      return std::lexicographical_compare(std::begin(argc), std::end(argc),
         std::begin(r.argc), std::end(r.argc));

      return false;
   }

   //
   // PrintProp::isMultiArg
   //
   bool PrintProp::isMultiArg() const
   {
      return isMultiArg(prop) ||
         isMultiArg(propGlobalArray) || isMultiArg(propGlobalRange) ||
         isMultiArg(propHubArray)    || isMultiArg(propHubRange)    ||
         isMultiArg(propLocalArray)  || isMultiArg(propLocalRange)  ||
         isMultiArg(propModuleArray) || isMultiArg(propModuleRange);
   }

   //
   // PrintProp::isMultiArg
   //
   bool PrintProp::isMultiArg(SR::Exp const *exp) const
   {
      if(!exp) return false;

      SR::Type::CRef type = exp->getType();
      if(!type->isTypePointer()) return false;

      type = type->getBaseType();
      if(!type->isCTypeFunction()) return false;

      return type->getParameters()->size() > 1;
   }
}

// EOF

