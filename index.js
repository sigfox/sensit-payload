/**
 * Module dependencies
 */

const bindings = require('bindings');
const Buffer = require('buffer').Buffer;

const lib = bindings('sensit_payload_lib');

const sensitPayload = module.exports = exports;

/**
 * Expose Sensit payload constants
 */

sensitPayload.PAYLOAD_TYPE_V2 = 2;
sensitPayload.PAYLOAD_TYPE_V3 = 3;

sensitPayload.MODE_STANDBY = 0b00000;
sensitPayload.MODE_TEMPERATURE = 0b00001;
sensitPayload.MODE_LIGHT = 0b00010;
sensitPayload.MODE_DOOR = 0b00011;
sensitPayload.MODE_VIBRATION = 0b00100;
sensitPayload.MODE_MAGNET = 0b00101;
sensitPayload.MODES = {
  [sensitPayload.MODE_STANDBY]: 'standby',
  [sensitPayload.MODE_TEMPERATURE]: 'temperature',
  [sensitPayload.MODE_LIGHT]: 'light',
  [sensitPayload.MODE_DOOR]: 'door',
  [sensitPayload.MODE_VIBRATION]: 'vibration',
  [sensitPayload.MODE_MAGNET]: 'magnet'
};

sensitPayload.DOOR_NONE = 0b00;
sensitPayload.DOOR_OPENING = 0b01;
sensitPayload.DOOR_CLOSING = 0b10;

sensitPayload.VIBRATION_NONE = 0b00;
sensitPayload.VIBRATION_START = 0b01;
sensitPayload.VIBRATION_STOP = 0b10;
sensitPayload.VIBRATION_ONGOING = 0b11;

sensitPayload.PARSE_ERR_NONE = 0x00;
sensitPayload.PARSE_ERR_TYPE = 0x01;
sensitPayload.PARSE_ERR_MODE = 0x02;

sensitPayload.BUTTON_PRESSED = 1;

sensitPayload.MAGNET_DETECTED = 1;

/**
 * Round number with the given `precision`
 *
 * @param {Number} number
 * @param {Number} precision - default to 0
 *
 * @return {Number}
 */

function round(number, precision = 0) {
  const factor = 10 ** precision;
  return Math.round(number * factor) / factor;
}

/**
 * Get battery percentage from battery level in mV
 * @param {Number} batteryLevel - battery level in mV
 *
 * @return {Number} battery energy percentage still left
 */

function getBatteryPercentage(batteryLevel) {
  const batteryVoltage = batteryLevel / 1000;
  let batteryPercentage;
  if (batteryVoltage >= 3.9 && batteryVoltage <= 4.2) {
    batteryPercentage = Math.round((batteryVoltage - 3.2) * 100);
  } else if (batteryVoltage >= 3.6 && batteryVoltage < 3.9) {
    batteryPercentage = Math.round((batteryVoltage - 3.48) / 0.006);
  } else if (batteryVoltage >= 2.7 && batteryVoltage < 3.6) {
    batteryPercentage = Math.round((batteryVoltage - 2.7) / 0.03);
  } else {
    batteryPercentage = 0;
  }
  batteryPercentage = batteryPercentage < 0 ? 0 : batteryPercentage;
  return batteryPercentage;
}

/**
 * Format object representation of the "data" part of the payload
 * do last changes required to have human readable data with
 * the following instructions:
 *
 * u8 error;              Parsing error code
 * payload_type_e type;   Payload type
 * u16 battery_level;     All modes: Value in mV
 * mode_e mode;           All modes: Active mode
 * bool button;           All modes: if 1, Double click message
 * s16 temperature;       Mode TEMPERATURE: Must be divided by 8 to get in °C
 * u8 humidity;           Mode TEMPERATURE: Must be divided by 2 to get in %
 * u16 brightness;        Mode LIGHT: Must be divided by 96 to get in lux
 * door_e door;           Mode DOOR
 * vibration_e vibration; Mode VIBRATION
 * bool magnet;           Mode MAGNET: if 1, Magnet detected
 * u16 event_counter;     Mode DOOR, VIBRATION & MAGNET: Number of events since last message
 * u8 version_major;      Mode STANDBY
 * u8 version_minor;      Mode STANDBY
 * u8 version_patch;      Mode STANDBY
 *
 * @param {Object} data
 *
 * @return {Object}
 */

