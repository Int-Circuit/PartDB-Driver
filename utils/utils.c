#include "libs.h"
int utilsOps(utilOpsType_t type, fileLoadOpts_t* options, fileWriteVal_t* values)
{
    switch (type) {
        default:
            fprintf(stderr, "Error: Operation type %d is not valid", type);
            return EXIT_FAILURE;
        case loadFile:
            loader(CONF_FILE, options);
            break;
        case writeFile:
            writer(CONF_FILE, values);
            break;
        case createDict:
            dictPreProcess();
            break;    
    }
    return EXIT_SUCCESS;
}