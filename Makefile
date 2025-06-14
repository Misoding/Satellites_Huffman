build:
	gcc ./tema2.c -o tema2 -Wall -Wextra -g

clean:
	rm -f ./tema2*

run_c1:
	make && valgrind ./tema2 -c1 ./Exemplu/cerinta1.in ./output_c1.out
run_c2:
	make && valgrind ./tema2 -c2 ./Exemplu/cerinta2.in ./output_c2.out
run_c3:
	make && valgrind ./tema2 -c3 ./Exemplu/cerinta3.in ./output_c3.out
run_c4:
	make && valgrind ./tema2 -c4 ./Exemplu/cerinta4.in ./output_c4.out
run_c5:
	make && valgrind ./tema2 -c5 ./Exemplu/cerinta5.in ./output_c5.out

run:
	make && ./run_tests.sh