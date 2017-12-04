default:
	@make clean 
	@make all
all:
	@make question1
	@make question2
	@make question3
question1:
	gcc question1.c -o question1 -Wall
question2:
	gcc question2.c -o question2 -Wall
question3:
	gcc question3.c -o question3 -Wall  -lpthread 
clean:
	rm -f question1 question2 question3 *.out
