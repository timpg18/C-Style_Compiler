label main:
	func_prologue  main
		n = 5
		a = 0
		$0 = n * 4
		param $0
		$1 = call malloc, 1
		arr = $1
		if arr == 0 goto @L1
		goto @L0
	label @L0:
		param "Memory allocation succeed\n"
		$2 = call printf, 1
		
		return 1
	label @L1:
		param arr
		call free, 1
		param "Memory freed successfully.\n"
		$4 = call printf, 1
		
		return 0
	func_epilogue  main
