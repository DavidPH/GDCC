;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Shift routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__ShLF_W1
;;
Function "___GDCC__ShLF_W1" \
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
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x7F800000)
   Casm   0, Lit(84), Lit(0x00000000), Lit(:"$emax") ; Jcnd_Lit
   Casm   0, Lit(84), Lit(0x7F800000), Lit(:"$emax") ; Jcnd_Lit
   ShRI_W 1, Stk(), Stk(), Lit(23)
   AddU_W 1, LocReg(Lit(1)), LocReg(Lit(1)), Stk()

   CmpI_GE_W 1, Stk(), LocReg(Lit(1)), Lit(0xFF)
   Jcnd_Tru  1, Stk(), Lit(:"$inf")

   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x807FFFFF)
   ShLU_W 1, Stk(), LocReg(Lit(1)), Lit(23)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   1, Stk()

:"$inf"
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   OrIU_W 1, Stk(), Stk(),             Lit(0x7F800000)
   Retn   1, Stk()

   ; If l is inf, nan, or zero, just return it.
:"$emax"
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   1, Stk()
}

;;
;; ___GDCC__ShLF_W2
;;
Function "___GDCC__ShLF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   Casm   0,   Lit(84), Lit(0x00000000), Lit(:"$emax") ; Jcnd_Lit
   Casm   0,   Lit(84), Lit(0x7FF00000), Lit(:"$emax") ; Jcnd_Lit
   ShRI_W 1, Stk(), Stk(), Lit(20)
   AddU_W 1, LocReg(Lit(2)), LocReg(Lit(2)), Stk()

   CmpI_GE_W 1, Stk(), LocReg(Lit(2)), Lit(0x7FF)
   Jcnd_Tru  1, Stk(), Lit(:"$inf")

   Move_W 1, Stk(), LocReg(Lit(0))
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x800FFFFF)
   ShLU_W 1, Stk(), LocReg(Lit(2)), Lit(20)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   2, Stk()

:"$inf"
   Move_W 1, Stk(), Lit(0)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   OrIU_W 1, Stk(), Stk(),             Lit(0x7FF00000)
   Retn   2, Stk()

   ; If l is inf, nan, or zero, just return it.
:"$emax"
   Move_W 2, Stk(), LocReg(Lit(0))
   Retn   2, Stk()
}

;;
;; ___GDCC__ShLU_W2
;;
Function "___GDCC__ShLU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   Move_W    1, Stk(), LocReg(Lit(2))
   Casm      0, Lit(84), Lit(0),  Lit(:"$0")  ; Jcnd_Lit
   Casm      0, Lit(84), Lit(32), Lit(:"$32") ; Jcnd_Lit
   CmpI_LT_W 1, Stk(), Stk(), Lit(32)
   Jcnd_Nil  1, Stk(), Lit(:"$gt32")

   ; ret0 = l0 << r
   ShLU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))

   ; ret1 = (l1 << r) | (l0 >> (32 - r))
   ShLU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   Move_W 1, Stk(), LocReg(Lit(0))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(2))
   ShRU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   Retn   2, Stk()

:"$gt32"
   ; ret0 = 0
   Move_W 1, Stk(), Lit(0)

   ; ret1 = l0 << (r - 32)
   Move_W 1, Stk(), LocReg(Lit(0))
   SubU_W 1, Stk(), LocReg(Lit(2)), Lit(32)
   ShLU_W 1, Stk(), Stk(), Stk()

   Retn   2, Stk()

:"$0"
   Move_W 1, Stk(), LocReg(Lit(0))
   Move_W 1, Stk(), LocReg(Lit(1))
   Retn   2, Stk()

:"$32"
   Move_W 1, Stk(), Lit(0)
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   2, Stk()
}

;;
;; ___GDCC__ShLU_W3
;;
Function "___GDCC__ShLU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 3 \
   block
{
   Move_W    1, Stk(), LocReg(Lit(3))
   Casm      0, Lit(84), Lit(0),  Lit(:"$0")  ; Jcnd_Lit
   Casm      0, Lit(84), Lit(32), Lit(:"$32") ; Jcnd_Lit
   Casm      0, Lit(84), Lit(64), Lit(:"$64") ; Jcnd_Lit
   CmpI_LT_W 1, Stk(), Stk(), Lit(32)
   Jcnd_Nil  1, Stk(), Lit(:"$gt32")

   ; ret0 = l0 << r
   ShLU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))

   ; ret1 = (l1 << r) | (l0 >> (32 - r))
   ShLU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Move_W 1, Stk(), LocReg(Lit(0))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(3))
   ShRU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   ; ret2 = (l2 << r) | (l1 >> (32 - r))
   ShLU_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(3))
   Move_W 1, Stk(), LocReg(Lit(1))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(3))
   ShRU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   Retn   3, Stk()

