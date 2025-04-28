label func:
	func_begin func
		$0 = a#block1 + b#block1
		return $0
	func_end func
label main:
	func_begin main
		gg#block1 = 1
		param "%d "
		param gg#block1
		$0 = call printf, 2
	func_end main
