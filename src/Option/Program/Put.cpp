//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Option documentation generation.
//
//-----------------------------------------------------------------------------

#include "Option/Program.hpp"

#include "Option/Base.hpp"
#include "Option/StrUtil.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//
//

namespace GDCC
{
   namespace Option
   {
      //
      // Program::getGroupInfo
      //
      Program::GroupInfo Program::getGroupInfo(Base *opt)
      {
         GroupInfo info;

         info.lenL = info.lenS = 0;

         info.first = opt;

         while(!StrCmp(info.first->info.group, opt->info.group))
         {
            if(opt->info.nameL)
               info.lenL = std::max(info.lenL, std::strlen(opt->info.nameL) + 2);

            if(opt->info.nameS)
               info.lenS = 2;

            if((opt = opt->listNext) == listHead)
               break;
         }

         info.last = opt;

         // See: PutHelpPre
         info.len = 2;
         if(info.lenS)              info.len += 2;
         if(info.lenL && info.lenS) info.len += 2;
         if(info.lenL)              info.len += info.lenL;
         if(info.lenL || info.lenS) info.len += 2;

         return info;
      }

      //
      // Program::putHelp
      //
      void Program::putHelp(std::ostream &out, std::size_t width)
      {
         // Determine default width.
         if(!width) width = GetWidthDefault();

         // Put version and usage.
         putHelpUsage(out, width);

         // Put short description.
         if(descS)
         {
            out << '\n';
            PutWrapped(out, width, descS, 0);
         }

         // Put options.
         if(!listHead) return;

         GroupInfo info; info.last = listHead;
         do
         {
            info = getGroupInfo(info.last);
            Base *opt = info.first;

            out << '\n';
            if(opt->info.group)
               out << opt->info.group << ":\n";

            do
            {
               // Must have a description to put.
               if(!opt->info.descS) continue;

               // -o, --option
               PutHelpPre(out, info, opt);

               // It could happen. No need to deadlock over it.
               if(info.len >= width)
                  PutWrapped(out << "\n        ", width, opt->info.descS, 8);
               else
                  PutWrapped(out, width, opt->info.descS, info.len);
            }
            while((opt = opt->listNext) != info.last);
         }
         while(info.last != listHead);
      }

      //
      // Program::putHelpLong
      //
      void Program::putHelpLong(std::ostream &out, std::size_t width)
      {
         // Determine default width.
         if(!width) width = GetWidthDefault();

         // Put version and usage.
         putHelpUsage(out, width);

         // Put description.
         if(descL)
         {
            out << '\n';
            PutWrapped(out, width, descL, 0);
         }
         else if(descS)
         {
            out << '\n';
            PutWrapped(out, width, descS, 0);
         }

         // Put options.
         if(!listHead) return;

         GroupInfo info; info.last = listHead;
         do
         {
            info = getGroupInfo(info.last);
            Base *opt = info.first;

            out << '\n';
            if(opt->info.group)
               out << opt->info.group << ":\n";

            do
            {
               auto desc = opt->info.descL ? opt->info.descL : opt->info.descS;

               // Must have a description to put.
               if(!desc) continue;

               // -o, --option
               PutHelpPre(out, info, opt);

               // It could happen. No need to deadlock over it.
               if(info.len >= width)
                  PutWrapped(out << "\n        ", width, desc, 8);
               else
                  PutWrapped(out, width, desc, info.len);
            }
            while((opt = opt->listNext) != info.last);
         }
         while(info.last != listHead);
      }

      //
      // Program::putHelpUsage
      //
      void Program::putHelpUsage(std::ostream &out, std::size_t width)
      {
         if(name)
         {
            putVersion(out, width);

            if(usage)
            {
               out << "usage: " << name << ' ';
               PutWrapped(out, width, usage, 7, name);
            }
         }
      }

      //
      // Program::putVersion
      //
      void Program::putVersion(std::ostream &out, std::size_t)
      {
         if(name)
         {
            out << name;

            if(version)
               out << ' ' << version;

            out << '\n';
         }
      }

      //
      // Program::GetWidthDefault
      //
      std::size_t Program::GetWidthDefault()
      {
         // TODO: Read terminal width.
         return 80;
      }

      //
      // Program::PutHelpPre
      //
      void Program::PutHelpPre(std::ostream &out, GroupInfo const &info, Base *opt)
      {
         // Starting indent.
         out << "  ";

         // Short option, if any.
         if(info.lenS)
         {
            if(opt->info.nameS)
               out << '-' << opt->info.nameS;
            else
               out << "  ";
         }

         // Inter-option comma, if any.
         if(info.lenL && info.lenS)
            out << (opt->info.nameS ? ',' : ' ') << ' ';

         // Long option, if any.
         if(info.lenL)
         {
            std::size_t l = 2;
            if(opt->info.nameL)
            {
               out << "--";
               for(char const *s = opt->info.nameL; *s; ++s, ++l)
                  out << *s;
            }
            else
               out << "  ";

            for(; l != info.lenL; ++l)
               out << ' ';
         }

         // Post-option space, if any.
         if(info.lenL || info.lenS)
            out << "  ";
      }

      //
      // Program::PutWrapped
      //
      void Program::PutWrapped(std::ostream &out, std::size_t width,
         char const *str, std::size_t baselen, char const *prefix)
      {
         std::size_t linelen = baselen;

         if(prefix)
            linelen += std::strlen(prefix) + 1;

         while(*str)
         {
            // Linewrapping.
            if(*str == '\n' || linelen + WordLen(str) > width)
            {
               out.put('\n');

               // Only write line start stuff for the last linefeed.
               if(str[0] == '\n' && str[1] == '\n')
               {
                  ++str;
                  continue;
               }

               for(linelen = 0; linelen < baselen; ++linelen)
                  out.put(' ');

               if(prefix)
               {
                  // Only reprint prefix for an explicit line break.
                  if(*str == '\n')
                     for(auto pre = prefix; *pre; ++pre, ++linelen)
                        out.put(*pre);
                  else
                     for(auto pre = prefix; *pre; ++pre, ++linelen)
                        out.put(' ');

                  out.put(' ');
                  ++linelen;
               }

               if(*str == '\n') ++str;

               // Skip any leading spaces.
               while(*str == ' ') ++str;
            }

            out.put(*str++);
            ++linelen;
         }

         out.put('\n');
      }

      //
      // Program::WordLen
      //
      std::size_t Program::WordLen(char const *str)
      {
         std::size_t len = 0;

         while(*str && !std::isspace(*str++))
            len++;

         return len ? len : 1;
      }
   }
}

// EOF

