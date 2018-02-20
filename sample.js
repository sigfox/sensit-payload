/**
 * Module dependencies
 */

const bindings = require('bindings');
const Buffer = require('buffer').Buffer;
const sensitPayload = require('./');

// Sample

const raw = '895d205d00ff008f04027390';
const buffer = Buffer.from(raw, 'hex');
//console.info('buffer', buffer);
console.info('sensitPayload.parse() =', sensitPayload.parse(raw));
console.info('sensitPayload.serialize() =', sensitPayload.serializeConfig(
  {
    limited: true,
    isStandByPeriodic: false,
    isTemperaturePeriodic: true,
    isLightPeriodic: true,
    isDoorPeriodic: false,
    isVibrationPeriodic: false,
    isMagnetPeriodic: false,
    temperatureLower: -9,
    temperatureUpper: 54,
    humidityLower: 30,
    humidityUpper: 90,
    lightThreshold: 1,
    vibration: 2,
    vibrationClearTime: 2,
    door: 1,
    period: 1
  }, 3
));

console.info('sensitPayload.serialize() 00ff008f04027390 =', sensitPayload.serializeConfig(
  {
    isStandByPeriodic: 0,
    isTemperaturePeriodic: 0,
    isLightPeriodic: 0,
    isDoorPeriodic: 0,
    isVibrationPeriodic: 0,
    isMagnetPeriodic: 0,
    temperatureLower: -20,
    temperatureUpper: 107,
    humidityLower: 0,
    humidityUpper: 0,
    lightThreshold: 0,
    lightUpper: 10,
    lightLower: 0,
    vibrationSensitivity: 2,
    vibrationClearTime: 0,
    door: 0,
    period: 1,
    limited: 1
  }, 2
));
//console.info('lib.parseData(buffer) =', sensitPayload.lib.parseData(buffer));
