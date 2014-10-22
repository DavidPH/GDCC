;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Multi-word sub routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__SubU_W2
;;
Function "___GDCC__SubU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__SubU_W2$label" \
   linka    = ExtC \
   localReg = 5 \
   param    = 4 \
   retrn    = 2 \
   block
{
   SubU_W, LocReg(Lit(4), 0), LocReg(Lit(0), 0), LocReg(Lit(2), 0)

   CmpU_GT_W, Stk(), LocReg(Lit(4), 0), LocReg(Lit(0), 0)
   Cjmp_Nil,  Stk(), Lit("___GDCC__SubU_W2$notunder0")

   Move_W, Stk(), LocReg(Lit(4), 0)
   SubU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   SubU_W, Stk(), Stk(), Lit(1)
   Retn,   Stk(), Stk()

"___GDCC__SubU_W2$notunder0"
   Move_W, Stk(), LocReg(Lit(4), 0)
   SubU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Retn,   Stk(), Stk()
}

;;
;; ___GDCC__SubU_W3
;;
Function "___GDCC__SubU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__SubU_W3$label" \
   linka    = ExtC \
   localReg = 9 \
   param    = 6 \
   retrn    = 3 \
   block
{
   SubU_W, LocReg(Lit(6), 0), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   SubU_W, LocReg(Lit(7), 0), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   SubU_W, LocReg(Lit(8), 0), LocReg(Lit(2), 0), LocReg(Lit(5), 0)

   CmpU_GT_W, Stk(), LocReg(Lit(6), 0), LocReg(Lit(0), 0)
   Cjmp_Nil,  Stk(), Lit("___GDCC__SubU_W3$notunder0")

   CmpU_GT_W, Stk(), LocReg(Lit(7), 0), LocReg(Lit(1), 0)
   Cjmp_Nil,  Stk(), Lit("___GDCC__SubU_W3$notunder1")

   ; Both underflowed.
;  SubU_W,  LocReg(Lit(7), 0), LocReg(Lit(7), 0), Lit(1)
;  SubU_W,  LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(1)
   Casm,    Lit(49), Lit(7) ; DecU_LocReg
   Casm,    Lit(49), Lit(8) ; DecU_LocReg

   Move_W3, Stk(), LocReg(Lit(6), 0)
   Retn,    Stk(), Stk(), Stk()

"___GDCC__SubU_W3$notunder1"
   ; Only 0 underflowed, but check 1 if it is about to underflow.
   Move_W,   Stk(), LocReg(Lit(7), 0)
   Cjmp_Tru, Stk(), Lit("___GDCC__SubU_W3$notunder1+")

   ; Both underflowed, actually.
;  SubU_W,  LocReg(Lit(7), 0), LocReg(Lit(7), 0), Lit(1)
;  SubU_W,  LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(1)
   Casm,    Lit(49), Lit(7) ; DecU_LocReg
   Casm,    Lit(49), Lit(8) ; DecU_LocReg
   Move_W3, Stk(), LocReg(Lit(6), 0)
   Retn,    Stk(), Stk(), Stk()

"___GDCC__SubU_W3$notunder1+"
   ; Only 0 underflowed, actually.
;  SubU_W,  LocReg(Lit(7), 0), LocReg(Lit(7), 0), Lit(1)
   Casm,    Lit(49), Lit(7) ; DecU_LocReg
   Move_W3, Stk(), LocReg(Lit(6), 0)
   Retn,    Stk(), Stk(), Stk()

"___GDCC__SubU_W3$notunder0"
   CmpU_GT_W, Stk(), LocReg(Lit(7), 0), LocReg(Lit(1), 0)
   Cjmp_Nil,  Stk(), Lit("___GDCC__SubU_W3$notunder01")

   ; Only 1 underflowed.
;  SubU_W, LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(1)
   Casm,   Lit(49), Lit(8) ; DecU_LocReg

"___GDCC__SubU_W3$notunder01"
   ; Neither underflowed.
   Move_W3, Stk(), LocReg(Lit(6), 0)
   Retn,    Stk(), Stk(), Stk()
}

;; EOF

