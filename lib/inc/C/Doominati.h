//-----------------------------------------------------------------------------
//
// Copyright(C) 2016-2019 David Hill
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
#define DGE_CB(cb) (__glyph(DGE_Unsig, "{Callback}" #cb))
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
#define DGE_OM(mem) (__glyph(DGE_Unsig, "{ObjectMember}" #mem))
#else
#define DGE_OM(mem) 0
#endif
#define DGE_OM_ca       DGE_OM(ca)
#define DGE_OM_cb       DGE_OM(cb)
#define DGE_OM_cg       DGE_OM(cg)
#define DGE_OM_cr       DGE_OM(cr)
#define DGE_OM_bvx      DGE_OM(bvx)
#define DGE_OM_bvy      DGE_OM(bvy)
#define DGE_OM_bvz      DGE_OM(bvz)
#define DGE_OM_collideF DGE_OM(collideF)
#define DGE_OM_collideI DGE_OM(collideI)
#define DGE_OM_damage   DGE_OM(damage)
#define DFE_OM_emc      DGE_OM(emc)
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
#define DGE_OM_ic       DGE_OM(ic)
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
#define DGE_OM_shader   DGE_OM(shader)
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
#define DGE_OM_think    DGE_OM(think)
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
#define DGE_OME(mem) (__glyph(DGE_Unsig, "{ObjectMemberExt}" #mem))
#else
#define DGE_OME(mem) 0
#endif
#define DGE_OME_Object               DGE_OME(Object)
# define DGE_OME_Sector              DGE_OME(Sector)
# define DGE_OME_Team                DGE_OME(Team)
# define DGE_OME_Thinker             DGE_OME(Thinker)
#  define DGE_OME_ParticleSys        DGE_OME(ParticleSys)
#  define DGE_OME_PointThinker       DGE_OME(PointThinker)
#   define DGE_OME_RenderThinker     DGE_OME(RenderThinker)
#    define DGE_OME_PhysicsThinker   DGE_OME(PhysicsThinker)
#     define DGE_OME_Entity          DGE_OME(Entity)
#      define DGE_OME_MissileEntity  DGE_OME(MissileEntity)
#      define DGE_OME_ScriptedEntity DGE_OME(ScriptedEntity)

//
// DGE_OT_*
//
// Object Type macros.
//
#if __GDCC__
#define DGE_OT(type) (__glyph(DGE_Unsig, "{ObjectType}" #type))
#else
#define DGE_OT(type) 0
#endif
#define DGE_OT_Object               DGE_OT(Object)
# define DGE_OT_Sector              DGE_OT(Sector)
# define DGE_OT_Team                DGE_OT(Team)
# define DGE_OT_Thinker             DGE_OT(Thinker)
#  define DGE_OT_ParticleSys        DGE_OT(ParticleSys)
#  define DGE_OT_PointThinker       DGE_OT(PointThinker)
#   define DGE_OT_RenderThinker     DGE_OT(RenderThinker)
#    define DGE_OT_PhysicsThinker   DGE_OT(PhysicsThinker)
#     define DGE_OT_Entity          DGE_OT(Entity)
#      define DGE_OT_MissileEntity  DGE_OT(MissileEntity)
#      define DGE_OT_ScriptedEntity DGE_OT(ScriptedEntity)

//
// DGE_Object_MemberGetT
//
#define DGE_Object_MemberGetT(T, id, mem) \
   ((T (*)(DGE_Unsig, DGE_Unsig, DGE_Unsig) DGE_Native)DGE_Object_MemberGet) \
      (id, mem, sizeof(T) / 4)

//
// DGE_Object_MemberSetT
//
#define DGE_Object_MemberSetT(T, id, mem, ...) \
   ((void (*)(DGE_Unsig, DGE_Unsig, T) DGE_Native)DGE_Object_MemberSet) \
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
#define DGE_PropMem(T, name, mem) \
   __prop name {__get: DGE_Object_MemberGetT(T, ->id, mem), \
                __set: DGE_Object_MemberSetT(T, ->id, mem, __arg)}
#else
#define DGE_PropMem(T, name, mem) T name;
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// DGE_Angle
//
#if __GDCC__
typedef unsigned long _Fract DGE_Angle;
#else
typedef unsigned DGE_Angle;
#endif

//
// DGE_Fixed
//
#if __GDCC__
typedef short _Accum DGE_Fixed;
#else
typedef int DGE_Fixed;
#endif

