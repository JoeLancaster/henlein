source = main.c event.c mask_names.c timestamp.c dynamic_read.c
flags = -std=gnu99 -Wall -Wextra -Wpedantic

default:
	gcc $(source) $(flags) -Werror -o hen
debug:
	gcc -g  $(source) $(flags) -o hen
