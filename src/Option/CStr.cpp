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
      CStr::CStr(Program *list_, Info const &info_) :
         Base{list_, info_},

         str  {nullptr},
         len  {0},
         alloc{false}
      {
      }

      //
      // CStr constructor
      //
      CStr::CStr(Program *list_, Info const &info_, char const *str_, bool copy) :
         Base{list_, info_},

         str  {nullptr},
         len  {0},
         alloc{false}
      {
         reset(str_, copy);
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
      void CStr::reset(char const *str_, bool copy)
      {
         if(alloc) delete[] str;

         len = std::strlen(str_);

         if((alloc = copy))
            str = StrDup(str_, len).release();
         else
            str = str_;
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

