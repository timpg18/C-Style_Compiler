label main:
	func_begin main
		$0 = 1 * 4
		x#block1[$0] = 2
		$2 = 1 * 4
		y#block1 = x#block1[$2]
	func_end main