//
// DGE_Fract
//
#if __GDCC__
typedef long _Fract DGE_Fract;
#else
typedef int DGE_Fract;
#endif

//
// DGE_Integ
//
typedef int DGE_Integ;

//
// DGE_String
//
#if __GDCC__
typedef __str DGE_String;
#else
typedef unsigned DGE_String;
#endif

//
// DGE_Unsig
//
typedef unsigned DGE_Unsig;

//
// DGE_UnsigL
//
typedef unsigned long DGE_UnsigL;

//
// DGE_Callback*
//
typedef void (*DGE_CallbackType)(void) DGE_Callback;
typedef DGE_Unsig (*DGE_CallbackUUU)(DGE_Unsig, DGE_Unsig) DGE_Callback;
typedef void (*DGE_CallbackVU)(DGE_Unsig) DGE_Callback;

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
   DGE_Axis_Up,
   DGE_Axis_Down,
   DGE_Axis_Left,
   DGE_Axis_Right,
   DGE_Axis_Forward,
   DGE_Axis_Backward,
};

enum // DGE_ButtonState
{
   DGE_Button_Down    = 1 << 0,
   DGE_Button_WasDown = 1 << 1,
   DGE_Button_Hold    = DGE_Button_Down | DGE_Button_WasDown,
   DGE_Button_Release = DGE_Button_WasDown,
   DGE_Button_Press   = DGE_Button_Down,
};

enum // DGE_Key
{
   DGE_Key_Backspace = '\b',
   DGE_Key_Tab       = '\t',
   DGE_Key_Return    = '\r',
   DGE_Key_Escape    = '\033',
   DGE_Key_Delete    = '\177',

   DGE_Key_Special  = 0x40000000,
   DGE_Key_Capslock = DGE_Key_Special + 0x39,

   DGE_Key_F1, DGE_Key_F2,  DGE_Key_F3,  DGE_Key_F4,
   DGE_Key_F5, DGE_Key_F6,  DGE_Key_F7,  DGE_Key_F8,
   DGE_Key_F9, DGE_Key_F10, DGE_Key_F11, DGE_Key_F12,
   DGE_Key_SysRq,
   DGE_Key_ScrollLock,
   DGE_Key_Pause,

   DGE_Key_Insert, DGE_Key_Home, DGE_Key_PageUp,
                   DGE_Key_End,  DGE_Key_PageDown,

   DGE_Key_Right, DGE_Key_Left, DGE_Key_Down, DGE_Key_Up,

   DGE_Key_NumLock,
   DGE_Key_KP_Div, DGE_Key_KP_Mul, DGE_Key_KP_Sub, DGE_Key_KP_Add,
   DGE_Key_KP_Enter,
   DGE_Key_KP_1, DGE_Key_KP_2, DGE_Key_KP_3,
   DGE_Key_KP_4, DGE_Key_KP_5, DGE_Key_KP_6,
   DGE_Key_KP_7, DGE_Key_KP_8, DGE_Key_KP_9,
   DGE_Key_KP_0, DGE_Key_KP_Dot
};

enum // DGE_MouseButton
{
   DGE_Mouse_Left,
   DGE_Mouse_Middle,
   DGE_Mouse_Right,
   DGE_Mouse_Extra1,
   DGE_Mouse_Extra2,
   DGE_Mouse_Extra3,
   DGE_Mouse_Extra4
};

enum // DGE_PadControl
{
   DGE_Pad_A,
   DGE_Pad_B,
   DGE_Pad_X,
   DGE_Pad_Y,
   DGE_Pad_Back,
   DGE_Pad_Menu,
   DGE_Pad_Start,
   DGE_Pad_StickLeft,
   DGE_Pad_StickRight,
   DGE_Pad_ShoulderLeft,
   DGE_Pad_ShoulderRight,
   DGE_Pad_DPadUp,
   DGE_Pad_DPadDown,
   DGE_Pad_DPadLeft,
   DGE_Pad_DPadRight,
   DGE_Pad_LStickUp,
   DGE_Pad_LStickDown,
   DGE_Pad_LStickLeft,
   DGE_Pad_LStickRight,
   DGE_Pad_RStickUp,
   DGE_Pad_RStickDown,
   DGE_Pad_RStickLeft,
   DGE_Pad_RStickRight,
   DGE_Pad_TriggerLeft,
   DGE_Pad_TriggerRight
};

