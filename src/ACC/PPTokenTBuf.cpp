//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Preprocessor token converter token buffer.
//
//-----------------------------------------------------------------------------

#include "ACC/PPTokenTBuf.hpp"

#include "CPP/PPTokenTBuf.hpp"

#include "Core/Exception.hpp"

#include <cctype>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // PPTokenTBuf::underflow
      //
      void PPTokenTBuf::underflow()
      {
         if(tptr() != tend()) return;

         switch((buf[0] = src.get()).tok)
         {
         case Core::TOK_Identi:
            if(IsKeyWrd(buf[0].str))
               buf[0].tok = Core::TOK_KeyWrd;
            break;

         case Core::TOK_Number:
                 if(IsNumInt(buf[0].str)) buf[0].tok = Core::TOK_NumInt;
            else if(IsNumFix(buf[0].str)) buf[0].tok = Core::TOK_NumFix;
            else if(IsNumFlt(buf[0].str)) buf[0].tok = Core::TOK_NumFlt;
            else
               throw Core::ParseExceptExpect(buf[0], "number", false);
            break;

         default:
            break;
         }

         sett(buf, buf, buf + 1);
      }

      //
      // PPTokenTBuf::IsKeyWrd
      //
      bool PPTokenTBuf::IsKeyWrd(Core::String str)
      {
         switch(str)
         {
         case Core::STR_acs_executewait:
         case Core::STR_acs_namedexecutewait:
         case Core::STR_bluereturn:
         case Core::STR_bool:
         case Core::STR_break:
         case Core::STR_case:
         case Core::STR_clientside:
         case Core::STR_const:
         case Core::STR_continue:
         case Core::STR_createtranslation:
         case Core::STR_death:
         case Core::STR_default:
         case Core::STR_disconnect:
         case Core::STR_do:
         case Core::STR_else:
         case Core::STR_enter:
         case Core::STR_event:
         case Core::STR_for:
         case Core::STR_function:
         case Core::STR_global:
         case Core::STR_goto:
         case Core::STR_hudmessage:
         case Core::STR_hudmessagebold:
         case Core::STR_if:
         case Core::STR_int:
         case Core::STR_lightning:
         case Core::STR_log:
         case Core::STR_net:
         case Core::STR_open:
         case Core::STR_pickup:
         case Core::STR_print:
         case Core::STR_printbold:
         case Core::STR_redreturn:
         case Core::STR_respawn:
         case Core::STR_restart:
         case Core::STR_return:
         case Core::STR_script:
         case Core::STR_special:
         case Core::STR_static:
         case Core::STR_str:
         case Core::STR_strcpy:
         case Core::STR_strparam:
         case Core::STR_suspend:
         case Core::STR_switch:
         case Core::STR_terminate:
         case Core::STR_unloading:
         case Core::STR_until:
         case Core::STR_void:
         case Core::STR_while:
         case Core::STR_whitereturn:
         case Core::STR_world:
            return true;

            // Extension keywords.
         case Core::STR_enum:
         case Core::STR_fixed:
         case Core::STR_float:
         case Core::STR_sizeof:
         case Core::STR_struct:
            return true;

         default:
            return false;
         }
      }

      //
      // PPTokenTBuf::IsNumInt
      //
      bool PPTokenTBuf::IsNumInt(Core::String str)
      {
         return CPP::PPTokenTBuf::IsNumInt(str);
      }

      //
      // PPTokenTBuf::IsNumFix
      //
      bool PPTokenTBuf::IsNumFix(Core::String str)
      {
         char const *itr = str.begin(), *end = str.end();

         itr = CPP::PPTokenTBuf::SkipFlt(itr, end);

         // unsigned-suffix(opt)
         if(itr == end) return true;
         if(*itr == 'U' || *itr == 'u') ++itr;

         // fxp-suffix(opt)
         if(itr == end) return true;
         if(*itr == 'L' || *itr == 'l' || *itr == 'H' || *itr == 'h') ++itr;

         // fixed-qual(opt)
         if(itr == end) return true;
         return (*itr == 'K' || *itr == 'k') && ++itr == end;
      }

      //
      // PPTokenTBuf::IsNumFlt
      //
      bool PPTokenTBuf::IsNumFlt(Core::String str)
      {
         char const *itr = str.begin(), *end = str.end();

         itr = CPP::PPTokenTBuf::SkipFlt(itr, end);

         auto upr = [&itr](std::size_t i) {return std::toupper(itr[i]);};

         if(end - itr == 2 && upr(0) == 'L' && upr(1) == 'F') return true;
         if(end - itr == 3 && upr(0) == 'L' && itr[1] == itr[0] && upr(2) == 'F') return true;

         return false;
      }
   }
}

// EOF

