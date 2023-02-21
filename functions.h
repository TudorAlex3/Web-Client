#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H 1

void print_book(char *response);
void print_books(char *response);
int print_message(char *response);
char *find_jwt(char *response);
char *find_cookie(char *response);
char *json_add_book(char *title, char *author, char *publisher,
                    char *genre, char *pages);
char *json_register(char *username, char *password);
int number_verify(char *string);
char *add_book(int socket, char *jwt);
char *get_delete_book(int socket, char *jwt, int just_get);
char *enter_get_out(int socket, char *cookie, char *jwt, char *cmd);
char *login_register(int socket, char *username, char *password, char *cmd);

#endif