typedef struct DGE_Color
{
   DGE_Angle r, g, b, a;
} DGE_Color;

typedef struct DGE_Item
{
   DGE_Unsig type;
   DGE_Unsig data;
   DGE_Unsig size;
} DGE_Item;

typedef struct DGE_Point2X
{
   DGE_Fixed x, y;
} DGE_Point2X;

typedef struct DGE_Point2I
{
   DGE_Integ x, y;
} DGE_Point2I;

typedef struct DGE_Point3R
{
   DGE_Fract x, y, z;
} DGE_Point3R;

typedef struct DGE_Particle
{
   DGE_Integ   life;
   DGE_Point2X pos;
   DGE_Point2X vel;
   DGE_Point2X accel;
   DGE_Point2X size;
   DGE_Point2X sizedest;
   DGE_Fixed   sizespeed;
   DGE_Color   color;
   DGE_Color   colordest;
   DGE_Fixed   colorspeed;
   DGE_Fixed   rot;
   DGE_Fixed   rotspeed;
} DGE_Particle;

struct DGE_Object;
 struct DGE_Inventory;
 struct DGE_ItemData;
 struct DGE_ItemType;
 struct DGE_Sector;
 struct DGE_Team;
 struct DGE_Thinker;
  struct DGE_ParticleSys;
  struct DGE_PointThinker;
   struct DGE_RenderThinker;
    struct DGE_PhysicsThinker;
     struct DGE_Entity;
      struct DGE_MissileEntity;
      struct DGE_ScriptedEntity;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

DGE_Native void DGE_Audio_SetDopplerSpeed(DGE_Fixed meterspersecond);
DGE_OptArgs(4)
DGE_Native void DGE_Audio_SetListener(DGE_Fixed x, DGE_Fixed y, DGE_Fixed z, DGE_Fixed velx, DGE_Fixed vely, DGE_Fixed velz, DGE_Fixed angle);

DGE_Native DGE_Unsig DGE_BlockMap_Find(DGE_Fixed xl, DGE_Fixed yl, DGE_Fixed xu, DGE_Fixed yu);
DGE_Native DGE_Unsig DGE_BlockMap_FindAll(void);
DGE_Native DGE_Unsig DGE_BlockMap_FindCountSector(DGE_Unsig find);
DGE_Native DGE_Unsig DGE_BlockMap_FindCountThinker(DGE_Unsig find);
DGE_Native void DGE_BlockMap_FindFree(DGE_Unsig find);
DGE_Native DGE_Unsig DGE_BlockMap_FindGetSector(DGE_Unsig find, DGE_Unsig idx);
DGE_Native DGE_Unsig DGE_BlockMap_FindGetThinker(DGE_Unsig find, DGE_Unsig idx);
DGE_Native void DGE_BlockMap_Split(DGE_Fixed minSize, DGE_Unsig maxObj);

DGE_Native void DGE_Callback_Register(DGE_Unsig cb, DGE_CallbackType fn);
DGE_Native void DGE_Callback_Unregister(DGE_Unsig cb, DGE_CallbackType fn);

DGE_Native void DGE_DebugCallStk(void);
DGE_OptArgs(1)
DGE_Native void DGE_DebugDataStk(DGE_Unsig dump);
DGE_Native void DGE_DebugLocReg(void);

DGE_Native void DGE_Dir_Close(DGE_Integ fd);
DGE_Native DGE_Integ DGE_Dir_Create(char const *name);
DGE_Native DGE_Unsig DGE_Dir_Name(DGE_Integ fd, char *buf, DGE_Unsig len);
DGE_Native DGE_Integ DGE_Dir_Open(char const *name);
DGE_Native DGE_Integ DGE_Dir_Read(DGE_Integ fd);
DGE_Native DGE_Integ DGE_Dir_ReadDir(DGE_Integ fd);
DGE_Native DGE_Integ DGE_Dir_ReadFile(DGE_Integ fd);
DGE_Native DGE_Unsig DGE_Dir_ReadName(DGE_Integ fd, char *buf, DGE_Unsig len);
DGE_Native DGE_Integ DGE_Dir_Remove(char const *name);
DGE_Native void DGE_Dir_Rewind(DGE_Integ fd);

