//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Compiler warnings.
//
//-----------------------------------------------------------------------------

#include "Core/Warning.hpp"

#include "Core/Option.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace Core
   {
      Warning WarnAll   {nullptr,    "--warn-all"};
      Warning WarnExtra {&WarnAll,   "--warn-extra"};
      Warning WarnCommon{&WarnExtra, "--warn-common"};
      Warning WarnStrict{&WarnAll,   "--warn-strict"};
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // WarnOpt::v_process
      //
      std::size_t WarnOpt::v_process(Option::Args const &args)
      {
         dptr->state = args.optFalse ? Warning::State::Off : Warning::State::On;
         return 0;
      }

      //
      // WarnOptList constructor
      //
      WarnOptList::WarnOptList(Option::Program &list_) :
         list(list_),

         optAll
         {
            &list, Option::Base::Info()
               .setName("warn-all")
               .setGroup("warnings")
               .setDescS("Enables all warnings."),

            &WarnAll
         },

         optCommon
         {
            &list, Option::Base::Info()
               .setName("warn-common")
               .setGroup("warnings")
               .setDescS("Enables common warnings."),

            &WarnCommon
         },

         optExtra
         {
            &list, Option::Base::Info()
               .setName("warn-extra")
               .setGroup("warnings")
               .setDescS("Enables common and extra warnings."),

            &WarnExtra
         },

         optStrict
         {
            &list, Option::Base::Info()
               .setName("warn-strict")
               .setGroup("warnings")
               .setDescS("Enables strict warnings."),

            &WarnStrict
         }
      {
      }

      //
      // Warning constructor
      //
      Warning::Warning(Warning const *base_, char const *opt_) :
         base{base_}, opt{opt_}, state{State::Def}
      {
      }

      //
      // Warning::operator bool
      //
      Warning::operator bool () const
      {
         if(state == State::On)  return true;
         if(state == State::Off) return false;
         if(base) return base->operator bool();
         return false;
      }

      //
      // Warning::warnPre
      //
      void Warning::warnPre(Origin pos) const
      {
         std::cerr << "WARNING: " << pos << ": ";
      }

      //
      // Warning::warnPro
      //
      void Warning::warnPro() const
      {
         if(opt)
            std::cerr << " [" << opt << ']';

         std::cerr << std::endl;
      }

      //
      // GetWarnOptList
      //
      Option::Program &GetWarnOptList()
      {
         return GetWarnOpts().list;
      }

      //
      // GetWarnOpts
      //
      WarnOptList &GetWarnOpts()
      {
         static WarnOptList opts{GetOptionList()};
         return opts;
      }
   }
}

// EOF

