/*!******************************************************************
 * \file sensit_payload.c
 * \brief Functions to parse a Sens'it payload
 * \author Sens'it Team
 *******************************************************************/
/******* INCLUDES **************************************************/
#include <stdio.h>
#include <string.h>
#include "sensit_payload.h"
#include "sensit_payload_v3.h"
#include "sensit_payload_v2.h"

/******* DEFINE ****************************************************/
#define PAYLOAD_V3_ID 0b110
#define V3_ID 3
#define V2_ID 2

/*******************************************************************/

void PAYLOAD_parse_data(u8 *data_in, data_s *data_out)
{
    payload_v3_s payload3;
    payload_v2_s payload2;
    memcpy(&(payload3.data), data_in, PAYLOAD_DATA_SIZE);
    memcpy(&(payload2.data), data_in, PAYLOAD_DATA_SIZE);

    data_out->error = PARSE_ERR_NONE;

    if (payload3.data.reserved == PAYLOAD_V3_ID)
    {
        data_out->type = PAYLOAD_V3;
        PAYLOAD_V3_parse_data(payload3, data_out);
    }
    else if (payload3.data.reserved < PAYLOAD_V3_ID)
    {
        data_out->type = PAYLOAD_V2;
        PAYLOAD_V2_parse_data(payload2, data_out);
    }
    else
    {
        data_out->error = PARSE_ERR_TYPE;
    }
}

/*******************************************************************/

void PAYLOAD_parse_config(u8 *data_in, payload_type_e type, config_s *config_out)
{
    payload_v3_s payload3;
    payload_v2_s payload2;

    memcpy(&(payload3.config), data_in, PAYLOAD_CONFIG_SIZE);
    memcpy(&(payload2.config), data_in, PAYLOAD_CONFIG_SIZE);

    if (type == V3_ID)
    {
        PAYLOAD_V3_parse_config(payload3, config_out);
    }
    else if (type == V2_ID)
    {
        PAYLOAD_V2_parse_config(payload2, config_out);
    }
}

void PAYLOAD_serialize_config(config_s config_in, payload_type_e type, u8 *config_out)
{
    if (type == V3_ID)
    {
        PAYLOAD_V3_serialize_config(config_in, config_out);
    }
    else if (type == V2_ID)
    {
        PAYLOAD_V2_serialize_config(config_in, config_out);
    }
}