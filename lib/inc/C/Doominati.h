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
// DGE_OM_*
//
// Thinker Member macros
//
#define DGE_OM(mem) (__glyph(unsigned, "{ObjectMember}" #mem))
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


//----------------------------------------------------------------------------|
// Types                                                                      |
//

typedef void (*DGE_CallbackType)(void) [[call("StkCall")]];

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


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

[[call("Native")]] void DGE_CallbackRegister(char const *name, DGE_CallbackType callback);
[[call("Native")]] void DGE_CallbackUnregister(char const *name, DGE_CallbackType callback);

[[call("Native")]] unsigned DGE_CreateEntity(unsigned ext);
[[call("Native")]] unsigned DGE_CreateMissileEntity(unsigned ext);
[[call("Native")]] unsigned DGE_CreatePhysicsThinker(unsigned ext);
[[call("Native")]] unsigned DGE_CreatePointThinker(unsigned ext);
[[call("Native")]] unsigned DGE_CreateSector(unsigned pc, unsigned ext);
[[call("Native")]] unsigned DGE_CreateShaderData(__str name, char const *frag, char const *vert);
[[call("Native")]] unsigned DGE_CreateShaderFile(__str name, char const *frag, char const *vert);

[[call("Native")]] void DGE_DebugCallStk(void);
[[optional_args(1)]]
[[call("Native")]] void DGE_DebugDataStk(int dump);
[[call("Native")]] void DGE_DebugLocReg(void);

[[call("Native")]] void DGE_Delay(unsigned ticks);

[[call("Native")]] void DGE_DopplerSpeed(short _Accum meterspersecond);

[[optional_args(1)]]
[[call("Native")]] void DGE_DrawColor(unsigned long _Fract r,
   unsigned long _Fract g, unsigned long _Fract b, unsigned long _Fract a);
[[call("Native")]] void DGE_DrawCircle(short _Accum x, short _Accum y, short _Accum r);
[[call("Native")]] void DGE_DrawCircleLine(short _Accum x, short _Accum y, short _Accum r);
[[call("Native")]] void DGE_DrawCirclePrecision(int subdivisions);
[[call("Native")]] void DGE_DrawEllipse(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2);
[[call("Native")]] void DGE_DrawEllipseLine(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2);
[[call("Native")]] void DGE_DrawRectangle(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2);
[[call("Native")]] void DGE_DrawRectangleLine(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2);
[[call("Native")]] void DGE_DrawShader(unsigned shd);
[[call("Native")]] void DGE_DrawLine(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2);
[[call("Native")]] void DGE_DrawLineSmooth(_Bool on);
[[call("Native")]] void DGE_DrawLineWidth(int width);
[[call("Native")]] void DGE_DrawText(short _Accum x, short _Accum y, char const *str);
[[call("Native")]] void DGE_DrawTexture(unsigned tex);
[[call("Native")]] void DGE_DrawTriangle(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2, short _Accum x3, short _Accum y3);
[[call("Native")]] void DGE_DrawTriangleLine(short _Accum x1, short _Accum y1, short _Accum x2, short _Accum y2, short _Accum x3, short _Accum y3);

[[call("Native")]] void DGE_FileClose(int fd);
[[call("Native")]] int DGE_FileGet(int fd, unsigned idx);
[[call("Native")]] int DGE_FileOpen(char const *name);
[[call("Native")]] int DGE_FileRead(int fd, unsigned idx, char *buf, unsigned len);
[[call("Native")]] unsigned DGE_FileSize(int fd);

[[call("Native")]] void *DGE_FreestoreBegin(void);
[[call("Native")]] void *DGE_FreestoreEnd(void);

[[call("Native")]] long _Fract DGE_GetInputAxis(unsigned num, unsigned axis);
[[call("Native")]] unsigned DGE_GetInputButtons(void);
[[call("Native")]] DGE_Point2I DGE_GetInputCursor(void);
[[call("Native")]] unsigned DGE_GetSound(__str name);
[[call("Native")]] unsigned DGE_GetShader(__str name);
[[call("Native")]] unsigned DGE_GetTexture(__str name);

