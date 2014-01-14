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
         case ScriptType::None:       return out << Core::STR_None;
         case ScriptType::Death:      return out << Core::STR_Death;
         case ScriptType::Disconnect: return out << Core::STR_Disconnect;
         case ScriptType::Enter:      return out << Core::STR_Enter;
         case ScriptType::Lightning:  return out << Core::STR_Lightning;
         case ScriptType::Open:       return out << Core::STR_Open;
         case ScriptType::Respawn:    return out << Core::STR_Respawn;
         case ScriptType::Return:     return out << Core::STR_Return;
         case ScriptType::Unloading:  return out << Core::STR_Unloading;
         }

         std::cerr << "invalid enum ScriptType\n";
         throw EXIT_FAILURE;
      }

      //
      // operator std::ostream << ScriptType
      //
      std::ostream &operator << (std::ostream &out, ScriptType in)
      {
         switch(in)
         {
         case ScriptType::None:       return out << "None";
         case ScriptType::Death:      return out << "Death";
         case ScriptType::Disconnect: return out << "Disconnect";
         case ScriptType::Enter:      return out << "Enter";
         case ScriptType::Lightning:  return out << "Lightning";
         case ScriptType::Open:       return out << "Open";
         case ScriptType::Respawn:    return out << "Respawn";
         case ScriptType::Return:     return out << "Return";
         case ScriptType::Unloading:  return out << "Unloading";
         }

         std::cerr << "invalid enum ScriptType\n";
         throw EXIT_FAILURE;
      }

      //
      // operator IArchive >> ScriptType
      //
      IArchive &operator >> (IArchive &in, ScriptType &out)
      {
         switch(GetIR<Core::String>(in))
         {
         case Core::STR_None:       out = ScriptType::None;       return in;
         case Core::STR_Death:      out = ScriptType::Death;      return in;
         case Core::STR_Disconnect: out = ScriptType::Disconnect; return in;
         case Core::STR_Enter:      out = ScriptType::Enter;      return in;
         case Core::STR_Lightning:  out = ScriptType::Lightning;  return in;
         case Core::STR_Open:       out = ScriptType::Open;       return in;
         case Core::STR_Respawn:    out = ScriptType::Respawn;    return in;
         case Core::STR_Return:     out = ScriptType::Return;     return in;
         case Core::STR_Unloading:  out = ScriptType::Unloading;  return in;

         default:
            std::cerr << "invalid ScriptType\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

