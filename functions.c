#include "helpers.h"

// Extrage jwt-ul din raspunsul primit de la server
char *find_jwt(char *response) {
    char *jwt = calloc(LINELEN, sizeof(char));

    // Cauta "token" in mesaj
    char *found = strstr(response, "token");

    // Merge in mesaj pana la terminarea token-ului
    if (found) {
        int i = 8;
        int j = 0;
        while (found[i] != '"') {
            jwt[j] = found[i];
            j++;
            i++;
        }
        jwt[j] = '\0';
    }

    return jwt;
}

// Extrage cookie-ul din raspunsul primit de la server
char *find_cookie(char *response) {
    char *cookie = calloc(LINELEN, sizeof(char));

    // Cauta "Set-Cookie" in mesaj
    char *found = strstr(response, "Set-Cookie: ");

    // Merge in mesaj pana la terminarea cookie-ului
    if (found) {
        int i = 12;
        int j = 0;
        while (found[i] != ';') {
            cookie[j] = found[i];
            j++;
            i++;
        }
        cookie[j] = '\0';
    }

    return cookie;
}

// Crearea unui json cu datele cartilor
char *json_add_book(char *title, char *author, char *publisher,
                    char *genre, char *pages) {
    JSON_Value* value;
    value = json_value_init_object();

    JSON_Object* object;
    object = json_value_get_object(value);

    json_object_set_string(object, "title", title);
    json_object_set_string(object, "author", author);
    json_object_set_string(object, "genre", genre);
    json_object_set_string(object, "publisher", publisher);
    json_object_set_string(object, "page_count", pages);

    return json_serialize_to_string_pretty(value);
}

// Crearea unui json cu credentialele userului
char *json_register(char *username, char *password) {
    JSON_Value* value;
    value = json_value_init_object();

    JSON_Object* object;
    object = json_value_get_object(value);

    json_object_set_string(object, "username", username);
    json_object_set_string(object, "password", password);
    
    return json_serialize_to_string_pretty(value);
}

// Afiseaza un subsir dintr-un sir de caractere
void print_detail(char *string, int start, char stop) {
    while (string[start] != stop) {
        printf("%c", string[start]);
        start++;
    }
    printf("\n");
}

// Afiseaza detaliile unei carti
void print_book(char *response) {
    char *title = strstr(response, "title");
    char *author = strstr(response, "author");
    char *publisher = strstr(response, "publisher");
    char *genre = strstr(response, "genre");
    char *page_count = strstr(response, "page_count");

    printf("-> Title:      ");
    print_detail(title, 8, '"');

    printf("   Author:     ");
    print_detail(author, 9, '"');

    printf("   Publisher:  ");
    print_detail(publisher, 12, '"');

    printf("   Genre:      ");
    print_detail(genre, 8, '"');

    printf("   Pages:      ");
    print_detail(page_count, 12, '}');

    printf("\n");
}

// Afiseaza toate cartile din biblioteca
void print_books(char *response) {
    int i = 0;
    while (response[i] != '[')
        i++;

    i++;
    while(response[i] != ']') {
        printf("%c", response[i]);

        if (response[i-1] == '}' && response[i] == ',')
            printf("\n");

        i++;
    }
    printf("\n\n");
}

// Afiseaza codul, mesajul si eroarea primite de la server
int print_message(char *response) {
    
    // Codul format din 3 cifre
    printf("\n");
    printf("-> Code:    ");
    printf("%c", response[9]);
    printf("%c", response[10]);
    printf("%c\n", response[11]);

    // Mesajul de la server, pana la intalnirea new-line-ului
    printf("   Message: ");
    int i = 13;
    while (response[i] != '\n') {
        printf("%c", response[i]);
        i++;
    }
    printf("\n");

    // Daca se gaseste cuvantul "error" in mesaj afisez eroarea
    char *details;
    details = strstr(response, "error");
    if (details) {
        printf("   Error:   ");
        int i = 8;
        while (details[i] != '"') {
            printf("%c", details[i]);
            i++;
        }
        printf("\n\n");
        return 0;
    }

    printf("\n");
    return 1;
}

