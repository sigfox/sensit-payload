/*!******************************************************************
 * \file sensi_payload_v2.h
 * \brief Functions & Types to parse a Sens'it 2 payload
 * \author Sens'it Team
 *******************************************************************/

/*!******************************************************************
 * \struct payload_v2_s
 * \brief Payload v2 structure.
 *******************************************************************/
typedef struct
{

    struct
    {
        struct
        {
            u8 mode : 3;         /* 0 : BUTTON, 1 : TEMPERATURE, 2 : LIGHT, 3 : DOOR, 4 : VIBRATION, 5 : MAGNET */
            u8 uplinkPeriod : 2; /* 00 : 10 minutes, 01 : 1 hour, 10: 6 hours, 11 : 1 day */
            u8 frameType : 2;    /* 00 : PERIODIC, 01 : BUTTON, 10: ALERT, 11 : NEW_MODE */
            u8 batteryMSB : 1;   /* battery voltage : 5 bits (BL = 0-31) / Real battery : Vbatt = BL*0.05 + 2.7V -  2.7< Vbatt < 4.25 */
        };

        struct
        {
            u8 batteryLSB : 4;
            u8 temperatureMSB : 4;
        };

        union {
            struct
            {
                u8 temperatureLSB : 6; /* Temperature = ( TL(10bits)-200 ) / 8 =>  -25.0 to 102.875°C */
                u8 ils : 1;
                u8 spare : 1;
            };        /* Mode MAGNET / VIBRATION / TEMPERATURE / BUTTON */
            u8 light; /* Mode LIGHT */
        };

        union {
            u8 humidity;     /* Mode TEMPERATURE, RH = RHL * 0.5% */
            u8 alertCounter; /* Mode LIGHT / DOOR / VIBRATION / MAGNET */
            u8 version;      /* Mode BUTTON */
        };
    } data;

    struct
    {
        struct
        {
            u8 tempAlertLow : 7; //in degrees, from -20° ( 0=> -20° / 20 =>0° / 127=> 87°)
            u8 ULintervalMSB : 1;
        };

        struct
        {
            u8 tempAlertHigh : 7; //in degrees, from -20° ( 0=> -20° / 20 =>0° / 127=> 87°)
            u8 ULintervalLSB : 1;
        };

        u8 lightAlertLow;

        u8 lightAlertHigh;

        u8 accTransientThr;

        u8 accTransientCount;

        u8 accTransient;

        struct
        {
            u8 magnLvl : 7;
            u8 limited : 1;
        };
    } config;

} payload_v2_s;

/*!******************************************************************
 * \struct config_v2_s
 * \brief Configuration v2 structure
 *******************************************************************/
typedef struct
{
    bool limited;                        /*!< Message limitation must be TRUE. */
    uplink_period_e period;              /*!< Period of periodic message */
    s8 temperature_low_threshold;        /*!< Temperature Range: -20°C to +107°C / Step: 1°C */
    s8 temperature_high_threshold;       /*!< Temperature Range: -20°C to +107°C / Step: 1°C */
    u16 brightness_low_threshold;        /*!< Must be divided by 96 to get in lux */
    u16 brightness_high_threshold;       /*!< Must be divided by 96 to get in lux */
    vibration_config_e vibration_config; /*!< Mode VIBRATION sensitivity */
    door_config_e door_config;           /*!< Mode DOOR sensitivity */
} config_v2_s;

/*!************************************************************************
 * \fn void PAYLOAD_V2_parse_data(payload_v2_s payload, data_s* data)
 * \brief Function to parse Sens'it Discovery v2 payload.
 *
 * \param[in] payload               Payload to parse
 * \param[out] data                 Parsed data
 **************************************************************************/
void PAYLOAD_V2_parse_data(payload_v2_s payload, data_s *data);

/*!************************************************************************
 * \fn void PAYLOAD_V2_parse_config(u8* config_in, config_v2_s* config_out)
 * \brief Function to parse Sens'it Discovery v2 config.
 *
 * \param[in] config_in             Configuration to parse
 * \param[out] config_out           Parsed configuration
 **************************************************************************/
void PAYLOAD_V2_parse_config(payload_v2_s payload, config_s *config_out);

/*!************************************************************************
 * \fn void PAYLOAD_V2_serialize_config(config_v2_s connpm runig_in, u8* config_out)
 * \brief Function to parse Sens'it Discovery v2 config.
 *
 * \param[in] config_in            Configuration to set in the device
 * \param[out] config_out          Serialized configuration
 **************************************************************************/
void PAYLOAD_V2_serialize_config(config_s config_in, u8 *config_out);
