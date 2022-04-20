#include "doctest.h"
#include <iostream>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#include "doctest.h"
#include <string>
#include <algorithm>
#define BUFFSIZE 1024
using namespace std;

int client()
{
	int portNo;
	bool loop = false;
	struct sockaddr_in svrAdd;
	struct hostent *server;

	portNo = htons(3008);

	if ((portNo > 65535) || (portNo < 2000))
	{
		cerr << "Please enter a port number between 2000 - 65535" << endl;
		return -1;
	}

	int listenFd = socket(AF_INET, SOCK_STREAM, 0);

	if (listenFd < 0)
	{
		cerr << "Cannot open socket" << endl;
		return -1;
	}

	server = gethostbyname("localhost");

	if (server == NULL)
	{
		cerr << "Host does not exist" << endl;
		return -1;
	}

	bzero((char *)&svrAdd, sizeof(svrAdd));
	svrAdd.sin_family = AF_INET;

	bcopy((char *)server->h_addr, (char *)&svrAdd.sin_addr.s_addr, (size_t)server->h_length);

	svrAdd.sin_port = htons(portNo);

	int checker = connect(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd));

	if (checker < 0)
	{
		cerr << "Cannot connect!" << endl;
		return -1;
	}
	return listenFd;
}

char *run = "make run";
char *make_client = "make ./iserver";

TEST_CASE("Good")
{
	system(run);
	system(make_client);
	
	int c1 = client();
	int c2 = client();
	int c3 = client();
	char r[BUFFSIZE] = {0};
	char r1[BUFFSIZE] = {0};

	SUBCASE("PUSH CASE synchronized"){
		bzero(r, BUFFSIZE);
		for(int i = 0 ;i < 1000; i++){
			write(c1,"PUSH c1",7);
			read(c1, r, BUFFSIZE);
			CHECK(strcmp(r, "Pushed") == 0);

			write(c2,"PUSH c2",7);
			read(c2, r, BUFFSIZE);
			CHECK(strcmp(r, "Pushed") == 0);

			write(c3,"PUSH c3",7);
			read(c3, r, BUFFSIZE);
			CHECK(strcmp(r, "Pushed") == 0);
		}
	}

	SUBCASE("POP"){
		bzero(r, BUFFSIZE);
		write(c1, "PUSH c1", 7);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Pushed") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "1") == 0);

		write(c1, "POP", 3);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "c1") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "0") == 0);
	}

	SUBCASE("TOP"){
		bzero(r, BUFFSIZE);
		write(c1, "PUSH c1", 7);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Pushed") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "1") == 0);

		write(c1, "TOP", 3);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "c1") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "1") == 0);
	}

	SUBCASE("COUNT"){
		bzero(r, BUFFSIZE);
		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "0") == 0);

		write(c1, "PUSH c1", 7);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Pushed") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "1") == 0);

		write(c1, "POP", 3);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "c1") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "0") == 0);
	}
}