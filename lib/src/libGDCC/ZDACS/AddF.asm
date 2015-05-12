;;-----------------------------------------------------------------------------
;;
;; Copyright(C) 2014 David Hill
;;
;; See COPYLIB for license information.
;;
;;-----------------------------------------------------------------------------
;;
;; Float add routines for ZDACS.
;;
;;-----------------------------------------------------------------------------


;;----------------------------------------------------------------------------|
;; Functions                                                                  |
;;

;;
;; ___GDCC__AddF_W1
;;
Function "___GDCC__AddF_W1" \
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
   ; Is l negative? l + r = r + l = r - -l
   AndU_W   1, Stk(), LocReg(Lit(0)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$lpos")
   Move_W   1, Stk(), LocReg(Lit(1))
   NegF_W   1, Stk(), LocReg(Lit(0))
   SubF_W   1, Stk(), Stk(), Stk()
   Retn     1, Stk()
:"$lpos"

   ; Is r negative? l + r = l - -r
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$rpos")
   Move_W   1, Stk(), LocReg(Lit(0))
   NegF_W   1, Stk(), LocReg(Lit(1))
   SubF_W   1, Stk(), Stk(), Stk()
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

   ; Is l.exp > r.exp?
   CmpI_GT_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(3))
   Jcnd_Tru  1, Stk(), Lit(:"$lgtr")

   ; Is l.exp < r.exp?
   CmpI_LT_W 1, Stk(), LocReg(Lit(2)), LocReg(Lit(3))
   Jcnd_Tru  1, Stk(), Lit(:"$lltr")


   ; l.exp == r.exp

   ; l.manfull + r.manfull
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00800000)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00800000)
   AddU_W 1, LocReg(Lit(4)), Stk(), Stk()

   CmpI_LE_W 1, Stk(), LocReg(Lit(4)), Lit(0x00FFFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$leqr_ret")

   ShRI_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)
   AddI_W 1, LocReg(Lit(2)), LocReg(Lit(2)), Lit(1)

   ; If exponent increased to 0xFF, return infinity.
   CmpI_EQ_W 1, Stk(), LocReg(Lit(2)), Lit(0xFF)
   Jcnd_Tru  1, Stk(), Lit(:"$infinity")

:"$leqr_ret"
   ShLU_W 1, Stk(), LocReg(Lit(2)), Lit(23)
   AndU_W 1, Stk(), LocReg(Lit(4)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   1, Stk()


   ; l.exp > r.exp
:"$lgtr"
   ; Calculate exponent difference.
   SubU_W 1, LocReg(Lit(4)), LocReg(Lit(2)), LocReg(Lit(3))

   ; If difference is >=24, r is too small to affect l.
   CmpI_GE_W 1, Stk(), LocReg(Lit(4)), Lit(24)
   Jcnd_Tru  1, Stk(), Lit(:"$remin")

   ; l.manfull + (r.manfull >> difference)
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00800000)
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00800000)
   ShRI_W 1, Stk(), Stk(),          LocReg(Lit(4))
   AddU_W 1, LocReg(Lit(4)), Stk(), Stk()

   CmpI_LE_W 1, Stk(), LocReg(Lit(4)), Lit(0x00FFFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lgtr_ret")

   ShRI_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)
   AddI_W 1, LocReg(Lit(2)), LocReg(Lit(2)), Lit(1)

   ; If exponent increased to 0xFF, return infinity.
   CmpI_EQ_W 1, Stk(), LocReg(Lit(2)), Lit(0xFF)
   Jcnd_Tru  1, Stk(), Lit(:"$infinity")

:"$lgtr_ret"
   ShLU_W 1, Stk(), LocReg(Lit(2)), Lit(23)
   AndU_W 1, Stk(), LocReg(Lit(4)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   1, Stk()


   ; l.exp < r.exp
:"$lltr"
   ; Calculate exponent difference.
   SubU_W 1, LocReg(Lit(4)), LocReg(Lit(3)), LocReg(Lit(2))

   ; If difference is >=24, l is too small to affect r.
   CmpI_GE_W 1, Stk(), LocReg(Lit(4)), Lit(24)
   Jcnd_Tru  1, Stk(), Lit(:"$lemin")

   ; (l.manfull >> difference) + r.manfull
   AndU_W 1, Stk(), LocReg(Lit(0)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00800000)
   ShRI_W 1, Stk(), Stk(),          LocReg(Lit(4))
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00800000)
   AddU_W 1, LocReg(Lit(4)), Stk(), Stk()

   CmpI_LE_W 1, Stk(), LocReg(Lit(4)), Lit(0x00FFFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lltr_ret")

   ShRI_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)
   AddI_W 1, LocReg(Lit(3)), LocReg(Lit(3)), Lit(1)

   ; If exponent increased to 0xFF, return infinity.
   CmpI_EQ_W 1, Stk(), LocReg(Lit(3)), Lit(0xFF)
   Jcnd_Tru  1, Stk(), Lit(:"$infinity")

