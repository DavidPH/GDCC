//-----------------------------------------------------------------------------
//
// Copyright(C) 2016-2017 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libdge.
//
// Doominati confirmed.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__Doominati_h__
#define __GDCC_Header__C__Doominati_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define DGE_GlobalSound 1

//
// DGE_Callback
//
#if __GDCC__
#define DGE_Callback [[call("StkCall")]]
#else
#define DGE_Callback
#endif

//
// DGE_Native
//
#if __GDCC__
#define DGE_Native [[call("Native")]]
#else
#define DGE_Native
#endif

//
// DGE_OM_*
//
// Object Member macros
//
#if __GDCC__
#define DGE_OM(mem) (__glyph(unsigned, "{ObjectMember}" #mem))
#else
#define DGE_OM(mem) 0
#endif
#define DGE_OM_damage DGE_OM(damage)
#define DGE_OM_health DGE_OM(health)
#define DGE_OM_id     DGE_OM(id)
#define DGE_OM_mass   DGE_OM(mass)
#define DGE_OM_next   DGE_OM(next)
#define DGE_OM_owner  DGE_OM(owner)
#define DGE_OM_pc     DGE_OM(pc)
#define DGE_OM_pitch  DGE_OM(pitch)
#define DGE_OM_prev   DGE_OM(prev)
#define DGE_OM_rsx    DGE_OM(rsx)
#define DGE_OM_rsy    DGE_OM(rsy)
#define DGE_OM_sprite DGE_OM(sprite)
#define DGE_OM_sx     DGE_OM(sx)
#define DGE_OM_sy     DGE_OM(sy)
#define DGE_OM_sz     DGE_OM(sz)
#define DGE_OM_texc   DGE_OM(texc)
#define DGE_OM_texf   DGE_OM(texf)
#define DGE_OM_vx     DGE_OM(vx)
#define DGE_OM_vy     DGE_OM(vy)
#define DGE_OM_vz     DGE_OM(vz)
#define DGE_OM_x      DGE_OM(x)
#define DGE_OM_xh     DGE_OM(xh)
#define DGE_OM_xl     DGE_OM(xl)
#define DGE_OM_y      DGE_OM(y)
#define DGE_OM_yh     DGE_OM(yh)
#define DGE_OM_yl     DGE_OM(yl)
#define DGE_OM_yaw    DGE_OM(yaw)
#define DGE_OM_z      DGE_OM(z)
#define DGE_OM_zh     DGE_OM(zh)
#define DGE_OM_zl     DGE_OM(zl)

//
// DGE_Object_MemberGet
//
#define DGE_Object_MemberGet(T, ...) \
   _Generic((T)0, \
        signed       int:    DGE_ObjectMemberGetI, \
      unsigned long  _Fract: DGE_ObjectMemberGetLA, \
      unsigned       int:    DGE_ObjectMemberGetU, \
        signed short _Accum: DGE_ObjectMemberGetX \
   )(__VA_ARGS__)

//
// DGE_Object_MemberSet
//
#define DGE_Object_MemberSet(T, ...) \
   _Generic((T)0, \
        signed       int:    DGE_ObjectMemberSetI, \
      unsigned long  _Fract: DGE_ObjectMemberSetLA, \
      unsigned       int:    DGE_ObjectMemberSetU, \
        signed short _Accum: DGE_ObjectMemberSetX \
   )(__VA_ARGS__)

//
// DGE_OptArgs
//
#if __GDCC__
#define DGE_OptArgs(n) [[optional_args(n)]]
#else
#define DGE_OptArgs(n)
#endif

