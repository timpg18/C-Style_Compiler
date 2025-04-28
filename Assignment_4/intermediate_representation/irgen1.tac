label main:
	func_begin main
		$0 = 0 * 4
		x#block3[$0] = 10
		$2 = 5 * 1
		ch#block3[$2] = 'k'
		$4 = 0 * 4
		y#block3 = x#block3[$4]
		$5 = 5 * 1
		g#block3 = ch#block3[$5]
		param "%d %c k"
		param y#block3
		param g#block3
		$6 = call printf, 3
		$7 = 0 + 0
		c#block3[$7] = 1
		$9 = 0 + 4
		c#block3[$9] = 'k'
		$11 = 0 + 0
		xx#block3 = c#block3[$11]
		$12 = 0 + 4
		ck#block3 = c#block3[$12]
		param "%d %c \n"
		param xx#block3
		param ck#block3
		$13 = call printf, 3
		$14 = 0 + 0
		obj1#block3[$14] = 5
		$16 = 0 + 0
		obj1#block3[$16] = 'k'
		$18 = 0 + 0
		xx#block3 = obj1#block3[$18]
		$20 = 0 + 0
		ck#block3 = obj1#block3[$20]
		param "%d %c \n"
		param xx#block3
		param ck#block3
		$22 = call printf, 3
		gg#block3 = 1
		xx#block3 = 1
		param "%d %c \n"
		param xx#block3
		param ck#block3
		$24 = call printf, 3
	func_end main
