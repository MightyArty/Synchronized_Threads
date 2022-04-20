#include "doctest.h"
#include "server.hpp"
#include "iclient.hpp"
#include <pthread.h>

#include <iostream>

#define BUFFSIZE 1024
using namespace std;

int client()
{
	int portNo;
	bool loop = false;
	struct sockaddr_in svrAdd;
	struct hostent *server;

	portNo = htons(3002);

	if ((portNo > 65535) || (portNo < 2000))
	{
		cerr << "Please enter a port number between 2000 - 65535" << endl;
		return -1;
	}

	int sockTest = socket(AF_INET, SOCK_STREAM, 0);

	if (sockTest < 0)
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

	int checker = connect(sockTest, (struct sockaddr *)&svrAdd, sizeof(svrAdd));

	if (checker < 0)
	{
		cerr << "Cannot connect!" << endl;
		return -1;
	}
	return sockTest;
}
void *server(void *dummyPt)
{
	system("./server");
	return 0;
}

TEST_CASE("Good")
{
	pthread_t threadA[1];

	int error = pthread_create(&threadA[0], NULL, server, NULL);
	if (error != 0)
		printf("\nThread can't be created :[%s]",
			   strerror(error));
	sleep(5);
	int c1 = client();
	int c2 = client();
	int c3 = client();

	cout << c1 << endl;
	cout << c2 << endl;
	cout << c3 << endl;

	SUBCASE("PUSH CASE syn")
	{
		char r[BUFFSIZE] = {0};
		cout << "Inside" << endl;
		for (int i = 0; i < 2; i++)
		{
			write(c1, "PUSH c1", 7);
			read(c1, r, BUFFSIZE);
			CHECK(strcmp(r, "Pushed")==0);

			write(c2, "PUSH c2", 7);
			read(c2, r, BUFFSIZE);
			CHECK(strcmp(r, "Pushed")==0);

			write(c3, "PUSH c3", 7);
			read(c3, r, BUFFSIZE);
			CHECK(strcmp(r, "Pushed")==0);
		}

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK_EQ(r, "6");
	}
	// 	write(c1, "PUSH c1", 7);
	// 	read(c1, r, BUFFSIZE);
	// 	CHECK_EQ(r, "Pushed");

	// 	write(c1, "COUNT", 5);
	// 	read(c1, r, BUFFSIZE);
	// 	CHECK_EQ(r, "1");

	// 	write(c1, "POP", 3);
	// 	read(c1, r, BUFFSIZE);
	// 	CHECK_EQ(r, "c1");

	// 	write(c1, "COUNT", 5);
	// 	read(c1, r, BUFFSIZE);
	// 	CHECK_EQ(r, "0");
	// }
	// SUBCASE("exit check")
	// {
	// 	char r[BUFFSIZE] = {0};
	// 	write(c1, "exit", 4);
	// 	read(c1, r, BUFFSIZE);
	// 	CHECK_EQ(r, "succ");
	// 	write(c2, "exit", 4);
	// 	read(c2, r, BUFFSIZE);
	// 	CHECK_EQ(r, "succ");
	// 	write(c3, "exit", 4);
	// 	read(c3, r, BUFFSIZE);
	// 	CHECK_EQ(r, "succ");

	// 	close(c1);
	// 	close(c2);
	// 	close(c3);
	// }
	pthread_join(threadA[0], NULL);
}
