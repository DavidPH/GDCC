//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Option processing library.
//
//-----------------------------------------------------------------------------

#include "Option.hpp"

#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace Option
{
   class Option::OptionHead : public Option
   {
   public:
      virtual uint v_handle(strp, uint, uint, strv) {return 0;}
   };
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Option
{
   Option *Option::ArgHandler = &OptionStrV::DefaultArgHandler;

   Option::strp Option::Help_Program = nullptr;
   Option::strp Option::Help_Version = nullptr;
   Option::strp Option::Help_Usage   = nullptr;
   Option::strp Option::Help_DescS   = nullptr;
   Option::strp Option::Help_DescL   = nullptr;

   OptionStrV OptionStrV::DefaultArgHandler{'\0', nullptr, nullptr, nullptr, nullptr, 0};

   Option::uint const *ArgC = &OptionStrV::DefaultArgHandler.argC;
   Option::strv const *ArgV = &OptionStrV::DefaultArgHandler.argV;

   OptionStrV &DefaultArgHandler = OptionStrV::DefaultArgHandler;
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// CountArgs
//
static Option::Option::uint CountArgs(Option::Option::uint argc, Option::Option::strv argv)
{
   for(Option::Option::uint argi = 0; argi != argc; ++argi)
   {
      if(argv[argi][0] == '-' && argv[argi][1] != '\0')
         return argi;
   }

   return argc;
}

//
// GroupCmp
//
static int GroupCmp(char const *s1, char const *s2)
{
   if(s1 == s2) return 0;

   if(!s1) return -1;
   if(!s2) return +1;

   return std::strcmp(s1, s2);
}

//
// StrDup
//
static std::unique_ptr<char[]> StrDup(char const *s, std::size_t len)
{
   std::unique_ptr<char[]> dup{new char[len + 1]};
   std::memcpy(dup.get(), s, len);
   dup[len] = '\0';

   return dup;
}

//
// WordLength
//
static Option::Option::uint WordLength(char const *str)
{
   Option::Option::uint len = 0;

   while(*str && !std::isspace(*str++))
      len++;

   return len ? len : 1;
}

//
// WriteWrapped
//
static void WriteWrapped(std::ostream &out, Option::Option::uint width,
   char const *str, Option::Option::uint baselen, char const *prefix = nullptr)
{
   Option::Option::uint linelen = baselen;

   if(prefix)
      linelen += std::strlen(prefix) + 1;

   while(*str)
   {
      // Linewrapping.
      if(*str == '\n' || linelen + WordLength(str) > width)
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


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Option
{
   //
   // Exception copy constructor
   //
   Exception::Exception(Exception const &e) :
      msg{StrDup(e.msg.get(), std::strlen(e.msg.get()))}
   {
   }

   //
   // Exception::Error
   //
   [[noreturn]] void Exception::Error(char const *opt, int optf, char const *format, ...)
   {
      va_list args;
      std::size_t len, pos;

      if(optf & OPTF_SHORT)
         len = std::snprintf(nullptr, 0, "'-%c': ", *opt);
      else
         len = std::snprintf(nullptr, 0, "'--%s': ", opt);

      va_start(args, format);
      len += std::vsnprintf(nullptr, 0, format, args);
      va_end(args);

      std::unique_ptr<char[]> msg{new char[++len]};

      if(optf & OPTF_SHORT)
         pos = std::snprintf(&msg[0], len, "'-%c': ", *opt);
      else
         pos = std::snprintf(&msg[0], len, "'--%s': ", opt);

      va_start(args, format);
      std::vsnprintf(&msg[pos], len - pos, format, args);
      va_end(args);

      throw Exception(std::move(msg));
   }

   //
   // Option constructor
   //
   Option::Option(char nameS_, strp nameL_, strp group_, strp descS_, strp descL_) :
      descL{descL_ ? descL_ : descS_}, descS{descS_}, group{group_}, nameL{nameL_},
      nameS{nameS_}, handled{false}
   {
      Option *opt = GroupBegin(group);
      Option *end = GroupEnd(group, opt);

      for(; opt != end; opt = opt->next)
      {
         if(GroupCmp(opt->nameL, nameL) > 0)
            break;
      }

      next = opt;
      prev = opt->prev;

      next->prev = this;
      prev->next = this;
   }

   //
   // Option destructor
   //
   Option::~Option()
   {
      prev->next = next;
      next->prev = prev;
   }

   //
   // Option::FindL
   //
   Option *Option::FindL(char const *nameL)
   {
      Option *end = &GetHead();
      Option *opt = end;

      // Shorthand longopts.
      std::size_t slen = std::strlen(nameL);
      Option     *sopt = nullptr;
      unsigned    scnt = 0;

      while((opt = opt->next) != end)
      {
         if(!opt->nameL) continue;

         if(std::strcmp(opt->nameL, nameL) == 0)
            return opt;

         if(std::memcmp(opt->nameL, nameL, slen) == 0)
            sopt = opt, ++scnt;
      }

      if(sopt)
      {
         if(scnt > 1)
            Exception::Error(nameL, 0, "ambiguous shorthand (%u matches)", scnt);

         return sopt;
      }

      Exception::Error(nameL, 0, "no such option");
   }

   //
   // Option::FindS
   //
   Option *Option::FindS(char nameS)
   {
      Option *end = &GetHead();
      Option *opt = end;

      while((opt = opt->next) != end)
      {
         if(opt->nameS == nameS)
            return opt;
      }

      Exception::Error(&nameS, OPTF_SHORT, "no such option");
   }

   //
   // Option::GetHead
   //
   Option &Option::GetHead()
   {
      static OptionHead head;
      return head;
   }

   //
   // Option::GroupBegin
   //
   Option *Option::GroupBegin(char const *group)
   {
      Option *end = &GetHead();
      Option *opt = end;

      while((opt = opt->next) != end)
      {
         if(GroupCmp(opt->group, group) >= 0)
            break;
      }

      return opt;
   }

   //
   // Option::GroupEnd
   //
   Option *Option::GroupEnd(char const *group, Option *opt)
   {
      Option *end = &GetHead();

      for(; opt != end; opt = opt->next)
      {
         if(GroupCmp(opt->group, group))
            break;
      }

      return opt;
   }

   //
   // Option::GroupLen
   //
   auto Option::GroupLen(Option *opt, Option *end, uint *lenL, uint *lenS) -> uint
   {
      *lenL = 0;
      *lenS = 0;

      for(; opt != end; opt = opt->next)
      {
         if(opt->nameL) *lenL = std::max(*lenL, std::strlen(opt->nameL) + 2); // --X
         if(opt->nameS) *lenS = 2; // -X
      }

      // "  " starting indent.
      uint len = 2 + *lenL + *lenS;

      // ", " inter-option comma.
      if(*lenL && *lenS)
         len += 2;

      // "  " post-option space.
      if(*lenL || *lenS)
         len += 2;

      return len;
   }

   //
   // Option::PrintHelp
   //
   void Option::PrintHelp(std::ostream &out, uint width)
   {
      // TODO: Determine terminal width.
      if(!width || width < 80)
         width = 80;

      if(Help_Program)
      {
         PrintVersion(out, width);
         PrintUsage(  out, width);
         out << '\n';
      }

      if(Help_DescS)
      {
         WriteWrapped(out, width, Help_DescS, 0);
         out << '\n';
      }

      Option *end = &GetHead();
      Option *opt = end->next;

      char const *group = nullptr;

      uint lenL, lenS;
      GroupLen(opt, GroupEnd(opt->group, opt), &lenL, &lenS);

      for(; opt != end; opt = opt->next)
      {
         uint baselen;
         char const *desc = opt->descS;

         if(!desc) continue;

         // This gets us past the starting null groups and makes checking faster
         // for folded string literals.
         if(GroupCmp(opt->group, group))
         {
            if((group = opt->group))
               out << '\n' << group << ":\n";

            GroupLen(opt, GroupEnd(opt->group, opt), &lenL, &lenS);
         }

         // Starting indent.
         out << "  ";
         baselen = 2;

         // Short option, if any.
         if(lenS)
         {
            if(opt->nameS)
               out << '-' << opt->nameS;
            else
               out << "  ";

            baselen += 2;
         }

         // Inter-option comma, if any.
         if(lenL && lenS)
         {
            out << (opt->nameS ? ',' : ' ') << ' ';
            baselen += 2;
         }

         // Long option, if any.
         if(lenL)
         {
            uint l = 2;
            if(opt->nameL)
            {
               out << "--";
               for(char const *s = opt->nameL; *s; ++s, ++l)
                  out << *s;
            }
            else
               out << "  ";

            for(; l != lenL; ++l)
               out << ' ';

            baselen += lenL;
         }

         // Post-option space, if any.
         if(lenL || lenS)
         {
            out << "  ";
            baselen += 2;
         }

         // It could happen. No need to deadlock over it.
         if(baselen >= width)
         {
            out << "\n          ";
            WriteWrapped(out, width, desc, 10);
            continue;
         }

         WriteWrapped(out, width, desc, baselen);
      }
   }

   //
   // Option::PrintUsage
   //
   void Option::PrintUsage(std::ostream &out, uint width)
   {
      if(Help_Program && Help_Usage)
      {
         out << "usage: " << Help_Program << ' ';
         WriteWrapped(out, width, Help_Usage, 7, Help_Program);
      }
   }

   //
   // Option::PrintVersion
   //
   void Option::PrintVersion(std::ostream &out, uint)
   {
      if(Help_Program)
      {
         out << Help_Program;

         if(Help_Version)
            out << ' ' << Help_Version;

         out << '\n';
      }
   }

   //
   // Option::ProcessOptionArg
   //
   auto Option::ProcessOptionArg(uint argc, strv argv, uint optf) -> uint
   {
      argc = ArgHandler->handle("", optf, CountArgs(argc, argv), argv);
      return argc;
   }

   //
   // Option::ProcessOptionLong
   //
   auto Option::ProcessOptionLong(uint argc, strv argv, uint optf) -> uint
   {
      uint used;
      uint optc = CountArgs(argc - 1, argv + 1);
      strp opts = argv[0] + 2;

      // Amend flags.
      if(opts[0] == 'n' && opts[1] == 'o' && opts[2] == '-')
      {
         opts += 3;
         optf |= OPTF_FALSE;
      }

      if(optc == argc - 1)
         optf |= OPTF_FINAL;

      // Check for attached argument.
      if(auto arg0 = std::strchr(opts, '='))
      {
         auto optsTemp = StrDup(opts, arg0 - opts);
         auto opt = FindL(optsTemp.get());

         // Create temporary argument array and invoke handler.
         if(++optc <= 8)
         {
            // For small arg counts, use a local array.
            strp optv[8];
            optv[0] = arg0 + 1;
            for(uint i = 1; i != optc; ++i)
               optv[i] = argv[i];

            // Handle the option.
            used = opt->handle(optsTemp.get(), optf, optc, optv);
         }
         else
         {
            // Otherwise, fall back to a dynamic allocation.
            std::unique_ptr<strp[]> optv{new strp[optc]};
            optv[0] = arg0 + 1;
            for(uint i = 1; i != optc; ++i)
               optv[i] = argv[i];

            // Handle the option.
            used = opt->handle(optsTemp.get(), optf, optc, optv.get());
         }

         // An argument was explicitly attached to this option. If it went
         // unused, that's an error.
         if(!used)
            Exception::Error(optsTemp.get(), optf, "extraneous argument");
      }
      else
      {
         // Handle the option.
         used = FindL(opts)->handle(opts, optf, optc, argv + 1) + 1;
      }

      return used;
   }

   //
   // Option::ProcessOptionShort
   //
   auto Option::ProcessOptionShort(uint argc, strv argv, uint optf) -> uint
   {
      uint  used = 1;
      uint  optc = CountArgs(argc-1, argv+1) + 1;
      strp  opts = argv[0] + 1;
      strp *optv;

      // Amend flags.
      optf |= OPTF_SHORT;
      if(optc == argc)
         optf |= OPTF_FINAL;

      // Optimization for single short option.
      if(!opts[1])
      {
         auto opt = FindS(*opts);
         // Add 1 so that the count includes the opts in argv.
         used = opt->handle(opts, optf, optc - 1, argv + 1) + 1;
         return used;
      }

      // Choose automatic or dynamic allocation for optv.
      std::unique_ptr<strp[]> optv_p;
      strp optv_a[8];

      if(optc <= 8)
         optv = optv_a;
      else
         optv_p.reset(optv = new strp[optc]);

      for(uint i = 1; i < optc; ++i)
         optv[i] = argv[i];

      for(;; ++opts)
      {
         auto opt = FindS(*opts);

         if(opts[1])
         {
            optv[0] = opts + 1;
            used = opt->handle(opts, optf, optc, optv);

            // used includes the opts in argv, so can return as-is.
            if(used)
               break;
         }
         else
         {
            // Add 1 so that the count includes the opts in argv.
            used = opt->handle(opts, optf, optc - 1, argv + 1) + 1;
            break;
         }
      }

      return used;
   }

   //
   // Option::ProcessOptions
   //
   void Option::ProcessOptions(uint argc, strv argv, uint optf)
   {
      uint used;

      while(argc)
      {
         auto argp = argv[0];

         if(argp[0] == '-')
         {
            // -*
            if(argp[1] == '-')
            {
               // --*
               if(argp[2] == '\0')
               {
                  // --
                  --argc; ++argv;
                  break;
               }
               else
               {
                  // --*
                  used = ProcessOptionLong(argc, argv, optf);
               }
            }
            else if(argp[1] == '\0')
            {
               // -
               used = ProcessOptionArg(argc, argv, optf);
            }
            else
            {
               // -*
               used = ProcessOptionShort(argc, argv, optf);
            }
         }
         else
         {
            // *
            used = ProcessOptionArg(argc, argv, optf);
         }

         argc -= used;
         argv += used;
      }

      // If argc is still nonzero, we encountered an explicit end of options.
      // Process all remaining args as freestanding.
      while(argc)
      {
         used = ArgHandler->handle("", optf | OPTF_FINAL, argc, argv);

         argc -= used;
         argv += used;
      }
   }

   //
   // OptionCStr::reset
   //
   void OptionCStr::reset(strp data_, bool alloc_)
   {
      reset();

      if((alloc = alloc_))
         data = StrDup(data_, dlen = std::strlen(data_)).release();
      else
         dlen = std::strlen(data = data_);
   }

   //
   // OptionCStr::v_handle
   //
   auto OptionCStr::v_handle(strp opt, uint optf, uint argc, strv argv) -> uint
   {
      if(optf & OPTF_FALSE)
         return reset(), 0;

      if(!argc)
         Exception::Error(opt, optf, "requires argument");

      reset(argv[0], !(optf & OPTF_KEEPA));

      return 1;
   }

   //
   // OptionDPtr<bool>::v_handle
   //
   template<>
   auto OptionDPtr<bool>::v_handle(strp, uint optf, uint, strv) -> uint
   {
      *dptr = !(optf & OPTF_FALSE);
      return 0;
   }

   //
   // OptionDPtr<int>::v_handle
   //
   template<>
   auto OptionDPtr<int>::v_handle(strp opt, uint optf, uint argc, strv argv) -> uint
   {
      if(optf & OPTF_FALSE)
      {
         *dptr = 0;
         return 0;
      }

      if(!argc)
         Exception::Error(opt, optf, "requires argument");

      *dptr = static_cast<int>(std::strtol(argv[0], nullptr, 10));
      return 1;
   }

   //
   // OptionDPtr<std::string>::v_handle
   //
   template<>
   auto OptionDPtr<std::string>::v_handle(strp opt, uint optf, uint argc,
      strv argv) -> uint
   {
      if(optf & OPTF_FALSE)
      {
         dptr->clear();
         return 0;
      }

      if(!argc)
         Exception::Error(opt, optf, "requires argument");

      *dptr = argv[0];
      return 1;
   }

   //
   // OptionDPtr<std::vector<std::string>>::v_handle
   //
   template<>
   auto OptionDPtr<std::vector<std::string>>::v_handle(strp opt, uint optf,
      uint argc, strv argv) -> uint
   {
      if(optf & OPTF_FALSE)
      {
         dptr->clear();
         return 0;
      }

      if(!argc)
         Exception::Error(opt, optf, "requires argument");

      dptr->emplace_back(argv[0]);
      return 1;
   }

   //
   // OptionStrV destructor
   //
   OptionStrV::~OptionStrV()
   {
      while(argC)
      {
         if(argF[--argC]) delete[] argV[argC];
      }

      delete[] argF;
      delete[] argV;
   }

   //
   // OptionStrV::argPop
   //
   void OptionStrV::argPop(Option *opt, strp optn, uint optf)
   {
      if(!argC) return;

      // Pass to option, if any.
      if(opt)
      {
         if(!argF[argC-1])
            optf |= OPTF_KEEPA;

         opt->handle(optn, optf, 1, argV + argC - 1);
      }

      // Pop arg.
      if(argF[argC--])
         delete[] argV[argC];
   }

   //
   // OptionStrV::v_handle
   //
   auto OptionStrV::v_handle(strp opt, uint optf, uint argc, strv argv) -> uint
   {
      if(argMax && !argc)
         Exception::Error(opt, optf, "requires argument");

      if(argMax && argc > argMax)
         argc = argMax;

      uint newC = argC + argc;

      if(newC > argA)
      {
         uint newA;

         if(optf & OPTF_FINAL)
            newA = newC;
         else
            newA = argA * 2 + argc;

         auto newF = new bool[newA];
         auto newV = new strp[newA];

         std::memcpy(newF, argF, sizeof(*argF) * argC);
         std::memcpy(newV, argV, sizeof(*argV) * argC);

         delete[] argF;
         delete[] argV;

         argA = newA;
         argF = newF;
         argV = newV;
      }

      if(optf & OPTF_KEEPA)
      {
         std::memcpy(argV + argC, argv, sizeof(*argv) * argc);
         while(argC != newC) argF[argC++] = false;
      }
      else for(uint argi = 0; argi != argc; ++argi, ++argC)
      {
         argV[argC] = StrDup(argv[argi], std::strlen(argv[argi])).release();
         argF[argC] = true;
      }

      return argc;
   }
}

// EOF

