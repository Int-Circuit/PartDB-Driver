#include "types.h"
#include <curl/curl.h>
#include <stdlib.h>
int consumeHTTPS(CURL* curl, request_t input, requestType_t type, fileLoadOpts_t options);
int consumeSerial(fileLoadOpts_t options);
int utilsOps(utilOpsType_t type, fileLoadOpts_t* options, fileWriteVal_t* values);