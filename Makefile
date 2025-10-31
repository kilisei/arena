build: 
	@clang -std=c23 -O2 -Wall -Werror -Wextra -Wformat ./src/main.c -o ./bin/arena

run: build 
	./bin/arena

valgrind: build 
	valgrind ./bin/arena

