source = main.c event.c mask_names.c timestamp.c

default:
	gcc $(source) -Wall -Werror -Wextra -Wpedantic -o hen
debug:
	gcc -g  $(source) -Wall -Wextra -Wpedantic -o hen
