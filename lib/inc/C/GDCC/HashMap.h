//-----------------------------------------------------------------------------
//
// Copyright(C) 2017 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Generic HashMap generator.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__GDCC__HashMap_h__
#define __GDCC_Header__C__GDCC__HashMap_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_HashMap_Decl
//
// Required functions:
//   Key    name##_GetKey (T *obj);
//   T    **name##_GetNext(T *obj);
//   T   ***name##_GetPrev(T *obj);
//   size_t name##_HashKey(Key key);
//   size_t name##_HashObj(T *obj);
//   int    name##_KeyCmp (Key l, Key r);
//
// These may be implemented as macros, but must evaluate their arguments
// exactly once.
//
#define GDCC_HashMap_Decl(name, Key, T) \
   typedef struct name name; \
   \
   void   name##_ctor(name *map, size_t chainC, size_t growC); \
   void   name##_dtor(name *map); \
   \
   void   name##_Clear (name *map); \
   T     *name##_Find  (name *map, Key key); \
   void   name##_Insert(name *map, T *obj); \
   void   name##_Resize(name *map, size_t count); \
   void   name##_Unlink(name *map, T *obj); \
   \
   GDCC_HashMap_DeclStruct(name, Key, T)

//
// GDCC_HashMap_DeclStruct
//
#if __GDCC__ // GDCC properties version.
# define GDCC_HashMap_DeclStruct(name, Key, T) \
   struct name \
   { \
      __prop clear  {call: name##_Clear(this)} \
      __prop find   {call: name##_Find(this)} \
      __prop insert {call: name##_Insert(this)} \
      __prop resize {call: name##_Resize(this)} \
      __prop unlink {call: name##_Unlink(this)} \
      \
      T    **chainV; \
      size_t chainC; \
      \
      size_t objC; \
      size_t growC; \
   };
#else // Generic C version.
# define GDCC_HashMap_DeclStruct(name, Key, T) \
   struct name \
   { \
      T    **chainV; \
      size_t chainC; \
      \
      size_t objC; \
      size_t growC; \
   };
#endif

//
// GDCC_HashMap_Defn
//
#define GDCC_HashMap_Defn(name, Key, T) \
   GDCC_HashMap_Defn_ctor(name, Key, T) \
   GDCC_HashMap_Defn_dtor(name, Key, T) \
   \
   GDCC_HashMap_Defn_Clear( name, Key, T) \
   GDCC_HashMap_Defn_Find(  name, Key, T) \
   GDCC_HashMap_Defn_Insert(name, Key, T) \
   GDCC_HashMap_Defn_Resize(name, Key, T) \
   GDCC_HashMap_Defn_Unlink(name, Key, T) \

//
// GDCC_HashMap_Defn_ctor
//
#define GDCC_HashMap_Defn_ctor(name, Key, T) \
   void name##_ctor(name *map, size_t chainC, size_t growC) \
   { \
      map->chainV = malloc(sizeof(T *) * chainC); \
      map->chainC = chainC; \
      map->objC   = 0; \
      map->growC  = growC; \
      \
      for(T **chain = map->chainV, **chainE = chain + chainC; chain != chainE; ++chain) \
         *chain = NULL; \
   }

//
// GDCC_HashMap_Defn_dtor
//
#define GDCC_HashMap_Defn_dtor(name, Key, T) \
   void name##_dtor(name *map) \
   { \
      free(map->chainV); \
   }

//
// GDCC_HashMap_Defn_Clear
//
#define GDCC_HashMap_Defn_Clear(name, Key, T) \
   void name##_Clear(name *map) \
   { \
      for(T **chain = map->chainV, **chainE = chain + map->chainC; \
         chain != chainE; ++chain) \
      { \
         while(*chain) \
         { \
            T  **next = name##_GetNext(*chain); \
            T ***prev = name##_GetPrev(*chain); \
            \
            if((**prev = *next)) \
               *name##_GetPrev(*next) = *prev; \
            \
            *next = NULL; \
            *prev = NULL; \
         } \
      } \
      \
      map->objC = 0; \
   }

//
// GDCC_HashMap_Defn_Find
//
#define GDCC_HashMap_Defn_Find(name, Key, T) \
   T *name##_Find(name *map, Key key) \
   { \
      for(T *obj = map->chainV[name##_HashKey(key) % map->chainC]; obj; \
         obj = *name##_GetNext(obj)) \
      { \
         if(!name##_KeyCmp(key, name##_GetKey(obj))) \
            return obj; \
      } \
      \
      return NULL; \
   }

//
// GDCC_HashMap_Defn_Insert
//
#define GDCC_HashMap_Defn_Insert(name, Key, T) \
   void name##_Insert(name *map, T *obj) \
   { \
      if(map->objC >= map->chainC) \
         name##_Resize(map, map->chainC + map->chainC / 2 + map->growC); \
      \
      ++map->objC; \
      \
      T **head = &map->chainV[name##_HashObj(obj) % map->chainC]; \
      T **next = name##_GetNext(obj); \
      \
      if((*next = *head)) \
         *name##_GetPrev(*head) = next; \
      *name##_GetPrev(obj) = head; \
      *head = obj; \
   }

//
// GDCC_HashMap_Defn_Resize
//
#define GDCC_HashMap_Defn_Resize(name, Key, T) \
   void name##_Resize(name *map, size_t count) \
   { \
      \
      T    **oldChainV = map->chainV; \
      size_t oldChainC = map->chainC; \
      \
      map->chainV = malloc(sizeof(T *) * count); \
      map->chainC = count; \
      \
      for(T **chain = map->chainV, **chainE = chain + count; chain != chainE; ++chain) \
         *chain = NULL; \
      \
      /* Relink elements. */ \
      for(T **chain = oldChainV, **chainE = chain + oldChainC; chain != chainE; ++chain) \
      { \
         while(*chain) \
         { \
            T   *obj  = *chain; \
            T  **next = name##_GetNext(obj); \
            T ***prev = name##_GetPrev(obj); \
            \
            /* Unlink from old chain. */ \
            if((**prev = *next)) \
               *name##_GetPrev(*next) = *prev; \
            \
            /* Insert into new chain. */ \
            T **head = &map->chainV[name##_HashObj(obj) % map->chainC]; \
            \
            if((*next = *head)) \
               *name##_GetPrev(*head) = next; \
            *name##_GetPrev(obj) = head; \
            *head = obj; \
         } \
      } \
      \
      free(oldChainV); \
   }

//
// GDCC_HashMap_Defn_Unlink
//
#define GDCC_HashMap_Defn_Unlink(name, Key, T) \
   void name##_Unlink(name *map, T *obj) \
   { \
      --map->objC; \
      \
      T  **next = name##_GetNext(obj); \
      T ***prev = name##_GetPrev(obj); \
      \
      if((**prev = *next)) \
         *name##_GetPrev(*next) = *prev; \
      \
      *next = NULL; \
      *prev = NULL; \
   }

#endif//__GDCC_Header__C__GDCC__HashMap_h__

