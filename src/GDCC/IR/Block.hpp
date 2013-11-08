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
         using value_type             = Statement;
         using const_value_type       = value_type const;
         using const_iterator         = ListItr<const_value_type>;
         using const_pointer          = const_value_type *;
         using const_reference        = const_value_type &;
         using const_reverse_iterator = std::reverse_iterator<const_iterator>;
         using difference_type        = std::ptrdiff_t;
         using iterator               = ListItr<value_type>;
         using pointer                = value_type *;
         using reference              = value_type &;
         using reverse_iterator       = std::reverse_iterator<iterator>;
         using size_type              = std::size_t;


         Block() = default;
         Block(Block &&block) = default;
         ~Block() {while(head.next != &head) delete head.next;}

         Block &operator = (Block &&block) = default;

         Block &addLabel(String lab) {labs.push_back(lab); return *this;}

         // addStatement
         Block &addStatement(Statement *link, Code code);
         Block &addStatement(Code code) {return addStatement(&head, code);}

         // addStatementArgs
         Block &addStatementArgs(Statement *link, Code code)
            {return setArgs().addStatement(link, code);}
         Block &addStatementArgs(Code code)
            {return setArgs().addStatement(&head, code);}

         // addStatementArgs
         template<typename... Args>
         Block &addStatementArgs(Statement *link, Code code, Args &&...args)
            {return setArgs(std::forward<Args>(args)...).addStatement(link, code);}
         template<typename... Args>
         Block &addStatementArgs(Code code, Args &&...args)
            {return setArgs(std::forward<Args>(args)...).addStatement(&head, code);}

               iterator begin()       {return static_cast<      iterator>(head.next);}
         const_iterator begin() const {return static_cast<const_iterator>(head.next);}

               iterator end()       {return static_cast<      iterator>(&head);}
         const_iterator end() const {return static_cast<const_iterator>(&head);}

         Origin getOrigin() const {return head.pos;}

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

         size_type size() const {return std::distance(begin(), end());}


         friend OArchive &operator << (OArchive &out, Block const &in);

         friend IArchive &operator >> (IArchive &in, Block &out);

      private:
         void unpackArg(Arg *argv, Arg const &arg0) {*argv = arg0;}
         void unpackArg(Arg *argv, Arg      &&arg0) {*argv = std::move(arg0);}

         void unpackArg(Arg *argv, Exp *arg0);

         //
         // unpackArgs
         //
         template<typename Arg0>
         void unpackArgs(Arg *argv, Arg0 &&arg0)
         {
            unpackArg(argv, std::forward<Arg0>(arg0));
         }

         //
         // unpackArgs
         //
         template<typename Arg0, typename... Args>
         void unpackArgs(Arg *argv, Arg0 &&arg0, Args &&...args)
         {
            unpackArg(argv, std::forward<Arg0>(arg0));
            unpackArgs(argv + 1, std::forward<Args>(args)...);
         }

         std::vector<String> labs;
         Statement           head;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      OArchive &operator << (OArchive &out, Block const &in);

      IArchive &operator >> (IArchive &in, Block &out);
   }
}

#endif//GDCC__IR__Block_H__

