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
      //
      // CStr constructor
      //
      CStr::CStr(Program *program, Info const &optInfo) :
         Base{program, optInfo},

         str  {nullptr},
         len  {0},
         alloc{false}
      {
      }

      //
      // CStr constructor
      //
      CStr::CStr(Program *program, Info const &optInfo, char const *s, bool copy) :
         Base{program, optInfo},

         str  {nullptr},
         len  {0},
         alloc{false}
      {
         reset(s, copy);
      }

      //
      // CStr destructor
      //
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

      //
      // CStr::v_process
      //
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

