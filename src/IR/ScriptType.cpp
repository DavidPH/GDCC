//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation function script type handling.
//
//-----------------------------------------------------------------------------

#include "IR/ScriptType.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"

#include "Core/String.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // operator OArchive << ScriptType
      //
      OArchive &operator << (OArchive &out, ScriptType in)
      {
         switch(in)
         {
         #define GDCC_IR_ScriptTypeList(name) \
            case ScriptType::name: return out << Core::STR_##name;
         #include "IR/ScriptTypeList.hpp"
         }

         std::cerr << "invalid enum GDCC::IR::ScriptType\n";
         throw EXIT_FAILURE;
      }

      //
      // operator std::ostream << ScriptType
      //
      std::ostream &operator << (std::ostream &out, ScriptType in)
      {
         switch(in)
         {
         #define GDCC_IR_ScriptTypeList(name) \
            case ScriptType::name: return out << #name;
         #include "IR/ScriptTypeList.hpp"
         }

         std::cerr << "invalid enum GDCC::IR::ScriptType\n";
         throw EXIT_FAILURE;
      }

      //
      // operator IArchive >> ScriptType
      //
      IArchive &operator >> (IArchive &in, ScriptType &out)
      {
         switch(GetIR<Core::String>(in))
         {
         #define GDCC_IR_ScriptTypeList(name) \
            case Core::STR_##name: out = ScriptType::name; return in;
         #include "IR/ScriptTypeList.hpp"

         default:
            std::cerr << "invalid GDCC::IR::ScriptType\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