:"$gt32"
   CmpI_LT_W 1, Stk(), LocReg(Lit(3)), Lit(64)
   Jcnd_Nil  1, Stk(), Lit(:"$gt64")

   ; r -= 32
   SubU_W 1, LocReg(Lit(3)), LocReg(Lit(3)), Lit(32)

   ; ret0 = 0
   Move_W 1, Stk(), Lit(0)

   ; ret1 = l0 << r
   ShLU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))

   ; ret2 = (l1 << r) | (l0 >> (32 - r))
   ShLU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Move_W 1, Stk(), LocReg(Lit(0))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(3))
   ShRU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   Retn   3, Stk()

:"$gt64"
   ; ret0 = 0
   Move_W 1, Stk(), Lit(0)

   ; ret1 = 0
   Move_W 1, Stk(), Lit(0)

   ; ret2 = l0 << (r - 64)
   Move_W 1, Stk(), LocReg(Lit(0))
   SubU_W 1, Stk(), LocReg(Lit(3)), Lit(64)
   ShLU_W 1, Stk(), Stk(), Stk()

   Retn   3, Stk()

:"$0"
   Move_W 1, Stk(), LocReg(Lit(0))
   Move_W 1, Stk(), LocReg(Lit(1))
   Move_W 1, Stk(), LocReg(Lit(2))
   Retn   3, Stk()

:"$32"
   Move_W 1, Stk(), Lit(0)
   Move_W 1, Stk(), LocReg(Lit(0))
   Move_W 1, Stk(), LocReg(Lit(1))
   Retn   3, Stk()

:"$64"
   Move_W 1, Stk(), Lit(0)
   Move_W 1, Stk(), Lit(0)
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   3, Stk()
}

;;
;; ___GDCC__ShRF_W1
;;
Function "___GDCC__ShRF_W1" \
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
   ShLU_W 1, LocReg(Lit(1)), LocReg(Lit(1)), Lit(23)

   AndU_W    1, Stk(), LocReg(Lit(0)), Lit(0x7F800000)
   Casm      0, Lit(84), Lit(0x7F800000), Lit(:"$emax") ; Jcnd_Lit
   CmpI_LE_W 1, Stk(), Stk(), LocReg(Lit(1))
   Jcnd_Tru  1, Stk(), Lit(:"$zero")

   SubU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Retn   1, Stk()

:"$zero"
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Retn   1, Stk()

   ; If l is inf or nan, just return it.
:"$emax"
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   1, Stk()
}

;;
;; ___GDCC__ShRF_W2
;;
Function "___GDCC__ShRF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   ShLU_W 1, LocReg(Lit(2)), LocReg(Lit(2)), Lit(20)

   AndU_W    1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   Casm      0, Lit(84), Lit(0x7FF00000), Lit(:"$emax") ; Jcnd_Lit
   CmpI_LE_W 1, Stk(), Stk(), LocReg(Lit(2))
   Jcnd_Tru  1, Stk(), Lit(:"$zero")

   Move_W 1, Stk(), LocReg(Lit(0))
   SubU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))
   Retn   2, Stk()

:"$zero"
   Move_W 1, Stk(), Lit(0)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Retn   2, Stk()

   ; If l is inf or nan, just return it.
:"$emax"
   Move_W 2, Stk(), LocReg(Lit(0))
   Retn   2, Stk()
}

;;
;; ___GDCC__ShRI_W2
;;
Function "___GDCC__ShRI_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   Move_W    1, Stk(), LocReg(Lit(2))
   Casm      0, Lit(84), Lit(0),  Lit(:"$0")  ; Jcnd_Lit
   Casm      0, Lit(84), Lit(32), Lit(:"$32") ; Jcnd_Lit
   CmpI_LT_W 1, Stk(), Stk(), Lit(32)
   Jcnd_Nil  1, Stk(), Lit(:"$gt32")

   ; ret0 = (l0 >> r) | (l1 << (32 - r))
   ShRU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Move_W 1, Stk(), LocReg(Lit(1))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(2))
   ShLU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   ; ret1 = l1 >> r
   ShRI_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))

   Retn   2, Stk()

