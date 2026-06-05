#include "libs.h"
bool configDone = false;
int consumeHTTPS(CURL *curl, request_t input, requestType_t type, fileLoadOpts_t options) {

  if (!configDone) {
    if (config(curl, options) != EXIT_SUCCESS) {
      fprintf(stderr, "HTTP configuration failed");
      curl_easy_cleanup(curl);
      return 1;
    }
      configDone = true;
    
  }
  requestHelper(curl, input, type, options);

  return 0;
}