//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Option processing base class.
//
//-----------------------------------------------------------------------------

#include "Option/Base.hpp"

#include "Option/Program.hpp"
#include "Option/StrUtil.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Option
{
   //
   // Base constructor
   //
   Base::Base(Program *program, Info const &optInfo) :
      info{optInfo},

      processed{false},

      hashNext{this}, hashPrev{this},
      shrtNext{this}, shrtPrev{this},

      hash{StrHash(optInfo.nameL)},
      lenL{optInfo.nameL ? std::strlen(optInfo.nameL) : 0}
   {
      if(program)
         program->insert(this);
      else
         prog = nullptr;
   }

   //
   // Base destructor
   //
   Base::~Base()
   {
      if(prog) prog->remove(this);
   }

   //
   // Base::insert
   //
   void Base::insert(Program *program)
   {
      if(prog   ) prog   ->remove(this);
      if(program) program->insert(this);
   }

   //
   // Base::process
   //
   std::size_t Base::process(Args const &args)
   {
      auto ret = v_process(args);
      processed = true;
      return ret;
   }

   //
   // Base::remove
   //
   void Base::remove()
   {
      if(prog) prog->remove(this);
   }
}

// EOF

