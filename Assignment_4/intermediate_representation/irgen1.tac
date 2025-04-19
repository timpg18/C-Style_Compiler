label main:
	func_begin main
		t#block1 = 1
		$0 = t#block1
		t#block1 = t#block1 + 1
		$1 = t#block1
		t#block1 = t#block1 + 1
		
		return t#block1
	func_end main
