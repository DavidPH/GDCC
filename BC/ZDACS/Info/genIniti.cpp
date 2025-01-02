//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS initializer generator.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Module.hpp"

#include "Core/Option.hpp"

#include "IR/Program.hpp"

#include "Target/Info.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::genIniti
   //
   void Info::genIniti()
   {
      bool isGblArr = isInitiGblArr();
      bool isHubArr = isInitiHubArr();

      if(!isGblArr && !isHubArr)
         return;

      // If no name set, generate one.
      // Even if not using a named init script, this is needed for labels.
      if(!InitScriptName)
      {
         std::ostringstream oss;
         if(char const *s = Core::GetOptionOutput())
            oss << s;
         oss << "$init";

         auto const &s = oss.str();
         InitScriptName = {s.data(), s.size()};
      }

      Core::String label = InitScriptName;
      Core::String labelEnd = label + "$end";

      // Add script table entries.
      {
         Core::FastU entry = getCodePos();
         Core::FastU value = InitScriptNumber;

         Core::FastU stype, param;
         if(isInitScriptEvent())
            stype = 16, param = 1;
         else
            stype = 1, param = 0;

         // Extra information for named init script.
         if(InitScriptNamed)
         {
            value = ~module->chunkSNAM.elem.size();

            module->chunkSNAM[~value] = ElemSNAM{InitScriptName};

            if(Target::EngineCur == Target::Engine::Zandronum)
               module->chunkSNAM[~--value] = ElemSNAM{InitScriptName + "$cs"};
         }

         module->chunkSPTR.add(ElemArg{entry}, value, stype, param);

         if(Target::EngineCur == Target::Engine::Zandronum)
         {
            module->chunkSFLG.add(value + InitScriptNamed, 0x0002);
            module->chunkSPTR.add(ElemArg{entry}, value + 1, stype, param);
         }
      }

      if(isInitScriptEvent())
      {
         Core::String labelEvent = label + "$event";

         // Check event type.
         genCode(Code::Push_LocReg, 0);
         genCode(Code::Jcnd_Lit, 16, labelEvent);
         genCode(Code::Drop_Nul);
         genCode(Code::Rscr);
         backGlyphLabel(labelEvent);
      }

      // Initialize world arrays.
      if(isHubArr)
      {
         // Check if already initialized.
         genCode(Code::Push_Lit,    getInitHubIndex());
         genCode(Code::Push_HubArr, getInitHubArray());
         genCode(Code::Jcnd_Tru,    labelEnd);

         // Write instructions needed for initializers.
         for(auto &itr : prog->rangeSpaceHubArs())
            genInitiSpace(itr, Code::Drop_HubArr);
      }

      // Initialize global arrays.
      if(isGblArr)
      {
         // Check if already initialized.
         genCode(Code::Push_Lit,    getInitGblIndex());
         genCode(Code::Push_GblArr, getInitGblArray());
         genCode(Code::Jcnd_Tru,    labelEnd);

         // Write instructions needed for initializers.
         for(auto &itr : prog->rangeSpaceGblArs())
            genInitiSpace(itr, Code::Drop_GblArr);
         genInitiSpace(prog->getSpaceSta(), Code::Drop_GblArr);
      }

      // Set initialized flag(s) after a delay to allow other
      // initialization scripts to run on first tic.
      if(InitDelay)
         genCode(Code::Wait_Lit, 1);

      // Set initialized flag(s).
      // Using IncU saves an instruction (wow) and leaves a record of how many
      // init scripts ran for possible debugging.
      if(isHubArr)
      {
         genCode(Code::Push_Lit,    getInitHubIndex());
         genCode(Code::IncU_HubArr, getInitHubArray());
      }

      if(isGblArr)
      {
         genCode(Code::Push_Lit,    getInitGblIndex());
         genCode(Code::IncU_GblArr, getInitGblArray());
      }

      // Terminate script.
      backGlyphLabel(labelEnd);
      genCode(Code::Rscr);
   }

   //
   // Info::genInitiSpace
   //
   void Info::genInitiSpace(IR::Space &space_, Code code)
   {
      auto const &ini = init[&space_];

      // Write instructions needed for initializers.
      for(auto const &val : ini.vals) switch(val.second.tag)
      {
      case InitTag::Empty: break;

      case InitTag::Fixed:
         // Skip zeroes.
         if(!val.second.val) break;

         genCode(Code::Push_Lit, val.first);
         genCode(Code::Push_Lit, val.second.val);
         genCode(code, space_.value);
         break;

      case InitTag::Funct:
         genCode(Code::Push_Lit, val.first);
         genStmntPushFunct(val.second.val);
         genCode(code, space_.value);
         break;

      case InitTag::StrEn:
         genCode(Code::Push_Lit, val.first);
         genStmntPushStrEn(val.second.val);
         genCode(code, space_.value);
         break;
      }
   }
}

// EOF

