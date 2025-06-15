#include "PiSocket.h"

int main(){
	
	
	PiSocket server;
	server.socketInit("145.52.127.184", 9999); //192.168.68.104 // 145.52.127.184
	server.handleClient();
	
}
