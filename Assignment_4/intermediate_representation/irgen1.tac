label main:
	func_begin main
		x#block1 = 2
		if x#block1 == 1 goto @L0
		if x#block1 == 2 goto @L1
		goto @L2
	label @L0:
		x#block1 = 2
		goto @L3
	label @L1:
		x#block1 = 3
		goto @L3
	label @L2:
		x#block1 = 7
		goto @L3
	label @L3:
		
		return x#block1
	func_end main
