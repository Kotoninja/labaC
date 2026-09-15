.DEFAULT_GOAL=run

file ?= data.txt

run:
	clang main.c stack.c -o app
	./app -file=$(file)

list:
	clang list.c -o app
	./app

bin: 
	clang bin.c list.c -o app
	./app

stack:
	clang stack.c -o app
	./app