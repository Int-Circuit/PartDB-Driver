#include "libs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
bool configDone = false;
int ConsumeHTTPS(CURL *curl, request_t input, request_type_t type) {

  if (!configDone) {
    if (config(curl) != EXIT_SUCCESS) {
      fprintf(stderr, "HTTP configuration failed");
      curl_easy_cleanup(curl);
      return 1;
    }
      configDone = true;
    
  }
  request_helper(curl, input, type);

  return 0;
}