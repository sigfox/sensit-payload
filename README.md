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

The object representing the values contained in the payload will always contained the following properties:

- `error` {Number} - `0` if no error something else otherwise check sensitPayload.*_ERROR_CODE constants
- `type` {Number} - `2` for Sensit v2 payload, `3` for Sensit v3 payload
- `button` {Boolean} - `true` when the button was pressed, `false` otherwise
- `config` {Object} - `null` if the payload does not contain the config
- `mode` {String} - string representing the current mode, see exposed constants
- `modeCode` {Number} - number representing the current mode, see exposed constants
- `batteryLevel` {Number} - number of mV of the battery

The next properties are present depending of the current mode, in case of a v2 payload when the button is pressed the record corresponding to the mode is not sent except for the temperature in the temperature mode.

- `temperature` {Number} - temperature in celsius degrees
- `humidity` {Number} - humidity in percent
- `brightness` {Number} - brightness in lux
- `magnet` {Boolean} - `true` when magnet detected, `false` otherwise
- `vibration` {Number} - a number representing vibration state, see exposed constants
- `door` {Number} - a number representing door state, see exposed constants

```
const data = sensitPayload.parse("f6100065")
// { error: 0, type: 3, brightness: 1.05, button: false, modeCode: 2, mode: 'light', config: null }
```

### sensitPayload.serializeConfig(config, payloadType)

Serialize an object representating (`config` argument above) Sensit config into a 16 hexadecimals string.
The `payloadType` argument is either `sensitPayload.PAYLOAD_TYPE_V2` or `sensitPayload.PAYLOAD_TYPE_V3`, other value will throw an error.

// TODO finish off this example

```
const config = {};
const payload = sensitPayload.serializeConfig(config, sensitPayload.PAYLOAD_TYPE_V3);
// "hdudu"
```

## Sensit payload specification

### v2

// TODO add link to spec

### v3

// TODO add link to spec

## Licence

// TODO add licence