:"$lltr_ret"
   ShLU_W 1, Stk(), LocReg(Lit(3)), Lit(23)
   AndU_W 1, Stk(), LocReg(Lit(4)), Lit(0x007FFFFF)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   1, Stk()


   ; l has max exponent. It is either INF or NaN.
:"$lemax"
   ; Is r NaN? If so, return r.
   AndU_W    1, Stk(), LocReg(Lit(1)), Lit(0x7F800000)
   CmpU_EQ_W 1, Stk(), Stk(),          Lit(0x7F800000)
   AndU_W    1, Stk(), LocReg(Lit(1)), Lit(0x007FFFFF)
   LAnd      1, Stk(), Stk(), Stk()
   Jcnd_Tru  1, Stk(), Lit(:"$lemin")

   ; r has min exponent. Therefore, r == 0 and the result is l.
:"$remin"
   Move_W 1, Stk(), LocReg(Lit(0))
   Retn   1, Stk()

   ; r has max exponent. It is either INF or NaN. Either way, return it.
:"$remax"
   ; l has min exponent. Therefore, l == 0 and the result is r.
:"$lemin"
   Move_W 1, Stk(), LocReg(Lit(1))
   Retn   1, Stk()

:"$infinity"
   ; Return infinity.
   Move_W 1, Stk(), Lit(0x7F800000)
   Retn   1, Stk()
}

;;
;; ___GDCC__AddF_W2
;;
Function "___GDCC__AddF_W2" \
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
   ; Is l negative? l + r = r + l = r - -l
   AndU_W   1, Stk(), LocReg(Lit(1)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$lpos")
   Move_W   2, Stk(), LocReg(Lit(2))
   NegF_W   2, Stk(), LocReg(Lit(0))
   SubF_W   2, Stk(), Stk(), Stk()
   Retn     2, Stk()
:"$lpos"

   ; Is r negative? l + r = l - -r
   AndU_W   1, Stk(), LocReg(Lit(3)), Lit(0x80000000)
   Jcnd_Nil 1, Stk(), Lit(:"$rpos")
   Move_W   2, Stk(), LocReg(Lit(0))
   NegF_W   2, Stk(), LocReg(Lit(2))
   SubF_W   2, Stk(), Stk(), Stk()
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

   ; Is l.exp > r.exp?
   CmpI_GT_W 1, Stk(), LocReg(Lit(4)), LocReg(Lit(5))
   Jcnd_Tru  1, Stk(), Lit(:"$lgtr")

   ; Is l.exp < r.exp?
   CmpI_LT_W 1, Stk(), LocReg(Lit(4)), LocReg(Lit(5))
   Jcnd_Tru  1, Stk(), Lit(:"$lltr")


   ; l.exp == r.exp

   ; l.manfull + r.manfull
   Move_W 1, Stk(), LocReg(Lit(0))
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00100000)
   Move_W 1, Stk(), LocReg(Lit(2))
   AndU_W 1, Stk(), LocReg(Lit(3)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00100000)
   AddU_W 2, LocReg(Lit(6)), Stk(), Stk()

   CmpI_LE_W 1, Stk(), LocReg(Lit(7)), Lit(0x001FFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$leqr_ret")

   ShRI_W 2, LocReg(Lit(6)), LocReg(Lit(6)), Lit(1)
   AddI_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)

   ; If exponent increased to 0x7FF, return infinity.
   CmpI_EQ_W 1, Stk(), LocReg(Lit(4)), Lit(0x7FF)
   Jcnd_Tru  1, Stk(), Lit(:"$infinity")

