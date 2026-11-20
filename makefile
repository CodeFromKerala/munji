compile:
	gcc main.c -o munji
debug:
	gcc -g -O0 -fsanitize=address,undefined main.c -o munji
run_debug:
	./munji main.munji debug
run:
	./munji main.munji
clean:
	rm "\\"
