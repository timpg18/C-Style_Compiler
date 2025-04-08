label main:
func_begin main
i = 0
j = 0
i = 0
label for_begin1:
$0 = i < 3
if $0 goto @L0
goto for_end1
label @L0:
param "For loop i = %d\n"
param i
$2 = call printf, 2
$1 = i
i = i + 1
goto for_begin1
label for_end1:
i = 0
label for_begin3:
$3 = i < 2
if $3 goto @L2
goto for_end3
label @L2:
j = 0
label for_begin2:
$5 = j < 2
if $5 goto @L1
goto for_end2
label @L1:
param "Nested For: i = %d, j = %d\n"
param i
param j
$7 = call printf, 3
$6 = j
j = j + 1
goto for_begin2
label for_end2:
$4 = i
i = i + 1
goto for_begin3
label for_end3:
i = 0
label while_begin4:
$8 = i < 3
if $8 goto @L3
goto while_end4
label @L3:
param "While loop i = %d\n"
param i
$9 = call printf, 2
$10 = i
i = i + 1
goto while_begin4
label while_end4:
i = 0
label for_begin6:
$11 = i < 2
if $11 goto @L5
goto for_end6
label @L5:
j = 0
label while_begin5:
$13 = j < 2
if $13 goto @L4
goto while_end5
label @L4:
param "Nested While: i = %d, j = %d\n"
param i
param j
$14 = call printf, 3
$15 = j
j = j + 1
goto while_begin5
label while_end5:
$12 = i
i = i + 1
goto for_begin6
label for_end6:
i = 0
label do_until_begin7:
param "Do-While loop i = %d\n"
param i
$16 = call printf, 2
$17 = i
i = i + 1
label @L6:
$18 = i < 3
if $18 goto do_until_begin7
goto do_until_end7
label do_until_end7:
i = 0
label while_begin9:
$19 = i < 2
if $19 goto @L8
goto while_end9
label @L8:
j = 0
label do_while_begin8:
param "Nested Do-While: i = %d, j = %d\n"
param i
param j
$20 = call printf, 3
$21 = j
j = j + 1
label @L7:
$22 = j < 2
if $22 goto do_while_begin8
goto do_while_end8
label do_while_end8:
$23 = i
i = i + 1
goto while_begin9
label while_end9:
i = 0
label for_begin10:
$24 = i < 5
if $24 goto @L12
goto for_end10
label @L12:
$26 = i == 2
if $26 goto @L9
goto @L11
label @L9:
goto for_begin10
$27 = i == 4
if $27 goto @L10
goto @L11
label @L10:
goto for_end10
label @L11:
param "For with break/continue: i = %d\n"
param i
$28 = call printf, 2
$25 = i
i = i + 1
goto for_begin10
label for_end10:

return 0
func_end main
