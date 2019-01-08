#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

enum direction { UP, DOWN, RIGHT, LEFT };

struct state {
  enum direction dir;
  float acceleration;
  float heartrate;
  struct timeval time;
};

#pragma pack()

#define PORT	 32987
#define MAXLINE 32

// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr, cliaddr;

  // Creating socket file descriptor
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  // Bind the socket with the server address
  if ( bind(sockfd, (const struct sockaddr *)&servaddr,
      sizeof(servaddr)) < 0 )
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  int n;
  socklen_t len;
  while (true) {
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
          MSG_WAITALL, ( struct sockaddr *) &cliaddr,
          &len);
    state *s = (state*) buffer;
    printf("Movement : %u\n", s->dir);
    printf("Heartrate : %f\n", s->heartrate);
  }

	return 0;
}
