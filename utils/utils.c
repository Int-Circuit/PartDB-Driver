#include "libs.h"
int utilsOps(utilOpsType_t type, fileLoadOpts_t* options, fileWriteVal_t* values)
{
    switch (type) {
        default:
            fprintf(stderr, "Error: Operation type %d is not valid", type);
            return EXIT_FAILURE;
        case load_file:
            loader(CONF_FILE, options);
            break;
        case write_file:
            writer(CONF_FILE, values);
            break;
    }
    return EXIT_SUCCESS;
}