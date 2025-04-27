label main:
	func_begin main
		$0 = cast: int -> float 1
		a#block1 = $0
		param "%f"
		param a#block1
		$1 = call printf, 2
		
		return 0
	func_end main
