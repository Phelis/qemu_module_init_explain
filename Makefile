
make:
	clang main.c ./util/* ./hw/net/* `pkg-config --cflags glib-2.0`


clean:
	rm ./a.out