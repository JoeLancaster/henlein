source = main.c event.c mask_names.c timestamp.c dynamic_read.c dir_utils.c hen_action.c
flags = -std=gnu99 -Wall -Wextra -Wpedantic

default:
	gcc $(source) $(flags) -Werror -o hen
debug:
	gcc -g -O0 $(source) $(flags) -o hen
opt:
	gcc -O3 $(source) $(flags) -o hen
