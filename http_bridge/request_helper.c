#include "libs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int requestHelper(CURL *curl, request_t request, requestType_t type,
                  fileLoadOpts_t confOpts) {

  char *returnURl = NULL;
  responseBuffer_t responseChunks = {0};
  struct curl_slist *header = NULL;


  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL); // Clear POST data
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, NULL); //Clear header (needs to be setup manualy for POST + dictionary download)
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&responseChunks); //Specify HTTP response buffer

  if (confOpts.dictDwl != 1) {
  header = curl_slist_append(header, "Accept: application/vnd.openapi+json");
  }
  else {
    header = curl_slist_append(header, "Accept: application/json");

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
  //Present to force to GET if POST has been used before, 
  //removing it makes  curl sometime use POST instead of GET
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
    // PATCH needs specific header, if not present function fails
    header =
        curl_slist_append(header, "Content-Type: application/merge-patch+json");
    curl_easy_setopt(
        curl, CURLOPT_URL,
        urlConstructor(confOpts.url, request.type, request.id, type));
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,
                     requestConstructor(request, type));

    break;
  case POST:
    //Force header to be sure that it is set
    header = curl_slist_append(header, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_URL,
                     urlConstructor(confOpts.url, request.type, NULL, type));

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,
                     requestConstructor(request, type));

    break;
  default:
    fprintf(stderr, "Unknown request type: %d\n", type);
    return EXIT_FAILURE;
  }
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

  // helper performs request
  CURLcode result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(result));
    goto free;
    return (int)result;
  }
  responseHelper(&responseChunks, confOpts, type, request.type);
free:
  free(responseChunks.response);

  return 0;
}
char *urlConstructor(char *url, char *type, char *id, requestType_t reqType) {
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

char *requestConstructor(request_t input, requestType_t type) {
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