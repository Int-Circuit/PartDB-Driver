#include "libs.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>

int response_formatter(responseBuffer_t input) {
  cJSON *response = cJSON_CreateObject();
  char *res_out = NULL;
  response = cJSON_Parse(input.response);
  if (!response) {
    const char *error = cJSON_GetErrorPtr();
    fprintf(stderr, "Error %.30s", error);
    goto destroy;
  }
  res_out = cJSON_Print(response);
  cJSON* info = cJSON_GetObjectItemCaseSensitive(response, "hydra:member");
  char* info_out = cJSON_Print(info);
  printf("\n%s\n", res_out);
  printf("\n%s\n",info_out);
  free(info_out);
  response_helper(response);

destroy:
  cJSON_Delete(response);
  free(res_out);

  return EXIT_SUCCESS;
}
int response_helper(cJSON* input)
{
fileLoadOpts_t confOpts = {0};
loader(CONF_FILE, &confOpts);
//printf("%d", confOpts.dictDwl);
//FILE* fp = fopen("placeholder", "r") ;   
//if (!fp) {
        //perror("Cannot open file");
        //return EXIT_FAILURE;
    //}
return EXIT_SUCCESS;
  }