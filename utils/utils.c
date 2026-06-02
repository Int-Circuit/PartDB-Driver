#include "libs.h"
int utilsOps(utilOpsType_t type, fileLoadOpts_t* options)
{
    switch (type) {
        default:
            fprintf(stderr, "Error: Operation type %d is not valid", type);
            return EXIT_FAILURE;
        case load:
            loader(CONF_FILE, options);
    }
    return EXIT_SUCCESS;
}