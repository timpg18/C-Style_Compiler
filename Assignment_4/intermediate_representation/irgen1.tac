label main:
	func_begin main
		a#block1 = 9.0
		param "this is a float = %f"
		param 9.0
		$0 = call printf, 2
		
		return 0
	func_end main
