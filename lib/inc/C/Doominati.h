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
// DGE_CB_*
//
#if __GDCC__
#define DGE_CB(cb) (__glyph(unsigned, "{Callback}" #cb))
#else
#define DGE_CB(cb) 0
#endif
#define DGE_CB_Draw     DGE_CB(Draw)
#define DGE_CB_DrawPost DGE_CB(DrawPost)
#define DGE_CB_DrawPre  DGE_CB(DrawPre)
#define DGE_CB_Resize   DGE_CB(Resize)

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
// Object Member macros.
//
#if __GDCC__
#define DGE_OM(mem) (__glyph(unsigned, "{ObjectMember}" #mem))
#else
#define DGE_OM(mem) 0
#endif
#define DGE_OM_bvx      DGE_OM(bvx)
#define DGE_OM_bvy      DGE_OM(bvy)
#define DGE_OM_bvz      DGE_OM(bvz)
#define DGE_OM_damage   DGE_OM(damage)
#define DGE_OM_entfi    DGE_OM(entfi)
#define DGE_OM_entla    DGE_OM(entla)
#define DGE_OM_frictair DGE_OM(frictair)
#define DGE_OM_friction DGE_OM(friction)
#define DGE_OM_grabx    DGE_OM(grabx)
#define DGE_OM_graby    DGE_OM(graby)
#define DGE_OM_grabz    DGE_OM(grabz)
#define DGE_OM_gx       DGE_OM(gx)
#define DGE_OM_gy       DGE_OM(gy)
#define DGE_OM_gz       DGE_OM(gz)
#define DGE_OM_health   DGE_OM(health)
#define DGE_OM_id       DGE_OM(id)
#define DGE_OM_mass     DGE_OM(mass)
#define DGE_OM_next     DGE_OM(next)
#define DGE_OM_owner    DGE_OM(owner)
#define DGE_OM_pc       DGE_OM(pc)
#define DGE_OM_pitch    DGE_OM(pitch)
#define DGE_OM_prev     DGE_OM(prev)
#define DGE_OM_rsx      DGE_OM(rsx)
#define DGE_OM_rsy      DGE_OM(rsy)
#define DGE_OM_sprite   DGE_OM(sprite)
#define DGE_OM_sx       DGE_OM(sx)
#define DGE_OM_sy       DGE_OM(sy)
#define DGE_OM_sz       DGE_OM(sz)
#define DGE_OM_team     DGE_OM(team)
#define DGE_OM_teamfi   DGE_OM(teamfi)
#define DGE_OM_teamla   DGE_OM(teamla)
#define DGE_OM_teamne   DGE_OM(teamne)
#define DGE_OM_teampr   DGE_OM(teampr)
#define DGE_OM_texc     DGE_OM(texc)
#define DGE_OM_texf     DGE_OM(texf)
#define DGE_OM_vx       DGE_OM(vx)
#define DGE_OM_vy       DGE_OM(vy)
#define DGE_OM_vz       DGE_OM(vz)
#define DGE_OM_x        DGE_OM(x)
#define DGE_OM_xh       DGE_OM(xh)
#define DGE_OM_xl       DGE_OM(xl)
#define DGE_OM_y        DGE_OM(y)
#define DGE_OM_yh       DGE_OM(yh)
#define DGE_OM_yl       DGE_OM(yl)
#define DGE_OM_yaw      DGE_OM(yaw)
#define DGE_OM_z        DGE_OM(z)
#define DGE_OM_zh       DGE_OM(zh)
#define DGE_OM_zl       DGE_OM(zl)

//
// DGE_OT_*
//
// Object Type macros.
//
#if __GDCC__
#define DGE_OT(type) (__glyph(unsigned, "{ObjectType}" #type))
#else
#define DGE_OT(type) 0
#endif
#define DGE_OT_Object              DGE_OT(Object)
# define DGE_OT_Sector             DGE_OT(Sector)
# define DGE_OT_Team               DGE_OT(Team)
# define DGE_OT_Thinker            DGE_OT(Thinker)
#  define DGE_OT_PointThinker      DGE_OT(PointThinker)
#   define DGE_OT_RenderThinker    DGE_OT(RenderThinker)
#    define DGE_OT_PhysicsThinker  DGE_OT(PhysicsThinker)
#     define DGE_OT_Entity         DGE_OT(Entity)
#      define DGE_OT_MissileEntity DGE_OT(MissileEntity)

