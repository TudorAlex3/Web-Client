#include "helpers.h"

int main(int argc, char *argv[])
{
    char *cookie = NULL;
    char *jwt = NULL;
    char username[INPUT];
    char password[INPUT];

    // Variabila pentru a verifica daca s-a folosit cu succes comanda "login"
    int login = 0;

    while (1) {
        int socket = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
        
        char command[INPUT];
        fgets(command, INPUT, stdin);
        command[strlen(command)-1]='\0';
        
        // Variabila pentru verificarea introducerii unei comenzi valide
        int right = 0;

        // Comanda "register"
        if (!strcmp(command, "register")) {
            // Se trimite cererea si se primeste raspunsul
            char *response;
            response = login_register(socket, username, password, command);

            // Se afiseaza raspunsul de la server si mesaje de eroare/succes
            int error = print_message(response);
            if (error)
                printf("<< You have successfully registered! >>\n\n");
            else 
                printf("<< Try again! >>\n\n");
                
            right = 1;
        }

        // Cazul in care s-a introdus comanda "login" cand era deja logat
        if (!strcmp(command, "login") && login) {
            printf("\n<< Already logged in! >>\n\n");
            continue;
        }

        // Comanda "login"
        if (!strcmp(command, "login") && !login) {
            // Se trimite cererea si se primeste raspunsul
            char *response;
            response = login_register(socket, username, password, command);

            // Se afiseaza raspunsul de la server si mesaje de eroare/succes
            int error = print_message(response);
            if (error) {
                printf("<< You have successfully loged in! >>\n");
                printf("<< Username: %s  >>\n\n", username);
                login = 1;
                cookie = find_cookie(response);
            }
            else 
                printf("<< Try again! >>\n\n");

            right = 1;
        }

        // Comanda "enter_library"
        if (!strcmp(command, "enter_library")) {
            if (cookie != NULL) {
                // Se trimite cererea si se primeste raspunsul
                char *response;
                response = enter_get_out(socket, cookie, jwt, command);

                // Se afiseaza raspunsul de la server si mesaje de eroare/succes
                int error = print_message(response);
                if (error) {
                    printf("<< You have successfully accessed the library!");
                    printf(" >>\n\n");
                    jwt = find_jwt(response);
                }
            } else 
                printf("<< There is no user logged in! >>\n\n");

            right = 1;
        }

        // Comanda "get_books"
        if (!strcmp(command, "get_books")) {
            if (jwt != NULL) {
                // Se trimite cererea si se primeste raspunsul
                char *response;
                response = enter_get_out(socket, cookie, jwt, command);

                // Se afiseaza raspunsul de la server si mesaje de eroare/succes
                int error = print_message(response);
                if (error) {
                    printf("<< There are your books! >>\n\n");
                    print_books(response);
                }
            } else 
                printf("<< This user is not authorized! >>\n\n");

            right = 1;
        }

        // Comanda "get_book"
        if (!strcmp(command, "get_book")) {
            if (jwt != NULL) {
                // Se trimite cererea si se primeste raspunsul
                char *response;
                response = get_delete_book(socket, jwt, 1);

                // Se afiseaza raspunsul de la server si mesaje de eroare/succes
                int error = print_message(response);
                if (error) {
                    printf("<< Book details: >>\n\n");
                    print_book(response);
                }
            } else 
                printf("<< This user is not authorized! >>\n\n");

            right = 1;
        }

        // Comanda "add_book"
        if (!strcmp(command, "add_book")) {
            if (jwt != NULL) {
                // Se trimite cererea si se primeste raspunsul
                char *response;
                response = add_book(socket, jwt);

                // Se afiseaza raspunsul de la server si mesaje de eroare/succes
                int error = print_message(response);
                if (error)
                    printf("<< New book has been added! >>\n\n");
            } else 
                printf("<< This user is not authorized! >>\n\n");

            right = 1;
        }

        // Comanda "delete_book"
        if (!strcmp(command, "delete_book")) {
            if (jwt != NULL) {
                // Se trimite cererea si se primeste raspunsul
                char *response;
                response = get_delete_book(socket, jwt, 0);

                // Se afiseaza raspunsul de la server si mesaje de eroare/succes
                int error = print_message(response);
                if (error) {
                    printf("<< The book has been successfully deleted! >>");
                    printf("\n\n");
                }
            } else 
                printf("<< This user is not authorized! >>\n\n");
            right = 1;
        }

        // Comanda "logout"
        if (!strcmp(command, "logout")) {
            if (cookie != NULL) {
                // Se trimite cererea si se primeste raspunsul
                char *response;
                response = enter_get_out(socket, cookie, jwt, command);

                // Se afiseaza raspunsul de la server si mesaje de eroare/succes
                int error = print_message(response);
                if (error) {
                    printf("<< Logout successfully! >>\n\n");
                    login = 0;
                    cookie = NULL;
                    jwt = NULL;
                }
            }
            else
                printf("<< No user logged in! >>\n\n");
            right = 1;
        }

        // Comanda "exit"
        if (!strcmp(command, "exit")) {
            close_connection(socket);
            break;
        }

        // Daca nu a fost introdusa o comanda corecta
        if (!right && strcmp(command, "\0"))
            printf("\n<< Invalid command! >>\n\n");

        close_connection(socket);
    }

    return 0;
}
