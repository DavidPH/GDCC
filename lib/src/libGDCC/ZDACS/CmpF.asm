;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Float comparison routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Macros                                                                     |
;;

;;
;; CmpF_SP_W1, labelNaN, labelZero
;;
;; Performs checks for special cases of NaN and signed zero.
;;
Macro CmpF_SP_W1
{
   ; Is l NaN? If so, l != r.
   AndU_W    1, Stk(), LocReg(Lit(0)), Lit(0x7FFFFFFF)
   CmpI_GT_W 1, Stk(), Stk(),          Lit(0x7F800000)
   Jcnd_Tru  1, Stk(), Cpy(0)

   ; Is r NaN? If so, l != r.
   AndU_W    1, Stk(), LocReg(Lit(1)), Lit(0x7FFFFFFF)
   CmpI_GT_W 1, Stk(), Stk(),          Lit(0x7F800000)
   Jcnd_Tru  1, Stk(), Cpy(0)

   ; Is l and r +/-0? If so, l == r.
   OrIU_W    1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   AndU_W    1, Stk(), Stk(),          Lit(0x7FFFFFFF)
   Jcnd_Nil  1, Stk(), Cpy(1)
}

;;
;; CmpF_SP_W2, labelNaN, labelZero
;;
;; As above, but for 2-word floats.
;;
Macro CmpF_SP_W2
{
   ; Is l NaN? If so, l != r.
   AndU_W    1, Stk(), LocReg(Lit(1)), Lit(0x7FFFFFFF)
   CmpI_GT_W 1, Stk(), Stk(),          Lit(0x7FF00000)
   Jcnd_Tru  1, Stk(), Cpy(0)

   ; Is r NaN? If so, l != r.
   AndU_W    1, Stk(), LocReg(Lit(3)), Lit(0x7FFFFFFF)
   CmpI_GT_W 1, Stk(), Stk(),          Lit(0x7FF00000)
   Jcnd_Tru  1, Stk(), Cpy(0)

   ; Is l and r +/-0? If so, l == r.
   OrIU_W    1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   AndU_W    1, Stk(), Stk(),          Lit(0x7FFFFFFF)
   Jcnd_Nil  1, Stk(), Cpy(1)
}


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__CmpF_EQ_W1
;;
Function "___GDCC__CmpF_EQ_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W1, Lit("___GDCC__CmpF_EQ_W1$0"), Lit("___GDCC__CmpF_EQ_W1$1")

   ; Otherwise, bitwise equality.
   CmpU_EQ_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()

   ; Common returns for all CmpF functions.

"___GDCC__CmpF_EQ_W1$0"
   Move_W 1, Stk(), Lit(0)
   Retn   1, Stk()

"___GDCC__CmpF_EQ_W1$1"
   Move_W 1, Stk(), Lit(1)
   Retn   1, Stk()
}

;;
;; ___GDCC__CmpF_EQ_W2
;;
Function "___GDCC__CmpF_EQ_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W2, Lit("___GDCC__CmpF_EQ_W1$0"), Lit("___GDCC__CmpF_EQ_W1$1")

   ; Otherwise, bitwise equality.
   CmpU_EQ_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpF_GE_W1
;;
Function "___GDCC__CmpF_GE_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W1, Lit("___GDCC__CmpF_EQ_W1$0"), Lit("___GDCC__CmpF_EQ_W1$1")

   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1")

   ; l > 0

   ; Is r negative? If so, l > r.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$1")

   ; l > 0, r > 0
   CmpI_GE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()

:"$l1"
   ; l < 0

   ; Is r negative? If not, l < r.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$0")

   ; l < 0, r < 0
   CmpI_LE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpF_GE_W2
;;
Function "___GDCC__CmpF_GE_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W2, Lit("___GDCC__CmpF_EQ_W1$0"), Lit("___GDCC__CmpF_EQ_W1$1")

   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1")

   ; l > 0

   ; Is r negative? If so, l > r.
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$1")

   ; l > 0, r > 0
   CmpI_GE_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()

