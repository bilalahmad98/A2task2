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

#define nameOhostLEN 40 // maximal host name length; can make it variable if you want
#define BUFLEN 1024 // maximum response size; can make it variable if you want
#define BUFFER_SIZE 256
// prototype
void parametercheck(int argc, char *argv[]);
int openSocket(void);
void connectwithserver(int socket_id, struct sockaddr_in srv_ad);
struct hostent* getHostName(char* nameOhost);
void RcvMsg(int socket_id, char buff[], int buff_size);
void sendMsg(int socket_id, char buff[], int buff_size);
void clearbuff(char buff[]);
bool isExit(char buff[]);

int main(int argc, char *argv[])
{
  	// variable definitions
	int c_sock_fd, port;
	struct sockaddr_in srv_ad;
	struct hostent *serv;
	char buff[BUFFER_SIZE];

  	// check that there are enough parameters
 	parametercheck(argc, argv);

    port = atoi(argv[2]);	// getting port no.

	c_sock_fd = openSocket();	// open socket
	
	char *nameOhost = argv[1];	
	serv = getHostName(nameOhost);	// get hostname

	// server details
	srv_ad.sin_family = AF_INET;
	srv_ad.sin_port = htons(port);
	int serv_l = serv->h_length;
	bcopy((char *)serv->h_addr, (char *) &srv_ad.sin_addr.s_addr, serv_l);
	
	connectwithserver(c_sock_fd, srv_ad);	// Connect to server

	clearbuff(buff);
	
	RcvMsg(c_sock_fd, buff, BUFFER_SIZE);

	cout << endl << buff << endl << endl;

	do
	{
		clearbuff(buff);
		cout << "\nc : ";
		fgets(buff, BUFFER_SIZE, stdin);
		sendMsg(c_sock_fd, buff, strlen(buff));

	} while (isExit(buff));

	close(c_sock_fd);

  return 0;
}

void parametercheck(int argc, char* argv[])
{
	if (argc != 3)
    {
      fprintf(stderr, "Usage: %s <nameOhost> <port>\n", argv[0]);
      exit(-1);
    }
}

int openSocket(void)
{
	int socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_id < 0)
	{
		cout << "Socket Opening Failed\n";
		exit(-1);
	}
	return socket_id;
}

void connectwithserver(int socket_id, struct sockaddr_in srv_ad)
{
	int srv_size = sizeof(srv_ad);
	if (connect(socket_id, (struct sockaddr *) &srv_ad, srv_size) < 0)
	{
		cout << "Connection Failed\n";
		exit(-1);
	}
}

struct hostent* getHostName(char* nameOhost)
{
	struct hostent *serv;
	serv = getHostName(nameOhost);
	if (serv == NULL)
	{
		cout << "Error: Host Unavailable\n";
		exit(-1);
	}
	return serv;
}

void RcvMsg(int socket_id, char buff[], int buff_size)
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

void clearbuff(char buff[])
{
	bzero(buff, BUFFER_SIZE);	
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