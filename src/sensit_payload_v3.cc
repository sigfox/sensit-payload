/*!******************************************************************
 * \file sensit_payload_v3.c
 * \brief Functions to parse a Sens'it 3 payload
 * \author Sens'it Team
 *******************************************************************/
/******* INCLUDES **************************************************/
#include <stdio.h>
#include <string.h>
#include "sensit_payload.h"
#include "sensit_payload_v3.h"

/******* DEFINE ****************************************************/
#define BATTERY_OFFSET 2700
#define BATTERY_STEP 50

#define TEMPERATURE_OFFSET -200

#define TEMPERATURE_THRESHOLD_OFFSET -9
#define TEMPERATURE_THRESHOLD_STEP 1

#define HUMIDITY_THRESHOLD_OFFSET 30
#define HUMIDITY_THRESHOLD_STEP 4

#define BRIGHTNESS_THRESHOLD_OFFSET 1
#define BRIGHTNESS_THRESHOLD_STEP 5

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

#define DOOR_CLOSE_THRESHOLD_SENSITIVE 4
#define DOOR_OPEN_THRESHOLD_SENSITIVE 2

#define DOOR_CLOSE_THRESHOLD_STANDARD 4
#define DOOR_OPEN_THRESHOLD_STANDARD 7

#define DOOR_CLOSE_THRESHOLD_NOT_VERY_SENSITIVE 4
#define DOOR_OPEN_THRESHOLD_NOT_VERY_SENSITIVE 12

/*******************************************************************/

void PAYLOAD_V3_parse_data(payload_v3_s payload, data_s *data)
{
    data->mode = (mode_e)payload.data.mode;
    data->button = payload.data.button;
    data->battery_level = (payload.data.battery * BATTERY_STEP) + BATTERY_OFFSET;

    if (payload.data.mode == MODE_STANDBY)
    {
        data->version_major = payload.data.fw_major;
        data->version_minor = (payload.data.fw_minorMSB << 4) | payload.data.fw_minorLSB;
        data->version_patch = payload.data.fw_patch;
    }
    else if (payload.data.mode == MODE_TEMPERATURE)
    {
        data->temperature = ((payload.data.special_value << 8) | payload.data.temperatureLSB) + TEMPERATURE_OFFSET;
        data->humidity = payload.data.humidity;
    }
    else if (payload.data.mode == MODE_LIGHT)
    {
        data->brightness = (payload.data.brightnessMSB << 8) | payload.data.brightnessLSB;
    }
    else if (payload.data.mode == MODE_DOOR)
    {
        data->door = (door_e)payload.data.special_value;
        data->event_counter = (payload.data.event_counterMSB << 8) | payload.data.event_counterLSB;
    }
    else if (payload.data.mode == MODE_VIBRATION)
    {
        data->vibration = payload.data.special_value;
        data->event_counter = (payload.data.event_counterMSB << 8) | payload.data.event_counterLSB;
    }
    else if (payload.data.mode == MODE_MAGNET)
    {
        data->magnet = payload.data.special_value;
        data->event_counter = (payload.data.event_counterMSB << 8) | payload.data.event_counterLSB;
    }
    else
    {
        data->error = PARSE_ERR_MODE;
    }
}

/*******************************************************************/