DGE_Native void DGE_Draw_Circle(DGE_Fixed x, DGE_Fixed y, DGE_Fixed r);
DGE_Native void DGE_Draw_CircleLine(DGE_Fixed x, DGE_Fixed y, DGE_Fixed r);
DGE_Native void DGE_Draw_CirclePrecision(DGE_Integ subdivisions);
DGE_Native void DGE_Draw_Ellipse(DGE_Fixed x1, DGE_Fixed y1, DGE_Fixed x2, DGE_Fixed y2);
DGE_Native void DGE_Draw_EllipseLine(DGE_Fixed x1, DGE_Fixed y1, DGE_Fixed x2, DGE_Fixed y2);
DGE_OptArgs(1)
DGE_Native void DGE_Draw_Rectangle(DGE_Fixed x1, DGE_Fixed y1, DGE_Fixed x2, DGE_Fixed y2, DGE_Angle rot);
DGE_OptArgs(1)
DGE_Native void DGE_Draw_RectangleLine(DGE_Fixed x1, DGE_Fixed y1, DGE_Fixed x2, DGE_Fixed y2, DGE_Angle rot);
DGE_OptArgs(4)
DGE_Native void DGE_Draw_SetColor(DGE_Angle r, DGE_Angle g, DGE_Angle b, DGE_Angle a);
DGE_OptArgs(1)
DGE_Native void DGE_Draw_SetTextAlign(DGE_Integ h, DGE_Integ v);
DGE_Native void DGE_Draw_Line(DGE_Fixed x1, DGE_Fixed y1, DGE_Fixed x2, DGE_Fixed y2);
DGE_Native void DGE_Draw_LineSmooth(DGE_Unsig on);
DGE_Native void DGE_Draw_LineWidth(DGE_Integ width);
DGE_OptArgs(1)
DGE_Native DGE_Point2X DGE_Draw_Text(DGE_Fixed x, DGE_Fixed y, char const *str, DGE_Fixed maxwidth);
DGE_Native void DGE_Draw_Triangle(DGE_Fixed x1, DGE_Fixed y1, DGE_Fixed x2, DGE_Fixed y2, DGE_Fixed x3, DGE_Fixed y3);
DGE_Native void DGE_Draw_TriangleLine(DGE_Fixed x1, DGE_Fixed y1, DGE_Fixed x2, DGE_Fixed y2, DGE_Fixed x3, DGE_Fixed y3);

DGE_Native DGE_Unsig DGE_Entity_Create(DGE_Unsig ext);

DGE_Native void DGE_File_Close(DGE_Integ fd);
DGE_Native DGE_Integ DGE_File_Create(char const *name, char const *data, DGE_Unsig size);
DGE_Native DGE_Integ DGE_File_Get(DGE_Integ fd, DGE_UnsigL idx);
DGE_Native DGE_Unsig DGE_File_Name(DGE_Integ fd, char *buf, DGE_Unsig len);
DGE_Native DGE_Integ DGE_File_Open(char const *name);
DGE_Native DGE_Integ DGE_File_Read(DGE_Integ fd, DGE_UnsigL idx, char *buf, DGE_Unsig len);
DGE_Native DGE_Integ DGE_File_Remove(char const *name);
DGE_Native DGE_UnsigL DGE_File_Size(DGE_Integ fd);

DGE_Native void DGE_Font_Bind(DGE_Unsig fnt);
DGE_Native DGE_Unsig DGE_Font_Create(DGE_String name, char const *fname, DGE_Integ ptSize);
DGE_Native DGE_Unsig DGE_Font_Get(DGE_String name);
DGE_Native DGE_Point2X DGE_Font_GetTextSize(DGE_Unsig fnt, char const *text);

DGE_Native void *DGE_FreestoreBegin(void);
DGE_Native void *DGE_FreestoreEnd(void);

DGE_Native DGE_Point3R DGE_Input_GetAxis(DGE_Unsig player, DGE_Unsig num);
DGE_Native DGE_Unsig DGE_Input_GetButton(DGE_Unsig player, DGE_Unsig btn);
DGE_Native DGE_Point2I DGE_Input_GetCursor(DGE_Unsig player);
DGE_Native void DGE_Input_SetAxisKey(DGE_Unsig num, DGE_Unsig axis, DGE_Integ set);
DGE_Native void DGE_Input_SetAxisPad(DGE_Unsig num, DGE_Unsig axis, DGE_Unsig set);
DGE_Native void DGE_Input_SetAxisMouse(DGE_Unsig num, DGE_Unsig axis, DGE_Unsig set);
DGE_Native void DGE_Input_SetBindKey(DGE_Unsig btn, DGE_Integ ch);
DGE_Native void DGE_Input_SetBindPad(DGE_Unsig btn, DGE_Unsig num);
DGE_Native void DGE_Input_SetBindMouse(DGE_Unsig btn, DGE_Unsig num);

