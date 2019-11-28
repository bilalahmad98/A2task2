#include <stdio.h> // basic I/O
#include <stdlib.h>
#include <sys/types.h> // standard system types
#include <netinet/in.h> // Internet address structures
#include <sys/socket.h> // socket API
#include <arpa/inet.h>
#include <netdb.h> // host to IP resolution
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <iostream>

using namespace std;

#define BUFFER_SIZE 256

// prototype
void parametercheck(int argc, char *argv[]);
int openSocket(void);
void socketBind(int socket_id, struct sockaddr_in srv_ad);
int acceptConnection(int s_socket_id, struct sockaddr_in cli_ad);
void rcvMsg(int socket_id, char buff[], int buff_size);
void sendMsg(int socket_id, char buff[], int buff_size);
void clearBuff(char buff[]);
void sendMsgHello(int socket_id);
void welcomeMsg(int port);
void acceptMsg(struct sockaddr_in cli_ad);
void ClientDisconnetMsg(void);
bool isExit(char buff[]);

int main(int argc, char *argv[])
{
	// variable definitions
	int s_sock_fd, c_sock_fd, port;
	socklen_t clilen;
	struct sockaddr_in srv_ad, cli_ad;
	char buff[BUFFER_SIZE];

	parametercheck(argc, argv);	// check parameters

	port = atoi(argv[1]);	// getting port no.

	s_sock_fd = openSocket();	// open socket

	// server details
	srv_ad.sin_family = AF_INET;
	srv_ad.sin_addr.s_addr = INADDR_ANY;
	srv_ad.sin_port = htons(port);

	socketBind(s_sock_fd, srv_ad);	// binding socket

	welcomeMsg(port);

	listen(s_sock_fd, 5);	// starts listening for clients

	while (true)
	{
		c_sock_fd = acceptConnection(s_sock_fd, cli_ad);	// accepting connection from client

		acceptMsg(cli_ad);

		sendMsgHello(c_sock_fd);
		
		do
		{
			clearBuff(buff);
			cout << "c : ";
			rcvMsg(c_sock_fd, buff, BUFFER_SIZE);
			cout << buff << endl;

		} while (isExit(buff));

		ClientDisconnetMsg();

		close(c_sock_fd);	
	}
	
	close(s_sock_fd);
	return 0;

}

void parametercheck(int argc, char* argv[])
{
	if (argc != 2)
    {
      fprintf(stderr, "Usage: %s <port>\n", argv[0]);
      exit(-1);
    }
}

int openSocket(void)
{
	int socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_id < 0)
	{
		cout << "Socket Opening failed\n";
		exit(-1);
	}
	return socket_id;
}

void socketBind(int socket_id, struct sockaddr_in srv_ad)
{
	int srv_size = sizeof(srv_ad);
	if (bind(socket_id, (struct sockaddr *) &srv_ad, srv_size) < 0)
	{
		cout << "Socket Binding Failed\n";
		exit(-1);
	}
}

int acceptConnection(int s_socket_id, struct sockaddr_in cli_ad)
{
	socklen_t clilen;
	clilen = sizeof(cli_ad);

	int c_socket_id = accept(s_socket_id, (struct sockaddr *) &cli_ad, &clilen);
	if (c_socket_id < 0)
	{
		cout << "Connection acceptance failed\n";
		exit(-1);
	}	
	return c_socket_id;
}

void rcvMsg(int socket_id, char buff[], int buff_size)
{
	int x = read(socket_id, buff, buff_size);
	if (x < 0)
	{
		cout << "Failure of reading from socket\n";
		exit(-1);
	}
}

void sendMsg(int socket_id, char buff[], int buff_size)
{
	int x = write(socket_id, buff, buff_size);
	if (x < 0)
	{
		cout << "Failure of writing to socket\n";
		exit(-1);
	}
}

void clearBuff(char buff[])
{
	bzero(buff, BUFFER_SIZE);
}

void sendMsgHello(int socket_id)
{
	int x = write(socket_id, "Hello from Asgard", 28);
	if (x < 0)
	{
		cout << "Failure of writing to socket\n";
		exit(-1);
	}	
}

void welcomeMsg(int port)
{
	cout << "\nStarting to run server at port " << port
		<< "\n.. creating local listener socket\n"
		<< ".. binding socket to port:"<< port
		<< "\n.. starting to listen at the port\n"
		<< ".. waiting for connection \n\n";
}

void acceptMsg(struct sockaddr_in cli_ad)
{
	cout << "\nClient has connected to server successfully...\n\n";
}

void ClientDisconnetMsg(void)
{
	cout << "\nPlease don't leave me...\n\n"
		<< "Listening to clients again... \n\n";
}

bool isExit(char buff[])
{
	for (int i = 0; buff[i]; i++)
	{
		buff[i] = tolower(buff[i]);
	}

	if (strncmp(buff, "exit", 4) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
