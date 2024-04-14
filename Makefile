out = main.exe
srcs = main.c stringfuncs.c structs.c
objs = $(srcs:.c=.o)

run:
	@gcc -c $(srcs)
	gcc $(objs) -o $(out)
	@rm -f *.o
	@echo ''
	@./$(out)
	@echo ''
