linpop.out: data.o login_gui.o register_gui.o network.o friend_linked_list.o friend_interface.o chat_window.o
	gcc -o linpop.out data.o login_gui.o register_gui.o network.o friend_linked_list.o friend_interface.o chat_window.o \
	`pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0` -lgthread-2.0

data.o: data.c
	gcc -c -o data.o data.c `pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0` -lgthread-2.0

login_gui.o: login_gui.c
	gcc -c -o login_gui.o login_gui.c `pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0` -lgthread-2.0

register_gui.o: register_gui.c
	gcc -c -o register_gui.o register_gui.c `pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0` -lgthread-2.0

network.o: network.c
	gcc -c -o network.o network.c `pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0` -lgthread-2.0

friend_linked_list.o: friend_linked_list.c
	gcc -c -o friend_linked_list.o friend_linked_list.c `pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0` -lgthread-2.0

friend_interface.o: friend_interface.c
	gcc -c -o friend_interface.o friend_interface.c `pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0` -lgthread-2.0

chat_window.o: chat_window.c
	gcc -c -o chat_window.o chat_window.c `pkg-config --cflags --libs gtk+-3.0 libmongoc-1.0`-lgthread-2.0

clean:
	rm *.out *.o