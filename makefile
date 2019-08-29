linpop.out: data.o login_gui.o register_gui.o
	gcc -o linpop.out data.o login_gui.o register_gui.o `pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0`

data.o: data.c
	gcc -c -o data.o data.c `pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0`

login_gui.o: login_gui.c
	gcc -c -o login_gui.o login_gui.c `pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0`

register_gui.o: register_gui.c
	gcc -c -o register_gui.o register_gui.c `pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0`

clean:
	rm *.out *.o