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
   label    = :"$label" \
   linka    = ExtC \
   localReg = 5 \
   param    = 4 \
   retrn    = 2 \
   block
{
   SubU_W 1, LocReg(Lit(4)), LocReg(Lit(0)), LocReg(Lit(2))

   CmpU_GT_W 1, Stk(), LocReg(Lit(4)), LocReg(Lit(0))
   Jcnd_Nil  1, Stk(), Lit(:"$notunder0")

   Move_W 1, Stk(), LocReg(Lit(4))
   SubU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   SubU_W 1, Stk(), Stk(), Lit(1)
   Retn   2, Stk()

:"$notunder0"
   Move_W 1, Stk(), LocReg(Lit(4))
   SubU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Retn   2, Stk()
}

;;
;; ___GDCC__SubU_W3
;;
Function "___GDCC__SubU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 9 \
   param    = 6 \
   retrn    = 3 \
   block
{
   SubU_W 1, LocReg(Lit(6)), LocReg(Lit(0)), LocReg(Lit(3))
   SubU_W 1, LocReg(Lit(7)), LocReg(Lit(1)), LocReg(Lit(4))
   SubU_W 1, LocReg(Lit(8)), LocReg(Lit(2)), LocReg(Lit(5))

   CmpU_GT_W 1, Stk(), LocReg(Lit(6)), LocReg(Lit(0))
   Jcnd_Nil  1, Stk(), Lit(:"$notunder0")

   CmpU_GT_W 1, Stk(), LocReg(Lit(7)), LocReg(Lit(1))
   Jcnd_Nil  1, Stk(), Lit(:"$notunder1")

   ; Both underflowed.
   SubU_W 1, LocReg(Lit(7)), LocReg(Lit(7)), Lit(1)
   SubU_W 1, LocReg(Lit(8)), LocReg(Lit(8)), Lit(1)

   Move_W 3, Stk(), LocReg(Lit(6))
   Retn   3, Stk()

:"$notunder1"
   ; Only 0 underflowed, but check 1 if it is about to underflow.
   Move_W   1, Stk(), LocReg(Lit(7))
   Jcnd_Tru 1, Stk(), Lit(:"$notunder1+")

   ; Both underflowed, actually.
   SubU_W 1, LocReg(Lit(7)), LocReg(Lit(7)), Lit(1)
   SubU_W 1, LocReg(Lit(8)), LocReg(Lit(8)), Lit(1)
   Move_W 3, Stk(), LocReg(Lit(6))
   Retn   3, Stk()

:"$notunder1+"
   ; Only 0 underflowed, actually.
   SubU_W 1, LocReg(Lit(7)), LocReg(Lit(7)), Lit(1)
   Move_W 3, Stk(), LocReg(Lit(6))
   Retn   3, Stk()

:"$notunder0"
   CmpU_GT_W 1, Stk(), LocReg(Lit(7)), LocReg(Lit(1))
   Jcnd_Nil  1, Stk(), Lit(:"$notunder01")

   ; Only 1 underflowed.
   SubU_W 1, LocReg(Lit(8)), LocReg(Lit(8)), Lit(1)

:"$notunder01"
   ; Neither underflowed.
   Move_W 3, Stk(), LocReg(Lit(6))
   Retn   3, Stk()
}

;; EOF