//
// DGE_PropMem
//
#if __GDCC__
#define DGE_PropMem(T, name) \
   __prop name {__get: DGE_Object_MemberGet(T, ->id, DGE_OM_##name), \
                __set: DGE_Object_MemberSet(T, ->id, DGE_OM_##name)}
#else
#define DGE_PropMem(T, name) T name;
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

#if __GDCC__
typedef __str DGE_Str;
#else
typedef unsigned DGE_Str;
#endif

typedef void (*DGE_CallbackType)(void) DGE_Callback;

enum // DGE_Align
{
   DGE_Align_Keep,

   DGE_Align_Left = 1,
   DGE_Align_Right,
   DGE_Align_Center,
   DGE_Align_RightBox,
   DGE_Align_CenterBox,

   DGE_Align_Top = 1,
   DGE_Align_Bottom,
 //DGE_Align_Center,
};

enum // DGE_Axis
{
   DGE_Axis_X,
   DGE_Axis_Y,
};

enum // DGE_Button
{
   DGE_Button1 = 1 << 0,
   DGE_Button2 = 1 << 1,
   DGE_Button3 = 1 << 2,
   DGE_Button4 = 1 << 3,
   DGE_Button5 = 1 << 4,
   DGE_Button6 = 1 << 5,
   DGE_Button7 = 1 << 6,
   DGE_Button8 = 1 << 7,
};

typedef struct DGE_Point2
{
   short _Accum x, y;
} DGE_Point2;

typedef struct DGE_Point2I
{
   int x, y;
} DGE_Point2I;

struct DGE_Object;
 struct DGE_Sector;
 struct DGE_Thinker;
  struct DGE_PointThinker;
   struct DGE_RenderThinker;
    struct DGE_PhysicsThinker;
     struct DGE_Entity;
      struct DGE_MissileEntity;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

DGE_Native unsigned DGE_BlockMap_Find(short _Accum xl, short _Accum yl, short _Accum xh, short _Accum yh);
DGE_Native unsigned DGE_BlockMap_FindCountSector(unsigned find);
DGE_Native unsigned DGE_BlockMap_FindCountThinker(unsigned find);
DGE_Native void DGE_BlockMap_FindFree(unsigned find);
DGE_Native unsigned DGE_BlockMap_FindGetSector(unsigned find, unsigned idx);
DGE_Native unsigned DGE_BlockMap_FindGetThinker(unsigned find, unsigned idx);

DGE_Native void DGE_CallbackRegister(char const *name, DGE_CallbackType callback);
DGE_Native void DGE_CallbackUnregister(char const *name, DGE_CallbackType callback);

DGE_Native unsigned DGE_CreateEntity(unsigned ext);
DGE_Native unsigned DGE_CreateMissileEntity(unsigned ext);
DGE_Native unsigned DGE_CreatePhysicsThinker(unsigned ext);
DGE_Native unsigned DGE_CreatePointThinker(unsigned ext);
DGE_Native unsigned DGE_CreateSector(unsigned pc, unsigned ext);
DGE_Native unsigned DGE_CreateShaderData(DGE_Str name, char const *frag, char const *vert);
DGE_Native unsigned DGE_CreateShaderFile(DGE_Str name, char const *frag, char const *vert);

DGE_Native void DGE_DebugCallStk(void);
DGE_OptArgs(1)
DGE_Native void DGE_DebugDataStk(int dump);
DGE_Native void DGE_DebugLocReg(void);

DGE_Native void DGE_Delay(unsigned ticks);

DGE_Native void DGE_DopplerSpeed(short _Accum meterspersecond);

DGE_OptArgs(1)
DGE_Native void DGE_DrawColor(unsigned long _Fract r,
   unsigned long _Fract g, unsigned long _Fract b, unsigned long _Fract a);
DGE_Native void DGE_DrawCircle(short _Accum x, short _Accum y, short _Accum r);
DGE_Native void DGE_DrawCircleLine(short _Accum x, short _Accum y, short _Accum r);
DGE_Native void DGE_DrawCirclePrecision(int subdivisions);
DGE_Native void DGE_DrawEllipse(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2);
DGE_Native void DGE_DrawEllipseLine(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2);
DGE_Native void DGE_DrawRectangle(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2);
DGE_Native void DGE_DrawRectangleLine(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2);
DGE_Native void DGE_DrawShader(unsigned shd);
DGE_Native void DGE_DrawLine(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2);
DGE_Native void DGE_DrawLineSmooth(_Bool on);
DGE_Native void DGE_DrawLineWidth(int width);
DGE_Native void DGE_DrawText(short _Accum x, short _Accum y, char const *str);
DGE_Native void DGE_DrawTexture(unsigned tex);
DGE_Native void DGE_DrawTriangle(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2, short _Accum x3, short _Accum y3);
DGE_Native void DGE_DrawTriangleLine(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2, short _Accum x3, short _Accum y3);

DGE_Native void DGE_FileClose(int fd);
DGE_Native int DGE_FileGet(int fd, unsigned idx);
DGE_Native int DGE_FileOpen(char const *name);
DGE_Native int DGE_FileRead(int fd, unsigned idx, char *buf, unsigned len);
DGE_Native unsigned DGE_FileSize(int fd);

DGE_Native void *DGE_FreestoreBegin(void);
DGE_Native void *DGE_FreestoreEnd(void);

DGE_Native long _Fract DGE_GetInputAxis(unsigned num, unsigned axis);
DGE_Native unsigned DGE_GetInputButtons(void);
DGE_Native DGE_Point2I DGE_GetInputCursor(void);
DGE_Native unsigned DGE_GetSound(DGE_Str name);
DGE_Native unsigned DGE_GetShader(DGE_Str name);
DGE_Native unsigned DGE_GetTexture(DGE_Str name);

DGE_Native int DGE_ObjectMemberGetI(unsigned id, unsigned mem);
DGE_Native unsigned long _Fract DGE_ObjectMemberGetLA(unsigned id, unsigned mem);
DGE_Native unsigned DGE_ObjectMemberGetU(unsigned id, unsigned mem);
DGE_Native short _Accum DGE_ObjectMemberGetX(unsigned id, unsigned mem);
DGE_Native void DGE_ObjectMemberSetI(unsigned id, unsigned mem, int val);
DGE_Native void DGE_ObjectMemberSetLA(unsigned id, unsigned mem, unsigned long _Fract val);
DGE_Native void DGE_ObjectMemberSetU(unsigned id, unsigned mem, unsigned val);
DGE_Native void DGE_ObjectMemberSetX(unsigned id, unsigned mem, short _Accum val);
DGE_Native void DGE_ObjectRefAdd(unsigned id);
DGE_Native void DGE_ObjectRefSub(unsigned id);

DGE_Native void DGE_PhysicsThinkerBlock(unsigned id);
DGE_Native void DGE_PhysicsThinkerUnblock(unsigned id);

DGE_Native void DGE_PrintChar(unsigned c);
DGE_Native void DGE_PrintWordD(unsigned d);

DGE_Native void DGE_SectorBlock(unsigned id);
DGE_Native void DGE_SectorCalcBounds(unsigned id);
DGE_Native DGE_Point2 DGE_SectorPointGet(unsigned id, unsigned p);
DGE_Native void DGE_SectorPointSet(unsigned id, unsigned p, DGE_Point2 val);
DGE_Native void DGE_SectorUnblock(unsigned id);

DGE_Native void DGE_SetViewpoint(unsigned id);
DGE_Native void DGE_SetVirtualResolution(unsigned w, unsigned h);
DGE_Native void DGE_SetWindowTitle(char const *str);

DGE_Native void DGE_SoundChanLoop(unsigned src, unsigned chan, _Bool on);
DGE_OptArgs(1)
DGE_Native void DGE_SoundChanPitch(unsigned src, unsigned chan, unsigned long _Fract pitch);
DGE_Native void DGE_SoundChanPlay(unsigned src, unsigned chan);
DGE_Native void DGE_SoundChanPosition(unsigned src, unsigned chan, short _Accum x, short _Accum y, short _Accum z);
DGE_Native void DGE_SoundChanStop(unsigned src, unsigned chan);
DGE_Native void DGE_SoundChanVolume(unsigned src, unsigned chan, unsigned long _Fract volume);

DGE_OptArgs(4)
DGE_Native void DGE_SoundListener(short _Accum x, short _Accum y, short _Accum z, short _Accum velx, short _Accum vely, short _Accum velz, short _Accum angle);

DGE_Native unsigned DGE_SoundSource(short _Accum x, short _Accum y, short _Accum z);
DGE_OptArgs(1)
DGE_Native unsigned DGE_SoundSrcBind(unsigned src, unsigned snd, unsigned chan);
DGE_Native void DGE_SoundSrcDestroy(unsigned src);
DGE_OptArgs(1)
DGE_Native unsigned DGE_SoundSrcPlay(unsigned src, unsigned snd, unsigned chan);
DGE_Native void DGE_SoundSrcPosition(unsigned src, short _Accum x, short _Accum y, short _Accum z);
DGE_Native void DGE_SoundSrcVelocity(unsigned src, short _Accum x, short _Accum y, short _Accum z);

DGE_Native unsigned DGE_SysRead(char *buf, unsigned len);
DGE_Native unsigned DGE_SysWrite(char const *buf, unsigned len);
DGE_Native unsigned DGE_SysWriteErr(char const *buf, unsigned len);

DGE_Native unsigned DGE_Task_Create(unsigned thread, DGE_CallbackType fn, ...);

DGE_OptArgs(1)
DGE_Native void DGE_TextAlignment(int h, int v);

DGE_Native unsigned DGE_ThinkerCap(void);
DGE_Native void DGE_ThinkerUnlink(unsigned id);


//----------------------------------------------------------------------------|
// Property Types                                                             |
//

//
// DGE_Object
//
#define DGE_ObjectProps()
typedef struct DGE_Object
{
   int id;

   DGE_ObjectProps()
} DGE_Object;

//
// DGE_Sector
//
#define DGE_SectorProps() DGE_ObjectProps() \
   DGE_PropMem(unsigned       int,    pc) \
   DGE_PropMem(unsigned       int,    texc) \
   DGE_PropMem(unsigned       int,    texf) \
   DGE_PropMem(  signed short _Accum, xh) \
   DGE_PropMem(  signed short _Accum, xl) \
   DGE_PropMem(  signed short _Accum, yh) \
   DGE_PropMem(  signed short _Accum, yl) \
   DGE_PropMem(  signed short _Accum, zh) \
   DGE_PropMem(  signed short _Accum, zl)
typedef struct DGE_Sector
{
   int id;

   DGE_SectorProps()
} DGE_Sector;

//
// DGE_Thinker
//
#define DGE_ThinkerProps() DGE_ObjectProps() \
   DGE_PropMem(unsigned int, next) \
   DGE_PropMem(unsigned int, prev)
typedef struct DGE_Thinker
{
   int id;

   DGE_ThinkerProps()
} DGE_Thinker;

//
// DGE_PointThinker
//
#define DGE_PointThinkerProps() DGE_ThinkerProps() \
   DGE_PropMem(unsigned long  _Fract, pitch) \
   DGE_PropMem(  signed short _Accum, x) \
   DGE_PropMem(  signed short _Accum, y) \
   DGE_PropMem(unsigned long  _Fract, yaw) \
   DGE_PropMem(  signed short _Accum, z)
typedef struct DGE_PointThinker
{
   int id;

   DGE_PointThinkerProps()
} DGE_PointThinker;

//
// DGE_RenderThinker
//
#define DGE_RenderThinkerProps() DGE_PointThinkerProps() \
   DGE_PropMem(  signed short _Accum, rsx) \
   DGE_PropMem(  signed short _Accum, rsy) \
   DGE_PropMem(unsigned       int,    sprite)
typedef struct DGE_RenderThinker
{
   int id;

   DGE_RenderThinkerProps()
} DGE_RenderThinker;

//
// DGE_PhysicsThinker
//
#define DGE_PhysicsThinkerProps() DGE_RenderThinkerProps() \
   DGE_PropMem(unsigned       int,    mass) \
   DGE_PropMem(  signed short _Accum, sx) \
   DGE_PropMem(  signed short _Accum, sy) \
   DGE_PropMem(  signed short _Accum, sz) \
   DGE_PropMem(  signed short _Accum, vx) \
   DGE_PropMem(  signed short _Accum, vy) \
   DGE_PropMem(  signed short _Accum, vz)
typedef struct DGE_PhysicsThinker
{
   int id;

   DGE_PhysicsThinkerProps()
} DGE_PhysicsThinker;

//
// DGE_Entity
//
#define DGE_EntityProps() DGE_PhysicsThinkerProps() \
   DGE_PropMem(signed int, health)
typedef struct DGE_Entity
{
   int id;

   DGE_EntityProps()
} DGE_Entity;

//
// DGE_MissileEntity
//
#define DGE_MissileEntityProps() DGE_EntityProps() \
   DGE_PropMem(unsigned int, damage) \
   DGE_PropMem(unsigned int, owner)
typedef struct DGE_MissileEntity
{
   int id;

   DGE_MissileEntityProps()
} DGE_MissileEntity;

#endif//__GDCC_Header__C__Doominati_h__