//
// DGE_Object_MemberGet
//
#define DGE_Object_MemberGet(T, ...) \
   _Generic((T)0, \
      int:         DGE_Object_MemberGetI, \
      DGE_ULFract: DGE_Object_MemberGetLA, \
      DGE_LFract:  DGE_Object_MemberGetLR, \
      unsigned:    DGE_Object_MemberGetU, \
        DGE_Accum: DGE_Object_MemberGetX \
   )(__VA_ARGS__)

//
// DGE_Object_MemberSet
//
#define DGE_Object_MemberSet(T, ...) \
   _Generic((T)0, \
      int:         DGE_Object_MemberSetI, \
      DGE_ULFract: DGE_Object_MemberSetLA, \
      DGE_LFract:  DGE_Object_MemberSetLR, \
      unsigned:    DGE_Object_MemberSetU, \
      DGE_Accum:   DGE_Object_MemberSetX \
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

//
// DGE_Accum
//
#if __GDCC__
typedef short _Accum DGE_Accum;
#else
typedef int DGE_Accum;
#endif

//
// DGE_CallbackType
//
typedef void (*DGE_CallbackType)(void) DGE_Callback;

//
// DGE_LFract
//
#if __GDCC__
typedef long _Fract DGE_LFract;
#else
typedef int DGE_LFract;
#endif

//
// DGE_String
//
#if __GDCC__
typedef __str DGE_String;
#else
typedef unsigned DGE_String;
#endif

//
// DGE_ULFract
//
#if __GDCC__
typedef unsigned long _Fract DGE_ULFract;
#else
typedef unsigned DGE_ULFract;
#endif

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
   DGE_Accum x, y;
} DGE_Point2;

typedef struct DGE_Point2I
{
   int x, y;
} DGE_Point2I;

struct DGE_Object;
 struct DGE_Sector;
 struct DGE_Team;
 struct DGE_Thinker;
  struct DGE_PointThinker;
   struct DGE_RenderThinker;
    struct DGE_PhysicsThinker;
     struct DGE_Entity;
      struct DGE_MissileEntity;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

DGE_Native unsigned DGE_BlockMap_Find(DGE_Accum xl, DGE_Accum yl, DGE_Accum xh, DGE_Accum yh);
DGE_Native unsigned DGE_BlockMap_FindAll(void);
DGE_Native unsigned DGE_BlockMap_FindCountSector(unsigned find);
DGE_Native unsigned DGE_BlockMap_FindCountThinker(unsigned find);
DGE_Native void DGE_BlockMap_FindFree(unsigned find);
DGE_Native unsigned DGE_BlockMap_FindGetSector(unsigned find, unsigned idx);
DGE_Native unsigned DGE_BlockMap_FindGetThinker(unsigned find, unsigned idx);
DGE_Native void DGE_BlockMap_Split(DGE_Accum minSize, unsigned maxObj);

DGE_Native void DGE_Callback_Register(unsigned cb, DGE_CallbackType fn);
DGE_Native void DGE_Callback_Unregister(unsigned cb, DGE_CallbackType fn);

DGE_Native unsigned DGE_CreateShaderData(DGE_String name, char const *frag, char const *vert);
DGE_Native unsigned DGE_CreateShaderFile(DGE_String name, char const *frag, char const *vert);

DGE_Native void DGE_DebugCallStk(void);
DGE_OptArgs(1)
DGE_Native void DGE_DebugDataStk(int dump);
DGE_Native void DGE_DebugLocReg(void);

DGE_Native void DGE_DopplerSpeed(DGE_Accum meterspersecond);

