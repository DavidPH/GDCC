;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Unsigned word division routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__DivU_W
;;
Function "___GDCC__DivU_W" \
   alloc=1 \
   ctype=StkCall \
   defin=1 \
   label="___GDCC__DivU_W$label" \
   linka=ExtC \
   localReg=3 \
   param=2 \
   retrn=1 \
   block
{
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__DivU_W$l0")

   ; l has high bit set.
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__DivU_W$l1r0")

   ; l and r have high bit set
   ; Therefore, if l < r, result is 0, otherwise result is 1.
   ; Therefore, result is l >= r.
   CmpI_GE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,      Stk()

   ; l has high bit set, r does not.
"___GDCC__DivU_W$l1r0"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(1)
   Cjmp_Nil, Stk(), Lit("___GDCC__DivU_W$l1r00")

   ; r has low bit set.

   ; Halve l and divide by r, then double result.
   ; If the result of the division is 0, then immediately return 1.
   ShRU_W, Stk(), LocReg(Lit(0), 0), Lit(1)
   DivI_W, Stk(), Stk(), LocReg(Lit(1), 0)
   Casm,   Lit(84), Lit(0), Lit("___GDCC__DivU_W$l1r01_1") ; Cjmp_Lit
   ShLU_W, Stk(), Stk(), Lit(1)
   Move_W, LocReg(Lit(2), 0), Stk()

   ; If l - res * r >= r, add one to result.
   Move_W,    Stk(), LocReg(Lit(0), 0)
   MulU_W,    Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   SubU_W,    Stk(), Stk(), Stk()
   CmpI_GE_W, Stk(), Stk(), LocReg(Lit(1), 0)
   Cjmp_Nil,  Stk(), Lit("___GDCC__DivU_W$l1r01_r")
   Casm,      Lit(46), Lit(2) ; IncU

"___GDCC__DivU_W$l1r01_r"
   Move_W, Stk(), LocReg(Lit(2), 0)
   Retn,   Stk()

"___GDCC__DivU_W$l1r01_1"
   Move_W, Stk(), Lit(1)
   Retn,   Stk()

   ; r has low bit unset.
   ; Therefore, result is (l >> 1) / (r >> 1).
"___GDCC__DivU_W$l1r00"
   ShRU_W, Stk(), LocReg(Lit(0), 0), Lit(1)
   ShRI_W, Stk(), LocReg(Lit(1), 0), Lit(1)
   DivI_W, Stk(), Stk(), Stk()
   Retn,   Stk()

   ; l does not have high bit set.
"___GDCC__DivU_W$l0"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__DivU_W$l0r0")

   ; l does not have high bit set, r does.
   ; Therefore, l < r, result is 0.
   Move_W, Stk(), Lit(0)
   Retn,   Stk()

   ; l and r do not have high bit set.
   ; Therefore, signed division will work.
"___GDCC__DivU_W$l0r0"
   DivI_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,   Stk()
}

;;
;; ___GDCC__ModU_W
;;
Function "___GDCC__ModU_W" \
   alloc=1 \
   ctype=StkCall \
   defin=1 \
   label="___GDCC__ModU_W$label" \
   linka=ExtC \
   localReg=3 \
   param=2 \
   retrn=1 \
   block
{
   AndU_W,   Stk(), LocReg(Lit(0), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__ModU_W$l0")

   ; l has high bit set.
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__ModU_W$l1r0")

   ; l and r have high bit set
   ; Therefore, if l < r, result is 0, otherwise result is 1.
   ; Therefore, remainder is l >= r ? l - r : l.
   Move_W,    Stk(), LocReg(Lit(0), 0)
   CmpI_GE_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Cjmp_Nil,  Stk(), Lit("___GDCC__ModU_W$l1r1_ret")
   SubU_W,    Stk(), Stk(), LocReg(Lit(1), 0)

"___GDCC__ModU_W$l1r1_ret"
   Retn,      Stk()

   ; l has high bit set, r does not.
"___GDCC__ModU_W$l1r0"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(1)
   Cjmp_Nil, Stk(), Lit("___GDCC__ModU_W$l1r00")

   ; r has low bit set.

   ; Halve l and divide by r, then double result.
   ; If the result of the division is 0, then immediately return l - r.
   ShRU_W, Stk(), LocReg(Lit(0), 0), Lit(1)
   DivI_W, Stk(), Stk(), LocReg(Lit(1), 0)
   Casm,   Lit(84), Lit("___GDCC__ModU_W$l1r01_1") ; Cjmp_Lit
   ShLU_W, Stk(), Stk(), Lit(1)
   Move_W, LocReg(Lit(2), 0), Stk()

   ; If l - res * r >= r, add one to result.
   Move_W,    Stk(), LocReg(Lit(0), 0)
   MulU_W,    Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   SubU_W,    Stk(), Stk(), Stk()
   CmpI_GE_W, Stk(), Stk(), LocReg(Lit(1), 0)
   Cjmp_Nil,  Stk(), Lit("___GDCC__ModU_W$l1r01_r")
   Casm,      Lit(46), Lit(2) ; IncU

"___GDCC__ModU_W$l1r01_r"
   Move_W, Stk(), LocReg(Lit(0), 0)
   MulU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(2), 0)
   SubU_W, Stk(), Stk(), Stk()
   Retn,   Stk()

"___GDCC__ModU_W$l1r01_1"
   SubU_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,   Stk()

   ; r has low bit unset.
   ; Therefore, result is l - (l >> 1) / (r >> 1) * 2 * r.
"___GDCC__ModU_W$l1r00"
   Move_W, Stk(), LocReg(Lit(0), 0)
   ShRU_W, Stk(), LocReg(Lit(0), 0), Lit(1)
   ShRI_W, Stk(), LocReg(Lit(1), 0), Lit(1)
   DivI_W, Stk(), Stk(), Stk()
   ShLU_W, Stk(), Stk(), Lit(1)
   MulU_W, Stk(), Stk(), LocReg(Lit(1), 0)
   SubU_W, Stk(), Stk(), Stk()
   Retn,   Stk()

   ; l does not have high bit set.
"___GDCC__ModU_W$l0"
   AndU_W,   Stk(), LocReg(Lit(1), 0), Lit(0x80000000)
   Cjmp_Nil, Stk(), Lit("___GDCC__ModU_W$l0r0")

   ; l does not have high bit set, r does.
   ; Therefore, l < r, remainder is l.
   Move_W, Stk(), LocReg(Lit(0), 0)
   Retn,   Stk()

   ; l and r do not have high bit set.
   ; Therefore, signed division will work.
"___GDCC__ModU_W$l0r0"
   ModI_W, Stk(), LocReg(Lit(0), 0), LocReg(Lit(1), 0)
   Retn,   Stk()
}

;; EOF

