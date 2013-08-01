//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly parsing utilities.
//
//-----------------------------------------------------------------------------

#include "Parse.hpp"

#include "GDCC/IR/Addr.hpp"
#include "GDCC/IR/Linkage.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // ParseAddrBase
   //
   GDCC::IR::AddrBase ParseAddrBase(GDCC::Token const &tok)
   {
      switch(static_cast<GDCC::StringIndex>(tok.str))
      {
         #define GDCC_IR_AddrList(name) \
            case GDCC::STR_##name: return GDCC::IR::AddrBase::name;
         #include "GDCC/IR/AddrList.hpp"

      default:
         std::cerr << "ERROR: " << tok.pos << ": bad AddrBase: '" << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }

   //
   // ParseLinkage
   //
   GDCC::IR::Linkage ParseLinkage(GDCC::Token const &tok)
   {
      switch(static_cast<GDCC::StringIndex>(tok.str))
      {
      case GDCC::STR_ExtACS: return GDCC::IR::Linkage::ExtACS;
      case GDCC::STR_ExtASM: return GDCC::IR::Linkage::ExtASM;
      case GDCC::STR_ExtC:   return GDCC::IR::Linkage::ExtC;
      case GDCC::STR_ExtCXX: return GDCC::IR::Linkage::ExtCXX;
      case GDCC::STR_ExtDS:  return GDCC::IR::Linkage::ExtDS;
      case GDCC::STR_IntC:   return GDCC::IR::Linkage::IntC;
      case GDCC::STR_IntCXX: return GDCC::IR::Linkage::IntCXX;

      default:
         std::cerr << "ERROR: " << tok.pos << ": bad Linkage: '" << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

