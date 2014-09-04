//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C-string option handling.
//
//-----------------------------------------------------------------------------

#include "Option/CStr.hpp"

#include "Option/Exception.hpp"
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
      /// Initializes string to null and invokes Base's insertion constructor.
      ///
      /// @param program Program to insert this into.
      /// @param optInfo Basic option information.
      ///
      CStr::CStr(Program *program, Info const &optInfo) :
         Base{program, optInfo},

         str  {nullptr},
         len  {0},
         alloc{false}
      {
      }

      ///
      /// Initializes string from s and invokes Base's insertion constructor.
      /// If copy is true, string is duplicated. Otherwise, it is kept.
      ///
      /// @param program Program to insert this into.
      /// @param optInfo Basic option information.
      /// @param s Initial string value.
      /// @param copy Whether s can be kept or must be copied.
      ///
      CStr::CStr(Program *program, Info const &optInfo, char const *s, bool copy) :
         Base{program, optInfo},

         str  {nullptr},
         len  {0},
         alloc{false}
      {
         reset(s, copy);
      }

      ///
      /// Frees the associated string, if necessary.
      ///
      CStr::~CStr()
      {
         if(alloc) delete[] str;
      }

      //
      // CStr::reset
      //
      void CStr::reset()
      {
         if(alloc) delete[] str;

         str   = nullptr;
         len   = 0;
         alloc = false;
      }

      //
      // CStr::reset
      //
      void CStr::reset(char const *s, bool copy)
      {
         if(alloc) delete[] str;

         len = std::strlen(s);

         if((alloc = copy))
            str = StrDup(s, len).release();
         else
            str = s;
      }

      ///
      /// If optFalse is set, sets string to null and length to 0 and consumes
      /// no arguments. Otherwise, consumes one argument to set string and
      /// length. If optKeepA is set, the argument string is kept instead of
      /// duplicated.
      ///
      /// @param args Program arguments.
      ///
      /// @return Number of consumed arguments.
      ///
      /// @exception Option::Exception Thrown for lack of argument.
      ///
      std::size_t CStr::v_process(Args const &args)
      {
         if(args.optFalse) return reset(), 0;
         if(!args.argC) Exception::Error(args, "argument required");

         reset(args.argV[0], !args.optKeepA);

         return 1;
      }
   }
}

// EOF

