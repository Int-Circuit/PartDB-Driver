#include "libs.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

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
char *JSONPathFormatter(char *requestPath) {
  char *JSONFullPath = NULL;
  if (!requestPath) {
    fprintf(stderr, "Failed to get request path");
    return NULL;
  }
  for (int i = 0; i < (sizeof(AJL.APIPath)/ sizeof(AJL.APIPath[0])); i++) {
    if (strcmp(requestPath, AJL.APIPath[i]) == 0) {

      asprintf(&JSONFullPath, "%s.%s", "schemas", AJL.JSONKeys[i]);
      //printf("%s", JSONFullPath);
      return JSONFullPath;
    }
  }
  return NULL;
}
char* DictDwl(responseBuffer_t* input)
{
    cJSON *output = cJSON_CreateObject();
  char *charOutput = NULL;
  if (!input) {
    fprintf(stderr, "Request response buffer empty");
    return NULL;
  }
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
char* responseFormatter(responseBuffer_t *input) {
  cJSON *output = cJSON_CreateObject();
  char *charOutput = NULL;
  if (!input->response || !input) {
    fprintf(stderr, "Request response buffer empty");
    return NULL;
  }
  output = cJSON_Parse(input->response);
  if (!output) {
    const char *error = cJSON_GetErrorPtr();
    fprintf(stderr, "Error %.30s", error);
    goto destroy;
  }
  charOutput = cJSON_Print(output);
  if (!charOutput) {
    fprintf(stderr, "JSON print failed");
    cJSON_Delete(output);
    output = NULL;
  }
  char* charOutputStart = strchr(charOutput, '{');
  char* CharOutputEnd = strrchr(charOutput, '}');
  if (charOutputStart && CharOutputEnd && CharOutputEnd > charOutputStart) {
    charOutputStart++;
    *CharOutputEnd = '\0';
    memmove(charOutput, charOutputStart, strlen(charOutputStart));
  }
  // for (int i = (strchr(charOutput, '"')) - charOutput; i < (strchr(charOutput, ',')) - charOutput; i++) {
  //   printf("string found between %d and %d", )
  // }

  return charOutput;
destroy:
  free(charOutput);
  return NULL;
}
// utiliser le jsonopenapi
int responseHelper(responseBuffer_t *input, fileLoadOpts_t confOpts,
                   requestType_t type, char *requestPath) {
  if (!input) {
    fprintf(stderr, "Request response buffer empty");
    return EXIT_FAILURE;
  }
    if (confOpts.dictDwl != 1) {
    DictDwl(input);
    return EXIT_SUCCESS;
  }
  char* out = responseFormatter(input);
  if (!out) {
     return EXIT_FAILURE;
  }

  printf("%s", out);
  int outDepth;
  //char* outFind = out;
//   while ((outFind = strchr(outFind, ',')) != NULL) {
//     outDepth++;
//     ++outFind;
//   }
// char* outKeyBuffer[outDepth];
// char* outValBuffer[outDepth];
// char* tokenSave;
// char* outTokenize = strtok_r(out, ",", &tokenSave);
//  for (int i = 0; i < outDepth && outTokenize != NULL; i++) {
//     char* split = strchr(outTokenize, ':');
//     if (split != NULL) {
//       *split = '\0';
//       outKeyBuffer[i] = outTokenize;
//       outValBuffer[i] = split + 1;
//     }
//     else {
//       outKeyBuffer[i] = outTokenize;
//       outValBuffer[i] = "";
//     }
//   outTokenize = strtok_r(NULL, ",", &tokenSave);
   
// //   outBuffer[i] = strdup(outTokenize);
 
// //     outTokenize = strtok(NULL, ",");
//     printf("\nKey: %s | Val: %s\n", outKeyBuffer[i], outValBuffer[i]);
//  }

  // Parse the JSON data
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
    //printf("\ndico\n");
   // printf("%s", res);
  }

  // fileLoadOpts_t confOpts = {0};
  // loader(CONF_FILE, &confOpts);
  // printf("%d", confOpts.dictDwl);
  return EXIT_SUCCESS;
}
cJSON *JSONPathTraverser(cJSON *json, char *path, requestType_t type) {
  char *pathCopy;
  char *pathFindChar = NULL;
  int pathDepth = 0;
  char **pathArray;
  char *pathTokenized = NULL;

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
  cJSON *JSONContainer = json;
  for (int i = 0; i < pathDepth + 1; i++) {
    //printf("\n%s\n", pathArray[i]);
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
