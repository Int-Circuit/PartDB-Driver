#include "libs.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

char *url = NULL;

int config(CURL *curl, fileLoadOpts_t confOpts) {
 
  if (!curl) {
    fprintf(stderr, "curl handle is NULL\n");
    return 1;
  }



#ifdef DEBUG
  // Disable host and peer certificate verification, DEBUG ONLY
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
  // Set HTTP Auth Type
  curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
  // Set HTTP Version, default v3
  curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_3);
  // Input Bearer toke
  curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, confOpts.token);
  // Select request output storage location
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_chunk);
  curl_easy_setopt(curl, CURLOPT_CA_CACHE_TIMEOUT, 604800L);
  curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
  printf("| URL %s | SSL Status %d|\n", confOpts.url, confOpts.SSLOn);

  if (confOpts.SSLOn) {
    // SSL ENABLED: Enforce verification
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, confOpts.certLocation);
  } else {
    // SSL DISABLED: Bypass all verification
    printf("You seem to be connecting to Part-DB using HTTP. Please only use "
           "this method on a local and trusted network!\n");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    // DO NOT set CURLOPT_CAINFO
  }
  return 0;
}
size_t write_chunk(void *data, size_t size, size_t nmemb, void *userData) {
  size_t responseRealSize = size * nmemb;
  responseBuffer_t *resBuffer = (responseBuffer_t *)userData;

  char *bufferRealloc =
      realloc(resBuffer->response, resBuffer->size + responseRealSize + 1);
  if (!bufferRealloc) {
    fprintf(stderr, "Out of Memory error (realloc returned NULL)\n");
    return 0;
  }
  resBuffer->response = bufferRealloc;
  memcpy(&(resBuffer->response[resBuffer->size]), data, responseRealSize);
  resBuffer->size += responseRealSize;
  resBuffer->response[resBuffer->size] = 0;

  return responseRealSize;
}