DGE_OptArgs(1)
DGE_Native void DGE_DrawColor(DGE_ULFract r, DGE_ULFract g, DGE_ULFract b, DGE_ULFract a);
DGE_Native void DGE_DrawCircle(DGE_Accum x, DGE_Accum y, DGE_Accum r);
DGE_Native void DGE_DrawCircleLine(DGE_Accum x, DGE_Accum y, DGE_Accum r);
DGE_Native void DGE_DrawCirclePrecision(int subdivisions);
DGE_Native void DGE_DrawEllipse(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2);
DGE_Native void DGE_DrawEllipseLine(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2);
DGE_Native void DGE_DrawRectangle(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2);
DGE_Native void DGE_DrawRectangleLine(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2);
DGE_Native void DGE_DrawShader(unsigned shd);
DGE_Native void DGE_DrawLine(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2);
DGE_Native void DGE_DrawLineSmooth(unsigned on);
DGE_Native void DGE_DrawLineWidth(int width);
DGE_Native void DGE_DrawText(DGE_Accum x, DGE_Accum y, char const *str);
DGE_Native void DGE_DrawTexture(unsigned tex);
DGE_Native void DGE_DrawTriangle(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2, DGE_Accum x3, DGE_Accum y3);
DGE_Native void DGE_DrawTriangleLine(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2, DGE_Accum x3, DGE_Accum y3);

DGE_Native unsigned DGE_Entity_Create(unsigned ext);

DGE_Native void DGE_FileClose(int fd);
DGE_Native int DGE_FileGet(int fd, unsigned idx);
DGE_Native int DGE_FileOpen(char const *name);
DGE_Native int DGE_FileRead(int fd, unsigned idx, char *buf, unsigned len);
DGE_Native unsigned DGE_FileSize(int fd);

DGE_Native void *DGE_FreestoreBegin(void);
DGE_Native void *DGE_FreestoreEnd(void);

DGE_Native DGE_LFract DGE_GetInputAxis(unsigned num, unsigned axis);
DGE_Native unsigned DGE_GetInputButtons(void);
DGE_Native DGE_Point2I DGE_GetInputCursor(void);
DGE_Native unsigned DGE_GetSound(DGE_String name);
DGE_Native unsigned DGE_GetShader(DGE_String name);
DGE_Native unsigned DGE_GetTexture(DGE_String name);

DGE_Native unsigned DGE_MissileEntity_Create(unsigned ext);

DGE_Native unsigned DGE_Object_Cast(unsigned id, unsigned type);
DGE_Native int DGE_Object_MemberGetI(unsigned id, unsigned mem);
DGE_Native DGE_ULFract DGE_Object_MemberGetLA(unsigned id, unsigned mem);
DGE_Native DGE_LFract DGE_Object_MemberGetLR(unsigned id, unsigned mem);
DGE_Native unsigned DGE_Object_MemberGetU(unsigned id, unsigned mem);
DGE_Native DGE_Accum DGE_Object_MemberGetX(unsigned id, unsigned mem);
DGE_Native void DGE_Object_MemberSetI(unsigned id, unsigned mem, int val);
DGE_Native void DGE_Object_MemberSetLA(unsigned id, unsigned mem, DGE_ULFract val);
DGE_Native void DGE_Object_MemberSetLR(unsigned id, unsigned mem, DGE_LFract val);
DGE_Native void DGE_Object_MemberSetU(unsigned id, unsigned mem, unsigned val);
DGE_Native void DGE_Object_MemberSetX(unsigned id, unsigned mem, DGE_Accum val);
DGE_Native void DGE_Object_RefAdd(unsigned id);
DGE_Native void DGE_Object_RefSub(unsigned id);

DGE_Native void DGE_PhysicsThinker_Block(unsigned id);
DGE_Native unsigned DGE_PhysicsThinker_Create(unsigned ext);
DGE_Native void DGE_PhysicsThinker_Unblock(unsigned id);

DGE_Native unsigned DGE_PointThinker_Create(unsigned ext);

DGE_Native void DGE_PrintChar(unsigned c);
DGE_Native void DGE_PrintWordD(unsigned d);

