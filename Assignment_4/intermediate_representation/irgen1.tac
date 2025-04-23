label main:
	func_begin main
		$0 = 3 * 4
		x#block1[$0] = 1
		$1 = 4 * 20
		$2 = $1 + 1
		$3 = $2 * 4
		y#block1[$3] = 10
		$4 = 4 * 20
		$4 = x#block1
		$5 = 10 * 20
		$6 = $5 + 20
		$7 = $6 * 30
		$7 = x#block1
		$8 = 5 * 10
		$8 = p#block1
	func_end main
