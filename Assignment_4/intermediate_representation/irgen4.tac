label main:
	func_begin main
		a#block1 = 5
		$0 = a#block1 << 4
		b#block1 = $0
		$1 = a#block1 >> 1
		c#block1 = $1
		param "%d %d\n"
		param b#block1
		param c#block1
		$2 = call printf, 3
		
		return 0
	func_end main
