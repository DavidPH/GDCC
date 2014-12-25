;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Float sub routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__SubF_W1
;;
Function "___GDCC__SubF_W1" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 5 \
   param    = 2 \
   retrn    = 1 \
   block
{
   ; +0 - +0 = +0
   OrIU_W   1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Jcnd_Nil 1, Stk(), Lit(:"$remin")

   ; -0 - -0 = +0
   CmpU_EQ_W 1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   AndU_W    1, Stk(), Stk(), Stk()
   Jcnd_Tru  1, Stk(), Lit(:"$lemin")

   ; Is l negative? l - r = -(-l + r)
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$lpos")
   NegF_W   1, Stk(), LocReg(Lit(0))
   Move_W   1, Stk(), LocReg(Lit(1))
   AddF_W   1, Stk(), Stk(), Stk()
   NegF_W   1, Stk(), Stk()
   Retn     1, Stk()
:"$lpos"

   ; Is r negative? l - r = l + -r
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$rpos")
   Move_W   1, Stk(), LocReg(Lit(0))
   NegF_W   1, Stk(), LocReg(Lit(1))
   AddF_W   1, Stk(), Stk(), Stk()
   Retn     1, Stk()
:"$rpos"

   ; Does l have special exponent?
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x7F800000)
   Casm   0, Lit(84), Lit(0x7F800000), Lit(:"$lemax") ; Jcnd_Lit
   Casm   0, Lit(84), Lit(0x00000000), Lit(:"$lemin") ; Jcnd_Lit
   ShRI_W 1, LocReg(Lit(2)), Stk(), Lit(23)

   ; Does r have special exponent?
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x7F800000)
   Casm   0, Lit(84), Lit(0x7F800000), Lit(:"$remax") ; Jcnd_Lit
   Casm   0, Lit(84), Lit(0x00000000), Lit(:"$remin") ; Jcnd_Lit
   ShRI_W 1, LocReg(Lit(3)), Stk(), Lit(23)


   ; Both are normalized.

   ; Is l > r?
   CmpI_GT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Jcnd_Tru  1, Stk(), Lit(:"$lgtr")

   ; Is l < r?
   CmpI_LT_W 1, Stk(), LocReg(Lit(0)), LocReg(Lit(1))
   Jcnd_Tru  1, Stk(), Lit(:"$lltr")


   ; l == r

   ; Return 0.
   Move_W 1, Stk(), Lit(0x00000000)
   Retn   1, Stk()


   ; l > r
:"$lgtr"
   ; Calculate exponent difference.
   SubU_W 1, LocReg(Lit(4)), LocReg(Lit(2)), LocReg(Lit(3))

   ; If difference is >=24, r is too small to affect l.
   CmpI_GE_W 1, Stk(), LocReg(Lit(4)), Lit(24)
   Jcnd_Tru  1, Stk(), Lit(:"$remin")

   ; l.manfull - (r.manfull >> difference)
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00800000)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00800000)
   ShRI_W 1, Stk(), Stk(),          LocReg(Lit(4))
   SubU_W 1, LocReg(Lit(4)), Stk(), Stk()

   CmpI_LE_W 1, Stk(), LocReg(Lit(4)), Lit(0x00FFFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lgtr_shlc")

   ShRI_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)
   AddI_W 1, LocReg(Lit(2)), LocReg(Lit(2)), Lit(1)
   Jump   0, Lit(:"$lgtr_ret")

:"$lgtr_shl"
   ShLU_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)
   SubI_W 1, LocReg(Lit(2)), LocReg(Lit(2)), Lit(1)

   Move_W   1, Stk(), LocReg(Lit(2))
   Jcnd_Nil 1, Stk(), Lit(:"$poszero")

