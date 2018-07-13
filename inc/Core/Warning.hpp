//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Compiler warnings.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Warning_H__
#define GDCC__Core__Warning_H__

#include "../Core/Origin.hpp"

#include "../Option/Base.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // WarnOpt
   //
   class WarnOpt : public Option::Base
   {
   public:
      WarnOpt(Option::Program *program, Info const &optInfo, Warning *ptr) :
         Option::Base{program, optInfo}, dptr{ptr} {}

   protected:
      virtual std::size_t v_process(Option::Args const &args);

   private:
      Warning *const dptr;
   };

   //
   // WarnOptList
   //
   // Common warning options.
   //
   class WarnOptList
   {
   public:
      explicit WarnOptList(Option::Program &list);

      Option::Program &list;

      WarnOpt optAll;
      WarnOpt optCommon;
      WarnOpt optExtra;
      WarnOpt optStrict;
   };

   //
   // Warning
   //
   class Warning
   {
   public:
      enum class State {Off, On, Def};


      Warning(Warning const *base, char const *opt, State state = State::Def);

      explicit operator bool () const;

      //
      // warn
      //
      template<typename... Args>
      void operator () (Origin pos, Args const &...args) const
      {
         if(!*this) return;

         warnPre(pos);
         warnArg(args...);
         warnPro();
      }


      friend class WarnOpt;

   private:
      void warnArg() const {}

      //
      // warnArg
      //
      template<typename Arg0, typename... Args>
      void warnArg(Arg0 const &arg0, Args const &...args) const
      {
         std::cerr << arg0;
         warnArg(args...);
      }

      void warnPre(Origin pos) const;
      void warnPro() const;


      Warning const *base;
      char const    *opt;
      State          state;
   };
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::Core
{
   extern Warning WarnAll;
   extern Warning WarnCommon;
   extern Warning WarnExtra;
   extern Warning WarnStrict;
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   Option::Program &GetWarnOptList();

   WarnOptList &GetWarnOpts();
}

#endif//GDCC__Core__Warning_H__

