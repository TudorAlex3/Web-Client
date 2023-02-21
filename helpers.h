#ifndef _HELPERS_
#define _HELPERS_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>

#include "buffer.h"
#include "parson.h"
#include "requests.h"
#include "functions.h"

#define BUFLEN 4096
#define LINELEN 1000
#define INPUT 50

#define HOST "34.241.4.235"
#define PORT 8080
#define REGISTER_PAYLOAD "json"
#define REGISTER_ROUTE "/api/v1/tema/auth/register"
#define LOGIN_ROUTE "/api/v1/tema/auth/login"
#define ACCESS_ROUTE "/api/v1/tema/library/access"
#define BOOKS_ROUTE "/api/v1/tema/library/books"
#define LOGOUT_ROUTE "/api/v1/tema/auth/logout"

// shows the current error
void error(const char *msg);

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);

#endif
