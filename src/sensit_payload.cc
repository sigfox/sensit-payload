/*!******************************************************************
 * \file payload.c
 * \brief Functions to parse a Sens'it payload
 * \author Sens'it Team
 * \copyright Copyright (c) 2018 Sigfox, All Rights Reserved.
 *******************************************************************/
/******* INCLUDES **************************************************/
#include "sensit_payload.h"
#include <stdio.h>



/******* DEFINE ****************************************************/
#define BATTERY_OFFSET         2700 /* 2.7 V  */
#define BATTERY_STEP           50   /* 50 mV */
#define TEMPERATURE_OFFSET     200  /* 25°C */


/******* STATIC FUNCTION *******************************************/
static void my_memcpy(u8* data_out, u8* data_in, u32 data_size);


/*******************************************************************/

void PAYLOAD_parse_data(u8* data_in, data_s* data_out)
{
    payload_s payload;

    my_memcpy((u8*)&payload, data_in, DATA_SIZE);

    // printf("reserved-> %d: ", payload.reserved);

    data_out->error = PARSE_ERR_NONE;

    if ( payload.reserved == 0b110 )
    {
        data_out->type = PAYLOAD_V3;
    }
    else
    {
        data_out->error = PARSE_ERR_TYPE;
        return;
    }

    data_out->mode = (mode_e) payload.mode;
    data_out->button = payload.button;
    data_out->battery_level = (payload.battery*BATTERY_STEP) + BATTERY_OFFSET;

    if ( payload.mode == MODE_STANDBY )
    {
        data_out->version_major = payload.fw_major;
        data_out->version_minor = (payload.fw_minorMSB << 4) | payload.fw_minorLSB;
        data_out->version_patch = payload.fw_patch;
    }
    else if ( payload.mode == MODE_TEMPERATURE )
    {
        data_out->temperature = (payload.spare << 8) | payload.temperatureLSB;
        data_out->temperature -= TEMPERATURE_OFFSET;
        data_out->humidity = payload.humidity;
    }
    else if ( payload.mode == MODE_LIGHT )
    {
        data_out->brightness = (payload.brightnessMSB << 8) | payload.brightnessLSB;
    }
    else if ( payload.mode == MODE_DOOR )
    {
        data_out->door = (door_e) payload.spare;
        data_out->event_counter = (payload.event_counterMSB << 8) | payload.event_counterLSB;
    }
    else if ( payload.mode == MODE_VIBRATION )
    {
        data_out->vibration = (vibration_e) payload.spare;
        data_out->event_counter = (payload.event_counterMSB << 8) | payload.event_counterLSB;
    }
    else if ( payload.mode == MODE_MAGNET )
    {
        data_out->magnet = payload.spare;
        data_out->event_counter = (payload.event_counterMSB << 8) | payload.event_counterLSB;
    }
    else
    {
        data_out->error = PARSE_ERR_MODE;
    }
}

/*******************************************************************/

static void my_memcpy(u8* data_out, u8* data_in, u32 data_size)
{
    u32 i;

    for (i=0 ; i < data_size ; i++)
    {
        *(data_out+i) = *(data_in+i);
    }
}

/*******************************************************************/
