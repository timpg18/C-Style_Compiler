label main:
	func_begin main
		$0 = 1 * 4
		ar#block1[$0] = 11
		$2 = 1 * 4
		$3 = ar#block1[$2]
		param "%d"
		param $3
		$4 = call printf, 2
	func_end main
