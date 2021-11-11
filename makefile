sudoku:	main.o grille.o sudoku.o common.o
	gcc -o $@ $^ -mpopcnt -Wall -g -lm
	rm -f *.o
	valgrind ./sudoku


sudoku.o: sudoku.c sudoku.h common.h
	gcc -c $< -Wall	-g


grille.o: grille.c grille.h common.h
	gcc -c $< -Wall -g

common.o: common.c common.h
	gcc -c $< -Wall -g


clean:
	rm -f *.o
