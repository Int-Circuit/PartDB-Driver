#include "libs.h"
#include <libfyaml.h>
#include <stdio.h>
#include <stdlib.h>
int writer(const char *envFile, fileWriteVal_t* values)
{  struct fy_document *fp = NULL;
  fp = fy_document_build_from_file(NULL, envFile);
  if (!fp) {
    fprintf(stderr, "Failed to create document\n");
    goto flush_yaml;
  }
  if (fy_document_insert_at(fp, values->key, FY_NT, fy_node_buildf(fp, "%s", values->value))) {
    fprintf(stderr, "Failed to write file");
    goto flush_yaml;
  }
  	printf("\nUpdated configuration:\n");
	if (fy_emit_document_to_fp(fp, FYECF_SORT_KEYS, stdout)) {
		fprintf(stderr, "Failed to emit document\n");
		goto flush_yaml;
	}
  return EXIT_SUCCESS;
flush_yaml:
  fy_document_destroy(fp);
  return EXIT_FAILURE;
}