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

TEST_CASE("Good")
{
	int c1 = client();
	int c2 = client();
	int c3 = client();
	char *r;

	for(;;){
		write(c1,"PUSH c1",7);
		bzero(r, BUFFSIZE);
        read(listenFd, r, BUFFSIZE);
		write(c1,"PUSH c1",7);
		write(c1,"PUSH c1",7);
	}


	

	CHECK()
}
// CHECK(nospaces(mat(9, 7, '@', '-')) == nospaces("@@@@@@@@@\n"
// 												 "@-------@\n"
// 												 "@-@@@@@-@\n"
// 												 "@-@---@-@\n"
// 												 "@-@@@@@-@\n"
// 												 "@-------@\n"
// 												 "@@@@@@@@@"));
// /* Add more test here */
}

TEST_CASE("Bad input")
{
	// CHECK_THROWS(mat(10, 5, '$', '%'));
	/* Add more test here */
}

/* Add more test cases here */

// TEST_CASE("Good inputs")
// {
//     SUBCASE("Operator check not throw")
//     {

//     }

//     SUBCASE("Operators correc")
//     {

//     }
//     // if (strcmp(w, "1") == 0)
//     //     for (int i = 0; i < 200000; i++)
//     //     {
//     //         write(listenFd, "PUSH Hello-1\0", 13);
//     //         read(listenFd, r, BUFFSIZE);
//     //         puts(r);
//     //     }
//     // else if(strcmp(w, "2") == 0)
//     // {
//     //     for (int i = 0; i < 200000; i++)
//     //     {
//     //         write(listenFd, "PUSH Hello-2\0", 13);
//     //         read(listenFd, r, BUFFSIZE);
//     //         puts(r);
//     //     }
//     // }else{
//     //     for (int i = 0; i < 200000; i++)
//     //     {
//     //         write(listenFd, "PUSH Hello-3\0", 13);
//     //         read(listenFd, r, BUFFSIZE);
//     //         puts(r);
//     //     }
//     // }
// }

// TEST_CASE("Bad inputs")
// {
//     SUBCASE("invalid Matrix size")
//     {

//     }
// }
int main()
{

	return 1;
}