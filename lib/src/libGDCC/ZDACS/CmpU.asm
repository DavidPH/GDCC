;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Unsigned comparison routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__CmpU_EQ_W2
;;
Function "___GDCC__CmpU_EQ_W2" \
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
   CmpU_EQ_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   AndU_W    1, Stk(), Stk(), Stk()
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpU_EQ_W3
;;
Function "___GDCC__CmpU_EQ_W3" \
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
   CmpU_EQ_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   AndU_W    1, Stk(), Stk(), Stk()
   CmpU_EQ_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   AndU_W    1, Stk(), Stk(), Stk()
   Retn      1, Stk()
}

;; Unsigned word compare logic:
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
;; ___GDCC__CmpU_GE_W1
;;
Function "___GDCC__CmpU_GE_W1" \
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
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$pos")

   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$cmp")
   Move_W   1, Stk(), Lit(1)
   Retn     1, Stk()

:"$pos"
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$cmp")
   Move_W   1, Stk(), Lit(0)
   Retn     1, Stk()

:"$cmp"
   CmpI_GE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpU_GE_W2
;;
Function "___GDCC__CmpU_GE_W2" \
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
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpU_GT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Retn      1, Stk()

:"$eq1"
   CmpU_GE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpU_GE_W3
;;
Function "___GDCC__CmpU_GE_W3" \
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
   CmpU_EQ_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Jcnd_Tru  1, Stk(), Lit(:"$eq2")

   CmpU_GT_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
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
;; ___GDCC__CmpU_GT_W1
;;
Function "___GDCC__CmpU_GT_W1" \
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
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$pos")

   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$cmp")
   Move_W   1, Stk(), Lit(1)
   Retn     1, Stk()

:"$pos"
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$cmp")
   Move_W   1, Stk(), Lit(0)
   Retn     1, Stk()

:"$cmp"
   CmpI_GT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpU_GT_W2
;;
Function "___GDCC__CmpU_GT_W2" \
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
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpU_GT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Retn      1, Stk()

:"$eq1"
   CmpU_GT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpU_GT_W3
;;
Function "___GDCC__CmpU_GT_W3" \
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
   CmpU_EQ_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Jcnd_Tru  1, Stk(), Lit(:"$eq2")

   CmpU_GT_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
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
;; ___GDCC__CmpU_LE_W1
;;
Function "___GDCC__CmpU_LE_W1" \
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
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$pos")

   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$cmp")
   Move_W   1, Stk(), Lit(0)
   Retn     1, Stk()

:"$pos"
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$cmp")
   Move_W   1, Stk(), Lit(1)
   Retn     1, Stk()

:"$cmp"
   CmpI_LE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpU_LE_W2
;;
Function "___GDCC__CmpU_LE_W2" \
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
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpU_LT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Retn      1, Stk()

:"$eq1"
   CmpU_LE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpU_LE_W3
;;
Function "___GDCC__CmpU_LE_W3" \
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
   CmpU_EQ_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Jcnd_Tru  1, Stk(), Lit(:"$eq2")

   CmpU_LT_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
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
;; ___GDCC__CmpU_LT_W1
;;
Function "___GDCC__CmpU_LT_W1" \
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
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$pos")

   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Tru 1, Stk(), Lit(:"$cmp")
   Move_W   1, Stk(), Lit(0)
   Retn     1, Stk()

:"$pos"
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$cmp")
   Move_W   1, Stk(), Lit(1)
   Retn     1, Stk()

:"$cmp"
   CmpI_LT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpU_LT_W2
;;
Function "___GDCC__CmpU_LT_W2" \
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
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Jcnd_Tru  1, Stk(), Lit(:"$eq1")

   CmpU_LT_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Retn      1, Stk()

:"$eq1"
   CmpU_LT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpU_LT_W3
;;
Function "___GDCC__CmpU_LT_W3" \
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
   CmpU_EQ_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   Jcnd_Tru  1, Stk(), Lit(:"$eq2")

   CmpU_LT_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
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

;;
;; ___GDCC__CmpU_NE_W2
;;
Function "___GDCC__CmpU_NE_W2" \
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
   CmpU_NE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   CmpU_NE_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   OrIU_W    1, Stk(), Stk(), Stk()
   Retn      1, Stk()
}

;;
;; ___GDCC__CmpU_NE_W3
;;
Function "___GDCC__CmpU_NE_W3" \
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
   CmpU_NE_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   CmpU_NE_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(4))
   OrIU_W    1, Stk(), Stk(), Stk()
   CmpU_NE_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(5))
   OrIU_W    1, Stk(), Stk(), Stk()
   Retn      1, Stk()
}

;; EOF

