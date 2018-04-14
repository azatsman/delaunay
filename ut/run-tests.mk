#### Tests are collected in this makefile instead of a shell script because
#### it is easier to run things in parallel using '-j' flag.

all:  tst01 tst02 tst03 tst04 tst05 tst06 tst07 tst08 tst09 tst10 tst11

tst01:
	run1test.sh ut-random 5000  ################ 57 seconds

tst02:
	run1test.sh ut-random-grid 3000  2048 1536    ## 1:43

tst03:
	run1test.sh ut-grid  71 81  ### 3:23

tst04:
	run1test.sh ut-random 2500  ################ 8 seconds

tst05:
	run1test.sh ut-random-grid 2000 57 47     ############## 15 seconds

tst06:
	run1test.sh ut-greg1

tst07:
	run1test.sh ut-greg2

tst08:
	run1test.sh ut-random 3531

tst09:
	run1test.sh ut-random-grid 3000  1677 1253

tst10:
	run1test.sh ut-grid  71 45 

tst11:
	run1test.sh ut-grid  37 45 
