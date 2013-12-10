//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation function handling.
//
//-----------------------------------------------------------------------------

#include "Function.hpp"

#include "Linkage.hpp"
#include "IArchive.hpp"
#include "OArchive.hpp"
#include "Program.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static std::unordered_map<GDCC::String, GDCC::IR::Function> FuncMap;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Function constructor
      //
      Function::Function(String glyph_) :
         block   {},
         ctype   {CallType::None},
         glyph   {glyph_},
         label   {STRNULL},
         linka   {Linkage::None},
         localArs{0},
         localReg{0},
         param   {0},
         retrn   {0},
         stype   {ScriptType::None},
         valueInt{0},
         valueStr{STRNULL},

         alloc   {false},
         defin   {false},
         sflagNet{false},
         sflagClS{false}
      {
      }

      //
      // Function::allocValue
      //
      void Function::allocValue(Program &prog, CallType const *ctypeVec, std::size_t ctypeLen)
      {
         for(;; ++valueInt)
         {
            for(auto const &func : prog.rangeFunction())
            {
               if(&func != this && !func.alloc && func.valueInt == valueInt)
               {
                  for(auto ctypeItr : MakeRange(ctypeVec, ctypeVec + ctypeLen))
                     if(func.ctype == ctypeItr) goto nextValue;
               }
            }

            break;

         nextValue:;
         }

         alloc = false;
      }

      //
      // operator OArchive << CallType
      //
      OArchive &operator << (OArchive &out, CallType in)
      {
         switch(in)
         {
         case CallType::None:    return out << STR_None;
         case CallType::Action:  return out << STR_Action;
         case CallType::AsmFunc: return out << STR_AsmFunc;
         case CallType::LangACS: return out << STR_LangACS;
         case CallType::LangASM: return out << STR_LangASM;
         case CallType::LangAXX: return out << STR_LangAXX;
         case CallType::LangC:   return out << STR_LangC;
         case CallType::LangCXX: return out << STR_LangCXX;
         case CallType::LangDS:  return out << STR_LangDS;
         case CallType::Native:  return out << STR_Native;
         case CallType::Script:  return out << STR_Script;
         case CallType::ScriptI: return out << STR_ScriptI;
         case CallType::ScriptS: return out << STR_ScriptS;
         case CallType::Special: return out << STR_Special;
         }
      }

      //
      // operator OArchive << Function
      //
      OArchive &operator << (OArchive &out, Function const &in)
      {
         return out << in.block << in.ctype << in.label << in.linka << in.linka
            << in.localArs << in.localReg << in.param << in.retrn << in.stype
            << in.valueInt << in.valueStr << in.alloc << in.defin << in.sflagNet
            << in.sflagClS;
      }

      //
      // operator OArchive << ScriptType
      //
      OArchive &operator << (OArchive &out, ScriptType in)
      {
         switch(in)
         {
         case ScriptType::None:       return out << STR_None;
         case ScriptType::Death:      return out << STR_Death;
         case ScriptType::Disconnect: return out << STR_Disconnect;
         case ScriptType::Enter:      return out << STR_Enter;
         case ScriptType::Lightning:  return out << STR_Lightning;
         case ScriptType::Open:       return out << STR_Open;
         case ScriptType::Respawn:    return out << STR_Respawn;
         case ScriptType::Return:     return out << STR_Return;
         case ScriptType::Unloading:  return out << STR_Unloading;
         }
      }

      //
      // operator std::ostream << CallType
      //
      std::ostream &operator << (std::ostream &out, CallType in)
      {
         switch(in)
         {
         case CallType::None:    return out << "None";
         case CallType::Action:  return out << "Action";
         case CallType::AsmFunc: return out << "AsmFunc";
         case CallType::LangACS: return out << "LangACS";
         case CallType::LangASM: return out << "LangASM";
         case CallType::LangAXX: return out << "LangAXX";
         case CallType::LangC:   return out << "LangC";
         case CallType::LangCXX: return out << "LangCXX";
         case CallType::LangDS:  return out << "LangDS";
         case CallType::Native:  return out << "Native";
         case CallType::Script:  return out << "Script";
         case CallType::ScriptI: return out << "ScriptI";
         case CallType::ScriptS: return out << "ScriptS";
         case CallType::Special: return out << "Special";
         }
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
      }

      //
      // operator IArchive >> CallType
      //
      IArchive &operator >> (IArchive &in, CallType &out)
      {
         switch(GetIR<StringIndex>(in))
         {
         case STR_None:    out = CallType::None;    return in;
         case STR_Action:  out = CallType::Action;  return in;
         case STR_AsmFunc: out = CallType::AsmFunc; return in;
         case STR_LangACS: out = CallType::LangACS; return in;
         case STR_LangASM: out = CallType::LangASM; return in;
         case STR_LangAXX: out = CallType::LangAXX; return in;
         case STR_LangC:   out = CallType::LangC;   return in;
         case STR_LangCXX: out = CallType::LangCXX; return in;
         case STR_LangDS:  out = CallType::LangDS;  return in;
         case STR_Native:  out = CallType::Native;  return in;
         case STR_Script:  out = CallType::Script;  return in;
         case STR_ScriptI: out = CallType::ScriptI; return in;
         case STR_ScriptS: out = CallType::ScriptS; return in;
         case STR_Special: out = CallType::Special; return in;

         default:
            std::cerr << "invalid CallType\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // operator IArchive >> Function
      //
      IArchive &operator >> (IArchive &in, Function &out)
      {
         in >> out.block >> out.ctype >> out.label >> out.linka >> out.linka
            >> out.localArs >> out.localReg >> out.param >> out.retrn >> out.stype
            >> out.valueInt >> out.valueStr;

         out.alloc    = GetIR<bool>(in);
         out.defin    = GetIR<bool>(in);
         out.sflagNet = GetIR<bool>(in);
         out.sflagClS = GetIR<bool>(in);

         return in;
      }

      //
      // operator IArchive >> ScriptType
      //
      IArchive &operator >> (IArchive &in, ScriptType &out)
      {
         switch(GetIR<StringIndex>(in))
         {
         case STR_None:       out = ScriptType::None;       return in;
         case STR_Death:      out = ScriptType::Death;      return in;
         case STR_Disconnect: out = ScriptType::Disconnect; return in;
         case STR_Enter:      out = ScriptType::Enter;      return in;
         case STR_Lightning:  out = ScriptType::Lightning;  return in;
         case STR_Open:       out = ScriptType::Open;       return in;
         case STR_Respawn:    out = ScriptType::Respawn;    return in;
         case STR_Return:     out = ScriptType::Return;     return in;
         case STR_Unloading:  out = ScriptType::Unloading;  return in;

         default:
            std::cerr << "invalid ScriptType\n";
            throw EXIT_FAILURE;
         }
      }

      //
      // FunctionRange
      //
      GDCC::Range<std::unordered_map<String, Function>::iterator> FunctionRange()
      {
         return MakeRange(FuncMap.begin(), FuncMap.end());
      }
   }
}

// EOF

