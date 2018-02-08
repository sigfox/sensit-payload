/*!******************************************************************
 * \file payload.h
 * \brief Functions & Types to parse a Sens'it payload
 * \author Sens'it Team
 * \copyright Copyright (c) 2018 Sigfox, All Rights Reserved.
 *******************************************************************/


#define PARSE_ERR_NONE          0x00
#define PARSE_ERR_TYPE          0x01
#define PARSE_ERR_MODE          0x02


#define DATA_SIZE               4
#define CONFIG_SIZE             8


typedef unsigned char       u8;  /*!< Unsigned 8 bits type  */
typedef unsigned short      u16; /*!< Unsigned 16 bits type */
typedef unsigned long       u32; /*!< Unsigned 32 bits type */

typedef signed char         s8;  /*!< Signed 8 bits type  */
typedef signed short        s16; /*!< Signed 16 bits type */
typedef signed long         s32; /*!< Signed 32 bits type */

/*!******************************************************************
 * \struct payload_s
 * \brief Payload structure.
 *******************************************************************/
typedef struct {

    struct {
        u8 reserved:3;  /* Must be 0b110 */
        u8 battery:5;	/* Battery level */
    };

    struct {
        u8 spare:2;  /* Mode TEMPERATURE: temperature MSB */
                     /* Mode DOOR: door event */
                     /* Mode VIBRATION: vibration event */
                     /* Mode MAGNET: reed switch state */
        u8 button:1; /* TRUE if double click */
        u8 mode:5;
    };

	union {
        struct {
            u8 fw_minorMSB:4;
            u8 fw_major:4;
        };                    /* Button message */
		u8 temperatureLSB;    /* TEMPERATURE message */
		u8 brightnessMSB;     /* LIGHT message */
		u8 event_counterMSB;  /* DOOR, VIBRATION & MAGNET message */
	};

	union {
        struct {
            u8 fw_patch:6;
            u8 fw_minorLSB:2;
        };                    /* Button message */
		u8 humidity;          /* TEMPERATURE message */
		u8 brightnessLSB;     /* LIGHT message */
		u8 event_counterLSB;  /* DOOR, VIBRATION & MAGNET message */
	};

} payload_s;


/*!******************************************************************
 * \enum payload_type_e
 * \brief Type of payload
 *******************************************************************/
typedef enum {
    PAYLOAD_V2 = 2,
    PAYLOAD_V3 = 3,
    PAYLOAD_LAST
} payload_type_e;


/*!******************************************************************
 * \enum mode_e
 * \brief List of Discovery Firmware mode.
 *******************************************************************/
typedef enum {
    MODE_STANDBY     = 0b00000,
    MODE_TEMPERATURE = 0b00001,
    MODE_LIGHT       = 0b00010,
    MODE_DOOR        = 0b00011,
    MODE_VIBRATION   = 0b00100,
    MODE_MAGNET      = 0b00101,
    MODE_LAST
} mode_e;


/*!******************************************************************
 * \enum door_e
 * \brief List of mode DOOR events.
 *******************************************************************/
typedef enum {
    DOOR_NONE    = 0b00,
    DOOR_OPENING = 0b01,
    DOOR_CLOSING = 0b10,
    DOOR_LAST
} door_e;


/*!******************************************************************
 * \enum vibration_e
 * \brief List of mode VIBRATION events.
 *******************************************************************/
typedef enum {
    VIBRATION_NONE    = 0b00,
    VIBRATION_START   = 0b01,
    VIBRATION_STOP    = 0b10,
    VIBRATION_ONGOING = 0b11,
    VIBRATION_LAST
} vibration_e;



typedef struct {
    u8 error;              /* Parsing error code */
    payload_type_e type;   /* Payload type */
    u16 battery_level;     /* All modes: Value in mV */
    mode_e mode;           /* All modes: Active mode */
    bool button;           /* All modes: if 1, Double click message */
    s16 temperature;       /* Mode TEMPERATURE: Must be diveded by 8 to get in °C */
    u8 humidity;           /* Mode TEMPERATURE: Must be diveded by 2 to get in % */
    u16 brightness;        /* Mode LIGHT: Must be diveded by 96 to get in lux */
    door_e door;           /* Mode DOOR */
    vibration_e vibration; /* Mode VIBRATION */
    bool magnet;           /* Mode MAGNET: if 1, Magnet detected */
    u16 event_counter;     /* Mode DOOR, VIBRATION & MAGNET: Number of events since last message */
    u8 version_major;      /* Mode STANDBY */
    u8 version_minor;      /* Mode STANDBY */
    u8 version_patch;      /* Mode STANDBY */
} data_s;


/*!************************************************************************
 * \fn void PAYLOAD_build(payload_s payload, data_s data)
 * \brief Function to parse Sens'it Discovery payload.
 *
 * \param[in] data_in               Payload to parse of DATA_SIZE lenght
 * \param[out] data_out             Parsed data
 **************************************************************************/
void PAYLOAD_parse_data(u8* data_in, data_s* data_out);
