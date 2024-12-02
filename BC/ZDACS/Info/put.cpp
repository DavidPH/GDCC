//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code output.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "IR/Function.hpp"
#include "IR/Program.hpp"

#include "Target/CallType.hpp"
#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::put
   //
   void Info::put()
   {
      if(Target::FormatCur == Target::Format::ACS0)
         putACS0();
      else
         putACSE();
   }

   //
   // Info::putACS0
   //
   void Info::putACS0()
   {
      putData("ACS\0", 4);
      putWord(16 + numChunkCODE);

      // <shamelessplug>
      putData("GDCC::BC", 8);
      // </shamelessplug>

      putACS0_Code();
      putACS0_Scripts();
      putACS0_Strings();
   }

   //
   // Info::putACS0_Code
   //
   void Info::putACS0_Code()
   {
      // Put statements.
      for(auto &itr : prog->rangeFunction())
         putFunc(itr);

      // Put initializers.
      putIniti();
   }

   //
   // Info::putACS0_Scripts
   //
   void Info::putACS0_Scripts()
   {
      // Write script count.
      putWord(numChunkSPTR);

      // Write script headers.
      for(auto const &itr : prog->rangeFunction())
      {
         if(!IsScript(itr.ctype))
            continue;

         if(!itr.defin) continue;

         // Write entry.
         putWord(GetScriptValue(itr) + GetScriptType(itr) * 1000);
         putWord(getWord(resolveGlyph(itr.label)));
         putWord(std::min(itr.param, GetParamMax(itr.ctype)));
      }

      // Initializer script.
      if(codeInit)
      {
         Core::FastU stype, param;
         if(isInitScriptEvent())
            stype = 16, param = 1;
         else
            stype = 1, param = 0;

         putWord(InitScriptNumber + stype * 1000);
         putWord(codeInit);
         putWord(param);

         if(Target::EngineCur == Target::Engine::Zandronum)
         {
            putWord(InitScriptNumber + 1 + stype * 1000);
            putWord(codeInit);
            putWord(param);
         }
      }
   }

   //
   // Info::putACS0_Strings
   //
   void Info::putACS0_Strings()
   {
      // Build string table.
      Core::Array<Core::String> strs{numChunkSTRL};

      for(auto &s : strs) s = Core::STR_;

      for(auto const &itr : prog->rangeStrEnt()) if(itr.defin)
         strs[itr.valueInt] = itr.valueStr;

      // Write string count.
      putWord(numChunkSTRL);

      // Write string offsets.
      std::size_t off = putPos + numChunkSTRL * 4;
      for(auto const &s : strs)
      {
         putWord(off);
         off += lenString(s);
      }

      // Write strings.
      for(auto const &s : strs)
         putString(s);
   }

   //
   // Info::putACSE
   //
   void Info::putACSE()
   {
      // Put header.
      if(UseFakeACS0)
      {
         putData("ACS\0", 4);
         putWord(24 + lenChunk());
      }
      else
      {
         putData("ACSE", 4);
         putWord(16);
      }

      // <shamelessplug>
      putData("GDCC::BC", 8);
      // </shamelessplug>

      // Put chunks.
      putChunk();

      // Put (real) header.
      if(UseFakeACS0)
      {
         putWord(16);
         putData("ACSE", 4);
         putACS0_Scripts();
         putWord(0);
      }
   }

   //
   // Info::putByte
   //
   void Info::putByte(Core::FastU i)
   {
      out->put(i & 0xFF);

      putPos += 1;
   }

   //
   // Info::putCode
   //
   void Info::putCode(Code code)
   {
      putWord(static_cast<Core::FastU>(code));
   }

   //
   // Info::putCode
   //
   void Info::putCode(Code code, Core::FastU arg0)
   {
      putCode(code);
      putWord(arg0);
   }

   //
   // Info::putCode
   //
   void Info::putCode(Code code, Core::FastU arg0, Core::FastU arg1)
   {
      putCode(code);
      putWord(arg0);
      putWord(arg1);
   }

   //
   // Info::putFunc
   //
   void Info::putFunc()
   {
      // Put function preamble.
      if(func->defin && func->allocAut)
      {
         putCode(Code::Push_Lit,    func->allocAut);
         putCode(Code::Call_Lit,    getWord(resolveGlyph("___GDCC__Plsa")));
         putCode(Code::Drop_LocReg, getStkPtrIdx());
      }

      Core::FastU paramMax = GetParamMax(func->ctype);
      if(func->defin && func->param > paramMax)
      {
         for(Core::FastU i = paramMax; i != func->param; ++i)
         {
            putCode(Code::Push_Lit,    ~(i - paramMax));
            putCode(Code::Push_GblArr, StaArray);
            putCode(Code::Drop_LocReg, i);
         }
      }

      InfoBase::putFunc();
   }

   //
   // Info::putHWord
   //
   void Info::putHWord(Core::FastU i)
   {
      out->put((i >> 0) & 0xFF);
      out->put((i >> 8) & 0xFF);

      putPos += 2;
   }

   //
   // Info::putString
   //
   void Info::putString(Core::String s)
   {
      for(auto i = s.begin(), e = s.end(); i != e; ++i) switch(*i)
      {
      case '\0':
         if('0' <= i[1] && i[1] <= '7')
            putData("\\000", 4);
         else
            putData("\\0", 2);
         break;

      case '\\':
         putData("\\\\", 2);
         break;

      default:
         putByte(*i);
         break;
      }

      putByte('\0');
   }

   //
   // Info::putString
   //
   // Handles STRE "encrypted" strings.
   //
   void Info::putString(Core::String s, Core::FastU key)
   {
      std::size_t idx = 0;

      //
      // putChar
      //
      auto putChar = [&](unsigned char c)
      {
         putByte(c ^ (idx++ / 2 + key));
      };

      for(auto i = s.begin(), e = s.end(); i != e; ++i) switch(*i)
      {
      case '\0':
         putChar('\\');
         putChar('0');

         if('0' <= i[1] && i[1] <= '7')
         {
            putChar('0');
            putChar('0');
         }

         break;

      case '\\':
         putChar('\\');
         putChar('\\');
         break;

      default:
         putChar(*i);
         break;
      }

      putChar('\0');
   }

   //
   // Info::putWord
   //
   void Info::putWord(Core::FastU i)
   {
      out->put((i >>  0) & 0xFF);
      out->put((i >>  8) & 0xFF);
      out->put((i >> 16) & 0xFF);
      out->put((i >> 24) & 0xFF);

      putPos += 4;
   }
}

// EOF

