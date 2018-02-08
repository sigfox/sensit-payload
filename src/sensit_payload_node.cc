#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include "sensit_payload.h"

void ParseData(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  unsigned char* payload = (unsigned char*) node::Buffer::Data(args[0]->ToObject());

  // printf("actual: %x%x%x%x \n", payload[0], payload[1], payload[2], payload[3]);

  data_s decoded_payload = {};
  PAYLOAD_parse_data(payload, &decoded_payload);

  v8::Local<v8::Object> obj = v8::Object::New(isolate);

  v8::Local<v8::Number> type = v8::Number::New(isolate, decoded_payload.type);
  obj->Set(v8::String::NewFromUtf8(isolate, "type"), type);

  v8::Local<v8::Number> battery_level = v8::Number::New(isolate, decoded_payload.battery_level);
  obj->Set(v8::String::NewFromUtf8(isolate, "batteryLevel"), battery_level);

  v8::Local<v8::Number> mode = v8::Number::New(isolate, decoded_payload.mode);
  obj->Set(v8::String::NewFromUtf8(isolate, "mode"), mode);

  v8::Local<v8::Number> button = v8::Number::New(isolate, decoded_payload.button);
  obj->Set(v8::String::NewFromUtf8(isolate, "button"), button);

  v8::Local<v8::Number> temperature = v8::Number::New(isolate, decoded_payload.temperature);
  obj->Set(v8::String::NewFromUtf8(isolate, "temperature"), temperature);

  v8::Local<v8::Number> humidity = v8::Number::New(isolate, decoded_payload.humidity);
  obj->Set(v8::String::NewFromUtf8(isolate, "humidity"), humidity);

  v8::Local<v8::Number> brightness = v8::Number::New(isolate, decoded_payload.brightness);
  obj->Set(v8::String::NewFromUtf8(isolate, "brightness"), brightness);

  v8::Local<v8::Number> door = v8::Number::New(isolate, decoded_payload.door);
  obj->Set(v8::String::NewFromUtf8(isolate, "door"), door);

  v8::Local<v8::Number> vibration = v8::Number::New(isolate, decoded_payload.vibration);
  obj->Set(v8::String::NewFromUtf8(isolate, "vibration"), vibration);

  v8::Local<v8::Number> magnet = v8::Number::New(isolate, decoded_payload.magnet);
  obj->Set(v8::String::NewFromUtf8(isolate, "magnet"), magnet);

  v8::Local<v8::Number> event_counter = v8::Number::New(isolate, decoded_payload.event_counter);
  obj->Set(v8::String::NewFromUtf8(isolate, "eventCounter"), event_counter);

  v8::Local<v8::Number> version_major = v8::Number::New(isolate, decoded_payload.version_major);
  obj->Set(v8::String::NewFromUtf8(isolate, "versionMajor"), version_major);

  v8::Local<v8::Number> version_minor = v8::Number::New(isolate, decoded_payload.version_minor);
  obj->Set(v8::String::NewFromUtf8(isolate, "versionMinor"), version_minor);

  v8::Local<v8::Number> version_patch = v8::Number::New(isolate, decoded_payload.version_patch);
  obj->Set(v8::String::NewFromUtf8(isolate, "versionPatch"), version_patch);

  v8::Local<v8::Number> error = v8::Number::New(isolate, decoded_payload.error);
  obj->Set(v8::String::NewFromUtf8(isolate, "error"), error);

  args.GetReturnValue().Set(obj);
}

void ParseConfig(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  unsigned char* payload = (unsigned char*) node::Buffer::Data(args[0]->ToObject());

  // TODO call C++ method

  v8::Local<v8::Object> obj = v8::Object::New(isolate);

  args.GetReturnValue().Set(obj);
}

void SerializeConfig(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  v8::Local<v8::Object> config = args[0]->ToObject();
  double payload_type = args[1]->NumberValue();

  // TODO call C++ method

  v8::Local<v8::String> payload = v8::String::NewFromUtf8(isolate, "");
  args.GetReturnValue().Set(payload);
}

void init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "parseData", ParseData);
  NODE_SET_METHOD(exports, "parseConfig", ParseConfig);
  NODE_SET_METHOD(exports, "serializeConfig", SerializeConfig);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, init)
