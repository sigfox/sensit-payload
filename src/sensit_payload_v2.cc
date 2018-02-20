/*!******************************************************************
 * \file sensit_payload_v2.c
 * \brief Functions to parse a Sens'it 2 payload
 * \author Sens'it Team
 *******************************************************************/
/******* INCLUDES **************************************************/
#include <stdio.h>
#include <string.h>
#include "sensit_payload.h"
#include "sensit_payload_v2.h"

/******* DEFINE ****************************************************/
#define BATTERY_OFFSET 2700
#define BATTERY_STEP 50

#define TEMPERATURE_OFFSET -200

#define FRAME_TYPE_PERIODIC 0b00
#define FRAME_TYPE_BUTTON 0b01
#define FRAME_TYPE_ALERT 0b10
#define FRAME_TYPE_NEW_MODE 0b11

#define TEMPERATURE_THRESHOLD_OFFSET -20
#define TEMPERATURE_THRESHOLD_STEP 1

#define VIBRATION_TRANSIENT_SETTINGS 0x73

#define VIBRATION_VERY_SENSITIVE_THRESHOLD 0x01
#define VIBRATION_VERY_SENSITIVE_DEBOUNCE_COUNTER 0x1

#define VIBRATION_SENSITIVE_THRESHOLD 0x03
#define VIBRATION_SENSITIVE_DEBOUNCE_COUNTER 0x1

#define VIBRATION_STANDARD_THRESHOLD 0x04
#define VIBRATION_STANDARD_DEBOUNCE_COUNTER 0x2

#define VIBRATION_NOT_VERY_SENSITIVE_THRESHOLD 0x08
#define VIBRATION_NOT_VERY_SENSITIVE_DEBOUNCE_COUNTER 0x2

#define VIBRATION_VERY_LITTLE_SENSITIVE_THRESHOLD 0x10
#define VIBRATION_VERY_LITTLE_SENSITIVE_DEBOUNCE_COUNTER 0x3

#define DOOR_THRESHOLD_SENSITIVE 16
#define DOOR_THRESHOLD_STANDARD 32
#define DOOR_THRESHOLD_NOT_VERY_SENSITIVE 50

/*******************************************************************/

void PAYLOAD_V2_parse_data(payload_v2_s payload, data_s *data)
{
    data->mode = (mode_e)payload.data.mode;
    data->battery_level = (((payload.data.batteryMSB << 4) | payload.data.batteryLSB) * BATTERY_STEP) + BATTERY_OFFSET;
    data->button = FALSE;

    if (payload.data.frameType == FRAME_TYPE_BUTTON)
    {
        data->button = TRUE;
        data->temperature = ((payload.data.temperatureMSB << 6) | payload.data.temperatureLSB) + TEMPERATURE_OFFSET;
        data->version_major = 0x0F & (payload.data.version >> 4);
        data->version_minor = 0x0F & payload.data.version;
        data->version_patch = 0;
    }
    else if (payload.data.mode == MODE_STANDBY)
    {
        data->version_major = 0x0F & (payload.data.version >> 4);
        data->version_minor = 0x0F & payload.data.version;
        data->version_patch = 0;
    }
    else if (payload.data.mode == MODE_TEMPERATURE)
    {
        data->temperature = ((payload.data.temperatureMSB << 6) | payload.data.temperatureLSB) + TEMPERATURE_OFFSET;
        data->humidity = payload.data.humidity;
    }
    else if (payload.data.mode == MODE_LIGHT)
    {
        if ((payload.data.light >> 6) == 0b11)
        {
            data->brightness = (payload.data.light & 0x3F) * 1024;
        }
        else if ((payload.data.light >> 6) == 0b10)
        {
            data->brightness = (payload.data.light & 0x3F) * 64;
        }
        else if ((payload.data.light >> 6) == 0b01)
        {
            data->brightness = (payload.data.light & 0x3F) * 8;
        }
        else
        {
            data->brightness = payload.data.light;
        }
    }
    else if (payload.data.mode == MODE_DOOR)
    {
        if (payload.data.frameType == FRAME_TYPE_ALERT)
        {
            data->door = DOOR_MOVEMENT;
        }
        else
        {
            data->door = DOOR_NONE;
        }

        data->event_counter = payload.data.alertCounter;
    }
    else if (payload.data.mode == MODE_VIBRATION)
    {
        if (payload.data.frameType == FRAME_TYPE_ALERT)
        {
            data->vibration = TRUE;
        }
        else
        {
            data->vibration = FALSE;
        }

        data->event_counter = payload.data.alertCounter;
    }
    else if (payload.data.mode == MODE_MAGNET)
    {
        data->magnet = payload.data.ils;
        data->event_counter = payload.data.alertCounter;
    }
    else
    {
        data->error = PARSE_ERR_MODE;
    }
}

