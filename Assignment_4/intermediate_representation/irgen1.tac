label main:
	func_begin main
		$0 = 3 * 4
		x#block1[$0] = 1
		y#block1 = x#block1
		$1 = 5 * 10
		$1 = p#block1
	func_end main