DGE_Native DGE_Unsig DGE_Inventory_Create(DGE_Unsig ic, DGE_Unsig ext);
DGE_Native DGE_Item DGE_Inventory_ItemGet(DGE_Unsig id, DGE_Unsig i);
DGE_Native void DGE_Inventory_ItemSet(DGE_Unsig id, DGE_Unsig i, DGE_Item val);

DGE_Native DGE_Unsig DGE_ItemData_Create(DGE_Unsig ext);

DGE_Native DGE_Unsig DGE_ItemType_Create(DGE_Unsig ext);

DGE_Native DGE_Unsig DGE_MissileEntity_Create(DGE_Unsig ext);

DGE_Native DGE_Unsig DGE_Object_Cast(DGE_Unsig id, DGE_Unsig type);
DGE_Native DGE_Unsig DGE_Object_MemberGet(DGE_Unsig id, DGE_Unsig mem, DGE_Unsig len);
DGE_Native void DGE_Object_MemberSet(DGE_Unsig id, DGE_Unsig mem, ...);
DGE_Native void DGE_Object_RefAdd(DGE_Unsig id);
DGE_Native void DGE_Object_RefSub(DGE_Unsig id);

DGE_Native void DGE_ParticleSys_Add(DGE_Unsig id, DGE_Particle prt);
DGE_Native DGE_Unsig DGE_ParticleSys_Create(DGE_Unsig ext, DGE_Unsig npr);

DGE_Native void DGE_PhysicsThinker_ApplyFriction(DGE_Unsig id, DGE_Fract f);
DGE_Native void DGE_PhysicsThinker_Block(DGE_Unsig id);
DGE_OptArgs(1)
DGE_Native DGE_Unsig DGE_PhysicsThinker_Collide(DGE_Unsig id, DGE_Fixed *oldx,
   DGE_Fixed *oldy, DGE_Fixed *oldz, DGE_Fract *fric);
DGE_Native DGE_Unsig DGE_PhysicsThinker_Create(DGE_Unsig ext);
DGE_Native void DGE_PhysicsThinker_Unblock(DGE_Unsig id);

DGE_Native DGE_Unsig DGE_PointThinker_Create(DGE_Unsig ext);

DGE_Native void DGE_PrintChar(DGE_Unsig c);
DGE_Native void DGE_PrintWordD(DGE_Unsig d);

DGE_Native DGE_Point2X DGE_Renderer_GetViewpoint(void);
DGE_Native void DGE_Renderer_SetViewpoint(DGE_Unsig id);
DGE_Native void DGE_Renderer_SetVirtualRes(DGE_Unsig w, DGE_Unsig h);

DGE_Native DGE_Unsig DGE_RenderThinker_Create(DGE_Unsig ext);

DGE_Native DGE_Unsig DGE_ScriptedEntity_Create(DGE_Unsig ext);

DGE_Native void DGE_Sector_Block(DGE_Unsig id);
DGE_Native void DGE_Sector_CalcBounds(DGE_Unsig id);
DGE_Native DGE_Unsig DGE_Sector_Create(DGE_Unsig pc, DGE_Unsig ext);
DGE_Native DGE_Point2X DGE_Sector_PointGet(DGE_Unsig id, DGE_Unsig p);
DGE_Native void DGE_Sector_PointSet(DGE_Unsig id, DGE_Unsig p, DGE_Point2X val);
DGE_Native void DGE_Sector_Unblock(DGE_Unsig id);

DGE_Native void DGE_Shader_Bind(DGE_Unsig shd);
DGE_Native DGE_Unsig DGE_Shader_CreateData(DGE_String name, char const *frag, char const *vert);
DGE_Native DGE_Unsig DGE_Shader_CreateFile(DGE_String name, char const *frag, char const *vert);
DGE_Native DGE_Unsig DGE_Shader_Get(DGE_String name);

