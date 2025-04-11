label main:
	func_begin main
		$0 = ee@block1 + 0
		$1 = ee[$0]
		ee[$0] = ee[$0] + 1
	func_end main
