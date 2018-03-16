/**
 * Module dependencies
 */

const tap = require('tap');
const sensitPayload = require('../');


const samples = [
  // Mode light v3
  {
    payload: 'f6100065',
    expected: {
      error: 0,
      type: 3,
      light: 1.05,
      button: false,
      modeCode: 2,
      mode: 'light',
      config: null,
      batteryLevel: 4200,
      battery: 95
    }
  },
  // Mode temperature v3
  {
    payload: 'f609744f',
    expected: {
      error: 0,
      type: 3,
      temperature: 21.5,
      humidity: 39.5,
      button: false,
      batteryLevel: 4200,
      modeCode: 1,
      mode: 'temperature',
      config: null,
      battery: 95
    }
  },
  // Mode door v3
  {
    payload: 'b6180000',
    expected: {
      error: 0,
      type: 3,
      button: false,
      door: 4,
      battery: 53,
      batteryLevel: 3800,
      modeCode: 3,
      mode: 'door',
      eventCounter: 0,
      config: null
    }
  },
  // Mode door v3
  {
    payload: 'b61e0000',
    expected: {
      error: 0,
      type: 3,
      button: true,
      door: 2,
      battery: 53,
      batteryLevel: 3800,
      modeCode: 3,
      mode: 'door',
      eventCounter: 0,
      config: null
    }
  },
  // Mode vibration v3
  {
    payload: 'ae210190',
    expected: {
      error: 0,
      type: 3,
      button: false,
      vibration: true,
      battery: 45,
      batteryLevel: 3750,
      modeCode: 4,
      mode: 'vibration',
      eventCounter: 400,
      config: null,
    }
  },
  // Mode magnet v3
  {
    payload: 'e6290001',
    expected: {
      error: 0,
      type: 3,
      button: false,
      magnet: true,
      batteryLevel: 4100,
      modeCode: 5,
      mode: 'magnet',
      config: null,
      eventCounter: 1,
      battery: 85
    }
  },
  // Mode standby v3 + config
  {
    payload: 'ae00304046003f0f8004223c',
    expected: {
      error: 0,
      type: 3,
      button: false,
      batteryLevel: 3750,
      battery: 45,
      modeCode: 0,
      mode: 'standby',
      version: '3.1.0',
      config: {
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
    }
  },
  // Mode temperature v2 + config
  {
    payload: '895d205d00ff008f04027390',
    expected: {
      error: 0,
      type: 2,
      button: false,
      batteryLevel: 4150,
      battery: 90,
      modeCode: 1,
      mode: 'temperature',
      temperature: 19,
      humidity: 46.5,
      config: {
        temperatureLower: -20,
        temperatureUpper: 107,
        humidityLower: 0,
        humidityUpper: 0,
        lightUpper: 10,
        lightLower: 0,
        vibrationSensitivity: 2,
        door: 0,
        period: 1,
        limited: true
      }
    }
  }
];

samples.forEach((sample) => {
  tap.test(`sensitPayload.parse(${sample.payload})`, (t) => {
    const actual = sensitPayload.parse(sample.payload);
    t.strictSame(actual, sample.expected);
    t.end();
  });
});
