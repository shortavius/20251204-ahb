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
#include "cmd_ntwrk.h"
#include "../../cpe/cpe.h"
#include "../../ntwrk/ntwrk.h"

#include <WiFi.h>

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
#define NTWRK_NUM_COMMANDS                  3

//
// Local Structures / Enumerations / Type Definitions
//

//
// Local Function Prototypes
//
static void cmd_ntwrk_help(void * x);
static void cmd_ntwrk_scan(void * x);
static void cmd_ntwrk_status(void * x);

//
// Local Global Variables
//
const static struct cpe_syntax_tkn syntax_ntwrk_help_tkns[2] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_NTWRK
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HELP
    }
};

const static struct cpe_syntax_tkn syntax_ntwrk_scan_tkns[2] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_NTWRK
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SCAN
    }
};

const static struct cpe_syntax_tkn syntax_ntwrk_status_tkns[2] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_NTWRK
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_STATUS
    }
};

const struct cpe_cmd_syntax syntax_cmd_ntwrk[NTWRK_NUM_COMMANDS] =
{
    {
        .count = 2,
        .syntax_tkns = &syntax_ntwrk_help_tkns[0],
        .action = cmd_ntwrk_help
    },
    {
        .count = 2,
        .syntax_tkns = &syntax_ntwrk_scan_tkns[0],
        .action = cmd_ntwrk_scan
    },
    {
        .count = 2,
        .syntax_tkns = &syntax_ntwrk_status_tkns[0],
        .action = cmd_ntwrk_status
    }
};

//
// Code
//
void cmd_ntwrk_root_help(void)
{
    struct uart_funcs * uart = cpe_get_info()->uart;

    console_printf(uart, "      ntwrk - Issue commands to the network module");
    console_printf(uart, "\r\n");
}

uint8_t cmd_ntwrk_syntax(void)
{
    int idx = (int)0;
    int idy = (int)0;
    uint8_t rv = (uint8_t)0;
    uint8_t match = (uint8_t)1;
    struct cpe_info * info = cpe_get_info();
    void (*action)(void * x) = NULL;

    while ((idx < (int)NTWRK_NUM_COMMANDS) && !rv)
    {
        if (syntax_cmd_ntwrk[idx].count != info->token_cnt)
        {
            idx++;
            continue;
        }

        idy = (int)0;
        match = (uint8_t)1;
        while ((idy < syntax_cmd_ntwrk[idx].count) && match)
        {
            struct cpe_syntax_tkn tmp = {
                .cat = info->token[idy].cat,
                .kyw = info->token[idy].kyw
            };

            if (!(cpe_cmp_syntax_tkns(syntax_cmd_ntwrk[idx].syntax_tkns[idy], tmp)))
            {
                match = 0;
            }

            idy++;
        }

        if (match && (syntax_cmd_ntwrk[idx].count == idy))
        {
            rv = 1;
            action = syntax_cmd_ntwrk[idx].action;
        }

        idx++;
    }

    if (NULL != action)
    {
        action((void *)NULL);
    }

    return rv;
}

static void cmd_ntwrk_help(void * x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    console_printf(uart,
        "ntwrk command actions\r\n\r\n");
    console_printf(uart,
        "status - Retrieve the current network status\r\n");
}

static void cmd_ntwrk_scan(void * x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    uint8_t num_ssid = WiFi.scanNetworks();
    uint8_t idx;

    console_printf(uart,
        "WiFi networksFound: %d\r\n", num_ssid);

    for (idx = 0; idx < num_ssid; idx++)
    {
        console_printf(uart, "\r\n%d) %s\r\nSignal %d dBm\tEncryption: ",
            idx+1u, WiFi.SSID(idx), WiFi.RSSI(idx));

        switch(WiFi.encryptionType(idx))
        {
            case ENC_TYPE_WEP:
                console_printf(uart, "WEP");
                break;
            case ENC_TYPE_TKIP:
                console_printf(uart, "WPA");
                break;
            case ENC_TYPE_CCMP:
                console_printf(uart, "WPA2");
                break;
            case ENC_TYPE_NONE:
                console_printf(uart, "None");
                break;
            case ENC_TYPE_AUTO:
                console_printf(uart, "Auto");
                break;
            case ENC_TYPE_UNKNOWN:
            default:
                console_printf(uart, "Unknown");
                break;
        }
        console_printf(uart, "\r\n");
    }
}

static void cmd_ntwrk_status(void * x)
{
    uint8_t status = ntwrk_get_wifi_status();
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    switch (status)
    {
        case WL_NO_SHIELD:
            console_printf(uart,
                "No WiFi shield has been detected\r\n");
            break;
        case WL_IDLE_STATUS:
            console_printf(uart,
                "WiFi is idle\r\n");
            break;
        case WL_NO_SSID_AVAIL:
            console_printf(uart,
                "No network SSID are available\r\n");
            break;
        case WL_SCAN_COMPLETED:
            console_printf(uart,
                "Scan of WiFi networks has completed\r\n");
            break;
        case WL_CONNECTED:
            console_printf(uart,
                "Connected to a WiFi network\r\n");
            break;
        case WL_CONNECT_FAILED:
            console_printf(uart,
                "All attempts to connect to a network failed\r\n");
            break;
        case WL_CONNECTION_LOST:
            console_printf(uart,
                "WiFi connection has been lost\r\n");
            break;
        case WL_DISCONNECTED:
            console_printf(uart,
                "Wifi has disconnected from a network\r\n");
            break;
        case WL_AP_LISTENING:
            console_printf(uart,
                "WiFi is listening for connections\r\n");
            break;
        case WL_AP_CONNECTED:
            console_printf(uart,
                "A device connected to our WiFi\r\n");
            break;
        default:
            console_printf(uart,
                "WL_AP_FAILED\r\n");
            break;
    }
}

END_C_DECLS
