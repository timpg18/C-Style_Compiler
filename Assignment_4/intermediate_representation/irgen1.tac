label main:
	func_begin main
		b#block1 = 'b'
		$0 = cast: char -> float b#block1
		a#block1 = $0
		param "%f\n"
		param a#block1
		$1 = call printf, 2
		
		return 0
	func_end main
