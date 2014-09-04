//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Option
   {
      ///
      /// Initializes basic information and inserts this into program.
      ///
      /// @param program Program to insert this into.
      /// @param optInfo Basic option information.
      ///
      Base::Base(Program *program, Info const &optInfo) :
         info{optInfo},

         processed{false},

         hashNext{this}, hashPrev{this},
         shrtNext{this}, shrtPrev{this},

         hash{StrHash(optInfo.nameL)},
         lenL{optInfo.nameL ? std::strlen(optInfo.nameL) : 0}
      {
         if(program) program->insert(this);
      }

      ///
      /// Unlinks from current program, if any.
      ///
      Base::~Base()
      {
         if(prog) prog->remove(this);
      }

      ///
      /// Inserts into a program. If this was already in a program, it will be
      /// removed first.
      ///
      /// @param program Program to insert this into.
      ///
      void Base::insert(Program *program)
      {
         if(prog   ) prog   ->remove(this);
         if(program) program->insert(this);
      }

      ///
      /// Invokes v_process then sets processed to true. Returns the result of
      /// v_process unaltered.
      ///
      /// @param args Program arguments to be processed.
      ///
      /// @return Number of arguments consumed.
      ///
      /// @exception Option::Exception May be thrown as a result of invalid
      ///    arguments.
      ///
      std::size_t Base::process(Args const &args)
      {
         auto ret = v_process(args);
         processed = true;
         return ret;
      }

      ///
      /// Unlinks from the current program, if any.
      ///
      void Base::remove()
      {
         if(prog) prog->remove(this);
      }
   }
}

// EOF

