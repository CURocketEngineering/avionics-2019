/*
 * Write here your custom declarations.
 *
 * They will be included automatically in
 * the project.
 *
 */
 
/* Additional XBee settings for SmartThings compatibility */
#define XBEE_PARAM_ZS       2
#define XBEE_PARAM_NJ       0x5A
#define XBEE_PARAM_NH       0x1E
#define XBEE_PARAM_NO       3
#define XBEE_PARAM_AP       1
#define XBEE_PARAM_EE       1
#define XBEE_PARAM_EO       1
#define XBEE_PARAM_KY       "5A6967426565416C6C69616E63653039"

/* Reported manufacturer and model in basic cluster
 *
 * For SmartThings local execution without custom device handler, set the following:
 * - ZCL_MANUFACTURER "Leviton"
 * - ZCL_MODEL        "ZSS-10"
 */
#define ZCL_MANUFACTURER    "CURE"
#define ZCL_MODEL           "Bypass"

/* Ignore On command received via Broadcast message */
#define PXBEE_TRIGGER_IGNORE_BROADCAST

/* Settings END */