:"$l1"
   ; l < 0

   ; Is r negative? If not, l < r.
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$0")

   ; l < 0, r < 0
   CmpI_LE_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpF_GT_W1
;;
Function "___GDCC__CmpF_GT_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W1, Lit("___GDCC__CmpF_EQ_W1$0"), Lit("___GDCC__CmpF_EQ_W1$0")

   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1")

   ; l > 0

   ; Is r negative? If so, l > r.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$1")

   ; l > 0, r > 0
   CmpI_GT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()

:"$l1"
   ; l < 0

   ; Is r negative? If not, l < r.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$0")

   ; l < 0, r < 0
   CmpI_LT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpF_GT_W2
;;
Function "___GDCC__CmpF_GT_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W2, Lit("___GDCC__CmpF_EQ_W1$0"), Lit("___GDCC__CmpF_EQ_W1$0")

   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1")

   ; l > 0

   ; Is r negative? If so, l > r.
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$1")

   ; l > 0, r > 0
   CmpI_GT_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()

:"$l1"
   ; l < 0

   ; Is r negative? If not, l < r.
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$0")

   ; l < 0, r < 0
   CmpI_LT_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpF_LE_W1
;;
Function "___GDCC__CmpF_LE_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W1, Lit("___GDCC__CmpF_EQ_W1$0"), Lit("___GDCC__CmpF_EQ_W1$1")

   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1")

   ; l > 0

   ; Is r negative? If so, l > r.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$0")

   ; l > 0, r > 0
   CmpI_LE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()

:"$l1"
   ; l < 0

   ; Is r negative? If not, l < r.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$1")

   ; l < 0, r < 0
   CmpI_GE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpF_LE_W2
;;
Function "___GDCC__CmpF_LE_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W2, Lit("___GDCC__CmpF_EQ_W1$0"), Lit("___GDCC__CmpF_EQ_W1$1")

   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1")

   ; l > 0

   ; Is r negative? If so, l > r.
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$0")

   ; l > 0, r > 0
   CmpI_LE_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()

:"$l1"
   ; l < 0

   ; Is r negative? If not, l < r.
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$1")

   ; l < 0, r < 0
   CmpI_GE_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpF_LT_W1
;;
Function "___GDCC__CmpF_LT_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W1, Lit("___GDCC__CmpF_EQ_W1$0"), Lit("___GDCC__CmpF_EQ_W1$0")

   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1")

   ; l > 0

   ; Is r negative? If so, l > r.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$0")

   ; l > 0, r > 0
   CmpI_LT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()

:"$l1"
   ; l < 0

   ; Is r negative? If not, l < r.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$1")

   ; l < 0, r < 0
   CmpI_GT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpF_LT_W2
;;
Function "___GDCC__CmpF_LT_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W2, Lit("___GDCC__CmpF_EQ_W1$0"), Lit("___GDCC__CmpF_EQ_W1$0")

   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$l1")

   ; l > 0

   ; Is r negative? If so, l > r.
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$0")

   ; l > 0, r > 0
   CmpI_LT_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()

:"$l1"
   ; l < 0

   ; Is r negative? If not, l < r.
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit("___GDCC__CmpF_EQ_W1$1")

   ; l < 0, r < 0
   CmpI_GT_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpF_NE_W1
;;
Function "___GDCC__CmpF_NE_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 2 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W1, Lit("___GDCC__CmpF_EQ_W1$1"), Lit("___GDCC__CmpF_EQ_W1$0")

   ; Otherwise, bitwise equality.
   CmpU_NE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpF_NE_W2
;;
Function "___GDCC__CmpF_NE_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 1 \
   block
{
   ; Check special cases.
  !CmpF_SP_W2, Lit("___GDCC__CmpF_EQ_W1$1"), Lit("___GDCC__CmpF_EQ_W1$0")

   ; Otherwise, bitwise equality.
   CmpU_NE_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;; EOF

