/**
 * Module dependencies
 */

const tap = require('tap');
const sensitPayload = require('../');


const samples = [
  // v3 config
  {
    type: 3,
    expected: '46003f0f8004023c',
    actual: {
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
      vibrationSensitivity: 2,
      vibrationClearTime: 2,
      door: 1,
      period: 1
    }
  },
  // v2 config
  {
    type: 2,
    expected: '00ff008f04027390',
    actual: {
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
    }
  }
];

samples.forEach((sample) => {
  tap.test(`sensitPayload.parse(${sample})`, (t) => {
    const actual = sensitPayload.serializeConfig(sample.actual, sample.type);
    t.same(actual, sample.expected);
    t.end();
  });
});
