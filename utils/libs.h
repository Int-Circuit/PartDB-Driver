#include <stdlib.h>
#include <libfyaml.h>
#include "../types.h"
#include <stdio.h>
#include <cjson/cJSON.h>

#define CONF_FILE "conf.yaml"
#define TOKEN_MAX_SIZE 256
int loader(const char *envFile, fileLoadOpts_t *options);
int writer(const char *envFile, fileWriteVal_t* values);
int dictPreProcess();


static inline const char* readJSON(char* filename)
{  FILE *dict;

    dict = fopen(filename, "r");
  if (!dict) {
    fprintf(stderr, "Could not open dictionary file, check presence.");
    return NULL;
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
  return buffer;
}