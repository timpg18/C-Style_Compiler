label factorial:
	func_prologue  factorial
		$0 = n == 0
		if $0 goto @L1
		goto @L0
	label @L0:
		$1 = n == 1
		if $1 goto @L1
		goto @L2
	label @L1:
		
		return 1
		goto @L3
	label @L2:
		$2 = n - 1
		param $2
		$3 = call factorial, 1
		$4 = n * $3
		return $4
	label @L3:
	func_epilogue  factorial
label main:
	func_prologue  main
		number = 5
		param "Factorial of %d is %d\n"
		param number
		param number
		$5 = call factorial, 1
		param $5
		$6 = call printf, 3
		
		return 0
	func_epilogue  main
