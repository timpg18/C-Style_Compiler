label main:
	func_begin main
		x#block1 = 0
		$0 = x#block1
		x#block1 = x#block1 + 1
		$1 = x#block2
		x#block2 = x#block2 + 1
		$2 = x#block3
		x#block3 = x#block3 + 1
		$3 = x#block4
		x#block4 = x#block4 + 1
		$4 = y#block3
		y#block3 = y#block3 + 1
		$5 = x#block4
		x#block4 = x#block4 + 1
		$6 = x#block5
		x#block5 = x#block5 + 1
	func_end main
