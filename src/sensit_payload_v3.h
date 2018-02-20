/*!******************************************************************
 * \file sensi_payload_v3.h
 * \brief Functions & Types to parse a Sens'it 3 payload
 * \author Sens'it Team
 *******************************************************************/

/*!******************************************************************
 * \struct payload_v3_s
 * \brief Payload v3 structure.
 *******************************************************************/
typedef struct
{

    struct
    {
        struct
        {
            u8 reserved : 3; /* Must be 0b110 */
            u8 battery : 5;  /* Battery level */
        };

        struct
        {
            u8 special_value : 2; /* Mode TEMPERATURE: temperature MSB */
                                  /* Mode DOOR: door state */
                                  /* Mode VIBRATION: 01 -> vibration detected */
                                  /* Mode MAGNET: 01 -> magnet detected */
            u8 button : 1;        /* If TRUE, double click message */
            u8 mode : 5;
        };

        union {
            struct
            {
                u8 fw_minorMSB : 4;
                u8 fw_major : 4;
            };                   /* Button message */
            u8 temperatureLSB;   /* TEMPERATURE message */
            u8 brightnessMSB;    /* LIGHT message */
            u8 event_counterMSB; /* DOOR, VIBRATION & MAGNET message */
        };

        union {
            struct
            {
                u8 fw_patch : 6;
                u8 fw_minorLSB : 2;
            };                   /* Button message */
            u8 humidity;         /* TEMPERATURE message */
            u8 brightnessLSB;    /* LIGHT message */
            u8 event_counterLSB; /* DOOR, VIBRATION & MAGNET message */
        };
    } data;

    struct
    {
        struct
        {
            u8 periodic_standby : 1;
            u8 periodic_temperature : 1;
            u8 periodic_light : 1;
            u8 periodic_door : 1;
            u8 periodic_vibration : 1;
            u8 periodic_magnet : 1;
            u8 uplink_period : 2;
        };

        struct
        {
            u8 temperature_low_threshold : 6;
            u8 spare0 : 2;
        };

        struct
        {
            u8 temperature_high_threshold : 6;
            u8 spare1 : 2;
        };

        struct
        {
            u8 humidity_high_threshold : 4;
            u8 humidity_low_threshold : 4;
        };

        struct
        {
            u8 brightness_threshold : 7;
            u8 limited : 1; /* Must be 1 */
        };

        u8 vibration_threshold;

        struct
        {
            u8 vibration_debounce_counter : 4;
            u8 vibration_delay : 2; /* Delay between 2 vibration trigger. If 0, en of vibration msg enable */
            u8 spare2 : 2;
        };

        struct
        {
            u8 door_close_threshold : 3;
            u8 door_open_threshold : 4;
            u8 reserved : 1; /* Must be 0 */
        };
    } config;

} payload_v3_s;

/*!******************************************************************
 * \enum config_s
 * \brief Sens'it configuration values
 *******************************************************************/
typedef struct
{
    bool limited;                   /*!< Message limitation must be TRUE. */
    uplink_period_e period;         /*!< Period of periodic message configuration */
    bool mode_standby_periodic_msg; /*!< TRUE to enable periodic message on yhr mode */
    bool mode_temperature_periodic_msg;
    bool mode_light_periodic_msg;
    bool mode_door_periodic_msg;
    bool mode_vibration_periodic_msg;
    bool mode_magnet_periodic_msg;
    s8 temperature_low_threshold;        /*!< Temperature Range: -9°C to +54°C / Step: 1°C */
    s8 temperature_high_threshold;       /*!< Temperature Range: -9°C to +54°C / Step: 1°C */
    u8 humidity_low_threshold;           /*!< Humidty Range: 30% to 90% / Step: 4% */
    u8 humidity_high_threshold;          /*!< Humidty Range: 30% to 90% / Step: 4% */
    u16 brightness_threshold;            /*!< Range 1 to 636 lux / Step 5 lux */
    vibration_config_e vibration_config; /*!< Mode VIBRATION sensitivity */
    vibration_clear_delay_e delay;       /*!< Delay between 2 vibration trigger. */
    door_config_e door_config;           /*!< Mode DOOR sensitivity */
} config_v3_s;

/*!************************************************************************
 * \fn void PAYLOAD_V3_parse_data(payload_v3_s payload, data_s* data)
 * \brief Function to parse Sens'it Discovery v3 payload.
 *
 * \param[in] payload               Payload to parse
 * \param[out] data                 Parsed data
 **************************************************************************/
void PAYLOAD_V3_parse_data(payload_v3_s payload, data_s *data);

/*!************************************************************************
 * \fn void PAYLOAD_V3_parse_config(u8* config_in, config_v3_s* config_out)
 * \brief Function to parse Sens'it Discovery v3 config.
 *
 * \param[in] config_in             Configuration to parse
 * \param[out] config_out           Parsed configuration
 **************************************************************************/
void PAYLOAD_V3_parse_config(payload_v3_s payload, config_s *config_out);

/*!************************************************************************
 * \fn void PAYLOAD_V3_serialize_config(config_v3_s config_in, u8* config_out)
 * \brief Function to serialize Sens'it Discovery v3 config.
 *
 * \param[in] config_in            Configuration to set in the device
 * \param[out] config_out          Serialized configuration
 **************************************************************************/
void PAYLOAD_V3_serialize_config(config_s config_in, u8 *config_out);
