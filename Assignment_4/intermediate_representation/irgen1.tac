label main:
	func_begin main
		$0 = 0 * 4
		a#block1[$0] = 9
		$2 = 2 * 4
		a#block1[$2] = 10
		$4 = 1 * 4
		$5 = 2 * 4
		$7 = a#block1[$4]
		$8 = a#block1[$5]
		$6 = $7 + $8
		f#block1 = $6
		$9 = cast: float -> double 9.0
		c#block1 = $9
		$10 = cast: float -> double 9.0
		d#block1 = $10
		$11 = c#block1 + d#block1
		b#block1 = $11
		param "%f"
		param b#block1
		$12 = call printf, 2
		$13 = cast: double -> int b#block1
		return $13
	func_end main
