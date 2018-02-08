/**
 * Module dependencies
 */

const tap = require('tap');
const sensitPayload = require('../');


const samples = [
  // Mode light
  {
    payload: 'f6100065',
    expected: {
      error: 0,
      type: 3,
      brightness: 1.05,
      button: false,
      modeCode: 2,
      mode: 'light',
      config: null,
      batteryLevel: 4200
    }
  },
  // Mode temperature
  {
    payload: 'f609744f',
    expected: {
      error: 0,
      type: 3,
      temperature: 21.5,
      humidity: 40,
      button: false,
      batteryLevel: 4200,
      modeCode: 1,
      mode: 'temperature',
      config: null
    }
  },
  // Mode magnet
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
      eventCounter: 1
    }
  }
];

samples.forEach((sample) => {
  tap.test(`sensitPayload.parse(${sample.payload})`, (t) => {
    const actual = sensitPayload.parse(sample.payload);
    t.same(actual, sample.expected);
    t.end();
  });
});