// Verifica daca un sir de caractere este un umar valid
int number_verify(char *string) {
    int i = 0;
    while (string[i] != '\0') {
        // Codurile ASCII ale cifrelor
        if (string[i] < 48 || string[i] > 57)
            return 0;
        i++;
    }
    return 1;
}

// Adaugarea unei carti in biblioteca
char *add_book(int socket, char *jwt) {
    char title[INPUT];
    char author[INPUT];
    char publisher[INPUT];
    char genre[INPUT];
    char page_count[INPUT];
    
    printf("title=");
    scanf("%s", title);
    printf("author=");
    scanf("%s", author);
    printf("publisher=");
    scanf("%s", publisher);
    printf("genre=");
    scanf("%s", genre);

    // Se citeste nuarul de pagini pana este introdus un numar valid
    int correct = 1;
    do {
        printf("page_count=");
        scanf("%s", page_count);
        if (!number_verify(page_count)) {
            printf("\n<< Incorrect format! >>\n\n");
            correct = 0;
        } else
            correct = 1;
    } while (!correct);

    // Cerere de tip POST catre server
    char *json_string, *message;
    json_string = json_add_book(title, author, publisher, genre, page_count);
    message = compute_post_request(HOST, BOOKS_ROUTE, REGISTER_PAYLOAD,
                                &json_string, strlen(json_string), 
                                NULL, 0, jwt);
    send_to_server(socket, message);
    
    char *response;
    response = receive_from_server(socket);

    return response;
}

// Afisarea sau stergerea unei carti din biblioteca
char *get_delete_book(int socket, char *jwt, int just_get) {
    char id[INPUT];

    // Se citeste un ID pentru carte pana cand ID-ul introdus exista
    int correct = 1;
    do {
        printf("id=");
        scanf("%s", id);
        if (!number_verify(id)) {
            printf("\n<< Incorrect ID! >>\n\n");
            correct = 0;
        } else
            correct = 1;
    } while (!correct);

    // Adaug la ruta de acces ID-ul cartii
    char route[50];
    strcpy(route, BOOKS_ROUTE);
    strcat(route, "/");
    strcat(route, id);

    // Daca se doreste doar afisarea cartii, se face o cerere de tip GET
    // Altfel se face o cerere de tip DELETE
    char *message;
    if (just_get)
        message = compute_get_request(HOST, route, NULL, NULL, 1, jwt);
    else
        message = compute_delete_message(HOST, route, NULL, NULL, 1, jwt);

    send_to_server(socket, message);

    char *response;
    response = receive_from_server(socket);

    return response;
}

// Accesarea librariei, afisarea cartilor sau delogarea
char *enter_get_out(int socket, char *cookie, char *jwt, char *cmd) {
    char *message;

    // Cerere de tip GET folosind ruta de acces in functie de comanda primita
    if (!strcmp(cmd, "enter_library"))
        message = compute_get_request(HOST, ACCESS_ROUTE, NULL, 
                                        &cookie, 1, NULL);
    if (!strcmp(cmd, "get_books"))
        message = compute_get_request(HOST, BOOKS_ROUTE, NULL, NULL, 1, jwt);
    if (!strcmp(cmd, "logout"))
        message = compute_get_request(HOST, LOGOUT_ROUTE, NULL,
                                        &cookie, 1, NULL);

    send_to_server(socket, message);

    char *response;
    response = receive_from_server(socket);

    return response;
}

// Inregistrarea sau logarea la server
char *login_register(int socket, char *username, char *password, char *cmd) {
    printf("username=");
    scanf("%s", username);
    printf("password=");
    scanf("%s", password);

    char *json_string, *message;
    json_string = json_register(username, password);

    // Cerere de tip POST folosind ruta de acces in functie de comanda primita
    if (!strcmp(cmd, "register"))
        message = compute_post_request(HOST, REGISTER_ROUTE, REGISTER_PAYLOAD,
                                        &json_string, strlen(json_string),
                                        NULL, 0, NULL);
    else
        message = compute_post_request(HOST, LOGIN_ROUTE, REGISTER_PAYLOAD,
                                        &json_string, strlen(json_string),
                                        NULL, 0, NULL);

    send_to_server(socket, message);

    char *response;
    response = receive_from_server(socket);

    return response;
}