void PAYLOAD_V3_parse_config(payload_v3_s payload, config_s *config_out)
{
    config_out->limited = (bool)(payload.config.limited);

    config_out->period = (uplink_period_e)(payload.config.uplink_period);

    config_out->is_standby_periodic = (bool)(payload.config.periodic_standby);
    config_out->is_temperature_periodic = (bool)(payload.config.periodic_temperature);
    config_out->is_light_periodic = (bool)(payload.config.periodic_light);
    config_out->is_door_periodic = (bool)(payload.config.periodic_door);
    config_out->is_vibration_periodic = (bool)(payload.config.periodic_vibration);
    config_out->is_magnet_periodic = (bool)(payload.config.periodic_magnet);

    /* Parsing temperature threshold */
    config_out->temperature_low_threshold = (s8)(TEMPERATURE_THRESHOLD_OFFSET + (payload.config.temperature_low_threshold * TEMPERATURE_THRESHOLD_STEP));
    config_out->temperature_high_threshold = (s8)(TEMPERATURE_THRESHOLD_OFFSET + (payload.config.temperature_high_threshold * TEMPERATURE_THRESHOLD_STEP));

    /* Parsing humidity threshold */
    config_out->humidity_low_threshold = (u8)(HUMIDITY_THRESHOLD_OFFSET + (payload.config.humidity_low_threshold * HUMIDITY_THRESHOLD_STEP));
    config_out->humidity_high_threshold = (u8)(HUMIDITY_THRESHOLD_OFFSET + (payload.config.humidity_high_threshold * HUMIDITY_THRESHOLD_STEP));

    /* Parsing light threshold */
    config_out->brightness_threshold = (u16)(BRIGHTNESS_THRESHOLD_OFFSET + (payload.config.brightness_threshold * BRIGHTNESS_THRESHOLD_STEP));

    config_out->delay = (vibration_clear_delay_e)(payload.config.vibration_delay);

    /* Parsing mode VIBRATION sensitivity */
    if ((payload.config.vibration_threshold == VIBRATION_VERY_SENSITIVE_THRESHOLD) && (payload.config.vibration_debounce_counter == VIBRATION_VERY_SENSITIVE_DEBOUNCE_COUNTER))
    {
        config_out->vibration_config = VIBRATION_VERY_SENSITIVE;
    }
    else if ((payload.config.vibration_threshold == VIBRATION_SENSITIVE_THRESHOLD) && (payload.config.vibration_debounce_counter == VIBRATION_SENSITIVE_DEBOUNCE_COUNTER))
    {
        config_out->vibration_config = VIBRATION_SENSITIVE;
    }
    else if ((payload.config.vibration_threshold == VIBRATION_STANDARD_THRESHOLD) && (payload.config.vibration_debounce_counter == VIBRATION_STANDARD_DEBOUNCE_COUNTER))
    {
        config_out->vibration_config = VIBRATION_STANDARD;
    }
    else if ((payload.config.vibration_threshold == VIBRATION_NOT_VERY_SENSITIVE_THRESHOLD) && (payload.config.vibration_debounce_counter == VIBRATION_NOT_VERY_SENSITIVE_DEBOUNCE_COUNTER))
    {
        config_out->vibration_config = VIBRATION_NOT_VERY_SENSITIVE;
    }
    else if ((payload.config.vibration_threshold == VIBRATION_VERY_LITTLE_SENSITIVE_THRESHOLD) && (payload.config.vibration_debounce_counter == VIBRATION_VERY_LITTLE_SENSITIVE_DEBOUNCE_COUNTER))
    {
        config_out->vibration_config = VIBRATION_VERY_LITTLE_SENSITIVE;
    }
    else
    {
        config_out->vibration_config = VIBRATION_CONFIG_UNKNOW;
    }

    /* Parsing mode DOOR sensitivity */
    if ((payload.config.door_close_threshold == DOOR_CLOSE_THRESHOLD_SENSITIVE) && (payload.config.door_open_threshold == DOOR_OPEN_THRESHOLD_SENSITIVE))
    {
        config_out->door_config = DOOR_SENSITIVE;
    }
    else if ((payload.config.door_close_threshold == DOOR_CLOSE_THRESHOLD_STANDARD) && (payload.config.door_open_threshold == DOOR_OPEN_THRESHOLD_STANDARD))
    {
        config_out->door_config = DOOR_STANDARD;
    }
    else if ((payload.config.door_close_threshold == DOOR_CLOSE_THRESHOLD_NOT_VERY_SENSITIVE) && (payload.config.door_open_threshold == DOOR_OPEN_THRESHOLD_NOT_VERY_SENSITIVE))
    {
        config_out->door_config = DOOR_NOT_VERY_SENSITIVE;
    }
    else
    {
        config_out->door_config = DOOR_CONFIG_UNKNOW;
    }
}

/*******************************************************************/