:"$leqr_ret"
   Move_W 1, Stk(), LocReg(Lit(6))
   ShLU_W 1, Stk(), LocReg(Lit(4)), Lit(20)
   AndU_W 1, Stk(), LocReg(Lit(7)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   2, Stk()


   ; l.exp > r.exp
:"$lgtr"
   ; Calculate exponent difference.
   SubU_W 1, LocReg(Lit(6)), LocReg(Lit(4)), LocReg(Lit(5))

   ; If difference is >=53, r is too small to affect l.
   CmpI_GE_W 1, Stk(), LocReg(Lit(6)), Lit(53)
   Jcnd_Tru  1, Stk(), Lit(:"$remin")

   ; l.manfull + (r.manfull >> difference)
   Move_W 1, Stk(), LocReg(Lit(0))
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00100000)
   Move_W 1, Stk(), LocReg(Lit(2))
   AndU_W 1, Stk(), LocReg(Lit(3)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00100000)
   ShRI_W 2, Stk(), Stk(),          LocReg(Lit(6))
   AddU_W 2, LocReg(Lit(6)), Stk(), Stk()

   CmpI_LE_W 1, Stk(), LocReg(Lit(7)), Lit(0x001FFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lgtr_ret")

   ShRI_W 2, LocReg(Lit(6)), LocReg(Lit(6)), Lit(1)
   AddI_W 1, LocReg(Lit(4)), LocReg(Lit(4)), Lit(1)

   ; If exponent increased to 0x7FF, return infinity.
   CmpI_EQ_W 1, Stk(), LocReg(Lit(4)), Lit(0x7FF)
   Jcnd_Tru  1, Stk(), Lit(:"$infinity")

:"$lgtr_ret"
   Move_W 1, Stk(), LocReg(Lit(6))
   ShLU_W 1, Stk(), LocReg(Lit(4)), Lit(20)
   AndU_W 1, Stk(), LocReg(Lit(7)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   2, Stk()


   ; l.exp < r.exp
:"$lltr"
   ; Calculate exponent difference.
   SubU_W 1, LocReg(Lit(6)), LocReg(Lit(5)), LocReg(Lit(4))

   ; If difference is >=24, l is too small to affect r.
   CmpI_GE_W 1, Stk(), LocReg(Lit(6)), Lit(24)
   Jcnd_Tru  1, Stk(), Lit(:"$lemin")

   ; (l.manfull >> difference) + r.manfull
   Move_W 1, Stk(), LocReg(Lit(0))
   AndU_W 1, Stk(), LocReg(Lit(1)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00100000)
   ShRI_W 2, Stk(), Stk(),          LocReg(Lit(6))
   Move_W 1, Stk(), LocReg(Lit(2))
   AndU_W 1, Stk(), LocReg(Lit(3)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(),          Lit(0x00100000)
   AddU_W 2, LocReg(Lit(6)), Stk(), Stk()

   CmpI_LE_W 1, Stk(), LocReg(Lit(7)), Lit(0x001FFFFF)
   Jcnd_Tru  1, Stk(), Lit(:"$lltr_ret")

   ShRI_W 2, LocReg(Lit(6)), LocReg(Lit(6)), Lit(1)
   AddI_W 1, LocReg(Lit(5)), LocReg(Lit(5)), Lit(1)

   ; If exponent increased to 0x7FF, return infinity.
   CmpI_EQ_W 1, Stk(), LocReg(Lit(5)), Lit(0x7FF)
   Jcnd_Tru  1, Stk(), Lit(:"$infinity")

:"$lltr_ret"
   Move_W 1, Stk(), LocReg(Lit(6))
   ShLU_W 1, Stk(), LocReg(Lit(5)), Lit(20)
   AndU_W 1, Stk(), LocReg(Lit(7)), Lit(0x000FFFFF)
   OrIU_W 1, Stk(), Stk(), Stk()
   Retn   2, Stk()


   ; l has max exponent. It is either INF or NaN. Either way, return it.
:"$lemax"
   ; Is r NaN? If so, return r.
   AndU_W    1, Stk(), LocReg(Lit(3)), Lit(0x7FF00000)
   CmpU_EQ_W 1, Stk(), Stk(),          Lit(0x7FF00000)
   AndU_W    1, Stk(), LocReg(Lit(3)), Lit(0x000FFFFF)
   OrIU_W    1, Stk(), Stk(), LocReg(Lit(2))
   LAnd      1, Stk(), Stk(), Stk()
   Jcnd_Tru  1, Stk(), Lit(:"$lemin")

   ; r has min exponent. Therefore, r == 0 and the result is l.
:"$remin"
   Move_W 2, Stk(), LocReg(Lit(0))
   Retn   2, Stk()

   ; r has max exponent. It is either INF or NaN. Either way, return it.
:"$remax"
   ; l has min exponent. Therefore, l == 0 and the result is r.
:"$lemin"
   Move_W 2, Stk(), LocReg(Lit(2))
   Retn   2, Stk()

:"$infinity"
   ; Return infinity.
   Move_W 2, Stk(), Lit(0x7FF0000000000000_64.0)
   Retn   2, Stk()
}

;; EOF

