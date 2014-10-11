;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Unsigned word comparison routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;; if(l & 0x80000000)
;; {
;;    if(!(r & 0x8000000)) return +1;
;; }
;; else
;; {
;;    if(r & 0x80000000) return -1;
;; }
;;
;; return cmp((int)l, (int)r)

;;
;; ___GDCC__CmpU_GE_W
;;
Function "___GDCC__CmpU_GE_W" \
   alloc=1 \
   ctype=StkCall \
   defin=1 \
   label="___GDCC__CmpU_GE_W$label" \
   linka=ExtC \
   localReg=2 \
   param=2 \
   retrn=1 \
   block
{
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__CmpU_GE_W$pos")

   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__CmpU_GE_W$cmp")
   Move_W,   Stk(), Lit(1)
   Retn,     Stk()

"___GDCC__CmpU_GE_W$pos"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__CmpU_GE_W$cmp")
   Move_W,   Stk(), Lit(0)
   Retn,     Stk()

"___GDCC__CmpU_GE_W$cmp"
   CmpI_GE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_GT_W
;;
Function "___GDCC__CmpU_GT_W" \
   alloc=1 \
   ctype=StkCall \
   defin=1 \
   label="___GDCC__CmpU_GT_W$label" \
   linka=ExtC \
   localReg=2 \
   param=2 \
   retrn=1 \
   block
{
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__CmpU_GT_W$pos")

   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__CmpU_GT_W$cmp")
   Move_W,   Stk(), Lit(1)
   Retn,     Stk()

"___GDCC__CmpU_GT_W$pos"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__CmpU_GT_W$cmp")
   Move_W,   Stk(), Lit(0)
   Retn,     Stk()

"___GDCC__CmpU_GT_W$cmp"
   CmpI_GT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_LE_W
;;
Function "___GDCC__CmpU_LE_W" \
   alloc=1 \
   ctype=StkCall \
   defin=1 \
   label="___GDCC__CmpU_LE_W$label" \
   linka=ExtC \
   localReg=2 \
   param=2 \
   retrn=1 \
   block
{
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__CmpU_LE_W$pos")

   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__CmpU_LE_W$cmp")
   Move_W,   Stk(), Lit(0)
   Retn,     Stk()

"___GDCC__CmpU_LE_W$pos"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__CmpU_LE_W$cmp")
   Move_W,   Stk(), Lit(1)
   Retn,     Stk()

"___GDCC__CmpU_LE_W$cmp"
   CmpI_LE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,      Stk()
}

;;
;; ___GDCC__CmpU_LT_W
;;
Function "___GDCC__CmpU_LT_W" \
   alloc=1 \
   ctype=StkCall \
   defin=1 \
   label="___GDCC__CmpU_LT_W$label" \
   linka=ExtC \
   localReg=2 \
   param=2 \
   retrn=1 \
   block
{
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__CmpU_LT_W$pos")

   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Tru, Stk(), Lit("___GDCC__CmpU_LT_W$cmp")
   Move_W,   Stk(), Lit(0)
   Retn,     Stk()

"___GDCC__CmpU_LT_W$pos"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__CmpU_LT_W$cmp")
   Move_W,   Stk(), Lit(1)
   Retn,     Stk()

"___GDCC__CmpU_LT_W$cmp"
   CmpI_LT_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,      Stk()
}

;; EOF

