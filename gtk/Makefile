1px-gtk: 1px-gtk.c
	gcc 1px-gtk.c -o 1px-gtk `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

run: 1px-gtk
	./1px-gtk

clean:
	rm -rf 1px-gtk

