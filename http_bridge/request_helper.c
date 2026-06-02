#include "libs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int request_helper(CURL *curl, request_t request, request_type_t type) {
  fileLoadOpts_t confOpts;
  char *returnURl = NULL;
  responseBuffer_t responseChunks = {0};
  struct curl_slist *header = NULL;

  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL);   // Clear POST data
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, NULL);   
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&responseChunks);
  if (loader(CONF_FILE, &confOpts) != EXIT_SUCCESS) {
    free(confOpts.url);
    perror("Failed to parse conf file, one or more values are empty");
    abort();
  }
  switch (type) {
  case DELETE:
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(
        curl, CURLOPT_URL,
        urlConstructor(confOpts.url, request.type, request.id, type));
    break;

  case GET_id:
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    curl_easy_setopt(
        curl, CURLOPT_URL,
        urlConstructor(confOpts.url, request.type, request.id, type));
    break;

  case GET:
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    asprintf(&returnURl, "%s%s",
             urlConstructor(confOpts.url, request.type, NULL, type),
             requestConstructor(request, type));
    curl_easy_setopt(curl, CURLOPT_URL, returnURl);

    break;
  case PATCH:

    header = curl_slist_append(header, "Content-Type: application/merge-patch+json");
    curl_easy_setopt(
        curl, CURLOPT_URL,
        urlConstructor(confOpts.url, request.type, request.id, type));
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,
                     requestConstructor(request, type));
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
                 
    break;
  case POST:
   header = curl_slist_append(header, "Content-Type: application/ld+json");
    curl_easy_setopt(curl, CURLOPT_URL,
                     urlConstructor(confOpts.url, request.type, NULL, type));

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,
                     requestConstructor(request, type));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
                 

    break;
  default:
    fprintf(stderr, "Unknown request type: %d\n", type);
    return EXIT_FAILURE;
  }
  // helper performs request
  CURLcode result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(result));
    goto free;
    return (int)result;
  }
  // printf("%s", responseChunks.response);
  responseFormatter(responseChunks);
free:
  free(responseChunks.response);

  return 0;
}
char *urlConstructor(char *url, char *type, char *id, request_type_t reqType) {
  char *returnData = NULL;
  if (!url || !type) {
    fprintf(stderr, "URl or type is null");
    return NULL;
  }
  switch (reqType) {
  case GET:
  case POST:
    asprintf(&returnData, "%s/%s", url, type);
    break;
  case GET_id:
  case DELETE:
  case PATCH:
    asprintf(&returnData, "%s/%s/%s", url, type, id);
    break;
  }

  return returnData;
}

char *requestConstructor(request_t input, request_type_t type) {
  char *pOutput = NULL;

  switch (type) {
  default:
    printf("Request construction not required");  
    break;
  case GET:
    // GET Request constructor

    asprintf(
        &pOutput,
        "?page=%d&itemsPerPage=%d&name=%s&order%%5Bname%%5D=%s&order%%5Bid%%"
        "5D=%s&order%%5BaddedDate%%5D=%s&order%%5BlastModified%%5D=%s",
        input.page, input.itemsPerPage, input.name,
        enumToString(input.orderName), enumToString(input.orderId),
        enumToString(input.orderAddedDate),
        enumToString(input.orderLastModified));
    break;
  case PATCH:
  case POST:
    // POST Request constructor

    asprintf(&pOutput,
             "{\"name\": \"%s\", \"category\": \"/api/categories/%s\"}",
             input.name, input.category);
    break;
  }
  return pOutput;
}