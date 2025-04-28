label main:
	func_begin main
<<<<<<< HEAD
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
=======
		$0 = 0 * 4
		arr#block1[$0] = 2
		$2 = 0 * 4
		x#block1 = arr#block1[$2]
		param "%d"
		param x#block1
		$3 = call printf, 2
>>>>>>> e0f8ed7c1e45a357bb6b13127ae841a14a86ffbb
	func_end main
