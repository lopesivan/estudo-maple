
libmysum.so: mysum.c
	gcc -fPIC -shared -o libmysum.so mysum.c

clean:
	rm libmysum.so

run: main.mpl libmysum.so
	maple -q  $<
