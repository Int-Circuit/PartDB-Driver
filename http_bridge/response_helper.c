#include "libs.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct APIJSONLink {
  const char *APIPath[2];
  const char *JSONKeys[2];
};
typedef struct APIJSONLink APIJSONLink_t;

APIJSONLink_t AJL = {{
                         "categories",
                         "parts",
                     },
                     {"Category", "Part"}}; // 23 paths, a completer peu a peu
// Request Path and JSON schema matching
char *JSONPathFormatter(char *requestPath) {
  char *JSONFullPath = NULL;
  if (!requestPath) {
    fprintf(stderr, "Failed to get request path");
    return NULL;
  }
  // checks if request path is correct and sets correct schema
  for (u_int i = 0; i < (sizeof(AJL.APIPath) / sizeof(AJL.APIPath[0])); i++) {
    if (strcmp(requestPath, AJL.APIPath[i]) == 0) {

      asprintf(&JSONFullPath, "%s.%s", "schemas", AJL.JSONKeys[i]);
      // printf("%s", JSONFullPath);
      return JSONFullPath;
    }
  }
  return NULL;
}
char *responseFormatterSimple(responseBuffer_t *input, int isDictdwld) {
  cJSON *output = cJSON_CreateObject();
  char *charOutput = NULL;
  if (!input) {
    fprintf(stderr, "Request response buffer empty");
    return NULL;
  }
  // adding response to JSON object
  output = cJSON_Parse(input->response);
  if (!output) {
    const char *error = cJSON_GetErrorPtr();
    fprintf(stderr, "Error %.30s", error);
    free(output);
  }
  charOutput = cJSON_Print(output);
  if (!charOutput) {
    fprintf(stderr, "JSON print failed");
    cJSON_Delete(output);
    output = NULL;
  }
  if (isDictdwld != 1) {
    FILE *fp = fopen("docs.jsonopenapi", "w");
    if (!fp) {
      fprintf(stderr, "Cannot open/create file to write");
      return NULL;
    }
    fprintf(fp, "%s", cJSON_Print(output));

    if (!cJSON_Print(output)) {
      fprintf(stderr, "JSON to string conversion of openAPI file failed");
      fclose(fp);
      cJSON_Delete(output);
      return NULL;
    }
    fclose(fp);
  }
  return NULL;
}
pformatedResponse_t responseFormatterAdvanced(responseBuffer_t *input) {
  cJSON *output = cJSON_CreateObject();
  char *charOutput = NULL;
  char *charOutputStart = NULL;
  char *CharOutputEnd = NULL;
  char **formattedOutput;
  char *key;
  char *value;
  int count = 0;
  pformatedResponse_t Format;
  
  if (!input->response || !input) {
    fprintf(stderr, "Request response buffer empty");
    return NULL;
  }
  output = cJSON_Parse(input->response);
  if (!output) {
    const char *error = cJSON_GetErrorPtr();
    fprintf(stderr, "Error %.30s", error);
    free(charOutput);
    return NULL;
  }

  charOutput = cJSON_Print(output);
  if (!charOutput) {
    fprintf(stderr, "JSON print failed");
    cJSON_Delete(output);
    output = NULL;
  }
  // remove spaces + braces
  cJSON_Minify(charOutput);
  charOutputStart = strstr(charOutput, "[{");
  CharOutputEnd = strstr(charOutput, "}]");
  if (charOutputStart && CharOutputEnd && CharOutputEnd > charOutputStart) {
    charOutputStart += 2;

    size_t lenght = CharOutputEnd - charOutputStart;
    memmove(charOutput, charOutputStart, lenght);
    charOutput[lenght] = '\0';
  }

  char *charCount = charOutput;

  while ((charCount = strchr(charCount, ',')) != NULL) {
    charCount++;
    count++;
  }
  formattedOutput = calloc(count + 1, sizeof(char *));
  Format = malloc((count+1) * sizeof(formatedResponse_t));
  char *token = strtok(charOutput, ",");
  for (int i = 0; i < count && token; i++) {
    formattedOutput[i] = strdup(token);
    token = strtok(NULL, ",");
  }
  for (int i = 0; i < count; i++) {
    key = strchr(formattedOutput[i], ':');
    if (key != NULL) {
      // Temporarily split the string by replacing ':' with '\0', allows to
      // separate key from value since format is "key":"value"
      *key = '\0';

      Format[i].key = strdup(formattedOutput[i]);
      Format[i].value = strdup(key + 1); // Value starts right after the colon

      //printf("key: %s - value: %s\n", Format[i].key, Format[i].value);
    } else {
      // Handle case where colon is missing
      Format[i].key = strdup(formattedOutput[i]);
      Format[i].value = strdup("");
      printf("Key + values not written properly\n");
    }
  }
  Format->size = count;
  if (formattedOutput) {
    for (int j = 0; j < count; j++) {
      free(formattedOutput[j]);
    }
    free(formattedOutput);
  }

  return Format;
}
// utiliser le jsonopenapi
int responseHelper(responseBuffer_t *input, fileLoadOpts_t confOpts,
                   requestType_t type, char *requestPath) {
  if (!input) {
    fprintf(stderr, "Request response buffer empty");
    return EXIT_FAILURE;
  }
  if (confOpts.dictDwl != 1) {
    responseFormatterSimple(input, confOpts.dictDwl);
    return EXIT_SUCCESS;
  }
  pformatedResponse_t out = responseFormatterAdvanced(input);
  if (!out) {
    return EXIT_FAILURE;
  }

  for (u_int i = 0; i < out->size; i++) {
      printf("%s %s %d\n", out[i].key, out[i].value, (out->size-i));
  }


  // Parse the minimized JSON 'schema' data
  // Produced from dictionary.c
  cJSON *json = cJSON_Parse(readJSON("write.json"));
  // Check if parsing was successful
  if (!json) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL) {
      fprintf(stderr, "Error before: %s\n", error_ptr);
    }
    cJSON_Delete(json);
    return EXIT_FAILURE;
  }

  char *format = JSONPathFormatter(requestPath);
  if (!format) {
    printf("C null");
    return EXIT_FAILURE;
  }
  if (JSONPathTraverser(json, format, type) != NULL) {
    char *res = cJSON_Print(JSONPathTraverser(json, format, type));
    // printf("\ndico\n");
    // printf("%s", res);
  }

  // fileLoadOpts_t confOpts = {0};
  // loader(CONF_FILE, &confOpts);
  // printf("%d", confOpts.dictDwl);
  return EXIT_SUCCESS;
}
// Traverse JSON file to correct element
cJSON *JSONPathTraverser(cJSON *json, char *path, requestType_t type) {
  char *pathCopy;
  char *pathFindChar = NULL;
  int pathDepth = 0;
  char **pathArray;
  char *pathTokenized = NULL;
  cJSON *JSONContainer = json;

  switch (type) {
  default:
    printf(
        "JSON output not recognized/ HTTP operation doesn't return JSON info");
    // path[0] = toupper(path[0]);
    break;
  case GET:
    asprintf(&pathCopy, "%s-Read", path);
    break;
  case POST:;
  }
  // printf("%s", path);
  pathFindChar = pathCopy;
  // printf("%s", pathFindChar);
  while ((pathFindChar = strchr(pathFindChar, '.')) != NULL) {
    pathDepth++;
    ++pathFindChar;
  }
  // printf("%s",pathFindChar);

  // printf("\n%d", pathDepth);
  pathArray = (char **)calloc(pathDepth + 1, sizeof(char *));
  if (!pathArray) {
    fprintf(stderr, "Failed to allocate Array");
    goto destroy;
    return NULL;
  }

  pathTokenized = strtok(pathCopy, ".");

  for (int i = 0; i < pathDepth + 1 && pathTokenized; i++) {
    pathArray[i] = strdup(pathTokenized);
    if (!pathArray[i]) {
      fprintf(stderr, "Failed to allocate Array element no %d", i);
      goto destroy;
      return NULL;
    }
    pathTokenized = strtok(NULL, ".");
  }
  JSONContainer = json;
  for (int i = 0; i < pathDepth + 1; i++) {
    // printf("\n%s\n", pathArray[i]);
    JSONContainer =
        cJSON_GetObjectItemCaseSensitive(JSONContainer, pathArray[i]);
    if (!JSONContainer) {
      fprintf(stderr, "Failed to get JSON container for key: %s\n",
              pathArray[i]);
      break;
    }
  }

destroy:
  if (pathArray) {
    for (int i = 0; i < pathDepth + 1; i++) {
      free(pathArray[i]);
    }
  }
  free(pathArray);
  return JSONContainer;
}
