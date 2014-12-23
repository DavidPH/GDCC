;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Signed comparison routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__CmpI_GE_W2
;;
Function "___GDCC__CmpI_GE_W2" \
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
   CmpI_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpI_GT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Retn      1, Stk()

:"$eq1"
   CmpU_GE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpI_GE_W3
;;
Function "___GDCC__CmpI_GE_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Jcnd_Tru  1, Stk(), Lit(:"$eq2")

   CmpI_GT_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Retn      1, Stk()

:"$eq2"
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpU_GT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   Retn      1, Stk()

:"$eq1"
   CmpU_GE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpI_GT_W2
;;
Function "___GDCC__CmpI_GT_W2" \
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
   CmpI_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpI_GT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Retn      1, Stk()

:"$eq1"
   CmpU_GT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpI_GT_W3
;;
Function "___GDCC__CmpI_GT_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Jcnd_Tru  1, Stk(), Lit(:"$eq2")

   CmpI_GT_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Retn      1, Stk()

:"$eq2"
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpU_GT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   Retn      1, Stk()

:"$eq1"
   CmpU_GT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpI_LE_W2
;;
Function "___GDCC__CmpI_LE_W2" \
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
   CmpI_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpI_LT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Retn      1, Stk()

:"$eq1"
   CmpU_LE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpI_LE_W3
;;
Function "___GDCC__CmpI_LE_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Jcnd_Tru  1, Stk(), Lit(:"$eq2")

   CmpI_LT_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Retn      1, Stk()

:"$eq2"
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpU_LT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   Retn      1, Stk()

:"$eq1"
   CmpU_LE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpI_LT_W2
;;
Function "___GDCC__CmpI_LT_W2" \
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
   CmpI_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpI_LT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Retn      1, Stk()

:"$eq1"
   CmpU_LT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpI_LT_W3
;;
Function "___GDCC__CmpI_LT_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 6 \
   param    = 6 \
   retrn    = 1 \
   block
{
   CmpI_EQ_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Jcnd_Tru  1, Stk(), Lit(:"$eq2")

   CmpI_LT_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Retn      1, Stk()

:"$eq2"
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpU_LT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   Retn      1, Stk()

:"$eq1"
   CmpU_LT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Retn      1, Stk()
}

;; EOF

