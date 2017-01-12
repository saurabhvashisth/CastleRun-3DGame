all: sample2D

sample2D: Sample_GL3_2D.cpp glad.c
	g++ -o sample2D Sample_GL3_2D.cpp glad.c -lSOIL -lGL -lglfw -ldl -I/usr/local/include -I/usr/local/include/freetype2 -L/usr/local/lib 

clean:
	rm sample2D