:"$lgtr_shlc"
   CmpI_LE_W 1, Stk(), LocReg(Lit(4)), Lit(0x007FFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lgtr_shl")

:"$lgtr_ret"
   ShLU_W 1, Stk(), LocReg(Lit(2)), Lit(23)
   AndU_W 1, Stk(), LocReg(Lit(4)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   1, Stk()


   ; l < r
:"$lltr"
   ; Calculate exponent difference.
   SubU_W 1, LocReg(Lit(4)), LocReg(Lit(3)), LocReg(Lit(2))

   ; If difference is >=24, l is too small to affect r.
   CmpI_GE_W 1, Stk(), LocReg(Lit(4)), Lit(24)
   Jcnd_Tru  1, Stk(), Lit(:"$lemin")

   ; -(r.manfull - (l.manfull >> difference))
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00800000)
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00800000)
   ShRI_W 1, Stk(), Stk(),          LocReg(Lit(4))
   SubU_W 1, LocReg(Lit(4)), Stk(), Stk()

   CmpI_LE_W 1, Stk(), LocReg(Lit(4)), Lit(0x00FFFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lltr_shlc")

   ShRI_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)
   AddI_W 1, LocReg(Lit(3)), LocReg(Lit(3)), Lit(1)
   Jump   0, Lit(:"$lltr_ret")

:"$lltr_shl"
   ShLU_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)
   SubI_W 1, LocReg(Lit(3)), LocReg(Lit(3)), Lit(1)

   Move_W   1, Stk(), LocReg(Lit(3))
   Jcnd_Nil 1, Stk(), Lit(:"$negzero")

:"$lltr_shlc"
   CmpI_LE_W 1, Stk(), LocReg(Lit(4)), Lit(0x007FFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lltr_shl")

:"$lltr_ret"
   ShLU_W 1, Stk(), LocReg(Lit(3)), Lit(23)
   AndU_W 1, Stk(), LocReg(Lit(4)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(), Stk()
   NegF_W 1, Stk(), Stk()
   Retn   1, Stk()


   ; l has max exponent. It is either INF or NaN.
:"$lemax"
   ; Is r also max exponent?
   AndU_W    1, Stk(), LocReg(Lit(1)), Lit(0x7F800000)
   CmpU_EQ_W 1, Stk(), Stk(),          Lit(0x7F800000)
   Jcnd_Tru  1, Stk(), Lit(:"$lremax")

   ; r has min exponent. Therefore, r == 0 and the result is l.
:"$remin"
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   1, Stk()

   ; r has max exponent. It is either INF or NaN. Either way, return it.
:"$remax"
   ; l has min exponent. Therefore, l == 0 and the result is -r.
:"$lemin"
   NegF_W 1, Stk(), LocReg(Lit(1))
   Retn   1, Stk()

   ; l and r have max exponent.
:"$lremax"
   ; Is l NaN? If so, return l.
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x007FFFFF)
   Jcnd_Tru 1, Stk(), Lit(:"$remin")

   ; Is r NaN? If so, return r. (Sign inversion is fine.)
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x007FFFFF)
   Jcnd_Tru 1, Stk(), Lit(:"$lemin")

   ; +INF - +INF = NaN
   Move_W 1, Stk(), Lit(0x7FFFFFFF)
   Retn   1, Stk()

:"$poszero"
   Move_W 1, Stk(), Lit(0x00000000)
   Retn   1, Stk()

:"$negzero"
   Move_W 1, Stk(), Lit(0x80000000)
   Retn   1, Stk()
}

