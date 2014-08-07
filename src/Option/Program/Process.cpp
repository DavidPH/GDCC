//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Option processing.
//
//-----------------------------------------------------------------------------

#include "Option/Program.hpp"

#include "Option/Base.hpp"
#include "Option/Exception.hpp"
#include "Option/Util.hpp"

#include <cstring>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// LimitArgs
//
static GDCC::Option::Args &LimitArgs(GDCC::Option::Args &args)
{
   for(std::size_t argi = 0; argi != args.argC; ++argi)
   {
      if(args.argV[argi][0] == '-' && args.argV[argi][1] != '\0')
      {
         args.argC     = argi;
         args.optFinal = false;
         return args;
      }
   }

   return args;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Option
   {
      //
      // Program::process
      //
      void Program::process(Args const &args)
      {
         Args argsTemp = args;
         argsTemp.optFalse = false;

         while(argsTemp.argC)
         {
            char const *argp = argsTemp.argV[0];

            if(argp[0] == '-')
            {
               // -*
               if(argp[1] == '-')
               {
                  // --*
                  if(argp[2] == '\0')
                  {
                     // --
                     argsTemp.drop();
                     break;
                  }
                  else
                  {
                     // --*
                     argsTemp.drop(processLong(argsTemp));
                  }
               }
               else if(argp[1] == '\0')
               {
                  // -
                  argsTemp.drop(processArgs(argsTemp));
               }
               else
               {
                  // -*
                  argsTemp.drop(processShrt(argsTemp));
               }
            }
            else
            {
               // *
               argsTemp.drop(processArgs(argsTemp));
            }
         }

         // If argC is still nonzero, we encountered an explicit end of options.
         // Process all remaining args as freestanding.
         if(processLoose)
         {
            argsTemp.optFinal = args.optFinal;

            while(argsTemp.argC)
               argsTemp.drop(processLoose->process(argsTemp));
         }
      }

      //
      // Program::processArgs
      //
      std::size_t Program::processArgs(Args args)
      {
         LimitArgs(args);

         if(processLoose)
            return processLoose->process(args);
         else
            return args.argC;
      }

      //
      // Program::processLong
      //
      std::size_t Program::processLong(Args args)
      {
         std::size_t used;

         // Determine option name.
         args.nameL = args.argV[0] + 2;

         if(args.nameL[0] == 'n' && args.nameL[1] == 'o' && args.nameL[2] == '-')
         {
            args.nameL += 3;
            args.optFalse = true;
         }

         // Determine available args.
         LimitArgs(args.drop());

         // Check for attached argument.
         if(auto arg0 = std::strchr(args.nameL, '='))
         {
            auto  nameTemp = StrDup(args.nameL, arg0 - args.nameL);
            auto &opt      = get((args.nameL = nameTemp.get(), args));

            // Choose automatic or dynamic allocation for optv.
            char const **optv;
            std::unique_ptr<char const *[]> optv_p;
            char const *optv_a[8];

            if(++args.argC <= 8)
               optv = optv_a;
            else
               optv_p.reset(optv = new char const *[args.argC]);

            // Populate temporary argument array.
            optv[0] = arg0 + 1;
            for(std::size_t i = 1; i != args.argC; ++i)
               optv[i] = args.argV[i - 1];
            args.argV = optv;

            // Handle the option.
            used = opt.process(args);

            // An argument was explicitly attached to this option. If it went
            // unused, that's an error.
            if(!used)
               Exception::Error(args, "extraneous argument");
         }
         else
         {
            // Handle the option. +1 for nameL.
            used = get(args).process(args) + 1;
         }

         return used;
      }

      //
      // Program::processShrt
      //
      std::size_t Program::processShrt(Args args)
      {
         std::size_t used = 1;
         char const *opts = args.argV[0] + 1;

         // Determine available args.
         LimitArgs(args.drop());

         // Optimization for single short option. +1 for opts.
         if(!opts[1])
            return get((args.nameS = *opts, args)).process(args.setName(*opts)) + 1;

         // Choose automatic or dynamic allocation for optv.
         char const **optv;
         std::unique_ptr<char const *[]> optv_p;
         char const *optv_a[8];

         if(++args.argC <= 8)
            optv = optv_a;
         else
            optv_p.reset(optv = new char const *[args.argC]);

         // Populate temporary argument array.
         for(std::size_t i = 1; i < args.argC; ++i)
            optv[i] = args.argV[i - 1];
         args.argV = optv;

         // Iterate over short options.
         for(;; ++opts)
         {
            auto &opt = get((args.nameS = *opts, args));

            if(opts[1])
            {
               optv[0] = opts + 1;
               used = opt.process(args);

               // used includes opts, so can return as-is.
               if(used)
                  break;
            }
            else
            {
               // +1 for opts.
               used = opt.process(args.drop()) + 1;
               break;
            }
         }

         return used;
      }
   }
}

// EOF

