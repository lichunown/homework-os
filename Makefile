all:
	@make question1
	@make question2
question1:
	gcc question1.c -o question1
question2:
	gcc question2.c -o question2
clean:
	rm -f question1 question2 *.out
default:
	clean all