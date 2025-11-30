///
/// @file
///
/// @brief
///
///
/// @details
///

//
// Local Includes
//
#include "ntwrk.h"

#include <WiFiNINA.h>

#undef BEGIN_C_DECLS
#undef END_C_DECLS
#if defined(__cplusplus)
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS }
#else
#  define BEGIN_C_DECLS /* empty */
#  define END_C_DECLS /* empty */
#endif

BEGIN_C_DECLS

//
// Local Definitions
//

//
// Local Structures / Enumerations / Type Definitions
//

//
// Local Function Prototypes
//

//
// Local Global Variables
//
struct ntwrk_info nw_info = {
    .status = WL_NO_MODULE
};

//
// Code
//
void
ntwrk_action(void)
{
    nw_info.status = WiFi.status();
}

void
ntwrk_cfg(void)
{
    nw_info.status = WiFi.status();
}

uint8_t
ntwrk_get_wifi_status(void)
{
    return nw_info.status;
}



END_C_DECLS
