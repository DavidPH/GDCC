//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common GDCC option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Option_H__
#define GDCC__Core__Option_H__

#include "../Option/CStr.hpp"
#include "../Option/CStrV.hpp"
#include "../Option/Function.hpp"
#include "../Option/Program.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // SystemSourceOption
      //
      class SystemSourceOption : public Option::CStrV
      {
      public:
         SystemSourceOption(Option::Program *program, Info const &optInfo);

      protected:
         virtual std::size_t v_process(Option::Args const &args);
      };

      //
      // OptionList
      //
      // Stores an option list and basic options.
      //
      class OptionList
      {
      public:
         OptionList();

         Option::Program list;

         Option::CStrV args;

         Option::Function optHelp;
         Option::Function optHelpADoc;
         Option::Function optHelpLong;
         Option::CStr     optOutput;
         Option::Function optVersion;

         Option::CStr       optLibPath;
         SystemSourceOption optSysSource;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      Option::CStrV &GetOptionArgs();

      std::string GetOptionLibPath();

      Option::Program &GetOptionList();

      char const *GetOptionOutput();

      OptionList &GetOptions();

      void ProcessOptions(OptionList &opts, int argc, char const *const *argv,
         bool needOutput = true);
   }
}

#endif//GDCC__Core__Option_H__

