#include "libs.h"

int loader(const char *envFile, fileLoadOpts_t *options) {
  struct fy_document *fp = NULL;
  fp = fy_document_build_from_file(NULL, envFile);
  if (!fp) {
    fprintf(stderr, "Failed to create document\n");
    return 1;
    goto flush_yaml;
  }
  options->url = calloc(TOKEN_MAX_SIZE, 1);
  options->token = calloc(TOKEN_MAX_SIZE, 1);
  options->certLocation = calloc(TOKEN_MAX_SIZE, 1);
  if ((!options->url || !options->token || !options->certLocation)) {
    printf("Could not initialize memory\n");
    goto flush_yaml;
  }

  int count =
      fy_document_scanf(fp,
                        "/server/url %255s "
                        "/server/token %255s "
                        "/server/certificate/location %255s "
                        "/server/certificate/enabled %d "
                        "/server/dict/isDictdwld %d ",
                        options->url, options->token, options->certLocation,
                        &options->SSLOn, &options->dictDwl);
  if (count != 5) {
    fprintf(stderr, "Failed to parse Yaml config file\n");
    goto flush_yaml;
  }

  fy_document_destroy(fp);

  return EXIT_SUCCESS;
flush_yaml:
  fy_document_destroy(fp);
  free(options->url);
  free(options->token);
  free(options->certLocation);
  return EXIT_FAILURE;
}