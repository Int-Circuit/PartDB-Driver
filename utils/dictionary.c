#include "libs.h"
#include <cjson/cJSON.h>
cJSON *dictJSONPathTraverser(cJSON *json, char *path) {

  char *pathFindChar = NULL;
  char targetChar = '.';
  int pathDepth = 0;
  char **pathArray;
  char *pathTokenized = NULL;

  // printf("%s", path);
  pathFindChar = path;
  // printf("%s", pathFindChar);
  while ((pathFindChar = strchr(pathFindChar, targetChar)) != NULL) {
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

  pathTokenized = strtok(path, ".");

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
int dictPreProcess() {
  FILE *write = fopen("write.json", "w");

  if (!write) {
    perror("Cannot open file");
    return EXIT_FAILURE;
  }

  // 2. Parser le JSON
  cJSON *json = cJSON_Parse(readJSON("docs.jsonopenapi"));

  if (!json) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr) {
      fprintf(stderr, "JSON parsing error: %s\n", error_ptr);
    }
    fclose(write);      // Fermer write avant de retourner
    cJSON_Delete(json); // json est NULL ici, mais cJSON_Delete gère les NULL
    return EXIT_FAILURE;
  }

  // 4. Convertir "components" en chaîne JSON et l'écrire
  char *out = cJSON_Print(dictJSONPathTraverser(json, "components"));
  if (!out) {
    fprintf(stderr, "Failed to print JSON\n");
    cJSON_Delete(json);
    fclose(write);
    return EXIT_FAILURE;
  }

  fprintf(write, "%s", out);
  fclose(write); // Fermer write après avoir écrit

  // 5. Nettoyer
  free(out);          // Libérer la chaîne imprimée
  cJSON_Delete(json); // Libérer l'arbre JSON complet (component est un
                      // sous-élément, pas besoin de le libérer séparément)
  remove("docs.jsonopenapi");

  return EXIT_SUCCESS;
}
