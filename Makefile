CC := g++


all: ipk-client ipk-server 

ipk-client: ipk-client.cpp
	$(CC) -static-libstdc++ -o ipk-client ipk-client.cpp

ipk-server: ipk-server.cpp
	$(CC) -static-libstdc++ -o ipk-server ipk-server.cpp

clean:
	rm -f ipk-client ipk-server