DGE_Native unsigned DGE_RenderThinker_Create(unsigned ext);

DGE_Native void DGE_Sector_Block(unsigned id);
DGE_Native void DGE_Sector_CalcBounds(unsigned id);
DGE_Native unsigned DGE_Sector_Create(unsigned pc, unsigned ext);
DGE_Native DGE_Point2 DGE_Sector_PointGet(unsigned id, unsigned p);
DGE_Native void DGE_Sector_PointSet(unsigned id, unsigned p, DGE_Point2 val);
DGE_Native void DGE_Sector_Unblock(unsigned id);

DGE_Native void DGE_SetViewpoint(unsigned id);
DGE_Native void DGE_SetVirtualResolution(unsigned w, unsigned h);
DGE_Native void DGE_SetWindowTitle(char const *str);

DGE_Native void DGE_SoundChanLoop(unsigned src, unsigned chan, unsigned on);
DGE_OptArgs(1)
DGE_Native void DGE_SoundChanPitch(unsigned src, unsigned chan, DGE_ULFract pitch);
DGE_Native void DGE_SoundChanPlay(unsigned src, unsigned chan);
DGE_Native void DGE_SoundChanPosition(unsigned src, unsigned chan, DGE_Accum x, DGE_Accum y, DGE_Accum z);
DGE_Native void DGE_SoundChanStop(unsigned src, unsigned chan);
DGE_Native void DGE_SoundChanVolume(unsigned src, unsigned chan, DGE_ULFract volume);

DGE_OptArgs(4)
DGE_Native void DGE_SoundListener(DGE_Accum x, DGE_Accum y, DGE_Accum z, DGE_Accum velx, DGE_Accum vely, DGE_Accum velz, DGE_Accum angle);

DGE_Native unsigned DGE_SoundSource(DGE_Accum x, DGE_Accum y, DGE_Accum z);
DGE_OptArgs(1)
DGE_Native unsigned DGE_SoundSrcBind(unsigned src, unsigned snd, unsigned chan);
DGE_Native void DGE_SoundSrcDestroy(unsigned src);
DGE_OptArgs(1)
DGE_Native unsigned DGE_SoundSrcPlay(unsigned src, unsigned snd, unsigned chan);
DGE_Native void DGE_SoundSrcPosition(unsigned src, DGE_Accum x, DGE_Accum y, DGE_Accum z);
DGE_Native void DGE_SoundSrcVelocity(unsigned src, DGE_Accum x, DGE_Accum y, DGE_Accum z);

DGE_Native DGE_String DGE_String_Create(char const *str, unsigned len);

DGE_Native unsigned DGE_SysRead(char *buf, unsigned len);
DGE_Native unsigned DGE_SysWrite(char const *buf, unsigned len);
DGE_Native unsigned DGE_SysWriteErr(char const *buf, unsigned len);

DGE_Native unsigned DGE_Task_Create(unsigned thread, DGE_CallbackType fn, ...);
DGE_Native void DGE_Task_Sleep(unsigned id, unsigned ticks);

DGE_Native unsigned DGE_Team_Create(unsigned ext);

DGE_OptArgs(1)
DGE_Native void DGE_TextAlignment(int h, int v);

DGE_Native unsigned DGE_Thinker_Head(void);
DGE_Native void DGE_Thinker_Insert(unsigned id);
DGE_Native void DGE_Thinker_Unlink(unsigned id);


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
   DGE_PropMem(DGE_LFract, frictair) \
   DGE_PropMem(DGE_LFract, friction) \
   DGE_PropMem(DGE_Accum,  gx) \
   DGE_PropMem(DGE_Accum,  gy) \
   DGE_PropMem(DGE_Accum,  gz) \
   DGE_PropMem(unsigned,   pc) \
   DGE_PropMem(unsigned,   texc) \
   DGE_PropMem(unsigned,   texf) \
   DGE_PropMem(DGE_Accum,  xh) \
   DGE_PropMem(DGE_Accum,  xl) \
   DGE_PropMem(DGE_Accum,  yh) \
   DGE_PropMem(DGE_Accum,  yl) \
   DGE_PropMem(DGE_Accum,  zh) \
   DGE_PropMem(DGE_Accum,  zl)
