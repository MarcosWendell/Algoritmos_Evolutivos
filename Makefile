all:
	gcc evolutivos.c -o evolutivos.out -lGL -lGLU -lglut -lpthread
clean:
	rm evolutivos.out
