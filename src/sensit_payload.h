/*!******************************************************************
 * \file sensit_payload.h
 * \brief Functions & Types to parse a Sens'it payload
 * \author Sens'it Team
 *******************************************************************/

#define PARSE_ERR_NONE 0x00
#define PARSE_ERR_TYPE 0x01
#define PARSE_ERR_MODE 0x02

#define PAYLOAD_DATA_SIZE 4
#define PAYLOAD_CONFIG_SIZE 8

#define BRIGHTNESS_THRESHOLD_FACTOR 96

#define FALSE (bool)0
#define TRUE (bool)1

typedef unsigned char u8;   /*!< Unsigned 8 bits type  */
typedef unsigned short u16; /*!< Unsigned 16 bits type */
typedef unsigned long u32;  /*!< Unsigned 32 bits type */

typedef signed char s8;   /*!< Signed 8 bits type  */
typedef signed short s16; /*!< Signed 16 bits type */
typedef signed long s32;  /*!< Signed 32 bits type */

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
    MODE_STANDBY = 0b00000,
    MODE_TEMPERATURE = 0b00001,
    MODE_LIGHT = 0b00010,
    MODE_DOOR = 0b00011,
    MODE_VIBRATION = 0b00100,
    MODE_MAGNET = 0b00101,
    MODE_LAST
} mode_e;

/*!******************************************************************
 * \enum door_e
 * \brief List of mode DOOR state.
 *******************************************************************/
typedef enum {
    DOOR_NONE = 0b00,
    DOOR_MOVEMENT = 0b01,
    DOOR_CLOSE = 0b10,
    DOOR_OPEN = 0b11,
    DOOR_LAST
} door_e;

/*!******************************************************************
 * \enum data_s
 * \brief Decoded data structure
 *******************************************************************/
typedef struct
{
    u8 error;            /*!< Parsing error code */
    payload_type_e type; /*!< Payload type */
    u16 battery_level;   /*!< All modes: Value in mV */
    mode_e mode;         /*!< All modes: Active mode */
    bool button;         /*!< All modes: if TRUE, double click message. Only temperature & version are valid for a Sens'it v2. */
    s16 temperature;     /*!< Mode TEMPERATURE: Must be diveded by 8 to get in °C */
    u8 humidity;         /*!< Mode TEMPERATURE: Must be diveded by 2 to get in % */
    u16 brightness;      /*!< Mode LIGHT: Must be diveded by 96 to get in lux */
    door_e door;         /*!< Mode DOOR */
    bool vibration;      /*!< Mode VIBRATION */
    bool magnet;         /*!< Mode MAGNET: if 1, Magnet detected */
    u16 event_counter;   /*!< Mode DOOR, VIBRATION & MAGNET: Number of events since last message */
    u8 version_major;    /*!< Mode STANDBY */
    u8 version_minor;    /*!< Mode STANDBY */
    u8 version_patch;    /*!< Mode STANDBY */
} data_s;

/*!******************************************************************
 * \enum config_s
 * \brief Decoded config structure
 *******************************************************************/
typedef struct
{
    u8 error;
    bool limited;
    bool is_standby_periodic;
    bool is_temperature_periodic;
    bool is_light_periodic;
    bool is_door_periodic;
    bool is_vibration_periodic;
    bool is_magnet_periodic;
    s8 temperature_low_threshold;
    s8 temperature_high_threshold;
    u8 humidity_low_threshold;
    u8 humidity_high_threshold;
    u16 brightness_threshold;
    u16 brightness_low_threshold;
    u16 brightness_high_threshold;
    u8 delay;
    u8 vibration_config;
    u8 door_config;
    u8 period;
} config_s;

/*!******************************************************************
 * \enum uplink_period_e
 * \brief List of configurable period
 *******************************************************************/
typedef enum {
    UPLINK_PERIOD_10M = 0b00,
    UPLINK_PERIOD_1H = 0b01,
    UPLINK_PERIOD_6H = 0b10,
    UPLINK_PERIOD_24H = 0b11,
    UPLINK_PERIOD_LAST
} uplink_period_e;

/*!******************************************************************
 * \enum vibration_config_e
 * \brief List of configuration of mode vibration.
 *******************************************************************/
typedef enum {
    VIBRATION_VERY_SENSITIVE,
    VIBRATION_SENSITIVE,
    VIBRATION_STANDARD,
    VIBRATION_NOT_VERY_SENSITIVE,
    VIBRATION_VERY_LITTLE_SENSITIVE,
    VIBRATION_CONFIG_UNKNOW
} vibration_config_e;

/*!******************************************************************
 * \enum vibration_clear_delay_e
 * \brief List of configurable delay between 2 vibration trigger.
 *******************************************************************/
typedef enum {
    VIBRATION_ENABLE_END_MESSAGE = 0b00, /*!< Send a message at the end of a vibration trigger. */
    VIBRATION_CLEAR_DELAY_10S = 0b01,
    VIBRATION_CLEAR_DELAY_30S = 0b10,
    VIBRATION_CLEAR_DELAY_60S = 0b11,
    VIBRATION_CLEAR_DELAY_LAST
} vibration_clear_delay_e;

/*!******************************************************************
 * \enum door_config_e
 * \brief List of configuration of mode door.
 *******************************************************************/
typedef enum {
    DOOR_SENSITIVE,
    DOOR_STANDARD,
    DOOR_NOT_VERY_SENSITIVE,
    DOOR_CONFIG_UNKNOW
} door_config_e;

/*!************************************************************************
 * \fn void PAYLOAD_parse_data(u8* data_in, data_s* data_out)
 * \brief Function to parse Sens'it Discovery payload.
 *
 * \param[in] data_in               Payload to parse of PAYLOAD_DATA_SIZE lenght
 * \param[out] data_out             Parsed data
 **************************************************************************/
void PAYLOAD_parse_data(u8 *data_in, data_s *data_out);

/*!************************************************************************
 * \fn void PAYLOAD_parse_config(u8* data_in,payload_type_e type,config_s* config_out)
 * \brief Function to parse Sens'it Discovery config.
 *
 * \param[in] data_in               Payload to parse of PAYLOAD_DATA_SIZE lenght
 * \param[out] config_out             Parsed config
 **************************************************************************/
void PAYLOAD_parse_config(u8 *data_in, payload_type_e type, config_s *config_out);

/*!************************************************************************
 * \fn void PAYLOAD_serialize_config(config_v3_s config_in,payload_type_e type, u8* config_out)
 * \brief Function to serialize Sens'it Discovery v3 config.
 *
 * \param[in] config_in            Configuration to set in the device
 * \param[out] config_out          Serialized configuration
 **************************************************************************/
void PAYLOAD_serialize_config(config_s config_in, payload_type_e type, u8 *config_out);
