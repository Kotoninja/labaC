.DEFAULT_GOAL=run

file ?= data.txt

run:
	clang main.c list.c -o app
	./app -file=$(file)

list:
	clang list.c -o app
	./app