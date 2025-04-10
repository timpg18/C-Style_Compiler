label main:
func_begin main
i = 0
label for_begin1:
$0 = i < 20
if $0 goto @L0
goto for_end1
label @L0:
$2 = i * 12
$3 = $2 + 0
arr[$3] = i
$4 = i * 12
$5 = $4 + 4
arr[$5] = i
$1 = i
i = i + 1
goto for_begin1
label for_end1:
$6 = 0 + 4
$7 = $6 + 4
$8 = $7 + 4
$9 = 0 + 0
$10 = 18 + a[$9]
$11 = 8 * 12
$12 = $11 + 0
$13 = $10 + arr[$12]
cast: int -> double 

s1[$8] = $13
$14 = 5 * 64
$15 = $14 + 4
$16 = $15 + 0
ar3[$16] = 9
$17 = 20 * 33
$18 = $17 + 1
$19 = $18 * 0
$20 = $19 + 9
$21 = $20 * 20
$22 = *$21
$23 = 1 * 0
$24 = $23 + 2
$25 = $24 * 20
$26 = $25 + 3
$27 = $26 * 1
$22 = $27
c1 = RED
c1 = c3
func_end main
