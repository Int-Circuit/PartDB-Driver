#include "libs.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int responseFormatter(responseBuffer_t* input, cJSON** output) {
  *output = cJSON_CreateObject();
  char *res_out = NULL;
  if (!input->response || !input) {
    fprintf(stderr, "Request response buffer empty");
    return EXIT_FAILURE;
  }
  *output = cJSON_Parse(input->response);
  if (!*output) {
    const char *error = cJSON_GetErrorPtr();
    fprintf(stderr, "Error %.30s", error);
    goto destroy;
  }
  res_out = cJSON_Print(*output);
  if (!res_out) {
    fprintf(stderr, "JSON print failed");
    cJSON_Delete(*output);
    *output = NULL;
  }
  //cJSON* info = cJSON_GetObjectItemCaseSensitive(response, "hydra:member");
  //char* info_out = cJSON_Print(info);
  printf("\n%s\n", res_out);
  //printf("\n%s\n",info_out);
  //free(info_out);
  //response_helper(response);
  return EXIT_SUCCESS;
destroy:
  free(res_out);
  return EXIT_FAILURE;
}
//utiliser le jsonopenapi
int responseHelper(responseBuffer_t* input, fileLoadOpts_t confOpts)
{
  cJSON* response = NULL;
  if (!input) {
      fprintf(stderr, "Request response buffer empty");
      return EXIT_FAILURE;
  }
      if (responseFormatter(input, &response) != EXIT_SUCCESS) {  // ✅ Vérifie le retour
        return EXIT_FAILURE;
    }
    if (confOpts.dictDwl !=1) {
        FILE* fp = fopen("docs.jsonopenapi", "w");  // Note: j'ai corrigé le nom du fichier (jsonopenapi → json)
        if (!fp) {
          fprintf(stderr, "Cannot open/create file to write");
          return EXIT_FAILURE;
        }
      char* openAPIJSON = cJSON_Print(response);  
      if (!openAPIJSON) {
        fprintf(stderr, "JSON to string conversion of openAPI file failed");
        fclose(fp);
        cJSON_Delete(response);
        return EXIT_FAILURE;
        
      }
      printf("\nj'ecris fichier");
      fprintf(fp, "%s", openAPIJSON);
      free(openAPIJSON);
      fclose(fp);
    }
    printf("\nj'ecris pas fichier");

//fileLoadOpts_t confOpts = {0};
//loader(CONF_FILE, &confOpts);
//printf("%d", confOpts.dictDwl);

return EXIT_SUCCESS;
}