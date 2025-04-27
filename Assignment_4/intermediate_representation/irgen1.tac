label main:
	func_begin main
		$0 = cast: int -> char 50
		a#block1 = $0
		b#block1 = 0
		param "%c"
		param a#block1
		$1 = call printf, 2
		
		return 0
	func_end main
