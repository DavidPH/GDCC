//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common typedefs and class forward declarations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Types_H__
#define GDCC__Core__Types_H__

#include "../Option/Types.hpp"

#include <cstdint>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   template<typename T>
   class Array;
   class ArrayTBuf;
   class ArrayTSource;
   class ArrayTStream;
   template<typename T>
   class ConditionalDeleter;
   template<typename T>
   class CounterRef;
   template<typename T>
   class CounterRef;
   class DirStream;
   class Exception;
   template<typename T, void(T::*D)(), void(T::*E)()>
   class FeatureHold;
   class FileBlock;
   template<typename I>
   class IntItr;
   class MoveType;
   template<typename T>
   class NumberAlloc;
   template<typename T>
   class NumberAllocMerge;
   class OptionList;
   class Origin;
   class OriginSource;
   class PackType;
   class PathRestore;
   template<typename T>
   class Range;
   class Stat;
   class String;
   class StringData;
   class StringBuf;
   class StringGen;
   class StringOption;
   class StringStream;
   class SystemSourceOption;
   class Token;
   class TokenBuf;
   class TokenSource;
   class TokenStream;
   class WSpaceTBuf;
   class WarnOpt;
   class WarnOptList;
   class Warning;
}

#endif//GDCC__Core__Types_H__