void PAYLOAD_V3_serialize_config(config_s config_in, u8 *config_out)
{

    payload_v3_s payload;

    payload.config.limited = config_in.limited;

    payload.config.uplink_period = config_in.period;

    payload.config.periodic_standby = config_in.is_standby_periodic;
    payload.config.periodic_temperature = config_in.is_temperature_periodic;
    payload.config.periodic_light = config_in.is_light_periodic;
    payload.config.periodic_door = config_in.is_door_periodic;
    payload.config.periodic_vibration = config_in.is_vibration_periodic;
    payload.config.periodic_magnet = config_in.is_magnet_periodic;

    payload.config.temperature_low_threshold = (config_in.temperature_low_threshold - TEMPERATURE_THRESHOLD_OFFSET) / TEMPERATURE_THRESHOLD_STEP;
    payload.config.temperature_high_threshold = (config_in.temperature_high_threshold - TEMPERATURE_THRESHOLD_OFFSET) / TEMPERATURE_THRESHOLD_STEP;

    payload.config.humidity_low_threshold = (config_in.humidity_low_threshold - HUMIDITY_THRESHOLD_OFFSET) / HUMIDITY_THRESHOLD_STEP;
    payload.config.humidity_high_threshold = (config_in.humidity_high_threshold - HUMIDITY_THRESHOLD_OFFSET) / HUMIDITY_THRESHOLD_STEP;

    payload.config.brightness_threshold = (config_in.brightness_threshold - BRIGHTNESS_THRESHOLD_OFFSET) / BRIGHTNESS_THRESHOLD_STEP;

    payload.config.vibration_delay = config_in.delay;

    if (config_in.vibration_config == VIBRATION_VERY_SENSITIVE)
    {
        payload.config.vibration_threshold = VIBRATION_VERY_SENSITIVE_THRESHOLD;
        payload.config.vibration_debounce_counter = VIBRATION_VERY_SENSITIVE_DEBOUNCE_COUNTER;
    }
    else if (config_in.vibration_config == VIBRATION_SENSITIVE)
    {
        payload.config.vibration_threshold = VIBRATION_SENSITIVE_THRESHOLD;
        payload.config.vibration_debounce_counter = VIBRATION_SENSITIVE_DEBOUNCE_COUNTER;
    }
    else if (config_in.vibration_config == VIBRATION_STANDARD)
    {
        payload.config.vibration_threshold = VIBRATION_STANDARD_THRESHOLD;
        payload.config.vibration_debounce_counter = VIBRATION_STANDARD_DEBOUNCE_COUNTER;
    }
    else if (config_in.vibration_config == VIBRATION_NOT_VERY_SENSITIVE)
    {
        payload.config.vibration_threshold = VIBRATION_NOT_VERY_SENSITIVE_THRESHOLD;
        payload.config.vibration_debounce_counter = VIBRATION_NOT_VERY_SENSITIVE_DEBOUNCE_COUNTER;
    }
    else if (config_in.vibration_config == VIBRATION_VERY_LITTLE_SENSITIVE)
    {
        payload.config.vibration_threshold = VIBRATION_VERY_LITTLE_SENSITIVE_THRESHOLD;
        payload.config.vibration_debounce_counter = VIBRATION_VERY_LITTLE_SENSITIVE_DEBOUNCE_COUNTER;
    }

    if (config_in.door_config == DOOR_SENSITIVE)
    {
        payload.config.door_close_threshold = DOOR_CLOSE_THRESHOLD_SENSITIVE;
        payload.config.door_open_threshold = DOOR_OPEN_THRESHOLD_SENSITIVE;
    }
    else if (config_in.door_config == DOOR_STANDARD)
    {
        payload.config.door_close_threshold = DOOR_CLOSE_THRESHOLD_STANDARD;
        payload.config.door_open_threshold = DOOR_OPEN_THRESHOLD_STANDARD;
    }
    else if (config_in.door_config == DOOR_NOT_VERY_SENSITIVE)
    {
        payload.config.door_close_threshold = DOOR_CLOSE_THRESHOLD_NOT_VERY_SENSITIVE;
        payload.config.door_open_threshold = DOOR_OPEN_THRESHOLD_NOT_VERY_SENSITIVE;
    }

    payload.config.reserved = 0;
    payload.config.spare0 = 0;
    payload.config.spare1 = 0;
    payload.config.spare2 = 0;
    memcpy(config_out, &(payload.config), PAYLOAD_CONFIG_SIZE);
}

/*******************************************************************/