[[call("Native")]] unsigned long _Fract DGE_ObjectMemberGetLA(unsigned id, unsigned mem);
[[call("Native")]] unsigned DGE_ObjectMemberGetU(unsigned id, unsigned mem);
[[call("Native")]] short _Accum DGE_ObjectMemberGetX(unsigned id, unsigned mem);
[[call("Native")]] void DGE_ObjectMemberSetLA(unsigned id, unsigned mem, unsigned long _Fract val);
[[call("Native")]] void DGE_ObjectMemberSetU(unsigned id, unsigned mem, unsigned val);
[[call("Native")]] void DGE_ObjectMemberSetX(unsigned id, unsigned mem, short _Accum val);
[[call("Native")]] void DGE_ObjectRefAdd(unsigned id);
[[call("Native")]] void DGE_ObjectRefSub(unsigned id);

[[call("Native")]] void DGE_PhysicsThinkerBlock(unsigned id);
[[call("Native")]] void DGE_PhysicsThinkerUnblock(unsigned id);

[[call("Native")]] void DGE_PrintChar(unsigned c);
[[call("Native")]] void DGE_PrintWordD(unsigned d);

[[call("Native")]] void DGE_SectorBlock(unsigned id);
[[call("Native")]] void DGE_SectorCalcBounds(unsigned id);
[[call("Native")]] DGE_Point2 DGE_SectorPointGet(unsigned id, unsigned p);
[[call("Native")]] void DGE_SectorPointSet(unsigned id, unsigned p, DGE_Point2 val);
[[call("Native")]] void DGE_SectorUnblock(unsigned id);

[[call("Native")]] void DGE_SetVirtualResolution(unsigned w, unsigned h);
[[call("Native")]] void DGE_SetWindowTitle(char const *str);

[[call("Native")]] void DGE_SoundChanLoop(unsigned src, unsigned chan, _Bool on);
[[optional_args(1)]]
[[call("Native")]] void DGE_SoundChanPitch(unsigned src, unsigned chan, unsigned long _Fract pitch);
[[call("Native")]] void DGE_SoundChanPlay(unsigned src, unsigned chan);
[[call("Native")]] void DGE_SoundChanPosition(unsigned src, unsigned chan, short _Accum x, short _Accum y, short _Accum z);
[[call("Native")]] void DGE_SoundChanStop(unsigned src, unsigned chan);
[[call("Native")]] void DGE_SoundChanVolume(unsigned src, unsigned chan, unsigned long _Fract volume);

[[optional_args(4)]]
[[call("Native")]] void DGE_SoundListener(short _Accum x, short _Accum y, short _Accum z, short _Accum velx, short _Accum vely, short _Accum velz, short _Accum angle);

[[call("Native")]] unsigned DGE_SoundSource(short _Accum x, short _Accum y, short _Accum z);
[[optional_args(1)]]
[[call("Native")]] unsigned DGE_SoundSrcBind(unsigned src, unsigned snd, unsigned chan);
[[call("Native")]] void DGE_SoundSrcDestroy(unsigned src);
[[optional_args(1)]]
[[call("Native")]] unsigned DGE_SoundSrcPlay(unsigned src, unsigned snd, unsigned chan);
[[call("Native")]] void DGE_SoundSrcPosition(unsigned src, short _Accum x, short _Accum y, short _Accum z);
[[call("Native")]] void DGE_SoundSrcVelocity(unsigned src, short _Accum x, short _Accum y, short _Accum z);

[[call("Native")]] unsigned DGE_SysRead(char *buf, unsigned len);
[[call("Native")]] unsigned DGE_SysWrite(char const *buf, unsigned len);
[[call("Native")]] unsigned DGE_SysWriteErr(char const *buf, unsigned len);

[[optional_args(1)]]
[[call("Native")]] void DGE_TextAlignment(int h, int v);

[[call("Native")]] void DGE_ThinkerUnlink(unsigned id);

#endif//__GDCC_Header__C__Doominati_h__

