{
  "targets": [
    {
      "target_name": "sensit_payload_lib",
      'defines': [ 'V8_DEPRECATION_WARNINGS=1' ],
      "sources": [ "src/sensit_payload_node.cc", "src/sensit_payload.cc", "src/sensit_payload_v3.cc", "src/sensit_payload_v2.cc" ]
    }
  ]
}
