game: game.c util.c util.h spatial_index.c spatial_index.h screen.c screen.h game_object.h ball_object.c ball_object.h paddle_object.c paddle_object.h brick_object.c brick_object.h game_consts.h
	gcc -g -Wall -Werror -std=c11 -pedantic -o game game.c util.c spatial_index.c screen.c ball_object.c paddle_object.c brick_object.c -lncurses -lm

test_screen: test_screen.c util.c util.h spatial_index.c spatial_index.h screen.c screen.h game_object.h ball_object.c ball_object.h paddle_object.c paddle_object.h game_consts.h
	gcc -g -Wall -std=c11 -pedantic -o test_screen test_screen.c util.c spatial_index.c screen.c ball_object.c paddle_object.c -lncurses -lm

test_vararray: test_vararray.c vararray.c vararray.h
	gcc -g -Wall -std=c11 -pedantic -o test_vararray test_vararray.c vararray.c

.PHONY: clean
clean:
	rm test_screen 
	re test_vararray