:"$gt32"
   ; ret0 = l1 >> (r - 32)
   Move_W 1, Stk(), LocReg(Lit(1))
   SubU_W 1, Stk(), LocReg(Lit(2)), Lit(32)
   ShRI_W 1, Stk(), Stk(), Stk()

   ; ret1 = l1 < 0 ? -1 : 0
   ShRI_W 1, Stk(), LocReg(Lit(1)), Lit(31)

   Retn   2, Stk()

:"$0"
   Move_W 1, Stk(), LocReg(Lit(0))
   Move_W 1, Stk(), LocReg(Lit(1))
   Retn   2, Stk()

:"$32"
   Move_W 1, Stk(), LocReg(Lit(1))
   ShRI_W 1, Stk(), LocReg(Lit(1)), Lit(31)
   Retn   2, Stk()
}

;;
;; ___GDCC__ShRI_W3
;;
Function "___GDCC__ShRI_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 3 \
   block
{
   Move_W    1, Stk(), LocReg(Lit(3))
   Casm      0, Lit(84), Lit(0),  Lit(:"$0")  ; Jcnd_Lit
   Casm      0, Lit(84), Lit(32), Lit(:"$32") ; Jcnd_Lit
   Casm      0, Lit(84), Lit(64), Lit(:"$64") ; Jcnd_Lit
   CmpI_LT_W 1, Stk(), Stk(), Lit(32)
   Jcnd_Nil  1, Stk(), Lit(:"$gt32")

   ; ret0 = (l0 >> r) | (l1 << (32 - r))
   ShRU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Move_W 1, Stk(), LocReg(Lit(1))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(3))
   ShLU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   ; ret1 = (l1 >> r) | (l2 << (32 - r))
   ShRU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Move_W 1, Stk(), LocReg(Lit(2))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(3))
   ShLU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   ; ret2 = l2 >> r
   ShRI_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(3))

   Retn   3, Stk()

:"$gt32"
   CmpI_LT_W 1, Stk(), LocReg(Lit(3)), Lit(64)
   Jcnd_Nil  1, Stk(), Lit(:"$gt64")

   ; r -= 32
   SubU_W 1, LocReg(Lit(3)), LocReg(Lit(3)), Lit(32)

   ; ret0 = (l1 >> r) | (l2 << (32 - r))
   ShRU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Move_W 1, Stk(), LocReg(Lit(2))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(3))
   ShLU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   ; ret1 = l2 >> r
   ShRI_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(3))

   ; ret2 = l2 < 0 ? -1 : 0
   ShRI_W 1, Stk(), LocReg(Lit(2)), Lit(31)

   Retn   3, Stk()

:"$gt64"
   ; ret0 = l2 >> (r - 64)
   Move_W 1, Stk(), LocReg(Lit(2))
   SubU_W 1, Stk(), LocReg(Lit(3)), Lit(64)
   ShRI_W 1, Stk(), Stk(), Stk()

   ; ret1 = l2 < 0 ? -1 : 0
   ShRI_W 1, Stk(), LocReg(Lit(2)), Lit(31)

   ; ret2 = l2 < 0 ? -1 : 0
   ShRI_W 1, Stk(), LocReg(Lit(2)), Lit(31)

   Retn   3, Stk()

:"$0"
   Move_W 1, Stk(), LocReg(Lit(0))
   Move_W 1, Stk(), LocReg(Lit(1))
   Move_W 1, Stk(), LocReg(Lit(2))
   Retn   3, Stk()

:"$32"
   Move_W 1, Stk(), LocReg(Lit(1))
   Move_W 1, Stk(), LocReg(Lit(2))
   ShRI_W 1, Stk(), LocReg(Lit(2)), Lit(31)
   Retn   3, Stk()

:"$64"
   Move_W 1, Stk(), LocReg(Lit(2))
   ShRI_W 1, Stk(), LocReg(Lit(2)), Lit(31)
   ShRI_W 1, Stk(), LocReg(Lit(2)), Lit(31)
   Retn   3, Stk()
}

