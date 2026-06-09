#include "global.h"
#include <stdio.h>
int main() {
  // Setup
  fileLoadOpts_t options = {0};
  CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
  if (global_result != CURLE_OK) {
    fprintf(stderr, "curl_global_init() failed\n");
    return (int)global_result;
  }

  CURL *curl = curl_easy_init();
  if (!curl) {
    fprintf(stderr, "curl_easy_init() failed\n");
    curl_global_cleanup();
    return 1;
  }

  if (utilsOps(loadFile, &options, NULL) != EXIT_SUCCESS) {
    perror("Failed to parse conf file, one or more values are empty");
    abort();
  }
  if (options.dictDwl != 1) {
    fileWriteVal_t values = {"/server/dict", "isDictdwld: 1"};
    request_t dict = {.type = "docs.jsonopenapi"};
    consumeHTTPS(curl, dict, GET, options);
    utilsOps(createDict, NULL, NULL);
    utilsOps(writeFile, NULL, &values);
    options.dictDwl = 1;
  }

  // utilsOps(writeFile, NULL, &values);
  request_t test = {.page = 1,
                    .type = "parts",
                    .itemsPerPage = 30,
                    .name = "test",
                    .orderName = 0,
                    .orderId = 0,
                    .orderAddedDate = asc,
                    .orderLastModified = 0};
  request_t cat1 = {.page = 1,
                    .type = "categories",
                    .itemsPerPage = 30,
                    .name = "test5",
                    .orderName = 0,
                    .orderId = 0,
                    .orderAddedDate = asc,
                    .orderLastModified = 0};
  request_t test1 = {.page = 1,
                     .type = "parts",
                     .itemsPerPage = 30,
                     .name = "test12",
                     .orderName = 0,
                     .orderId = 0,
                     .orderAddedDate = asc,
                     .orderLastModified = 0,
                     .category = "6",
                     .id = "96"};
  request_t test2 = {.page = 1,
                     .type = "parts",
                     .itemsPerPage = 30,
                     .name = "test2",
                     .orderName = 0,
                     .orderId = 0,
                     .orderAddedDate = asc,
                     .orderLastModified = 0,
                     .category = "6",
                     .id = "117"};

  // ConsumeHTTPS(curl, cat1, POST, options);
  //  ConsumeHTTPS(curl, test2, POST);
  consumeHTTPS(curl, test2, GET, options);
  // printf("\npatch\n");
  // ConsumeHTTPS(curl, test1, PATCH);
  // printf("\ndelete\n");
  // ConsumeHTTPS(curl, test2, DELETE);

  // ConsumeHTTPS(curl, cat1, GET);
  // consumeSerial();
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  return 0;
}