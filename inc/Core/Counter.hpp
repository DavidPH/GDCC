//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Reference counting.
//
//-----------------------------------------------------------------------------
/// @file

#ifndef GDCC__Core__Counter_H__
#define GDCC__Core__Counter_H__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_Core_CounterPreambleCommonTypes
//
// Used for common definitions for macros below.
//
#define GDCC_Core_CounterPreambleCommonTypes(type, base) \
public: \
   typedef type This; \
   typedef base Super; \
   typedef ::GDCC::Core::CounterPtr<type> Ptr; \
   typedef ::GDCC::Core::CounterPtr<type const> CPtr; \
   friend class ::GDCC::Core::CounterPtr<type>; \
   friend class ::GDCC::Core::CounterPtr<type const>; \
   typedef ::GDCC::Core::CounterRef<type> Ref; \
   typedef ::GDCC::Core::CounterRef<type const> CRef; \
   friend class ::GDCC::Core::CounterRef<type>; \
   friend class ::GDCC::Core::CounterRef<type const>

//
// GDCC_Core_CounterPreambleCommon
//
#define GDCC_Core_CounterPreambleCommon(type, base) \
public: \
   ::GDCC::Core::CounterRef<type> clone() const \
      {return static_cast<::GDCC::Core::CounterRef<type>>(cloneRaw());} \
   virtual char const *getClassName() const {return #type;} \
   GDCC_Core_CounterPreambleCommonTypes(type, base)

/// Reference-counted class preamble.

/// For use in defining typical reference-counted classes.
///
/// <pre>
/// Typedefs:
///    This  = type
///    Super = base
///    Ptr   = CounterPtr&lt;This&gt;
///    CPtr  = CounterPtr&lt;This const&gt;
///    Ref   = CounterRef&lt;This&gt;
///    CRef  = CounterRef&lt;This const&gt;
///
/// Functions:
///    Ref clone() const
///       Returns a duplicate of the object using a copy construction virtual.
///
///    virtual char const *getClassName() const;
///       Returns the name of the class.
/// </pre>
///
/// @param type The name of the class being defined.
/// @param base The parent class of the class being defined.
///
#define GDCC_Core_CounterPreamble(type, base) \
private: \
   virtual type *cloneRaw() const {return new type(*this);} \
   GDCC_Core_CounterPreambleCommon(type, base)

/// Reference-counted abstract class preable.

/// For use in defining reference-counted classes with pure virtual functions.
///
/// Works as in GDCC_Core_CounterPreamble, except that the clone virtual is
/// declared as pure.
///
/// @param type The name of the class being defined.
/// @param base The parent class of the class being defined.
///
#define GDCC_Core_CounterPreambleAbstract(type, base) \
private: \
   virtual type *cloneRaw() const = 0; \
   GDCC_Core_CounterPreambleCommon(type, base)

/// Reference-counted non-copyable class preable.

/// For use in defining reference-counted classes that cannot be copied.
///
/// Works as in GDCC_Core_CounterPreamble, except that the clone virtual throws
/// an exception derived from std::exception.
///
/// @param type The name of the class being defined.
/// @param base The parent class of the class being defined.
///
#define GDCC_Core_CounterPreambleNoClone(type, base) \
private: \
   virtual type *cloneRaw() const {NoClone("bad clone: " #type);} \
   GDCC_Core_CounterPreambleCommon(type, base)

/// Reference-counted non-virtual class preable.

/// For use in defining reference-counted classes without virtual functions.
///
/// Works as in GDCC_Core_CounterPreamble, except there is no clone.
///
/// @param type The name of the class being defined.
/// @param base The parent class of the class being defined.
///
#define GDCC_Core_CounterPreambleNoVirtual(type, base) \
public: \
   char const *getClassName() const {return #type;} \
   GDCC_Core_CounterPreambleCommonTypes(type, base)


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      template<typename T> class CounterRef;

      /// Reference-counting pointer.

      /// T must be derived from Counter or PlainCounter.
      ///
      /// All constructors taking a pointer increment its reference count if
      /// not null. All assignments taking a pointer increment its reference
      /// count if not null and then decrement the prior pointer's reference
      /// count. The destructor decrement's the current pointer's reference
      /// count if not null. If the reference count is decremented to 0, the
      /// object is deleted.
      ///
      template<typename T> class CounterPtr
      {
      public:
         /// Default constructor.

         /// Initializes to a null pointer.
         ///
         CounterPtr(decltype(nullptr) p_ = nullptr) :
            p{p_}
         {
         }

         /// Cast constructor.
         CounterPtr(T *p_) :
            p{static_cast<T *>(p_)}
         {
            if(p) ++p->refCount;
         }

         /// Cast constructor.
         template<typename T2>
         CounterPtr(T2 *p_) :
            p{static_cast<T *>(p_)}
         {
            if(p) ++p->refCount;
         }

         /// Copy constructor.
         CounterPtr(CounterPtr<T> const &p_) :
            p{static_cast<T *>(p_)}
         {
            if(p) ++p->refCount;
         }

         /// Cast constructor.
         template<typename T2>
         CounterPtr(CounterPtr<T2> const &p_) :
            p{static_cast<T *>(p_)}
         {
            if(p) ++p->refCount;
         }

         /// Cast constructor.
         CounterPtr(CounterRef<T> const &p_) :
            p{static_cast<T *>(p_)}
         {
            ++p->refCount;
         }

         /// Cast constructor.
         template<typename T2>
         CounterPtr(CounterRef<T2> const &p_) :
            p{static_cast<T *>(p_)}
         {
            ++p->refCount;
         }

         /// Destructor.
         ~CounterPtr() {if(p && !--p->refCount) delete p;}

         /// Cast operator.
         operator T * () const
         {
            return p;
         }

         /// Assignment operator.
         CounterPtr<T> &operator = (decltype(nullptr) p_)
         {
            if(p && !--p->refCount) delete p; p = p_;
            return *this;
         }

         /// Assignment operator.
         CounterPtr<T> &operator = (T *p_)
         {
            T *old = p; if((p = p_)) ++p->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }

         /// Assignment operator.
         template<typename T2>
         CounterPtr<T> &operator = (T2 *p_)
         {
            T *old = p; if((p = p_)) ++p->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }

         /// Assignment operator.
         CounterPtr<T> &operator = (CounterPtr<T> const &p_)
         {
            T *old = p; if((p = static_cast<T *>(p_))) ++p->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }

         /// Assignment operator.
         template<typename T2>
         CounterPtr<T> &operator = (CounterPtr<T2> const &p_)
         {
            T *old = p; if((p = static_cast<T *>(p_))) ++p->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }

         /// Assignment operator.
         CounterPtr<T> &operator = (CounterRef<T> const &p_)
         {
            T *old = p; ++(p = static_cast<T *>(p_))->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }

         /// Assignment operator.
         template<typename T2>
         CounterPtr<T> &operator = (CounterRef<T2> const &p_)
         {
            T *old = p; ++(p = static_cast<T *>(p_))->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }

         /// Indirect member operator.
         T *operator -> () const {return p;}

         /// Dereference operator.
         T &operator * () const {return *p;}

         /// Returns the reference count of the associated object.

         /// If this is null, 0 is returned.
         ///
         unsigned refCount() const {return p ? p->refCount : 0;}

      private:
         T *p;
      };

      /// Reference-counting pointer that is never null.

      /// Works as in CounterPtr, but forgoes any null checks. For safety,
      /// conversion from pointer or CounterPtr must be explicit.
      ///
      template<typename T> class CounterRef
      {
      public:
         // default constructor
         CounterRef(decltype(nullptr) p_ = nullptr) = delete;

         /// Explicit cast constructor.
         explicit CounterRef(T *p_) :
            p(static_cast<T *>(p_))
         {
            ++p->refCount;
         }

         /// Explicit cast constructor.
         template<typename T2>
         explicit CounterRef(T2 *p_) :
            p(static_cast<T *>(p_))
         {
            ++p->refCount;
         }

         /// Explicit cast constructor.
         explicit CounterRef(CounterPtr<T> const &p_) :
            p(static_cast<T *>(p_))
         {
            ++p->refCount;
         }

         /// Explicit cast constructor.
         template<typename T2>
         explicit CounterRef(CounterPtr<T2> const &p_) :
            p(static_cast<T *>(p_))
         {
            ++p->refCount;
         }

         /// Cast constructor
         CounterRef(CounterRef<T> const &p_) :
            p(static_cast<T *>(p_))
         {
            ++p->refCount;
         }

         /// Cast constructor
         template<typename T2>
         CounterRef(CounterRef<T2> const &p_) :
            p(static_cast<T *>(p_))
         {
            ++p->refCount;
         }

         /// Destructor.
         ~CounterRef()
         {
            if(!--p->refCount) delete p;
         }

         /// Cast operator.
         operator T * () const
         {
            return p;
         }

         /// Assignment operator.
         CounterRef<T> &operator = (CounterRef<T> const &p_)
         {
            T *old = p; ++(p = static_cast<T *>(p_))->refCount;
            if(!--old->refCount) delete old;
            return *this;
         }

         /// Assignment operator.
         template<typename T2>
         CounterRef<T> &operator = (CounterRef<T2> const &p_)
         {
            T *old = p; ++(p = static_cast<T2 *>(p_))->refCount;
            if(!--old->refCount) delete old;
            return *this;
         }

         /// Indirect member operator.
         T *operator -> () const {return p;}

         /// Dereference operator.
         T &operator * () const {return *p;}

         /// Returns the reference count of the associated object.
         unsigned refCount() const {return p->refCount;}

      private:
         T *p;
      };

      /// Reference-counted base class with no virtuals.

      /// Performs basic reference count management for construction and
      /// assignment.
      ///
      class CounterBase
      {
         GDCC_Core_CounterPreambleNoVirtual(
            GDCC::Core::CounterBase, GDCC::Core::CounterBase);

      protected:
         /// Default constructor.

         /// Initializes reference count to 0.
         ///
         CounterBase() : refCount{0} {}

         /// Copy constructor.

         /// Initializes reference count to 0.
         ///
         CounterBase(CounterBase const &) : refCount{0} {}

         /// Move constructor.

         /// Initializes reference count to 0.
         ///
         CounterBase(CounterBase &&) : refCount{0} {}

         /// Copy assignment.

         /// Leaves reference count unchanged.
         ///
         CounterBase &operator = (CounterBase const &) {return *this;}

         /// Move assignment.

         /// Leaves reference count unchanged.
         ///
         CounterBase &operator = (CounterBase &&) {return *this;}

         mutable unsigned refCount; ///< Reference count.


         [[noreturn]]
         static void NoClone(char const *msg);
      };

      /// Reference-counted base class.

      /// Works as in CounterBase, but includes a virtual destructor and the
      /// functions described in GDCC_Core_CounterPreable.
      ///
      class Counter : public CounterBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::Core::Counter, GDCC::Core::CounterBase);

      protected:
         /// Destructor.
         virtual ~Counter() {}
      };
   }
}

#endif//GDCC__Core__Counter_H__

