#include "../types.h"
#define _GNU_SOURCE 1
#include <assert.h>
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <libfyaml.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// defines
#define TOKEN_MAX_SIZE 256
#define DEBUG
// Global var defines
struct responseBuffer {
  char *response;
  size_t size;
};
typedef struct responseBuffer responseBuffer_t;
// Global func defines
struct formatedResponse {
  char *key;
  char *value;
  u_int size;
};
typedef struct formatedResponse formatedResponse_t;
typedef formatedResponse_t *pformatedResponse_t;

// config.c
int config(CURL *curl, fileLoadOpts_t options);
size_t write_chunk(void *data, size_t size, size_t nmemb, void *userData);
int loader(const char *envFile, fileLoadOpts_t *options);
// response_helper.c
pformatedResponse_t responseFormatterAdvanced(responseBuffer_t *input);
int responseHelper(responseBuffer_t *input, fileLoadOpts_t confOpts,
                   requestType_t type, char *requestPath);
cJSON* JSONPathTraverser(cJSON *json, char *path, requestType_t type);
// request_helper.c
int requestHelper(CURL *curl, request_t request, requestType_t type,
                  fileLoadOpts_t confOpts);
char *requestConstructor(request_t input, requestType_t type);
char *urlConstructor(char *url, char *type, char *id, requestType_t reqType);
// extern ;

// helper functions
static inline const char *enumToString(order_t order) {
  switch (order) {
  case 0:
    return "asc";
  case 1:
    return "desc";
  default:
    return "asc";
  }
}

static inline const char* readJSON(char* filename)
{  FILE *dict;

    dict = fopen(filename, "r");
  if (!dict) {
    fprintf(stderr, "Could not open dictionary file, check presence.");
    return NULL;
  }
  // Get the file size
  fseek(dict, 0, SEEK_END);
  long fileSize = ftell(dict);
  fseek(dict, 0, SEEK_SET);
  // Read the entire dict into a buffer
  char *buffer = (char *)malloc(fileSize + 1);
  fread(buffer, 1, fileSize, dict);
  buffer[fileSize] = '\0'; // Null-terminate the string
  // Close the dict
  fclose(dict);
  return buffer;
}