;;
;; ___GDCC__SubF_W2
;;
Function "___GDCC__SubF_W2" \
   alloc    = 1 \
   ctype    = StkCall \
   defin    = 1 \
   label    = :"$label" \
   linka    = ExtC \
   localReg = 8 \
   param    = 4 \
   retrn    = 2 \
   block
{
   ; +0 - +0 = +0
   OrIU_W   1, Stk(), LocReg(Lit(1)), LocReg(Lit(3))
   Jcnd_Nil 1, Stk(), Lit(:"$remin")

   ; -0 - -0 = +0
   CmpU_EQ_W 1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   CmpU_EQ_W 1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   AndU_W    1, Stk(), Stk(), Stk()
   Jcnd_Tru  1, Stk(), Lit(:"$lemin")

   ; Is l negative? l - r = -(-l + r)
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$lpos")
   NegF_W   2, Stk(), LocReg(Lit(0))
   Move_W   2, Stk(), LocReg(Lit(2))
   AddF_W   2, Stk(), Stk(), Stk()
   NegF_W   2, Stk(), Stk()
   Retn     2, Stk()
:"$lpos"

   ; Is r negative? l - r = l + -r
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$rpos")
   Move_W   2, Stk(), LocReg(Lit(0))
   NegF_W   2, Stk(), LocReg(Lit(2))
   AddF_W   2, Stk(), Stk(), Stk()
   Retn     2, Stk()
:"$rpos"

   ; Does l have special exponent?
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x7FF00000)
   Casm   0, Lit(84), Lit(0x7FF00000), Lit(:"$lemax") ; Jcnd_Lit
   Casm   0, Lit(84), Lit(0x00000000), Lit(:"$lemin") ; Jcnd_Lit
   ShRI_W 1, LocReg(Lit(4)), Stk(), Lit(20)

   ; Does r have special exponent?
   AndU_W 1, Stk(), LocReg(Lit(3)), Lit(0x7FF00000)
   Casm   0, Lit(84), Lit(0x7FF00000), Lit(:"$remax") ; Jcnd_Lit
   Casm   0, Lit(84), Lit(0x00000000), Lit(:"$remin") ; Jcnd_Lit
   ShRI_W 1, LocReg(Lit(5)), Stk(), Lit(20)


   ; Both are normalized.

   ; Is l > r?
   CmpI_GT_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Jcnd_Tru  1, Stk(), Lit(:"$lgtr")

   ; Is l < r?
   CmpI_LT_W 2, Stk(), LocReg(Lit(0)), LocReg(Lit(2))
   Jcnd_Tru  1, Stk(), Lit(:"$lltr")


   ; l == r

   ; Return 0.
   Move_W 2, Stk(), Lit(0x0000000000000000_64.0)
   Retn   2, Stk()


   ; l > r
:"$lgtr"
   ; Calculate exponent difference.
   SubU_W 1, LocReg(Lit(6)), LocReg(Lit(4)), LocReg(Lit(5))

   ; If difference is >=53, r is too small to affect l.
   CmpI_GE_W 1, Stk(), LocReg(Lit(6)), Lit(53)
   Jcnd_Tru  1, Stk(), Lit(:"$remin")

   ; l.manfull - (r.manfull >> difference)
   Move_W 1, Stk(), LocReg(Lit(0))
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00100000)
   Move_W 1, Stk(), LocReg(Lit(2))
   AndU_W 1, Stk(), LocReg(Lit(3)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00100000)
   ShRI_W 2, Stk(), Stk(),          LocReg(Lit(6))
   SubU_W 2, LocReg(Lit(6)), Stk(), Stk()

   CmpI_LE_W 1, Stk(), LocReg(Lit(7)), Lit(0x001FFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lgtr_shlc")

   ShRI_W 2, LocReg(Lit(6)), LocReg(Lit(6)), Lit(1)
   AddI_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)
   Jump   0, Lit(:"$lgtr_ret")

:"$lgtr_shl"
   ShLU_W 2, LocReg(Lit(6)), LocReg(Lit(6)), Lit(1)
   SubI_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)

   Move_W   1, Stk(), LocReg(Lit(4))
   Jcnd_Nil 1, Stk(), Lit(:"$poszero")

