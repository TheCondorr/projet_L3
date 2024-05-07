all : ./seuil.exe

seuil.exe : seuil.cpp carte.o
	g++ carte.o seuil.cpp -o seuil.exe

carte.o : carte.cpp
	g++ -c carte.cpp

clean:
	rm -rf *o main.exe