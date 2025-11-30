///
/// @file
///
/// @brief
///
///
/// @details
///

#if !defined(_NETWRK_H)
#define _NETWRK_H

//
// Shared Includes
//
#include <stdint.h>

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
// Shared Definitions
//

//
// Shared Structures / Enumerations / Type Definitions
//

struct ntwrk_info
{
    uint8_t status;
};

//
// Shared Function Prototypes
//
void
ntwrk_action(void);

void
ntwrk_cfg(void);

uint8_t
ntwrk_get_wifi_status(void);

//
// Shared Global Variables
//

END_C_DECLS

#endif
