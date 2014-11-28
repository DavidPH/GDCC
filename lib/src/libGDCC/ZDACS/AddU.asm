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
   label    = "___GDCC__AddU_W2$label" \
   linka    = ExtC \
   localReg = 5 \
   param    = 4 \
   retrn    = 2 \
   block
{
   AddU_W, LocReg(Lit(4), 0), LocReg(Lit(0), 0), LocReg(Lit(2), 0)

   CmpU_LT_W, Stk(), LocReg(Lit(4), 0), LocReg(Lit(0), 0)
   Jcnd_Nil,  Stk(), Lit("___GDCC__AddU_W2$notover0")

   Move_W, Stk(), LocReg(Lit(4), 0)
   AddU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   AddU_W, Stk(), Stk(), Lit(1)
   Retn,   Stk(), Stk()

"___GDCC__AddU_W2$notover0"
   Move_W, Stk(), LocReg(Lit(4), 0)
   AddU_W, Stk(), LocReg(Lit(1), 0), LocReg(Lit(3), 0)
   Retn,   Stk(), Stk()
}

;;
;; ___GDCC__AddU_W3
;;
Function "___GDCC__AddU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = "___GDCC__AddU_W3$label" \
   linka    = ExtC \
   localReg = 9 \
   param    = 6 \
   retrn    = 3 \
   block
{
   AddU_W, LocReg(Lit(6), 0), LocReg(Lit(0), 0), LocReg(Lit(3), 0)
   AddU_W, LocReg(Lit(7), 0), LocReg(Lit(1), 0), LocReg(Lit(4), 0)
   AddU_W, LocReg(Lit(8), 0), LocReg(Lit(2), 0), LocReg(Lit(5), 0)

   CmpU_LT_W, Stk(), LocReg(Lit(6), 0), LocReg(Lit(0), 0)
   Jcnd_Nil,  Stk(), Lit("___GDCC__AddU_W3$notover0")

   CmpU_LT_W, Stk(), LocReg(Lit(7), 0), LocReg(Lit(1), 0)
   Jcnd_Nil,  Stk(), Lit("___GDCC__AddU_W3$notover1")

   ; Both overflowed.
   AddU_W,  LocReg(Lit(7), 0), LocReg(Lit(7), 0), Lit(1)
   AddU_W,  LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(1)

   Move_W3, Stk(), LocReg(Lit(6), 0)
   Retn,    Stk(), Stk(), Stk()

"___GDCC__AddU_W3$notover1"
   ; Only 0 overflowed.
   AddU_W,   LocReg(Lit(7), 0), LocReg(Lit(7), 0), Lit(1)

   ; Recheck 1 for overflow.
   Move_W,   Stk(), LocReg(Lit(7), 0)
   Jcnd_Tru, Stk(), Lit("___GDCC__AddU_W3$notover1retn")

   AddU_W,   LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(1)

"___GDCC__AddU_W3$notover1retn"
   Move_W3,  Stk(), LocReg(Lit(6), 0)
   Retn,     Stk(), Stk(), Stk()

"___GDCC__AddU_W3$notover0"
   CmpU_LT_W, Stk(), LocReg(Lit(7), 0), LocReg(Lit(1), 0)
   Jcnd_Nil,  Stk(), Lit("___GDCC__AddU_W3$notover01")

   ; Only 1 overflowed.
   AddU_W, LocReg(Lit(8), 0), LocReg(Lit(8), 0), Lit(1)

"___GDCC__AddU_W3$notover01"
   ; Neither overflowed.
   Move_W3, Stk(), LocReg(Lit(6), 0)
   Retn,    Stk(), Stk(), Stk()
}

;; EOF