/*******************************************************************/

void PAYLOAD_V2_parse_config(payload_v2_s payload, config_s *config_out)
{
    config_out->limited = (bool)(payload.config.limited);

    config_out->period = (uplink_period_e)((payload.config.ULintervalMSB << 1) | payload.config.ULintervalLSB);

    /* Parsing temperature threshold */
    config_out->temperature_low_threshold = (s8)(TEMPERATURE_THRESHOLD_OFFSET + (payload.config.tempAlertLow * TEMPERATURE_THRESHOLD_STEP));
    config_out->temperature_high_threshold = (s8)(TEMPERATURE_THRESHOLD_OFFSET + (payload.config.tempAlertHigh * TEMPERATURE_THRESHOLD_STEP));

    /* Parsing light low threshold */
    if ((payload.config.lightAlertLow >> 6) == 0b11)
    {
        config_out->brightness_low_threshold = (payload.config.lightAlertLow & 0x3F) * 1024;
    }
    else if ((payload.config.lightAlertLow >> 6) == 0b10)
    {
        config_out->brightness_low_threshold = (payload.config.lightAlertLow & 0x3F) * 64;
    }
    else if ((payload.config.lightAlertLow >> 6) == 0b01)
    {
        config_out->brightness_low_threshold = (payload.config.lightAlertLow & 0x3F) * 8;
    }
    else
    {
        config_out->brightness_low_threshold = payload.config.lightAlertLow;
    }

    /* Parsing light high threshold */
    if ((payload.config.lightAlertHigh >> 6) == 0b11)
    {
        config_out->brightness_high_threshold = (payload.config.lightAlertHigh & 0x3F) * 1024;
    }
    else if ((payload.config.lightAlertHigh >> 6) == 0b10)
    {
        config_out->brightness_high_threshold = (payload.config.lightAlertHigh & 0x3F) * 64;
    }
    else if ((payload.config.lightAlertHigh >> 6) == 0b01)
    {
        config_out->brightness_high_threshold = (payload.config.lightAlertHigh & 0x3F) * 8;
    }
    else
    {
        config_out->brightness_high_threshold = payload.config.lightAlertHigh;
    }

    /* Parsing mode VIBRATION sensitivity */
    if ((payload.config.accTransientThr == VIBRATION_VERY_SENSITIVE_THRESHOLD) && (payload.config.accTransientCount == VIBRATION_VERY_SENSITIVE_DEBOUNCE_COUNTER))
    {
        config_out->vibration_config = VIBRATION_VERY_SENSITIVE;
    }
    else if ((payload.config.accTransientThr == VIBRATION_SENSITIVE_THRESHOLD) && (payload.config.accTransientCount == VIBRATION_SENSITIVE_DEBOUNCE_COUNTER))
    {
        config_out->vibration_config = VIBRATION_SENSITIVE;
    }
    else if ((payload.config.accTransientThr == VIBRATION_STANDARD_THRESHOLD) && (payload.config.accTransientCount == VIBRATION_STANDARD_DEBOUNCE_COUNTER))
    {
        config_out->vibration_config = VIBRATION_STANDARD;
    }
    else if ((payload.config.accTransientThr == VIBRATION_NOT_VERY_SENSITIVE_THRESHOLD) && (payload.config.accTransientCount == VIBRATION_NOT_VERY_SENSITIVE_DEBOUNCE_COUNTER))
    {
        config_out->vibration_config = VIBRATION_NOT_VERY_SENSITIVE;
    }
    else if ((payload.config.accTransientThr == VIBRATION_VERY_LITTLE_SENSITIVE_THRESHOLD) && (payload.config.accTransientCount == VIBRATION_VERY_LITTLE_SENSITIVE_DEBOUNCE_COUNTER))
    {
        config_out->vibration_config = VIBRATION_VERY_LITTLE_SENSITIVE;
    }
    else
    {
        config_out->vibration_config = VIBRATION_CONFIG_UNKNOW;
    }

    /* Parsing mode DOOR sensitivity */
    if (payload.config.magnLvl == DOOR_THRESHOLD_SENSITIVE)
    {
        config_out->door_config = DOOR_SENSITIVE;
    }
    else if (payload.config.magnLvl == DOOR_THRESHOLD_STANDARD)
    {
        config_out->door_config = DOOR_STANDARD;
    }
    else if (payload.config.magnLvl == DOOR_THRESHOLD_NOT_VERY_SENSITIVE)
    {
        config_out->door_config = DOOR_NOT_VERY_SENSITIVE;
    }
    else
    {
        config_out->door_config = DOOR_CONFIG_UNKNOW;
    }
}

/*******************************************************************/