;;
;; ___GDCC__ShRU_W2
;;
Function "___GDCC__ShRU_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 3 \
   param    = 3 \
   retrn    = 2 \
   block
{
   Move_W    1, Stk(), LocReg(Lit(2))
   Casm      0, Lit(84), Lit(0),  Lit(:"$0")  ; Jcnd_Lit
   Casm      0, Lit(84), Lit(32), Lit(:"$32") ; Jcnd_Lit
   CmpI_LT_W 1, Stk(), Stk(), Lit(32)
   Jcnd_Nil  1, Stk(), Lit(:"$gt32")

   ; ret0 = (l0 >> r) | (l1 << (32 - r))
   ShRU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Move_W 1, Stk(), LocReg(Lit(1))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(2))
   ShLU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   ; ret1 = l1 >> r
   ShRU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(2))

   Retn   2, Stk()

:"$gt32"
   ; ret0 = l1 >> (r - 32)
   Move_W 1, Stk(), LocReg(Lit(1))
   SubU_W 1, Stk(), LocReg(Lit(2)), Lit(32)
   ShRU_W 1, Stk(), Stk(), Stk()

   ; ret1 = 0
   Move_W 1, Stk(), Lit(0)

   Retn   2, Stk()

:"$0"
   Move_W 1, Stk(), LocReg(Lit(0))
   Move_W 1, Stk(), LocReg(Lit(1))
   Retn   2, Stk()

:"$32"
   Move_W 1, Stk(), LocReg(Lit(1))
   Move_W 1, Stk(), Lit(0)
   Retn   2, Stk()
}

;;
;; ___GDCC__ShRU_W3
;;
Function "___GDCC__ShRU_W3" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 4 \
   param    = 4 \
   retrn    = 3 \
   block
{
   Move_W    1, Stk(), LocReg(Lit(3))
   Casm      0, Lit(84), Lit(0),  Lit(:"$0")  ; Jcnd_Lit
   Casm      0, Lit(84), Lit(32), Lit(:"$32") ; Jcnd_Lit
   Casm      0, Lit(84), Lit(64), Lit(:"$64") ; Jcnd_Lit
   CmpI_LT_W 1, Stk(), Stk(), Lit(32)
   Jcnd_Nil  1, Stk(), Lit(:"$gt32")

   ; ret0 = (l0 >> r) | (l1 << (32 - r))
   ShRU_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(3))
   Move_W 1, Stk(), LocReg(Lit(1))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(3))
   ShLU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   ; ret1 = (l1 >> r) | (l2 << (32 - r))
   ShRU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Move_W 1, Stk(), LocReg(Lit(2))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(3))
   ShLU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   ; ret2 = l2 >> r
   ShRU_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(3))

   Retn   3, Stk()

:"$gt32"
   CmpI_LT_W 1, Stk(), LocReg(Lit(3)), Lit(64)
   Jcnd_Nil  1, Stk(), Lit(:"$gt64")

   ; r -= 32
   SubU_W 1, LocReg(Lit(3)), LocReg(Lit(3)), Lit(32)

   ; ret0 = (l1 >> r) | (l2 << (32 - r))
   ShRU_W 1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Move_W 1, Stk(), LocReg(Lit(2))
   SubU_W 1, Stk(), Lit(32), LocReg(Lit(3))
   ShLU_W 1, Stk(), Stk(), Stk()
   OrIU_W 1, Stk(), Stk(), Stk()

   ; ret1 = l2 >> r
   ShRU_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(3))

   ; ret2 = 0
   Move_W 1, Stk(), Lit(0)

   Retn   3, Stk()

:"$gt64"
   ; ret0 = l2 >> (r - 64)
   Move_W 1, Stk(), LocReg(Lit(2))
   SubU_W 1, Stk(), LocReg(Lit(3)), Lit(64)
   ShRU_W 1, Stk(), Stk(), Stk()

   ; ret1 = 0
   Move_W 1, Stk(), Lit(0)

   ; ret2 = 0
   Move_W 1, Stk(), Lit(0)

   Retn   3, Stk()

:"$0"
   Move_W 1, Stk(), LocReg(Lit(0))
   Move_W 1, Stk(), LocReg(Lit(1))
   Move_W 1, Stk(), LocReg(Lit(2))
   Retn   3, Stk()

:"$32"
   Move_W 1, Stk(), LocReg(Lit(1))
   Move_W 1, Stk(), LocReg(Lit(2))
   Move_W 1, Stk(), Lit(0)
   Retn   3, Stk()

:"$64"
   Move_W 1, Stk(), LocReg(Lit(2))
   Move_W 1, Stk(), Lit(0)
   Move_W 1, Stk(), Lit(0)
   Retn   3, Stk()
}

;; EOF

