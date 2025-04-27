label main:
	func_begin main
		$1 = 0 + 5
		$2 = $1 + 4
		$3 = cast: int -> float 1
		obj1#block6[$2] = $3
	func_end main
