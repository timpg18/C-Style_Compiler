label main:
	func_begin main
		$0 = 0 * 4
		arr#block1[$0] = 2
		$2 = 0 * 4
		x#block1 = arr#block1[$2]
		param "%d"
		param x#block1
		$3 = call printf, 2
	func_end main
