#include "doctest.h"
#include "server.hpp"
#include "iclient.hpp"
#include <pthread.h>

#include <iostream>
using namespace std;

#define BUFFSIZE 1024
char r[BUFFSIZE] = {0};
int c1;
int c2;
int c3;
void red()
{
	printf("\033[1;31m");
}
void yellow()
{
	printf("\033[1;33m");
}
void blue()
{
	printf("\033[0;34m");
}
void green()
{
	printf("\033[0;32m");
}
void purpule()
{
	printf("\033[0;35m");
}
void reset()
{
	printf("\033[0m");
}
void cyan()
{
	printf("\033[0;36m");
}
int client()
{
	int portNo;
	bool loop = false;
	struct sockaddr_in svrAdd;
	struct hostent *server;

	portNo = htons(3000);

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
/**
 * @brief Test for pushing into the stack
 */
TEST_CASE("Good Input")
{
	pthread_t threadA[1];
	int error = pthread_create(&threadA[0], NULL, server, NULL);
	if (error != 0)
		printf("\nThread can't be created :[%s]",
			   strerror(error));
	sleep(5);
	c1 = client();
	sleep(1);
	c2 = client();
	sleep(1);
	c3 = client();
	sleep(1);
	SUBCASE("PUSH")
	{
		green();
		cout << "Inside PUSH case" << endl;
		reset();
		bzero(r, BUFFSIZE);
		write(c1, "PUSH c1", 7);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Pushed") == 0);

		write(c2, "PUSH c2", 7);
		read(c2, r, BUFFSIZE);
		CHECK(strcmp(r, "Pushed") == 0);

		write(c3, "PUSH c3", 7);
		read(c3, r, BUFFSIZE);
		CHECK(strcmp(r, "Pushed") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "3") == 0);
	}
}
/**
 * @brief Test for poping out of the stack
 */
TEST_CASE("Good Input")
{

	SUBCASE("POP")
	{
		yellow();
		cout << "Inside POP case" << endl;
		reset();
		bzero(r, BUFFSIZE);
		write(c1, "PUSH c1", 7);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Pushed") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "4") == 0);

		write(c1, "POP", 3);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "c1") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "3") == 0);
	}
}
/**
 * @brief test for showing the top of the stack
 */
TEST_CASE("Good Input")
{
	SUBCASE("TOP")
	{
		blue();
		cout << "Inside TOP case" << endl;
		reset();
		bzero(r, BUFFSIZE);
		write(c1, "PUSH c1", 7);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Pushed") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "4") == 0);

		write(c1, "TOP", 3);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "c1") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "4") == 0);
	}
}
/**
 * @brief Testing the COUNT function
 * Should return the current number of elements in the stack
 * First check if there is 4 elements, the push one and check if changed to 5
 * Then pop one element from the stack  and check again if the counter turned back to 4
 */
TEST_CASE("Good input")
{
	SUBCASE("COUNT")
	{
		purpule();
		cout << "Inside COUNT case" << endl;
		reset();
		bzero(r, BUFFSIZE);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "4") == 0);

		write(c1, "PUSH c1", 7);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Pushed") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "5") == 0);

		write(c1, "POP", 3);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "c1") == 0);

		write(c1, "COUNT", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "4") == 0);
	}
}
/**
 * @brief
 */
TEST_CASE("Bad input")
{
	SUBCASE("Invaild Argument")
	{

		cout << "\033[0;36mInside \033[1;33mInvaild Argument \033[0;36mcase" << endl;

		cyan();
		bzero(r, BUFFSIZE);

		write(c1, "pop", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Invaild commands") == 0);

		write(c1, "push", 4);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Invaild commands") == 0);

		write(c1, "top", 3);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Invaild commands") == 0);

		write(c1, "fsf", 3);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Invaild commands") == 0);

		write(c1, "barak", 5);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "Invaild commands") == 0);
	}
}
/**
 * @brief test for the "exit" command
 * should close the connection both on serverl and client sides
 */
TEST_CASE("Good input")
{
	SUBCASE("EXIT")
	{
		red();
		cout << "Inside EXIT case" << endl;
		reset();

		bzero(r, BUFFSIZE);
		write(c1, "exit", 4);
		read(c1, r, BUFFSIZE);
		CHECK(strcmp(r, "succ") == 0);

		write(c2, "exit", 4);
		read(c2, r, BUFFSIZE);
		CHECK(strcmp(r, "succ") == 0);

		write(c3, "exit", 4);
		read(c3, r, BUFFSIZE);
		CHECK(strcmp(r, "succ") == 0);

		close(c1);
		close(c2);
		close(c3);
		pthread_join(threadA[0], NULL);
	}
}