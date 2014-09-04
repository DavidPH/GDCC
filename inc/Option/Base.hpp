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

#ifndef GDCC__Option__Base_H__
#define GDCC__Option__Base_H__

#include <cstddef>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Option
   {
      class Program;

      /// Option processing arguments.

      /// Aggregates option arguments and any information about them.
      ///
      class Args
      {
      public:
         /// Default constructor.

         /// Initializes all members to 0 or null.
         ///
         Args() :
            argV{nullptr},
            argC{0},

            nameL{nullptr},
            nameS{'\0'},

            optFalse{false},
            optFinal{false},
            optKeepA{false}
         {
         }

         /// Removes the first n arguments.
         Args &drop(std::size_t n = 1) {argC -= n; argV += n; return *this;}

         /// Sets the argument count and returns *this.
         Args &setArgC(std::size_t c) {argC = c; return *this;}

         /// Sets the arguments and returns *this.
         Args &setArgs(char const *const *v, std::size_t c)
            {argV = v; argC = c; return *this;}

         /// Sets the long name and returns *this.
         Args &setName (char const *name) {nameL = name; return *this;}
         /// Sets the short name and returns *this.
         Args &setName (char        name) {nameS = name; return *this;}

         /// Sets the Final flag and returns *this.
         Args &setOptFinal(bool opt = true) {optFinal = opt; return *this;}
         /// Sets the KeepA flag and returns *this.
         Args &setOptKeepA(bool opt = true) {optFinal = opt; return *this;}

         char const *const *argV; ///< Argument string array.
         std::size_t        argC; ///< Argument string count.

         char const *nameL; ///< Long name (null if none).
         char        nameS; ///< Short name (0 if none).

         bool optFalse : 1; ///< The option is negated.
         bool optFinal : 1; ///< There are no more args. (Hint only.)
         bool optKeepA : 1; ///< Argument strings may be kept.
      };

      /// Base class for all options.

      /// Stores basic option information and handles Program interaction.
      ///
      class Base
      {
      public:
         /// Stores core option information.

         /// Aggregates core option information to simplify constructor
         /// arguments.
         ///
         class Info
         {
         public:
            /// Sets the long description and returns *this.
            Info &setDescL(char const *desc) {descL = desc; return *this;}
            /// Sets the short decription and returns *this.
            Info &setDescS(char const *desc) {descS = desc; return *this;}
            /// Sets the group name and returns *this.
            Info &setGroup(char const *name) {group = name; return *this;}
            /// Sets the long name and returns *this.
            Info &setName (char const *name) {nameL = name; return *this;}
            /// Sets the short name and returns *this.
            Info &setName (char        name) {nameS = name; return *this;}

            char const *descL = nullptr; ///< Long description.
            char const *descS = nullptr; ///< Short description.
            char const *group = nullptr; ///< Option group name.
            char const *nameL = nullptr; ///< Long name.
            char        nameS = '\0';    ///< Short name.
         };


         Base(Base const &) = delete;
         Base(Base &&) = delete;

         Base &operator = (Base const &) = delete;
         Base &operator = (Base &&) = delete;

         /// Returns the current program.
         Program *getProgram() const {return prog;}

         /// Adds this option to a program.
         void insert(Program *program);

         /// Processes program arguments.
         std::size_t process(Args const &args);

         /// Removes this option from its current program.
         void remove();

         Info const info; ///< Core option information.

         bool processed : 1; ///< Set to true when process is called.


         friend class Program;

      protected:
         /// Insertion constructor.
         Base(Program *program, Info const &optInfo);

         /// Destructor.
         virtual ~Base();

         /// Virtual implementation of option processing.

         /// Implementations must return the number of arguments consumed.
         ///
         /// @param args Program arguments.
         ///
         /// @return Number of arguments consumed.
         ///
         /// @exception Option::Exception May be thrown as a result of missing
         ///    or invalid arguments.
         ///
         virtual std::size_t v_process(Args const &args) = 0;

      private:
         Program *prog;

         Base *hashNext, *hashPrev;
         Base *listNext, *listPrev;
         Base *shrtNext, *shrtPrev;

         std::size_t const hash;
         std::size_t const lenL;
      };
   }
}

#endif//GDCC__Option__Base_H__

