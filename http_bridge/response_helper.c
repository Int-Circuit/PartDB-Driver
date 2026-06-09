#include "libs.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int responseFormatter(responseBuffer_t *input, cJSON **output) {
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
  // cJSON* info = cJSON_GetObjectItemCaseSensitive(response, "hydra:member");
  // char* info_out = cJSON_Print(info);
  printf("\n%s\n", res_out);
  // printf("\n%s\n",info_out);
  // free(info_out);
  // response_helper(response);
  return EXIT_SUCCESS;
destroy:
  free(res_out);
  return EXIT_FAILURE;
}
// utiliser le jsonopenapi
int responseHelper(responseBuffer_t *input, fileLoadOpts_t confOpts, requestType_t type) {
  cJSON *response = NULL;
  FILE *dict;
  if (!input) {
    fprintf(stderr, "Request response buffer empty");
    return EXIT_FAILURE;
  }

  if (responseFormatter(input, &response) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  if (confOpts.dictDwl != 1) {
    FILE *fp = fopen("docs.jsonopenapi", "w");
    if (!fp) {
      fprintf(stderr, "Cannot open/create file to write");
      return EXIT_FAILURE;
    }
    if (!cJSON_Print(response)) {
      fprintf(stderr, "JSON to string conversion of openAPI file failed");
      fclose(fp);
      cJSON_Delete(response);
      return EXIT_FAILURE;
    }
    fprintf(fp, "%s", cJSON_Print(response));
    fclose(fp);
  }
  dict = fopen("write.json", "r");
  if (!dict) {
    fprintf(stderr, "Could not open dictionary file, check presence.");
    return EXIT_FAILURE;
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
  // Parse the JSON data
  cJSON *json = cJSON_Parse(buffer);
  // Check if parsing was successful
  if (json == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL) {
      fprintf(stderr, "Error before: %s\n", error_ptr);
    }
    cJSON_Delete(json);
    free(buffer);
    return EXIT_FAILURE;
  }


  // fileLoadOpts_t confOpts = {0};
  // loader(CONF_FILE, &confOpts);
  // printf("%d", confOpts.dictDwl);

  return EXIT_SUCCESS;
}