DGE_Native void DGE_Shell_GetArgBuf(DGE_Unsig sh, char *buf);
DGE_Native DGE_Unsig DGE_Shell_GetArgBufLen(DGE_Unsig sh);
DGE_Native DGE_Unsig DGE_Shell_GetArgC(DGE_Unsig sh);
DGE_Native void DGE_Shell_GetArgV(DGE_Unsig sh, char **argv, char *buf);
DGE_Native DGE_Unsig DGE_Shell_ReadStd(DGE_Unsig sh, char *buf, DGE_Unsig len);
DGE_Native DGE_Unsig DGE_Shell_WriteErr(DGE_Unsig sh, char const *buf, DGE_Unsig len);
DGE_Native DGE_Unsig DGE_Shell_WriteStd(DGE_Unsig sh, char const *buf, DGE_Unsig len);

DGE_Native DGE_Unsig DGE_Sound_Get(DGE_String name);
DGE_Native void DGE_Sound_Play(DGE_Unsig src, DGE_Unsig chan);
DGE_Native void DGE_Sound_SetLoop(DGE_Unsig src, DGE_Unsig chan, DGE_Unsig on);
DGE_OptArgs(1)
DGE_Native DGE_Fixed DGE_Sound_SetPitch(DGE_Unsig src, DGE_Unsig chan, DGE_Angle pitch);
DGE_Native void DGE_Sound_SetPos(DGE_Unsig src, DGE_Unsig chan, DGE_Fixed x, DGE_Fixed y, DGE_Fixed z);
DGE_Native void DGE_Sound_SetVolume(DGE_Unsig src, DGE_Unsig chan, DGE_Angle volume);
DGE_Native void DGE_Sound_Stop(DGE_Unsig src, DGE_Unsig chan);

DGE_OptArgs(1)
DGE_Native DGE_Unsig DGE_SoundSrc_Bind(DGE_Unsig src, DGE_Unsig snd, DGE_Unsig chan);
DGE_Native DGE_Unsig DGE_SoundSrc_Create(DGE_Fixed x, DGE_Fixed y, DGE_Fixed z);
DGE_Native void DGE_SoundSrc_Destroy(DGE_Unsig src);
DGE_OptArgs(1)
DGE_Native DGE_Unsig DGE_SoundSrc_Play(DGE_Unsig src, DGE_Unsig snd, DGE_Unsig chan);
DGE_Native void DGE_SoundSrc_SetPos(DGE_Unsig src, DGE_Fixed x, DGE_Fixed y, DGE_Fixed z);
DGE_Native void DGE_SoundSrc_SetVel(DGE_Unsig src, DGE_Fixed x, DGE_Fixed y, DGE_Fixed z);

DGE_Native DGE_String DGE_String_Create(char const *str, DGE_Unsig len);

DGE_Native DGE_Unsig DGE_Task_Create(DGE_Unsig thread, DGE_CallbackType fn, ...);
DGE_Native void DGE_Task_Sleep(DGE_Unsig id, DGE_Unsig ticks);

DGE_Native DGE_Unsig DGE_Team_Create(DGE_Unsig ext);

DGE_Native DGE_Unsig DGE_Text_Read(DGE_String name, char *buf, DGE_Unsig len);

DGE_Native void DGE_Texture_Bind(DGE_Unsig tex);
DGE_Native DGE_Unsig DGE_Texture_Create(DGE_String name, char const *fname);
DGE_Native DGE_Unsig DGE_Texture_Get(DGE_String name);
DGE_Native DGE_Point2I DGE_Texture_GetSize(DGE_Unsig tex);

DGE_Native DGE_Unsig DGE_Thinker_Head(void);
DGE_Native void DGE_Thinker_Insert(DGE_Unsig id);
DGE_Native void DGE_Thinker_Think(DGE_Unsig id);
DGE_Native void DGE_Thinker_ThinkAll(void);
DGE_Native void DGE_Thinker_Unlink(DGE_Unsig id);

DGE_Native DGE_Integ DGE_Window_GetHeight(void);
DGE_Native DGE_Integ DGE_Window_GetWidth(void);
DGE_Native void DGE_Window_SetTitle(char const *str);


//----------------------------------------------------------------------------|
// Property Types                                                             |
//

//
// DGE_Object
//
#define DGE_ObjectProps() \
   DGE_PropMem(DGE_Unsig, emc, DGE_OM_ic)
typedef struct DGE_Object
{
   DGE_Unsig id;

   DGE_ObjectProps()
} DGE_Object;

//
// DGE_Inventory
//
#define DGE_InventoryProps() DGE_ObjectProps() \
   DGE_PropMem(DGE_Unsig, ic, DGE_OM_ic)
