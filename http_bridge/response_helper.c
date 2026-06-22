#include "libs.h"
#include <ctype.h>
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  //printf("\n%s\n", res_out);
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
    fprintf(fp, "%s", cJSON_Print(response));

    if (!cJSON_Print(response)) {
      fprintf(stderr, "JSON to string conversion of openAPI file failed");
      fclose(fp);
      cJSON_Delete(response);
      return EXIT_FAILURE;
    }
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
  if (!json) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL) {
      fprintf(stderr, "Error before: %s\n", error_ptr);
    }
    cJSON_Delete(json);
    free(buffer);
    return EXIT_FAILURE;
  }

  JSONPathTraverser(json, "schemas.category", type);

  // fileLoadOpts_t confOpts = {0};
  // loader(CONF_FILE, &confOpts);
  // printf("%d", confOpts.dictDwl);

  return EXIT_SUCCESS;
}
int JSONPathTraverser(cJSON* json, char* path, requestType_t type)
{
  char* pathCopy;
  char* pathFindChar = NULL;
  char targetChar = '.';
  int pathDepth = 0;
  char** pathArray;
  char* pathTokenized = NULL;

  pathCopy = strdup(path);
  if (pathCopy) {
      ;

  }
      char *categoryPos = strstr(pathCopy, "category");
    if (categoryPos) {
        *categoryPos = toupper((unsigned char)*categoryPos); // Capitalize 'c' to 'C'
    } 
printf("%s", pathCopy);
    switch (type) {
    default:
      printf("JSON output not recognized/ HTTP operation doesn't return JSON info");
      //path[0] = toupper(path[0]);
      break;
    case GET:
    asprintf(&pathCopy,"%s-Read", pathCopy);
    break;
    case POST:
    ;
  }
  //printf("%s", path);
  pathFindChar = pathCopy;
  //printf("%s", pathFindChar);
     while ((pathFindChar = strchr(pathFindChar, targetChar))!= NULL) {
     pathDepth++;
     ++pathFindChar;
   }
     //printf("%s",pathFindChar);

  // printf("\n%d", pathDepth);
  pathArray = (char**)malloc(pathDepth +1 * sizeof(char*));
  if (!pathArray) {
    fprintf(stderr, "Failed to allocate Array");
    goto destroy;
    return EXIT_FAILURE;
  }

  pathTokenized = strtok(pathCopy, ".");

 
  for (int i = 0; i < pathDepth + 1 && pathTokenized; i++) {
    pathArray[i] = strdup(pathTokenized);
    if (!pathArray[i]) {
    fprintf(stderr, "Failed to allocate Array element no %d", i);
    goto destroy;
    return EXIT_FAILURE;
    }
    pathTokenized = strtok(NULL, ".");
    
  }
    cJSON* JSONContainer = json;
for (int i = 0; i < pathDepth + 1; i++) {
    printf("\n%s\n", pathArray[i]);
    JSONContainer = cJSON_GetObjectItemCaseSensitive(JSONContainer, pathArray[i]);
    if (!JSONContainer) {
        fprintf(stderr, "Failed to get JSON container for key: %s\n", pathArray[i]);
        continue; // Skip this iteration
    }
   
   // printf("\n%s\n", res);

  }  
  char* res = cJSON_Print(JSONContainer);

  if (!res) {
    goto destroy;
  }
  printf("%s", res);
destroy:
for (int i = 0; i < pathDepth + 1; i++) {
  free(pathArray[i]);
}  
free(pathArray);
return EXIT_SUCCESS; 
}