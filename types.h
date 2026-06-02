#include <stdbool.h>
enum utilOpsType {load = 0};
typedef enum utilOpsType utilOpsType_t;
enum order { asc = 0, desc = 1 };
typedef enum order order_t;
struct fileLoadOpts
{
    char* url;
    char* token;
    char* certLocation;
    int SSLOn;
    char* dictLocation;
    int dictDwl;
};
typedef struct fileLoadOpts fileLoadOpts_t ;
enum requestType { GET = 0, POST = 1, GET_id = 2, DELETE = 3, PATCH = 4 };
typedef enum requestType requestType_t;
struct request {

  int page;
  int itemsPerPage;
  bool favorite;
  bool needsReview;
  order_t orderName;
  order_t orderId;
  order_t orderAddedDate;
  order_t orderLastModified;
  char* id;
  char *type;
  char *properties;
  char *category;
  char *footprint;
  char *manufacturer;
  char *partUnit;
  char *partCustomState;
  char *storageLocation;
  char *name;
  char *comment;
  char *description;
  char *ipn;
  char *manufacturerProductNumber;
  char *tags;
};
typedef struct request request_t;