function format(data) {
  const {
    mode, button, magnet, temperature, brightness,
    humidity, versionMajor, versionMinor, versionPatch,
    door, vibration, type, error, eventCounter, batteryLevel
  } = data;
  const formattedData = { error, type };
  const isV2ButtonPressed = type === sensitPayload.PAYLOAD_TYPE_V2 && button === sensitPayload.BUTTON_PRESSED;
  // Exception: v2 payload will not return the value of the mode if the button has been pressed except for the temperature mode
  // but only the temperature not the humidity value
  if (mode === sensitPayload.MODE_TEMPERATURE) {
    /* Mode TEMPERATURE: Must be divided by 8 to get in °C */
    formattedData.temperature = round(temperature / 8, 2);
  }
  if (mode === sensitPayload.MODE_TEMPERATURE && !isV2ButtonPressed) {
    /* Mode TEMPERATURE: Must be divided by 2 to get in % */
    formattedData.humidity = round(humidity / 2);
  } else if (mode === sensitPayload.MODE_LIGHT && !isV2ButtonPressed) {
    /* Mode LIGHT: Must be divided by 96 to get in lux */
    formattedData.light = round(brightness / 96, 2);
  } else if (mode === sensitPayload.MODE_STANDBY) {
    formattedData.version = `${versionMajor}.${versionMinor}.${versionPatch}`;
  } else if (mode === sensitPayload.MODE_DOOR && !isV2ButtonPressed) {
    formattedData.door = door;
    formattedData.eventCounter = eventCounter;
  } else if (mode === sensitPayload.MODE_MAGNET && !isV2ButtonPressed) {
    /* Mode MAGNET: if 1, Magnet detected */
    formattedData.magnet = magnet === sensitPayload.MAGNET_DETECTED;
    formattedData.eventCounter = eventCounter;
  } else if (mode === sensitPayload.MODE_VIBRATION && !isV2ButtonPressed) {
    formattedData.vibration = vibration;
    formattedData.eventCounter = eventCounter;
  }
  formattedData.button = button === sensitPayload.BUTTON_PRESSED;
  formattedData.battery = getBatteryPercentage(batteryLevel);
  formattedData.batteryLevel = batteryLevel;
  formattedData.modeCode = mode;
  formattedData.mode = sensitPayload.MODES[formattedData.modeCode];
  return formattedData;
}

/**
 * Parse Sensit payload 4 bytes of "data" and the 8 bytes of "config" if present
 *
 * @param {String} payload
 *
 * @return {Object}
 */

sensitPayload.parse = (payload) => {
  // Check payload size
  if (payload.length !== 8 && payload.length !== 24) {
    throw new Error('Sensit payload is made of either 8 or 24 hexadecimal characters');
  }

  // 4 bytes for the "data" part
  const payloadData = payload.slice(0, 8);
  const data = sensitPayload.parseData(payloadData);

  // 8 bytes for the "config" part
  let config = null;
  if (payload.length === 24) {
    const payloadConfig = payload.slice(8, 24);
    config = sensitPayload.parseConfig(payloadConfig, data.type);
  }
  data.config = config;
  return data;
};

/**
 * Parse Sensit payload "data" part made of 4 bytes
 *
 * @param {String} payload
 *
 * @return {Object} data
 */

sensitPayload.parseData = (payload) => {
  // Check payload size
  if (payload.length !== 8) {
    throw new Error('Sensit payload "data" part is made of 8 hexadecimal characters');
  }
  const data = lib.parseData(Buffer.from(payload, 'hex'));
  return format(data);
};

/**
 * Parse Sensit payload "config" part made of 8 bytes
 *
 * @param {String} payload
 *
 * @return {Object} config
 */

sensitPayload.parseConfig = (payload) => {
  // Check payload size
  if (payload.length !== 16) {
    throw new Error('Sensit payload "config" part is made of 16 hexadecimal characters');
  }
  const config = lib.parseConfig(Buffer.from(payload, 'hex'));
  return config;
};

/**
 * Serialize Sens'it config object representation into a hexadecimal string
 *
 * @param {Object} config
 *
 * @return {String} payload
 */

sensitPayload.serializeConfig = (config, payloadType) => {
  if (payloadType === sensitPayload.PAYLOAD_TYPE_V2) {
    return sensitPayload.serializeV2Config(config);
  } else if (payloadType === sensitPayload.PAYLOAD_TYPE_V3) {
    return sensitPayload.serializeV3Config(config);
  }
  throw new Error('payload type not defined or not supported');
};

/**
 * Serialize Sens'it V2 config object representation into a hexadecimal string
 *
 * @param {Object} config
 *
 * @return {String} payload
 */

sensitPayload.serializeV2Config = config => lib.serializeConfig(config, sensitPayload.PAYLOAD_TYPE_V2);

/**
 * Serialize Sens'it V3 config object representation into a hexadecimal string
 *
 * @param {Object} config
 *
 * @return {String} payload
 */

sensitPayload.serializeV3Config = config => lib.serializeConfig(config, sensitPayload.PAYLOAD_TYPE_V3);


/**
 * Expose native lib
 */

sensitPayload.lib = lib;