typedef struct DGE_Inventory
{
   DGE_Unsig id;

   DGE_InventoryProps()
} DGE_Inventory;

//
// DGE_ItemData
//
#define DGE_ItemDataProps() DGE_ObjectProps()
typedef struct DGE_ItemData
{
   DGE_Unsig id;

   DGE_ItemDataProps()
} DGE_ItemData;

//
// DGE_ItemType
//
#define DGE_ItemTypeProps() DGE_ObjectProps()
typedef struct DGE_ItemType
{
   DGE_Unsig id;

   DGE_ItemTypeProps()
} DGE_ItemType;

//
// DGE_Sector
//
#define DGE_SectorProps() DGE_ObjectProps() \
   DGE_PropMem(DGE_Fract, frictair, DGE_OM_frictair) \
   DGE_PropMem(DGE_Fract, friction, DGE_OM_friction) \
   DGE_PropMem(DGE_Fixed, gx,       DGE_OM_gx) \
   DGE_PropMem(DGE_Fixed, gy,       DGE_OM_gy) \
   DGE_PropMem(DGE_Fixed, gz,       DGE_OM_gz) \
   DGE_PropMem(DGE_Unsig, pc,       DGE_OM_pc) \
   DGE_PropMem(DGE_Unsig, texc,     DGE_OM_texc) \
   DGE_PropMem(DGE_Unsig, texf,     DGE_OM_texf) \
   DGE_PropMem(DGE_Fixed, xl,       DGE_OM_xl) \
   DGE_PropMem(DGE_Fixed, xu,       DGE_OM_xu) \
   DGE_PropMem(DGE_Fixed, yl,       DGE_OM_yl) \
   DGE_PropMem(DGE_Fixed, yu,       DGE_OM_yu) \
   DGE_PropMem(DGE_Fixed, zl,       DGE_OM_zl) \
   DGE_PropMem(DGE_Fixed, zu,       DGE_OM_zu)
typedef struct DGE_Sector
{
   DGE_Unsig id;

   DGE_SectorProps()
} DGE_Sector;

//
// DGE_Team
//
#define DGE_TeamProps() DGE_ObjectProps() \
   DGE_PropMem(DGE_Unsig, entfi,  DGE_OM_entfi) \
   DGE_PropMem(DGE_Unsig, entla,  DGE_OM_entla) \
   DGE_PropMem(DGE_Unsig, owner,  DGE_OM_owner) \
   DGE_PropMem(DGE_Unsig, teamfi, DGE_OM_teamfi) \
   DGE_PropMem(DGE_Unsig, teamla, DGE_OM_teamla) \
   DGE_PropMem(DGE_Unsig, teamne, DGE_OM_teamne) \
   DGE_PropMem(DGE_Unsig, teampr, DGE_OM_teampr)
typedef struct DGE_Team
{
   DGE_Unsig id;

   DGE_TeamProps()
} DGE_Team;

//
// DGE_Thinker
//
#define DGE_ThinkerProps() DGE_ObjectProps() \
   DGE_PropMem(DGE_Unsig, next, DGE_OM_next) \
   DGE_PropMem(DGE_Unsig, prev, DGE_OM_prev)
typedef struct DGE_Thinker
{
   DGE_Unsig id;

   DGE_ThinkerProps()
} DGE_Thinker;

//
// DGE_ParticleSys
//
#define DGE_ParticleSysProps() DGE_ThinkerProps() \
   DGE_PropMem(DGE_Unsig, sprite, DGE_OM_sprite) \
   DGE_PropMem(DGE_Fixed, x,      DGE_OM_x) \
   DGE_PropMem(DGE_Fixed, y,      DGE_OM_y)
typedef struct DGE_ParticleSys
{
   DGE_Unsig id;

   DGE_ParticleSysProps()
} DGE_ParticleSys;

//
// DGE_PointThinker
//
#define DGE_PointThinkerProps() DGE_ThinkerProps() \
   DGE_PropMem(DGE_Angle, pitch, DGE_OM_pitch) \
   DGE_PropMem(DGE_Fixed, x,     DGE_OM_x) \
   DGE_PropMem(DGE_Fixed, y,     DGE_OM_y) \
   DGE_PropMem(DGE_Angle, yaw,   DGE_OM_yaw) \
   DGE_PropMem(DGE_Fixed, z,     DGE_OM_z)
