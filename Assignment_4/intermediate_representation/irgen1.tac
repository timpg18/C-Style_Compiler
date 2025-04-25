label main:
	func_begin main
		$0 = 4 * 4
		x#block1[$0] = 1
		$1 = 4 * 20
		$2 = $1 + 1
		$3 = $2 * 4
		y#block1[$3] = 10
		$4 = 5 * 10
		t#block1[$4] = p#block1
	func_end main
