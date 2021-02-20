#Name: Donghang Wu Tristan Que
#ID: 605346965
#Email: dwu20@g.ucla.edu
default:
	gcc -Wall -Wextra -g -o lab3a lab3a.c 
dist:
	tar -czvf lab3a-605346965.tar.gz ext2_fs.h lab3a.c Makefile README 
clean:
	rm -f lab3a lab3a-605346965.tar.gz