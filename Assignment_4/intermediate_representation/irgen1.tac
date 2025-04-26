label main:
	func_begin main
		a#block1 = 1
		$0 = cast: float -> int 2.0
		a#block1 = $0
		$1 = cast: float -> int 9.7
		return $1
	func_end main
