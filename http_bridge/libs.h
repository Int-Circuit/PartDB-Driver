
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
#define CONF_FILE "conf.yaml"
// Global var defines

enum order { asc = 0, desc = 1 };
typedef enum order order_t;
enum request_type { GET = 0, POST = 1, GET_id = 2, DELETE = 3, PATCH = 4 };
typedef enum request_type request_type_t;
struct request {

  int page;
  int itemsPerPage;
  bool favorite;
  bool needsReview;
  order_t orderName;
  order_t orderId;
  order_t orderAddedDate;
  order_t orderLastModified;
  char* id;
  char *type;
  char *properties;
  char *category;
  char *footprint;
  char *manufacturer;
  char *partUnit;
  char *partCustomState;
  char *storageLocation;
  char *name;
  char *comment;
  char *description;
  char *ipn;
  char *manufacturerProductNumber;
  char *tags;
};
struct response
{

};
typedef struct request request_t;
struct fileLoadOpts
{
    char* url;
    char* token;
    char* certLocation;
    int SSLOn;
    char* dictLocation;
    int dictDwl;
};
typedef struct fileLoadOpts fileLoadOpts_t ;
struct responseBuffer
{
    char* response;
    size_t size;
};
typedef struct responseBuffer responseBuffer_t;
// Global func defines
int ConsumeHTTPS(CURL* curl, request_t input, request_type_t type);
//config.c
int config(CURL *curl);
size_t write_chunk(void *data, size_t size, size_t nmemb, void *userData);
int loader(const char *envFile, fileLoadOpts_t *options);
//response_helper.c
int responseFormatter(responseBuffer_t input);
int responseHelper(cJSON* input);
//request_helper.c
int requestHelper(CURL *curl, request_t request, request_type_t type);
char *requestConstructor(request_t input, request_type_t type);
char* urlConstructor(char *url, char* type, char* id, request_type_t reqType);
int dictPreProcess();
// extern ;

// helper functions
inline const char *enumToString(order_t order) {
  switch (order) {
  case 0:
    return "asc";
  case 1:
    return "desc";
  default:
    return "asc";
  }
}
