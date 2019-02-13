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

#include "IR/Exception.hpp"
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
   Core::String Info::getFuncName(IR::Code code, Core::FastU n0, Core::FastU n1)
   {
      char        buf[sizeof("___GDCC__cccccccc_tttt_nn_nn")];
      std::size_t len;

      char const *base;
      char        type[5] = {code.type[0], code.type[1], code.type[2], code.type[3]};
      int         size[2];

      // Convert code base to string.
      switch(code.base)
      {
         #define GDCC_IR_CodeList(c) case IR::CodeBase::c: base = #c; break;
         #include "IR/CodeList.hpp"

      default:
         IR::ErrorCode(stmnt, "bad getFuncName base");
      }

      // Convert size to int.
      if(n0 > INT_MAX || n1 > INT_MAX)
         IR::ErrorCode(stmnt, "bad getFuncName size");

      size[0] = static_cast<int>(n0);
      size[1] = static_cast<int>(n1);

      // Format function name.
      len = std::snprintf(buf, sizeof(buf),
         type[0] ?
            size[1] ? "___GDCC__%s_%s_%i_%i" :
            size[0] ? "___GDCC__%s_%s%i" :
                      "___GDCC__%s_%s" :
            size[1] ? "___GDCC__%s%s_%i_%i" :
            size[0] ? "___GDCC__%s%s%i" :
                      "___GDCC__%s%s",
         base, type, size[0], size[1]);

      if(len >= sizeof(buf))
         IR::ErrorCode(stmnt, "bad getFuncName");

      return {buf, len};
   }
}

// EOF