typedef struct DGE_Sector
{
   int id;

   DGE_SectorProps()
} DGE_Sector;

//
// DGE_Team
//
#define DGE_TeamProps() DGE_ObjectProps() \
   DGE_PropMem(unsigned, entfi) \
   DGE_PropMem(unsigned, entla) \
   DGE_PropMem(unsigned, owner) \
   DGE_PropMem(unsigned, teamfi) \
   DGE_PropMem(unsigned, teamla) \
   DGE_PropMem(unsigned, teamne) \
   DGE_PropMem(unsigned, teampr)
typedef struct DGE_Team
{
   int id;

   DGE_TeamProps()
} DGE_Team;

//
// DGE_Thinker
//
#define DGE_ThinkerProps() DGE_ObjectProps() \
   DGE_PropMem(unsigned, next) \
   DGE_PropMem(unsigned, prev)
typedef struct DGE_Thinker
{
   int id;

   DGE_ThinkerProps()
} DGE_Thinker;

//
// DGE_PointThinker
//
#define DGE_PointThinkerProps() DGE_ThinkerProps() \
   DGE_PropMem(DGE_ULFract, pitch) \
   DGE_PropMem(DGE_Accum,   x) \
   DGE_PropMem(DGE_Accum,   y) \
   DGE_PropMem(DGE_ULFract, yaw) \
   DGE_PropMem(DGE_Accum,   z)
typedef struct DGE_PointThinker
{
   int id;

   DGE_PointThinkerProps()
} DGE_PointThinker;

//
// DGE_RenderThinker
//
#define DGE_RenderThinkerProps() DGE_PointThinkerProps() \
   DGE_PropMem(DGE_Accum, rsx) \
   DGE_PropMem(DGE_Accum, rsy) \
   DGE_PropMem(unsigned,  sprite)
typedef struct DGE_RenderThinker
{
   int id;

   DGE_RenderThinkerProps()
} DGE_RenderThinker;

//
// DGE_PhysicsThinker
//
#define DGE_PhysicsThinkerProps() DGE_RenderThinkerProps() \
   DGE_PropMem(DGE_Accum,  bvx) \
   DGE_PropMem(DGE_Accum,  bvy) \
   DGE_PropMem(DGE_Accum,  bvz) \
   DGE_PropMem(DGE_LFract, friction) \
   DGE_PropMem(DGE_Accum,  grabx) \
   DGE_PropMem(DGE_Accum,  graby) \
   DGE_PropMem(DGE_Accum,  grabz) \
   DGE_PropMem(DGE_Accum,  mass) \
   DGE_PropMem(DGE_Accum,  sx) \
   DGE_PropMem(DGE_Accum,  sy) \
   DGE_PropMem(DGE_Accum,  sz) \
   DGE_PropMem(DGE_Accum,  vx) \
   DGE_PropMem(DGE_Accum,  vy) \
   DGE_PropMem(DGE_Accum,  vz)
typedef struct DGE_PhysicsThinker
{
   int id;

   DGE_PhysicsThinkerProps()
} DGE_PhysicsThinker;

//
// DGE_Entity
//
#define DGE_EntityProps() DGE_PhysicsThinkerProps() \
   DGE_PropMem(int,      health) \
   DGE_PropMem(unsigned, team) \
   DGE_PropMem(unsigned, teamne) \
   DGE_PropMem(unsigned, teampr)
typedef struct DGE_Entity
{
   int id;

   DGE_EntityProps()
} DGE_Entity;

//
// DGE_MissileEntity
//
#define DGE_MissileEntityProps() DGE_EntityProps() \
   DGE_PropMem(unsigned, damage) \
   DGE_PropMem(unsigned, owner)
typedef struct DGE_MissileEntity
{
   int id;

   DGE_MissileEntityProps()
} DGE_MissileEntity;

#endif//__GDCC_Header__C__Doominati_h__

