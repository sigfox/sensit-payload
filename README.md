# sensit-payload

Sensit payload parser and serializer

## Installation

```
npm install sensit-payload
```

## API

```
const sensitPayload = require('sensit-payload');
```

### sensitPayload.parse(payload)

Parse a Sensit payload and returning an object representing the values contained in the payload. The `payload` argument is a 8 hexadecimals string or a 24 hexadecimals string if the config of the Sensit is sent.

The object representing the values contained in the payload will always containe the following properties:

- `error` {Number} - `0` if no error something else otherwise check sensitPayload.*_ERROR_CODE constants
- `type` {Number} - `2` for Sensit v2 payload, `3` for Sensit v3 payload
- `button` {Boolean} - `true` when the button was pressed, `false` otherwise
- `config` {Object} - `null` if the payload does not contain the config
- `mode` {String} - string representing the current mode, see exposed constants
- `modeCode` {Number} - number representing the current mode, see exposed constants
- `batteryLevel` {Number} - number of mV of the battery
- `battery` {Number} - percentage of the battery energy still left


The next properties are present depending on the current mode, in case of a v2 payload when the button is pressed the record corresponding to the mode is not sent except for the temperature in the temperature mode.

- `temperature` {Number} - temperature in celsius degrees
- `humidity` {Number} - humidity in percent
- `light` {Number} - brightness in lux
- `magnet` {Boolean} - `true` when magnet detected, `false` otherwise
- `vibration` {Number} - a number representing vibration state, see exposed constants
- `door` {Number} - a number representing door state, see exposed constants

```
const data = sensitPayload.parse("f6100065")
// { error: 0, type: 3, brightness: 1.05, button: false, modeCode: 2, mode: 'light', config: null }
```

### sensitPayload.parseConfig(config, payloadType)
The config is different depending on the current verion: 

#### V3 specific fields
- `isStandByPeriodic` {Boolean} - true if StandBy is in periodic mode
- `isTemperaturePeriodic` {Boolean} - true if Temperature is in periodic mode
- `isDoorPeriodic` {Boolean} - true if Door is in periodic mode
- `isMagnetPeriodic`{Boolean} - true if Magnet is in periodic mode 
- `isVibrationPeriodic`{Boolean} - true if Vibration is in periodic mode
- `isLightPeriodic`{Boolean} - true if Light is in periodic mode
- `lightThreshold`{Number} - the brightness threshold



#### V2 specifig fields

- `lightUpper` {Number} - upper brightness threshold
- `lightLower` {Number} - lower brightness threshold

#### Shared fields 
- `temperatureLower`{Number} - lower temperature threshold
- `temperatureUpper`{Number} - upper temperature threshold
- `humidityLower`{Number} - lower humidity threshold
- `humidityUpper`{Number} - upper humidity threshold
- `lightLower`{Number} - lower light threshold
- `lightUpper`{Number} - upper light threshold
- `vibrationSensitivity`{Number} - code that indicates how sensitive is the vibration sensor
- `door`{Number} - code that indicates how sensitive is the door detection
- `period`{Number} 
- `limited`{Boolean}

##### Example
```
const data = sensitPayload.parse("895d205d00ff008f04027390")

data should contain:
{
        error: 0,
        type: 2,
        button: false,
        batteryLevel: 4150,
        battery: 95,
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
```
### sensitPayload.serializeConfig(config, payloadType)

Serialize an object representating (`config` argument above) Sensit config into a 16 hexadecimals string.
The `payloadType` argument is either `sensitPayload.PAYLOAD_TYPE_V2` or `sensitPayload.PAYLOAD_TYPE_V3`, other value will throw an error.

#### Example
```
const config = {
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
    };
const payload = sensitPayload.serializeConfig(config, sensitPayload.PAYLOAD_TYPE_V3);

payload should be equal to 00ff008f04027390

```

## Test

Run test suite with:

```
npm install
npm test
```

## Development

### Requirements

- Install node.js, we recommand nvm to handle multiple version of node on your machine
- Install node-gyp with `npm install node-gyp -g`

### Build

```
node-gyp rebuild
```

### Run

```
node sample.js
```

## Sensit payload specification

### v2

// TODO add link to spec

### v3

// TODO add link to spec

## Licence

MIT
