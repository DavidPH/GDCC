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

//
// GDCC_Core_CounterPreamble
//
#define GDCC_Core_CounterPreamble(type, base) \
private: \
   virtual type *cloneRaw() const {return new type(*this);} \
   GDCC_Core_CounterPreambleCommon(type, base)

//
// GDCC_Core_CounterPreambleAbstract
//
#define GDCC_Core_CounterPreambleAbstract(type, base) \
private: \
   virtual type *cloneRaw() const = 0; \
   GDCC_Core_CounterPreambleCommon(type, base)

//
// GDCC_Core_CounterPreambleNoClone
//
#define GDCC_Core_CounterPreambleNoClone(type, base) \
private: \
   virtual type *cloneRaw() const {NoClone("bad clone: " #type);} \
   GDCC_Core_CounterPreambleCommon(type, base)

//
// GDCC_Core_CounterPreambleNoVirtual
//
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

      //
      // CounterPtr
      //
      template<typename T> class CounterPtr
      {
      public:
         // nullptr -> CounterPtr
         CounterPtr(decltype(nullptr) p_ = nullptr) : p{p_} {}

         // pointer -> CounterPtr
         CounterPtr(T *p_) : p{static_cast<T *>(p_)} {if(p) ++p->refCount;}
         template<typename T2>
         CounterPtr(T2 *p_) : p{static_cast<T *>(p_)} {if(p) ++p->refCount;}

         // CounterPtr -> CounterPtr
         CounterPtr(CounterPtr<T> const &p_) :
            p{static_cast<T *>(p_)} {if(p) ++p->refCount;}
         template<typename T2>
         CounterPtr(CounterPtr<T2> const &p_) :
            p{static_cast<T *>(p_)} {if(p) ++p->refCount;}

         // CounterRef -> CounterPtr
         CounterPtr(CounterRef<T> const &p_) :
            p{static_cast<T *>(p_)} {++p->refCount;}
         template<typename T2>
         CounterPtr(CounterRef<T2> const &p_) :
            p{static_cast<T *>(p_)} {++p->refCount;}

         ~CounterPtr() {if(p && !--p->refCount) delete p;}

         operator T * () const {return p;}

         // CounterPtr = nullptr
         CounterPtr<T> &operator = (decltype(nullptr) p_)
         {
            if(p && !--p->refCount) delete p; p = p_;
            return *this;
         }

         // CounterPtr = pointer
         CounterPtr<T> &operator = (T *p_)
         {
            T *old = p; if((p = p_)) ++p->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }
         template<typename T2>
         CounterPtr<T> &operator = (T2 *p_)
         {
            T *old = p; if((p = p_)) ++p->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }

         // CounterPtr = CounterPtr
         CounterPtr<T> &operator = (CounterPtr<T> const &p_)
         {
            T *old = p; if((p = static_cast<T *>(p_))) ++p->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }
         template<typename T2>
         CounterPtr<T> &operator = (CounterPtr<T2> const &p_)
         {
            T *old = p; if((p = static_cast<T *>(p_))) ++p->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }

         // CounterPtr = CounterRef
         CounterPtr<T> &operator = (CounterRef<T> const &p_)
         {
            T *old = p; ++(p = static_cast<T *>(p_))->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }
         template<typename T2>
         CounterPtr<T> &operator = (CounterRef<T2> const &p_)
         {
            T *old = p; ++(p = static_cast<T *>(p_))->refCount;
            if(old && !--old->refCount) delete old;
            return *this;
         }

         T *operator -> () const {return p;}

         T &operator * () const {return *p;}

         unsigned refCount() const {return p ? p->refCount : 0;}

      private:
         T *p;
      };

      //
      // CounterRef
      //
      template<typename T> class CounterRef
      {
      public:
         // nullptr -> CounterRef
         CounterRef(decltype(nullptr) p_ = nullptr) = delete;

         // pointer -> CounterRef
         explicit CounterRef(T *p_) : p(static_cast<T *>(p_)) {++p->refCount;}
         template<typename T2>
         explicit CounterRef(T2 *p_) : p(static_cast<T *>(p_)) {++p->refCount;}

         // CounterPtr -> CounterRef
         explicit CounterRef(CounterPtr<T> const &p_) :
            p(static_cast<T *>(p_)) {++p->refCount;}
         template<typename T2>
         explicit CounterRef(CounterPtr<T2> const &p_) :
            p(static_cast<T *>(p_)) {++p->refCount;}

         // CounterRef -> CounterRef
         CounterRef(CounterRef<T> const &p_) :
            p(static_cast<T *>(p_)) {++p->refCount;}
         template<typename T2>
         CounterRef(CounterRef<T2> const &p_) :
            p(static_cast<T *>(p_)) {++p->refCount;}

         ~CounterRef() {if(!--p->refCount) delete p;}

         operator T * () const {return p;}

         // CounterRef = CounterRef
         CounterRef<T> &operator = (CounterRef<T> const &p_)
         {
            T *old = p; ++(p = static_cast<T *>(p_))->refCount;
            if(!--old->refCount) delete old;
            return *this;
         }
         template<typename T2>
         CounterRef<T> &operator = (CounterRef<T2> const &p_)
         {
            T *old = p; ++(p = static_cast<T2 *>(p_))->refCount;
            if(!--old->refCount) delete old;
            return *this;
         }

         T *operator -> () const {return p;}

         T &operator * () const {return *p;}

         unsigned refCount() const {return p->refCount;}

      private:
         T *p;
      };

      //
      // CounterBase
      //
      class CounterBase
      {
         GDCC_Core_CounterPreambleNoVirtual(
            GDCC::Core::CounterBase, GDCC::Core::CounterBase);

      protected:
         CounterBase() : refCount{0} {}
         CounterBase(CounterBase const &) : refCount{0} {}
         CounterBase(CounterBase &&) : refCount{0} {}

         CounterBase &operator = (CounterBase const &) {return *this;}
         CounterBase &operator = (CounterBase &&) {return *this;}

         mutable unsigned refCount;


         [[noreturn]]
         static void NoClone(char const *msg);
      };

      //
      // Counter
      //
      class Counter : public CounterBase
      {
         GDCC_Core_CounterPreamble(
            GDCC::Core::Counter, GDCC::Core::CounterBase);

      protected:
         virtual ~Counter() {}
      };
   }
}

#endif//GDCC__Core__Counter_H__

