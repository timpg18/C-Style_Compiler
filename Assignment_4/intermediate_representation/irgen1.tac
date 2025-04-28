label main:
	func_begin main
		b#block1 = 'b'
		$0 = cast: char -> float b#block1
		a#block1 = $0
		param "%f\n"
		param a#block1
		$1 = call printf, 2
		
<<<<<<< HEAD
		return 0
=======
		return x#block1
=======
		$0 = 0 * 4
		arr#block1[$0] = 2
		$2 = 0 * 4
		x#block1 = arr#block1[$2]
		$3 = 8 * 1
		ar#block1[$3] = '3'
		$5 = 8 * 1
		c#block1 = ar#block1[$5]
		$6 = 8 * 1
		$5 = ar#blk1[$6]
		param "%d %c 5\n"
		param x#block1
		$3 = call printf, 2
>>>>>>> e0f8ed7c1e45a357bb6b13127ae841a14a86ffbb
>>>>>>> 6c709e2d185850fd73d69dd22dfb504d229871c5
	func_end main
