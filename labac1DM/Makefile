.DEFAULT_GOAL=run

file ?= data.txt

# Цвета для вывода (ANSI escape-последовательности)
COLOR_RESET   = \033[0m
COLOR_COMMENT = \033[33m
COLOR_TARGET  = \033[36m

.DEFAULT_GOAL := help

.PHONY: help
help: ## Показать вывод всех команд
	@awk ' \
		BEGIN {FS = ":.*##"; printf ""} \
		/^[a-zA-Z_-]+:.*?##/ { printf "    $(COLOR_TARGET)%-15s$(COLOR_RESET) %s\n", $$1, $$2 } \
		/^##@/ { printf "\n  $(COLOR_COMMENT)%s$(COLOR_RESET)\n", substr($$0, 5) } \
	' $(MAKEFILE_LIST)
	@echo ""



run: ## Запускает все приложение
	clang main.c stack.c bin.c hashmap.c list.c -o app
	./app -file=$(file)

list: ## Запускает пакет list
	clang list.c -o app
	./app

bin: ## Запускает пакет bin
	clang bin.c stack.c -o app
	./app

stack: ## Запускает пакет stack
	clang stack.c -o app
	./app