:"$lgtr_shlc"
   CmpI_LE_W 1, Stk(), LocReg(Lit(7)), Lit(0x000FFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lgtr_shl")

:"$lgtr_ret"
   Move_W 1, Stk(), LocReg(Lit(6))
   ShLU_W 1, Stk(), LocReg(Lit(4)), Lit(20)
   AndU_W 1, Stk(), LocReg(Lit(7)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   2, Stk()


   ; l < r
:"$lltr"
   ; Calculate exponent difference.
   SubU_W 1, LocReg(Lit(6)), LocReg(Lit(5)), LocReg(Lit(4))

   ; If difference is >=53, l is too small to affect r.
   CmpI_GE_W 1, Stk(), LocReg(Lit(6)), Lit(53)
   Jcnd_Tru  1, Stk(), Lit(:"$lemin")

   ; -(r.manfull - (l.manfull >> difference))
   Move_W 1, Stk(), LocReg(Lit(2))
   AndU_W 1, Stk(), LocReg(Lit(3)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00100000)
   Move_W 1, Stk(), LocReg(Lit(0))
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00100000)
   ShRI_W 2, Stk(), Stk(),          LocReg(Lit(6))
   SubU_W 2, LocReg(Lit(6)), Stk(), Stk()

   CmpI_LE_W 1, Stk(), LocReg(Lit(7)), Lit(0x001FFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lltr_shlc")

   ShRI_W 2, LocReg(Lit(6)), LocReg(Lit(6)), Lit(1)
   AddI_W 1, LocReg(Lit(5)), LocReg(Lit(5)), Lit(1)
   Jump   0, Lit(:"$lltr_ret")

:"$lltr_shl"
   ShLU_W 2, LocReg(Lit(6)), LocReg(Lit(6)), Lit(1)
   SubI_W 1, LocReg(Lit(5)), LocReg(Lit(5)), Lit(1)

   Move_W   1, Stk(), LocReg(Lit(5))
   Jcnd_Nil 1, Stk(), Lit(:"$negzero")

:"$lltr_shlc"
   CmpI_LE_W 1, Stk(), LocReg(Lit(7)), Lit(0x000FFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lltr_shl")

:"$lltr_ret"
   Move_W 1, Stk(), LocReg(Lit(6))
   ShLU_W 1, Stk(), LocReg(Lit(5)), Lit(20)
   AndU_W 1, Stk(), LocReg(Lit(7)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(), Stk()
   NegF_W 1, Stk(), Stk()
   Retn   2, Stk()


   ; l has max exponent. It is either INF or NaN.
:"$lemax"
   ; Is r also max exponent?
   AndU_W    1, Stk(), LocReg(Lit(3)), Lit(0x7FF00000)
   CmpU_EQ_W 1, Stk(), Stk(),          Lit(0x7FF00000)
   Jcnd_Tru  1, Stk(), Lit(:"$lremax")

   ; r has min exponent. Therefore, r == 0 and the result is l.
:"$remin"
   Move_W 2, Stk(), LocReg(Lit(0))
   Retn   2, Stk()

   ; r has max exponent. It is either INF or NaN. Either way, return it.
:"$remax"
   ; l has min exponent. Therefore, l == 0 and the result is -r.
:"$lemin"
   NegF_W 2, Stk(), LocReg(Lit(2))
   Retn   2, Stk()

   ; l and r have max exponent.
:"$lremax"
   ; Is l NaN? If so, return l.
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x000FFFFF)
   OrIU_W   1, Stk(), Stk(), LocReg(Lit(0))
   Jcnd_Tru 1, Stk(), Lit(:"$remin")

   ; Is r NaN? If so, return r. (Sign inversion is fine.)
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x000FFFFF)
   OrIU_W   1, Stk(), Stk(), LocReg(Lit(2))
   Jcnd_Tru 1, Stk(), Lit(:"$lemin")

   ; +INF - +INF = NaN
   Move_W 2, Stk(), Lit(0x7FFFFFFFFFFFFFFF_64.0)
   Retn   2, Stk()

:"$poszero"
   ; Return +0.
   Move_W 2, Stk(), Lit(0x0000000000000000_64.0)
   Retn   2, Stk()

:"$negzero"
   ; Return -0.
   Move_W 2, Stk(), Lit(0x8000000000000000_64.0)
   Retn   2, Stk()
}

;; EOF

