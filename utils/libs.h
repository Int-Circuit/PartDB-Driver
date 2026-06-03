#include <stdlib.h>
#include <libfyaml.h>
#include "../types.h"
#include <stdio.h>

#define CONF_FILE "conf.yaml"
#define TOKEN_MAX_SIZE 256
int loader(const char *envFile, fileLoadOpts_t *options);
int writer(const char *envFile, fileWriteVal_t* values);