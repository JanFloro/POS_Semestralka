all:
	g++ -c client.cpp
	g++ -c server.cpp
	g++ -c main.cpp
	g++ main.o client.o server.o -o POS_Semestralka -pthread
server: all
	./POS_Semestralka server 15555
client: all
	./POS_Semestralka client localhost 15555
clean:
	rm -f *.o