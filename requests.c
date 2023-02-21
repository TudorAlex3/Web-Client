#include "helpers.h"

char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // tipul cererii, url-ul si tipul continutul
    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // hostul
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // cookie-urile
    if (cookies != NULL) {
        memset(line, 0, LINELEN);
        strcat(line, "Cookie: ");

        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(line, cookies[i]);
            strcat(line, ";");
        }

        strcat(line, cookies[cookies_count - 1]);
        compute_message(message, line);
    }

    // header-ul de autorizare
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line); 
    }
    
    compute_message(message, "");

    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, int cookies_count, char* token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // tipul cererii, url-ul si tipul protocolului
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // hostul
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: application/%s\r\nContent-Length: %d", content_type, body_data_fields_count);
    compute_message(message, line);

    // cookie-urile
    if (cookies != NULL) {
        memset(line, 0, LINELEN);
        strcat(line, "Cookie: ");

        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(line, cookies[i]);
            strcat(line, ";");
        }

        strcat(line, cookies[cookies_count - 1]);
        compute_message(message,line);
    }

    // header-ul de autorizare
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line); 
    }

    compute_message(message, "");

    // continutul
    memset(line, 0, LINELEN);
    strcat(message, *body_data);
    compute_message(message, line);

    free(line);
    return message;
}

char *compute_delete_message(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char* token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // tipul cererii, url-ul si tipul continutului
    if (query_params != NULL) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // host-ul
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // cookie-urile
    if (cookies != NULL) {
        memset(line, 0, LINELEN);
        strcat(line, "Cookie: ");

        for (int i = 0; i < cookies_count - 1; i++) {
            strcat(line, cookies[i]);
            strcat(line, ";");
        }

        strcat(line, cookies[cookies_count - 1]);
        compute_message(message, line);
    }

    // header-ul de autorizare
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line); 
    } 
    
    compute_message(message, "");

    return message;
}