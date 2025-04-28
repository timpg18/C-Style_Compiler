label main:
	func_begin main
		x#block1 = 2
		$0 = x#block1 > 0
		if $0 goto @L0
		goto @L1
	label @L0:
		$1 = x#block1
		x#block1 = x#block1 + 1
		goto @L2
	label @L1:
		$2 = x#block1
		x#block1 = x#block1 - 1
	label @L2:
		param "%d \n"
		param x#block1
		$3 = call printf, 2
		param "%d\n"
		param x#block1
		$4 = call printf, 2
		t#block3 = 0
		param "%d %d\n"
		param x#block1
		param t#block3
		$5 = call printf, 3
	func_end main
