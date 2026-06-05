#include "libs.h"
int dictPreProcess() {
    FILE* fp = fopen("docs.jsonopenapi", "r");  // Note: j'ai corrigé le nom du fichier (jsonopenapi → json)
    FILE* write = fopen("write.json", "w");

    if (!fp || !write) {
        perror("Cannot open file");
        return EXIT_FAILURE;
    }

    // 1. Lire le fichier
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer = (char *)malloc(fileSize + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(fp);
        fclose(write);
        return EXIT_FAILURE;
    }
    fread(buffer, 1, fileSize, fp);
    buffer[fileSize] = '\0';
    fclose(fp);  // Fermer fp dès qu'on a fini de lire

    // 2. Parser le JSON
    cJSON *json = cJSON_Parse(buffer);
    free(buffer);  // Libérer buffer dès qu'on a fini de parser

    if (!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) {
            fprintf(stderr, "JSON parsing error: %s\n", error_ptr);
        }
        fclose(write);  // Fermer write avant de retourner
        cJSON_Delete(json);  // json est NULL ici, mais cJSON_Delete gère les NULL
        return EXIT_FAILURE;
    }

    // 3. Extraire "components"
    cJSON *component = cJSON_GetObjectItemCaseSensitive(json, "components");
    if (!component) {
        fprintf(stderr, "Key 'components' not found in JSON\n");
        cJSON_Delete(json);
        fclose(write);
        return EXIT_FAILURE;
    }

    // 4. Convertir "components" en chaîne JSON et l'écrire
    char *comp_out = cJSON_Print(component);
    if (!comp_out) {
        fprintf(stderr, "Failed to print JSON\n");
        cJSON_Delete(json);
        fclose(write);
        return EXIT_FAILURE;
    }

    fprintf(write, "%s", comp_out);
    fclose(write);  // Fermer write après avoir écrit

    // 5. Nettoyer
    free(comp_out);  // Libérer la chaîne imprimée
    cJSON_Delete(json);  // Libérer l'arbre JSON complet (component est un sous-élément, pas besoin de le libérer séparément)

    return EXIT_SUCCESS;
}