void PAYLOAD_V2_serialize_config(config_s config_in, u8 *config_out)
{
    payload_v2_s payload;

    payload.config.limited = config_in.limited;

    payload.config.ULintervalMSB = (config_in.period >> 1) & 0x01;
    payload.config.ULintervalLSB = config_in.period & 0x01;

    /* Serializing temperature threshold */
    payload.config.tempAlertLow = (config_in.temperature_low_threshold - TEMPERATURE_THRESHOLD_OFFSET) / TEMPERATURE_THRESHOLD_STEP;
    payload.config.tempAlertHigh = (config_in.temperature_high_threshold - TEMPERATURE_THRESHOLD_OFFSET) / TEMPERATURE_THRESHOLD_STEP;

    /* Serializing light low threshold */
    if (config_in.brightness_low_threshold & 0xF000)
    {
        payload.config.lightAlertLow = config_in.brightness_low_threshold >> 10;
        payload.config.lightAlertLow |= 0xC0;
    }
    else if (config_in.brightness_low_threshold & 0x0E00)
    {
        payload.config.lightAlertLow = config_in.brightness_low_threshold >> 6;
        payload.config.lightAlertLow |= 0x80;
    }
    else if (config_in.brightness_low_threshold & 0x01C0)
    {
        payload.config.lightAlertLow = config_in.brightness_low_threshold >> 3;
        payload.config.lightAlertLow |= 0x40;
    }
    else
    {
        payload.config.lightAlertLow = config_in.brightness_low_threshold;
    }

    /* Serializing light high threshold */
    if (config_in.brightness_high_threshold & 0xF000)
    {
        payload.config.lightAlertHigh = config_in.brightness_high_threshold >> 10;
        payload.config.lightAlertHigh |= 0xC0;
    }
    else if (config_in.brightness_high_threshold & 0x0E00)
    {
        payload.config.lightAlertHigh = config_in.brightness_high_threshold >> 6;
        payload.config.lightAlertHigh |= 0x80;
    }
    else if (config_in.brightness_high_threshold & 0x01C0)
    {
        payload.config.lightAlertHigh = config_in.brightness_high_threshold >> 3;
        payload.config.lightAlertHigh |= 0x40;
    }
    else
    {
        payload.config.lightAlertHigh = config_in.brightness_high_threshold;
    }

    /* Serializing mode VIBRATION config */
    payload.config.accTransient = VIBRATION_TRANSIENT_SETTINGS;

    if (config_in.vibration_config == VIBRATION_VERY_SENSITIVE)
    {
        payload.config.accTransientThr = VIBRATION_VERY_SENSITIVE_THRESHOLD;
        payload.config.accTransientCount = VIBRATION_VERY_SENSITIVE_DEBOUNCE_COUNTER;
    }
    else if (config_in.vibration_config == VIBRATION_SENSITIVE)
    {
        payload.config.accTransientThr = VIBRATION_SENSITIVE_THRESHOLD;
        payload.config.accTransientCount = VIBRATION_SENSITIVE_DEBOUNCE_COUNTER;
    }
    else if (config_in.vibration_config == VIBRATION_STANDARD)
    {
        payload.config.accTransientThr = VIBRATION_STANDARD_THRESHOLD;
        payload.config.accTransientCount = VIBRATION_STANDARD_DEBOUNCE_COUNTER;
    }
    else if (config_in.vibration_config == VIBRATION_NOT_VERY_SENSITIVE)
    {
        payload.config.accTransientThr = VIBRATION_NOT_VERY_SENSITIVE_THRESHOLD;
        payload.config.accTransientCount = VIBRATION_NOT_VERY_SENSITIVE_DEBOUNCE_COUNTER;
    }
    else if (config_in.vibration_config == VIBRATION_VERY_LITTLE_SENSITIVE)
    {
        payload.config.accTransientThr = VIBRATION_VERY_LITTLE_SENSITIVE_THRESHOLD;
        payload.config.accTransientCount = VIBRATION_VERY_LITTLE_SENSITIVE_DEBOUNCE_COUNTER;
    }

    /* Serializing mode DOOR config */
    if (config_in.door_config == DOOR_SENSITIVE)
    {
        payload.config.magnLvl = DOOR_THRESHOLD_SENSITIVE;
    }
    else if (config_in.door_config == DOOR_STANDARD)
    {
        payload.config.magnLvl = DOOR_THRESHOLD_STANDARD;
    }
    else if (config_in.door_config == DOOR_NOT_VERY_SENSITIVE)
    {
        payload.config.magnLvl = DOOR_THRESHOLD_NOT_VERY_SENSITIVE;
    }

    memcpy(config_out, &(payload.config), PAYLOAD_CONFIG_SIZE);
}

/*******************************************************************/
