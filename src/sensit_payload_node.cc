#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include "sensit_payload.h"

void ParseData(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();

  unsigned char *payload = (unsigned char *)node::Buffer::Data(args[0]->ToObject());

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

void ParseConfig(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();

  unsigned char *config = (unsigned char *)node::Buffer::Data(args[0]->ToObject());
  double type = args[1]->NumberValue();
  config_s decoded_config = {};
  PAYLOAD_parse_config(config, (type == 3) ? PAYLOAD_V3 : PAYLOAD_V2, &decoded_config);

  v8::Local<v8::Object> obj = v8::Object::New(isolate);

  if (type == 3)
  {
    v8::Local<v8::Number> mode_standby_periodic_msg = v8::Number::New(isolate, decoded_config.is_standby_periodic);
    obj->Set(v8::String::NewFromUtf8(isolate, "isStandByPeriodic"), mode_standby_periodic_msg);

    v8::Local<v8::Number> mode_temperature_periodic_msg = v8::Number::New(isolate, decoded_config.is_temperature_periodic);
    obj->Set(v8::String::NewFromUtf8(isolate, "isTemperaturePeriodic"), mode_temperature_periodic_msg);

    v8::Local<v8::Number> mode_light_periodic_msg = v8::Number::New(isolate, decoded_config.is_light_periodic);
    obj->Set(v8::String::NewFromUtf8(isolate, "isLightPeriodic"), mode_light_periodic_msg);

    v8::Local<v8::Number> mode_door_periodic_msg = v8::Number::New(isolate, decoded_config.is_door_periodic);
    obj->Set(v8::String::NewFromUtf8(isolate, "isDoorPeriodic"), mode_door_periodic_msg);

    v8::Local<v8::Number> mode_vibration_periodic_msg = v8::Number::New(isolate, decoded_config.is_vibration_periodic);
    obj->Set(v8::String::NewFromUtf8(isolate, "isVibrationPeriodic"), mode_vibration_periodic_msg);

    v8::Local<v8::Number> mode_magnet_periodic_msg = v8::Number::New(isolate, decoded_config.is_magnet_periodic);
    obj->Set(v8::String::NewFromUtf8(isolate, "isMagnetPeriodic"), mode_magnet_periodic_msg);

    v8::Local<v8::Number> delay = v8::Number::New(isolate, decoded_config.delay);
    obj->Set(v8::String::NewFromUtf8(isolate, "vibrationClearTime"), delay);

    v8::Local<v8::Number> brightness_threshold = v8::Number::New(isolate, decoded_config.brightness_threshold);
    obj->Set(v8::String::NewFromUtf8(isolate, "lightThreshold"), brightness_threshold);
  }

  if (type == 2)
  {
    v8::Local<v8::Number> brightness_high_threshold = v8::Number::New(isolate, decoded_config.brightness_high_threshold);
    obj->Set(v8::String::NewFromUtf8(isolate, "lightUpper"), brightness_high_threshold);

    v8::Local<v8::Number> brightness_low_threshold = v8::Number::New(isolate, decoded_config.brightness_low_threshold);
    obj->Set(v8::String::NewFromUtf8(isolate, "lightLower"), brightness_low_threshold);
  }

  v8::Local<v8::Number> temperature_low_threshold = v8::Number::New(isolate, decoded_config.temperature_low_threshold);
  obj->Set(v8::String::NewFromUtf8(isolate, "temperatureLower"), temperature_low_threshold);

  v8::Local<v8::Number> temperature_high_threshold = v8::Number::New(isolate, decoded_config.temperature_high_threshold);
  obj->Set(v8::String::NewFromUtf8(isolate, "temperatureUpper"), temperature_high_threshold);

  v8::Local<v8::Number> humidity_low_threshold = v8::Number::New(isolate, decoded_config.humidity_low_threshold);
  obj->Set(v8::String::NewFromUtf8(isolate, "humidityLower"), humidity_low_threshold);

  v8::Local<v8::Number> humidity_high_threshold = v8::Number::New(isolate, decoded_config.humidity_high_threshold);
  obj->Set(v8::String::NewFromUtf8(isolate, "humidityUpper"), humidity_high_threshold);

  v8::Local<v8::Number> vibration_config = v8::Number::New(isolate, decoded_config.vibration_config);
  obj->Set(v8::String::NewFromUtf8(isolate, "vibrationSensitivity"), vibration_config);

  v8::Local<v8::Number> door_config = v8::Number::New(isolate, decoded_config.door_config);
  obj->Set(v8::String::NewFromUtf8(isolate, "door"), door_config);

  v8::Local<v8::Number> period = v8::Number::New(isolate, decoded_config.period);
  obj->Set(v8::String::NewFromUtf8(isolate, "period"), period);

  v8::Local<v8::Number> limited = v8::Number::New(isolate, decoded_config.limited);
  obj->Set(v8::String::NewFromUtf8(isolate, "limited"), limited);

  args.GetReturnValue().Set(obj);
}

void SerializeConfig(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();
  config_s config;
  v8::Handle<v8::Object> config_in = v8::Handle<v8::Object>::Cast(args[0]);
  v8::Local<v8::Value> isLightPeriodic =
      config_in->Get(v8::String::NewFromUtf8(isolate, "isLightPeriodic"));
  config.is_light_periodic = isLightPeriodic->NumberValue();

  v8::Local<v8::Value> isTemperaturePeriodic =
      config_in->Get(v8::String::NewFromUtf8(isolate, "isTemperaturePeriodic"));
  config.is_temperature_periodic = isTemperaturePeriodic->NumberValue();

  v8::Local<v8::Value> isMagnetPeriodic =
      config_in->Get(v8::String::NewFromUtf8(isolate, "isMagnetPeriodic"));
  config.is_magnet_periodic = isMagnetPeriodic->NumberValue();

  v8::Local<v8::Value> isDoorPeriodic =
      config_in->Get(v8::String::NewFromUtf8(isolate, "isDoorPeriodic"));
  config.is_door_periodic = isDoorPeriodic->NumberValue();

  v8::Local<v8::Value> isStandByPeriodic =
      config_in->Get(v8::String::NewFromUtf8(isolate, "isStandByPeriodic"));
  config.is_standby_periodic = isStandByPeriodic->NumberValue();

  v8::Local<v8::Value> isVibrationPeriodic =
      config_in->Get(v8::String::NewFromUtf8(isolate, "isVibrationPeriodic"));
  config.is_vibration_periodic = isVibrationPeriodic->NumberValue();

  v8::Local<v8::Value> limited =
      config_in->Get(v8::String::NewFromUtf8(isolate, "limited"));
  config.limited = limited->NumberValue();

  v8::Local<v8::Value> temperatureLower =
      config_in->Get(v8::String::NewFromUtf8(isolate, "temperatureLower"));
  config.temperature_low_threshold = temperatureLower->NumberValue();

  v8::Local<v8::Value> temperatureUpper =
      config_in->Get(v8::String::NewFromUtf8(isolate, "temperatureUpper"));
  config.temperature_high_threshold = temperatureUpper->NumberValue();

  v8::Local<v8::Value> humidityLower =
      config_in->Get(v8::String::NewFromUtf8(isolate, "humidityLower"));
  config.humidity_low_threshold = humidityLower->NumberValue();

  v8::Local<v8::Value> humidityUpper =
      config_in->Get(v8::String::NewFromUtf8(isolate, "humidityUpper"));
  config.humidity_high_threshold = humidityUpper->NumberValue();

  v8::Local<v8::Value> lightThreshold =
      config_in->Get(v8::String::NewFromUtf8(isolate, "lightThreshold"));
  config.brightness_threshold = lightThreshold->NumberValue();

  v8::Local<v8::Value> lightUpper =
      config_in->Get(v8::String::NewFromUtf8(isolate, "lightUpper"));
  config.brightness_high_threshold = lightUpper->NumberValue() * BRIGHTNESS_THRESHOLD_FACTOR;
  v8::Local<v8::Value> lightLower =
      config_in->Get(v8::String::NewFromUtf8(isolate, "lightLower"));
  config.brightness_low_threshold = (lightLower->NumberValue()) * BRIGHTNESS_THRESHOLD_FACTOR;

  v8::Local<v8::Value> vibration =
      config_in->Get(v8::String::NewFromUtf8(isolate, "vibrationSensitivity"));
  config.vibration_config = vibration->NumberValue();

  v8::Local<v8::Value> door =
      config_in->Get(v8::String::NewFromUtf8(isolate, "door"));
  config.door_config = door->NumberValue();

  v8::Local<v8::Value> period =
      config_in->Get(v8::String::NewFromUtf8(isolate, "period"));
  config.period = period->NumberValue();

  v8::Local<v8::Value> delay =
      config_in->Get(v8::String::NewFromUtf8(isolate, "delay"));
  config.delay = delay->NumberValue();

  double type = args[1]->NumberValue();
  u8 decoded_config;

  PAYLOAD_serialize_config(config, (type == 3) ? PAYLOAD_V3 : PAYLOAD_V2, &decoded_config);

  v8::MaybeLocal<v8::Object>
      buffer = node::Buffer::Copy(isolate, reinterpret_cast<const char *>(&decoded_config), PAYLOAD_CONFIG_SIZE);
  v8::Local<v8::Object> out;
  buffer.ToLocal(&out);
  args.GetReturnValue().Set(out);
}

void init(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "parseData", ParseData);
  NODE_SET_METHOD(exports, "parseConfig", ParseConfig);
  NODE_SET_METHOD(exports, "serializeConfig", SerializeConfig);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, init)