typedef struct DGE_PointThinker
{
   DGE_Unsig id;

   DGE_PointThinkerProps()
} DGE_PointThinker;

//
// DGE_RenderThinker
//
#define DGE_RenderThinkerProps() DGE_PointThinkerProps() \
   DGE_PropMem(DGE_Angle, ca,     DGE_OM_ca) \
   DGE_PropMem(DGE_Angle, cb,     DGE_OM_cb) \
   DGE_PropMem(DGE_Angle, cg,     DGE_OM_cg) \
   DGE_PropMem(DGE_Angle, cr,     DGE_OM_cr) \
   DGE_PropMem(DGE_Fixed, rsx,    DGE_OM_rsx) \
   DGE_PropMem(DGE_Fixed, rsy,    DGE_OM_rsy) \
   DGE_PropMem(DGE_Unsig, sprite, DGE_OM_sprite) \
   DGE_PropMem(DGE_Unsig, shader, DGE_OM_shader)
typedef struct DGE_RenderThinker
{
   DGE_Unsig id;

   DGE_RenderThinkerProps()
} DGE_RenderThinker;

//
// DGE_PhysicsThinker
//
#define DGE_PhysicsThinkerProps() DGE_RenderThinkerProps() \
   DGE_PropMem(DGE_Fixed, bvx,      DGE_OM_bvx) \
   DGE_PropMem(DGE_Fixed, bvy,      DGE_OM_bvy) \
   DGE_PropMem(DGE_Fixed, bvz,      DGE_OM_bvz) \
   DGE_PropMem(DGE_Fract, friction, DGE_OM_friction) \
   DGE_PropMem(DGE_Fixed, grabx,    DGE_OM_grabx) \
   DGE_PropMem(DGE_Fixed, graby,    DGE_OM_graby) \
   DGE_PropMem(DGE_Fixed, grabz,    DGE_OM_grabz) \
   DGE_PropMem(DGE_Fixed, gx,       DGE_OM_gx) \
   DGE_PropMem(DGE_Fixed, gy,       DGE_OM_gy) \
   DGE_PropMem(DGE_Fixed, gz,       DGE_OM_gz) \
   DGE_PropMem(DGE_Fixed, mass,     DGE_OM_mass) \
   DGE_PropMem(DGE_Fixed, sx,       DGE_OM_sx) \
   DGE_PropMem(DGE_Fixed, sy,       DGE_OM_sy) \
   DGE_PropMem(DGE_Fixed, sz,       DGE_OM_sz) \
   DGE_PropMem(DGE_Fixed, vx,       DGE_OM_vx) \
   DGE_PropMem(DGE_Fixed, vy,       DGE_OM_vy) \
   DGE_PropMem(DGE_Fixed, vz,       DGE_OM_vz)
typedef struct DGE_PhysicsThinker
{
   DGE_Unsig id;

   DGE_PhysicsThinkerProps()
} DGE_PhysicsThinker;

//
// DGE_Entity
//
#define DGE_EntityProps() DGE_PhysicsThinkerProps() \
   DGE_PropMem(DGE_Integ, health, DGE_OM_health) \
   DGE_PropMem(DGE_Unsig, team,   DGE_OM_team) \
   DGE_PropMem(DGE_Unsig, teamne, DGE_OM_teamne) \
   DGE_PropMem(DGE_Unsig, teampr, DGE_OM_teampr)
typedef struct DGE_Entity
{
   DGE_Unsig id;

   DGE_EntityProps()
} DGE_Entity;

//
// DGE_MissileEntity
//
#define DGE_MissileEntityProps() DGE_EntityProps() \
   DGE_PropMem(DGE_Unsig, damage, DGE_OM_damage) \
   DGE_PropMem(DGE_Unsig, owner,  DGE_OM_owner)
typedef struct DGE_MissileEntity
{
   DGE_Unsig id;

   DGE_MissileEntityProps()
} DGE_MissileEntity;

//
// DGE_ScriptedEntity
//
#define DGE_ScriptedEntityProps() DGE_EntityProps() \
   DGE_PropMem(DGE_CallbackUUU, collideF, DGE_OM_collideF) \
   DGE_PropMem(DGE_CallbackUUU, collideI, DGE_OM_collideI) \
   DGE_PropMem(DGE_CallbackVU,  think,    DGE_OM_think)

#endif//__GDCC_Header__C__Doominati_h__

