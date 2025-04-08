glb1 = 10
label bar:
func_begin bar
lcl = 10
label while_begin1:
if b == 0 goto while_end1
goto @L0
label @L0:
$0 = b
b = b - 1
goto while_begin1
label while_end1:
$1 = a == 0
if $1 goto @L1
goto @L2
label @L1:

return 0
label @L2:
$2 = a - 1
param $2
$3 = a + lcl
param $3
$4 = call bar, 2
func_end bar
label main:
func_begin main
a = 100
i = 0
label for_begin2:
$5 = i < a
if $5 goto @L11
goto for_end2
label @L11:
b = 0
b = b + i
$7 = b % 2
$8 = b % 3
$9 = $7 != $8
if $9 goto @L8
goto @L9
label @L8:
param "%d\n"
$10 = a + b
param $10
$11 = call printf, 2
goto @L10
label @L9:
$12 = b % 2
$13 = $12 == 0
if $13 goto @L7
goto @L10
label @L7:
if b == 100 goto @L3
if b == 1000 goto @L4
goto @L5
label @L3:
param "1\n"
$15 = call printf, 1
goto @L6
label @L4:
param "2\n"
$16 = call printf, 1
label @L5:
param "2\n"
$17 = call printf, 1
label @L6:
label @L10:
ret = i
$6 = i
i = i + 1
goto for_begin2
label for_end2:
$18 = b1 + ret
c = $18
label jump:
$19 = a
a = a + 1
param "Final = %d"
$20 = 3 * 12
$21 = $20 + 4
$22 = newS[$21] + ret
param $22
$23 = call printf, 2
$24 = b1 + 2
$25 = *$24
$26 = $25 == 0
if $26 goto @L12
goto @L13
label @L12:
goto jump
label @L13:
param 1
$27 = *b1
param $27
$28 = call bar, 2
func_end main
