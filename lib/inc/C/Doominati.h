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
#define DGE_OM_xl       DGE_OM(xl)
#define DGE_OM_xu       DGE_OM(xu)
#define DGE_OM_y        DGE_OM(y)
#define DGE_OM_yl       DGE_OM(yl)
#define DGE_OM_yu       DGE_OM(yu)
#define DGE_OM_yaw      DGE_OM(yaw)
#define DGE_OM_z        DGE_OM(z)
#define DGE_OM_zl       DGE_OM(zl)
#define DGE_OM_zu       DGE_OM(zu)

//
// DGE_OME_*
//
// Object Member Extension macros.
//
#if __GDCC__
#define DGE_OME(mem) (__glyph(unsigned, "{ObjectMemberExt}" #mem))
#else
#define DGE_OME(mem) 0
#endif
#define DGE_OME_Object              DGE_OME(Object)
# define DGE_OME_Sector             DGE_OME(Sector)
# define DGE_OME_Team               DGE_OME(Team)
# define DGE_OME_Thinker            DGE_OME(Thinker)
#  define DGE_OME_ParticleSys       DGE_OME(ParticleSys)
#  define DGE_OME_PointThinker      DGE_OME(PointThinker)
#   define DGE_OME_RenderThinker    DGE_OME(RenderThinker)
#    define DGE_OME_PhysicsThinker  DGE_OME(PhysicsThinker)
#     define DGE_OME_Entity         DGE_OME(Entity)
#      define DGE_OME_MissileEntity DGE_OME(MissileEntity)

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
#  define DGE_OT_ParticleSys       DGE_OT(ParticleSys)
#  define DGE_OT_PointThinker      DGE_OT(PointThinker)
#   define DGE_OT_RenderThinker    DGE_OT(RenderThinker)
#    define DGE_OT_PhysicsThinker  DGE_OT(PhysicsThinker)
#     define DGE_OT_Entity         DGE_OT(Entity)
#      define DGE_OT_MissileEntity DGE_OT(MissileEntity)

//
// DGE_Object_MemberGetT
//
#define DGE_Object_MemberGetT(T, id, mem) \
   ((T (*)(unsigned, unsigned, unsigned) DGE_Native)DGE_Object_MemberGet) \
      (id, mem, sizeof(T))

