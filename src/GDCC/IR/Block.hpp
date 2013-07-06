//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation statement blocks.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Block_H__
#define GDCC__IR__Block_H__

#include "Statement.hpp"

#include "../List.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Block
      //
      class Block
      {
      public:
         typedef ListItr<Statement, &Statement::next, &Statement::prev> Itr;


         ~Block() {while(head.next != &head) delete head.next;}

         Block &addStatement(Code code) {new Statement(&head, code); return *this;}

         Block &addStatementArgs(Code code)
            {return setArgs().addStatement(code);}

         template<typename... Args>
         Block &addStatementArgs(Code code, Args const &...args)
            {return setArgs(args...).addStatement(code);}

         Itr begin() {return static_cast<Itr>(head.next);}
         Itr end() {return static_cast<Itr>(&head);}

         Block &setArgs() {head.args.clear(); return *this;}

         Block &setArgs(Array<Arg> const &argv) {head.args = argv; return *this;}
         Block &setArgs(Array<Arg> &&argv) {head.args = std::move(argv); return *this;}

         //
         // setArgs
         //
         template<typename... Args>
         Block &setArgs(Args &&... args)
         {
            Array<Arg> argv{sizeof...(Args)};
            unpackArgs(argv.data(), std::forward<Args>(args)...);
            return setArgs(std::move(argv));
         }

         Block &setOrigin(Origin pos) {head.pos = pos; return *this;}

      private:
         void unpackArgs(Arg *argv, Arg const &arg0) {*argv = arg0;}
         void unpackArgs(Arg *argv, Arg      &&arg0) {*argv = std::move(arg0);}

         void unpackArgs(Arg *argv, Exp *arg0);

         //
         // unpackArgs
         //
         template<typename Arg0, typename... Args>
         void unpackArgs(Arg *argv, Arg0 &&arg0, Args &&...args)
         {
            unpackArgs(argv, arg0);
            unpackArgs(argv + 1, std::forward<Args>(args)...);
         }

         Statement head;
      };
   }
}

#endif//GDCC__IR__Block_H__

