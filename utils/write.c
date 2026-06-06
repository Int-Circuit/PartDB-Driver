#include "libs.h"
#include <libfyaml.h>
#include <stdio.h>
#include <stdlib.h>
int writer(const char *envFile, fileWriteVal_t* values)
{  struct fy_document *fyd = NULL;
  fyd = fy_document_build_from_file(NULL, envFile);
  if (!fyd) {
    fprintf(stderr, "Failed to create YAML document, check if envFile isn't null\n");
    goto flush_yaml;
  }
  struct fy_node* fyn = NULL;
  fyn = fy_node_build_from_file(fyd, envFile);
  if (!fyn) {
    fprintf(stderr, "Failed to create YAML node, check if envFile isn't null");
    goto flush_yaml;
  }
  printf("Path: %s", fy_node_get_path(fy_node_by_path(fyn, values->key, -1, FYNWF_PTR_DEFAULT)));
 // if (fy_document_insert_at(fyd, "/server/dict", FY_NT,
	 //   fy_node_buildf(fyd,
	//	"isDictdwld: 1"))) {
   if (fy_document_insert_at(fyd, fy_node_get_path(fy_node_by_path(fyn, values->key, -1, FYNWF_PTR_DEFAULT)), FY_NT,  fy_node_buildf(fyd, "%s", values->value))) {
    fprintf(stderr, "Failed to write file");
    goto flush_yaml;
  }
  	printf("\nUpdated configuration:\n");
	//if (fy_emit_document_to_file(fyd, FYECF_DEFAULT | FYECF_SORT_KEYS, envFile)) {

    if (fy_emit_document_to_fp(fyd, FYECF_DEFAULT, stdout)) {
		fprintf(stderr, "Failed to emit document\n");
		goto flush_yaml;
	}
  return EXIT_SUCCESS;
flush_yaml:
  fy_document_destroy(fyd);
  return EXIT_FAILURE;
}