//
// DGE_Object_MemberSetT
//
#define DGE_Object_MemberSetT(T, id, mem, ...) \
   ((void (*)(unsigned, unsigned, T) DGE_Native)DGE_Object_MemberSet) \
      (id, mem, __VA_ARGS__)

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
   __prop name {__get: DGE_Object_MemberGetT(T, ->id, DGE_OM_##name), \
                __set: DGE_Object_MemberSetT(T, ->id, DGE_OM_##name, __arg)}
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

typedef struct DGE_Color
{
   DGE_ULFract r, g, b, a;
} DGE_Color;

typedef struct DGE_Particle
{
   int        life;
   DGE_Point2 pos;
   DGE_Point2 vel;
   DGE_Point2 accel;
   DGE_Point2 size;
   DGE_Point2 sizedest;
   DGE_Accum  sizespeed;
   DGE_Color  color;
   DGE_Color  colordest;
   DGE_Accum  colorspeed;
   DGE_Accum  rot;
   DGE_Accum  rotspeed;
} DGE_Particle;

struct DGE_Object;
 struct DGE_Sector;
 struct DGE_Team;
 struct DGE_Thinker;
  struct DGE_ParticleSys;
  struct DGE_PointThinker;
   struct DGE_RenderThinker;
    struct DGE_PhysicsThinker;
     struct DGE_Entity;
      struct DGE_MissileEntity;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

DGE_Native void DGE_Audio_SetDopplerSpeed(DGE_Accum meterspersecond);
DGE_OptArgs(4)
DGE_Native void DGE_Audio_SetListener(DGE_Accum x, DGE_Accum y, DGE_Accum z, DGE_Accum velx, DGE_Accum vely, DGE_Accum velz, DGE_Accum angle);

DGE_Native unsigned DGE_BlockMap_Find(DGE_Accum xl, DGE_Accum yl, DGE_Accum xu, DGE_Accum yu);
DGE_Native unsigned DGE_BlockMap_FindAll(void);
DGE_Native unsigned DGE_BlockMap_FindCountSector(unsigned find);
DGE_Native unsigned DGE_BlockMap_FindCountThinker(unsigned find);
DGE_Native void DGE_BlockMap_FindFree(unsigned find);
DGE_Native unsigned DGE_BlockMap_FindGetSector(unsigned find, unsigned idx);
DGE_Native unsigned DGE_BlockMap_FindGetThinker(unsigned find, unsigned idx);
DGE_Native void DGE_BlockMap_Split(DGE_Accum minSize, unsigned maxObj);

DGE_Native void DGE_Callback_Register(unsigned cb, DGE_CallbackType fn);
DGE_Native void DGE_Callback_Unregister(unsigned cb, DGE_CallbackType fn);

DGE_Native void DGE_DebugCallStk(void);
DGE_OptArgs(1)
DGE_Native void DGE_DebugDataStk(int dump);
DGE_Native void DGE_DebugLocReg(void);

DGE_Native void DGE_Draw_Circle(DGE_Accum x, DGE_Accum y, DGE_Accum r);
DGE_Native void DGE_Draw_CircleLine(DGE_Accum x, DGE_Accum y, DGE_Accum r);
DGE_Native void DGE_Draw_CirclePrecision(int subdivisions);
DGE_Native void DGE_Draw_Ellipse(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2);
DGE_Native void DGE_Draw_EllipseLine(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2);
DGE_OptArgs(1)
DGE_Native void DGE_Draw_Rectangle(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2, DGE_ULFract rot);
DGE_OptArgs(1)
DGE_Native void DGE_Draw_RectangleLine(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2, DGE_ULFract rot);
DGE_OptArgs(4)
DGE_Native void DGE_Draw_SetColor(DGE_ULFract r, DGE_ULFract g, DGE_ULFract b, DGE_ULFract a);
DGE_OptArgs(1)
DGE_Native void DGE_Draw_SetTextAlign(int h, int v);
DGE_Native void DGE_Draw_Line(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2);
DGE_Native void DGE_Draw_LineSmooth(unsigned on);
DGE_Native void DGE_Draw_LineWidth(int width);
DGE_OptArgs(1)
DGE_Native DGE_Point2 DGE_Draw_Text(DGE_Accum x, DGE_Accum y, char const *str, DGE_Accum maxwidth);
DGE_Native void DGE_Draw_Triangle(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2, DGE_Accum x3, DGE_Accum y3);
DGE_Native void DGE_Draw_TriangleLine(DGE_Accum x1, DGE_Accum y1, DGE_Accum x2, DGE_Accum y2, DGE_Accum x3, DGE_Accum y3);

DGE_Native unsigned DGE_Entity_Create(unsigned ext);

DGE_Native void DGE_FileClose(int fd);
DGE_Native int DGE_FileGet(int fd, unsigned idx);
DGE_Native int DGE_FileOpen(char const *name);
DGE_Native int DGE_FileRead(int fd, unsigned idx, char *buf, unsigned len);
DGE_Native unsigned DGE_FileSize(int fd);

DGE_Native void DGE_Font_Bind(unsigned fnt);
DGE_Native unsigned DGE_Font_Create(DGE_String name, char const *fname, int ptSize);
DGE_Native unsigned DGE_Font_Get(DGE_String name);
DGE_Native DGE_Point2 DGE_Font_GetTextSize(unsigned fnt, char const *text);

DGE_Native void *DGE_FreestoreBegin(void);
DGE_Native void *DGE_FreestoreEnd(void);

DGE_Native DGE_LFract DGE_GetInputAxis(unsigned num, unsigned axis);
DGE_Native unsigned DGE_GetInputButtons(void);
DGE_Native DGE_Point2I DGE_GetInputCursor(void);

DGE_Native unsigned DGE_MissileEntity_Create(unsigned ext);

DGE_Native unsigned DGE_Object_Cast(unsigned id, unsigned type);
DGE_Native unsigned DGE_Object_MemberGet(unsigned id, unsigned mem, unsigned len);
DGE_Native void DGE_Object_MemberSet(unsigned id, unsigned mem, unsigned len, ...);
DGE_Native void DGE_Object_RefAdd(unsigned id);
DGE_Native void DGE_Object_RefSub(unsigned id);

DGE_Native void DGE_ParticleSys_Add(unsigned id, DGE_Particle prt);
DGE_Native unsigned DGE_ParticleSys_Create(unsigned ext, unsigned npr);

DGE_Native void DGE_PhysicsThinker_ApplyFriction(unsigned id, DGE_LFract f);
DGE_Native void DGE_PhysicsThinker_Block(unsigned id);
DGE_OptArgs(1)
DGE_Native unsigned DGE_PhysicsThinker_Collide(unsigned id, DGE_Accum *oldx,
   DGE_Accum *oldy, DGE_Accum *oldz, DGE_LFract *fric);
DGE_Native unsigned DGE_PhysicsThinker_Create(unsigned ext);
DGE_Native void DGE_PhysicsThinker_Unblock(unsigned id);

DGE_Native unsigned DGE_PointThinker_Create(unsigned ext);

DGE_Native void DGE_PrintChar(unsigned c);
DGE_Native void DGE_PrintWordD(unsigned d);

DGE_Native DGE_Point2 DGE_Renderer_GetViewpoint(void);
DGE_Native void DGE_Renderer_SetViewpoint(unsigned id);
DGE_Native void DGE_Renderer_SetVirtualRes(unsigned w, unsigned h);

DGE_Native unsigned DGE_RenderThinker_Create(unsigned ext);

DGE_Native void DGE_Sector_Block(unsigned id);
DGE_Native void DGE_Sector_CalcBounds(unsigned id);
DGE_Native unsigned DGE_Sector_Create(unsigned pc, unsigned ext);
DGE_Native DGE_Point2 DGE_Sector_PointGet(unsigned id, unsigned p);
DGE_Native void DGE_Sector_PointSet(unsigned id, unsigned p, DGE_Point2 val);
DGE_Native void DGE_Sector_Unblock(unsigned id);

DGE_Native void DGE_Shader_Bind(unsigned shd);
DGE_Native unsigned DGE_Shader_CreateData(DGE_String name, char const *frag, char const *vert);
DGE_Native unsigned DGE_Shader_CreateFile(DGE_String name, char const *frag, char const *vert);
DGE_Native unsigned DGE_Shader_Get(DGE_String name);

DGE_Native unsigned DGE_Sound_Get(DGE_String name);
DGE_Native void DGE_Sound_Play(unsigned src, unsigned chan);
DGE_Native void DGE_Sound_SetLoop(unsigned src, unsigned chan, unsigned on);
DGE_OptArgs(1)
DGE_Native DGE_Accum DGE_Sound_SetPitch(unsigned src, unsigned chan, DGE_ULFract pitch);
DGE_Native void DGE_Sound_SetPos(unsigned src, unsigned chan, DGE_Accum x, DGE_Accum y, DGE_Accum z);
DGE_Native void DGE_Sound_SetVolume(unsigned src, unsigned chan, DGE_ULFract volume);
DGE_Native void DGE_Sound_Stop(unsigned src, unsigned chan);

DGE_OptArgs(1)
DGE_Native unsigned DGE_SoundSrc_Bind(unsigned src, unsigned snd, unsigned chan);
DGE_Native unsigned DGE_SoundSrc_Create(DGE_Accum x, DGE_Accum y, DGE_Accum z);
DGE_Native void DGE_SoundSrc_Destroy(unsigned src);
DGE_OptArgs(1)
DGE_Native unsigned DGE_SoundSrc_Play(unsigned src, unsigned snd, unsigned chan);
DGE_Native void DGE_SoundSrc_SetPos(unsigned src, DGE_Accum x, DGE_Accum y, DGE_Accum z);
DGE_Native void DGE_SoundSrc_SetVel(unsigned src, DGE_Accum x, DGE_Accum y, DGE_Accum z);

DGE_Native DGE_String DGE_String_Create(char const *str, unsigned len);

DGE_Native unsigned DGE_SysRead(char *buf, unsigned len);
DGE_Native unsigned DGE_SysWrite(char const *buf, unsigned len);
DGE_Native unsigned DGE_SysWriteErr(char const *buf, unsigned len);

DGE_Native unsigned DGE_Task_Create(unsigned thread, DGE_CallbackType fn, ...);
DGE_Native void DGE_Task_Sleep(unsigned id, unsigned ticks);

DGE_Native unsigned DGE_Team_Create(unsigned ext);

DGE_Native void DGE_Texture_Bind(unsigned tex);
DGE_Native unsigned DGE_Texture_Create(DGE_String name, char const *fname);
DGE_Native unsigned DGE_Texture_Get(DGE_String name);
DGE_Native DGE_Point2I DGE_Texture_GetSize(unsigned tex);

DGE_Native unsigned DGE_Thinker_Head(void);
DGE_Native void DGE_Thinker_Insert(unsigned id);
DGE_Native void DGE_Thinker_Unlink(unsigned id);

DGE_Native int DGE_Window_GetHeight(void);
DGE_Native int DGE_Window_GetWidth(void);
DGE_Native void DGE_Window_SetTitle(char const *str);


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
   DGE_PropMem(DGE_Accum,  xl) \
   DGE_PropMem(DGE_Accum,  xu) \
   DGE_PropMem(DGE_Accum,  yl) \
   DGE_PropMem(DGE_Accum,  yu) \
   DGE_PropMem(DGE_Accum,  zl) \
   DGE_PropMem(DGE_Accum,  zu)
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
// DGE_ParticleSys
//
#define DGE_ParticleSysProps() DGE_ThinkerProps() \
   DGE_PropMem(unsigned,  sprite) \
   DGE_PropMem(DGE_Accum, x) \
   DGE_PropMem(DGE_Accum, y)
typedef struct DGE_ParticleSys
{
   int id;

   DGE_ParticleSysProps()
} DGE_ParticleSys;

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
   DGE_PropMem(DGE_Accum,  gx) \
   DGE_PropMem(DGE_Accum,  gy) \
   DGE_PropMem(DGE_Accum,  gz) \
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

