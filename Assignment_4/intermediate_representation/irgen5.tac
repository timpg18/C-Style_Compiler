label main:
	func_begin main
		$0 = 0 + 4
		obj#block3[$0] = 1
		$2 = 0 + 4
		ww#block3 = obj#block3[$2]
		param "%d \n"
		param ww#block3
		$3 = call printf, 2
		$4 = 0 + 8
		obj1#block3[$4] = 'k'
		$6 = 0 + 8
		$7 = obj1#block3[$6]
		param "%c \n"
		param $7
		$8 = call printf, 2
		x#block3 = 0
	label @L0:
		$9 = x#block3
		x#block3 = x#block3 + 1
		goto @L2
	label @L1:
		$10 = x#block3 > 10
		if $10 goto @L2
		goto @L0
	label @L2:
		param "%d\n"
		param x#block3
		$11 = call printf, 2
	func_end main
