//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Function creation functions.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Linkage.hpp"
#include "IR/Program.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::addFunc
   //
   void Info::addFunc(Core::String name, Core::FastU retrn, Core::FastU param)
   {
      if(!prog->findFunction(name))
      {
         auto &newFn = prog->getFunction(name);

         newFn.ctype = IR::CallType::StkCall;
         newFn.linka = IR::Linkage::ExtC;
         newFn.param = param;
         newFn.retrn = retrn;

         newFn.alloc = true;

         prog->getGlyphData(name).type = IR::Type_Funct(newFn.ctype);

         // Signal iterator to start over.
         throw ResetFunc();
      }
   }

   //
   // Info::getFuncDefn
   //
   IR::Function *Info::getFuncDefn(Core::String name,
      Core::FastU retrn, Core::FastU param, Core::FastU localReg,
      char const *file)
   {
      try {addFunc(name, retrn, param);} catch(ResetFunc const &) {}

      IR::Function *newFunc = &prog->getFunction(name);

      if(newFunc->defin)
         return nullptr;

      newFunc->defin    = true;
      newFunc->label    = name + "$label";
      newFunc->localReg = localReg;

      newFunc->block.setOrigin({file, 0});

      return newFunc;
   }

   //
   // Info::getFuncName
   //
   Core::String Info::getFuncName(IR::Code code, Core::FastU n)
   {
      char        buf[sizeof("___GDCC__Code_XX_Wxx")];
      std::size_t len;

      char const *name;
      int         size;

      // Convert code to string.
      switch(code)
      {
         #define GDCC_IR_CodeList(c) case IR::Code::c: name = #c; break;
         #include "IR/CodeList.hpp"

      default:
         Core::Error(stmnt->pos, "bad getFuncName code");
      }

      // Convert size to int.
      if(n > INT_MAX)
         Core::Error(stmnt->pos, "bad getFuncName size");

      size = static_cast<int>(n);

      // Format function name.
      len = std::snprintf(buf, sizeof(buf), "___GDCC__%s%i", name, size);

      if(len >= sizeof(buf))
         Core::Error(stmnt->pos, "bad getFuncName");

      return {buf, len};
   }
}

// EOF

