linpop.out: login_gui.o register_gui.o
	gcc -o linpop.out login_gui.o register_gui.o `pkg-config --cflags --libs gtk+-3.0`

login_gui.o: login_gui.c
	gcc -c -o login_gui.o login_gui.c `pkg-config --cflags --libs gtk+-3.0`

register_gui.o: register_gui.c
	gcc -c -o register_gui.o register_gui.c `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm *.out *.o