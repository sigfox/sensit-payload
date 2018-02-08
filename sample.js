/**
 * Module dependencies
 */
 
const bindings = require('bindings');
const Buffer = require('buffer').Buffer;
const sensitProtocol = require('./');

// Sample

const raw = 'f6100065';
const buffer = Buffer.from(raw, 'hex');
console.info(`
  Byte flow   = f6100065
`);
console.info('buffer', buffer);
console.info('sensitPayload.parse() =', sensitProtocol.lib.parse(raw));
console.info('lib.parseData(buffer) =', lib.parseData(buffer));
