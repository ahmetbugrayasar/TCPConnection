#include<iostream>
#include <WS2tcpip.h>


#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main(){

	// Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		cerr << "Can't Initialize Winsock! Quitting." << endl;
		return;
	}
	cout << "Initializing Winsock" << endl;
	// Create a Socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting." << endl;
		return;
	}
	cout << "Creating a socket" << endl;

	// Bind the Socket to an ip address and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //Could also use inet.pton


	bind(listening, (sockaddr*) &hint, sizeof(hint));
	cout << "Binding the Winsock to an ip address: " << (sockaddr*)&hint << " and port: " << hint.sin_port << endl;
	// Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client,&clientSize);
	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "Invalid Socket! Quitting." << endl;
		return;
	}
	cout << "Valid Socket!" << endl;

	char host[NI_MAXHOST]; //Client remote name
	char service[NI_MAXSERV]; //Service the client is connected to

	ZeroMemory(host, NI_MAXHOST); //Same as memset(host, 0, NI_MAXHOST); 
	ZeroMemory(service, NI_MAXSERV);
	
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port" <<
			ntohs(client.sin_port) << endl;
	}
	cout << "Successfully connected!" << endl;
	//Close listening socket
	closesocket(listening);
	cout << "Closed listening socket" << endl;
	//While loop: accept and echo message back to client
	char buff[4096];

	while (true)
	{
		ZeroMemory(buff, 4096);

		//Wait for client to send data
		int bytesReceived = recv(clientSocket, buff, 4096, 0);

		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting!" << endl;
			break;
		}
		if (bytesReceived == 0)
		{
			cout << "Client disconnected" << endl;
			break;
		}

		//Echo message back to Client
		send(clientSocket, buff, bytesReceived + 1, 0);
		
		
	}

	// Close the Socket
	closesocket(clientSocket);
	cout << "Socket closed" << endl;

	// Shutdown winsock
	cout << "Winsock shutdown!" << endl;
	WSACleanup();
}