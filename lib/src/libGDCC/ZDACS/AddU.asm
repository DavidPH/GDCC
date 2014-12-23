;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Multi-word add routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__AddU_W2
;;
Function "___GDCC__AddU_W2" \
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
   AddU_W 1, LocReg(Lit(4)), LocReg(Lit(0)), LocReg(Lit(2))

   CmpU_LT_W 1, Stk(), LocReg(Lit(4)), LocReg(Lit(0))
   Jcnd_Nil  1, Stk(), Lit(:"$notover0")

   Move_W 1, Stk(), LocReg(Lit(4))
   AddU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   AddU_W 1, Stk(), Stk(), Lit(1)
   Retn   2, Stk()

:"$notover0"
   Move_W 1, Stk(), LocReg(Lit(4))
   AddU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Retn   2, Stk()
}

;;
;; ___GDCC__AddU_W3
;;
Function "___GDCC__AddU_W3" \
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
   AddU_W 1, LocReg(Lit(6)), LocReg(Lit(0)), LocReg(Lit(3))
   AddU_W 1, LocReg(Lit(7)), LocReg(Lit(1)), LocReg(Lit(4))
   AddU_W 1, LocReg(Lit(8)), LocReg(Lit(2)), LocReg(Lit(5))

   CmpU_LT_W 1, Stk(), LocReg(Lit(6)), LocReg(Lit(0))
   Jcnd_Nil  1, Stk(), Lit(:"$notover0")

   CmpU_LT_W 1, Stk(), LocReg(Lit(7)), LocReg(Lit(1))
   Jcnd_Nil  1, Stk(), Lit(:"$notover1")

   ; Both overflowed.
   AddU_W 1,  LocReg(Lit(7)), LocReg(Lit(7)), Lit(1)
   AddU_W 1,  LocReg(Lit(8)), LocReg(Lit(8)), Lit(1)

   Move_W 3, Stk(), LocReg(Lit(6))
   Retn   3, Stk()

:"$notover1"
   ; Only 0 overflowed.
   AddU_W   1, LocReg(Lit(7)), LocReg(Lit(7)), Lit(1)

   ; Recheck 1 for overflow.
   Move_W   1, Stk(), LocReg(Lit(7))
   Jcnd_Tru 1, Stk(), Lit(:"$notover1retn")

   AddU_W   1, LocReg(Lit(8)), LocReg(Lit(8)), Lit(1)

:"$notover1retn"
   Move_W 3, Stk(), LocReg(Lit(6))
   Retn   3, Stk()

:"$notover0"
   CmpU_LT_W 1, Stk(), LocReg(Lit(7)), LocReg(Lit(1))
   Jcnd_Nil  1, Stk(), Lit(:"$notover01")

   ; Only 1 overflowed.
   AddU_W 1, LocReg(Lit(8)), LocReg(Lit(8)), Lit(1)

:"$notover01"
   ; Neither overflowed.
   Move_W 3, Stk(), LocReg(Lit(6))
   Retn   3, Stk()
}

;; EOF

