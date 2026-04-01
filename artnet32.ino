#include <Arduino.h>
#include <WiFi.h>
#include <esp_dmx.h>
#include <ArtnetWifi.h>

// ── HOTSPOT ───────────────────────────────────
const char* ap_ssid     = "ESP32-DMX-Node";
const char* ap_password = "dmxdmxdmx";

// ── PINS UNIVERSUM 1 ──────────────────────────
const int transmitPin1 = 17;
const int receivePin1  = 35;  // Dummy, nicht verbinden
const int enablePin1   = 27;
dmx_port_t dmxPort1    = 1;

// ── PINS UNIVERSUM 2 ──────────────────────────
const int transmitPin2 = 13;
const int receivePin2  = 34;  // Dummy, nicht verbinden
const int enablePin2   = 26;
dmx_port_t dmxPort2    = 2;

// ── BUFFER & FLAGS ────────────────────────────
ArtnetWifi artnet;
byte dataDMX1[DMX_PACKET_SIZE];
byte dataDMX2[DMX_PACKET_SIZE];
volatile bool newFrame1 = false;
volatile bool newFrame2 = false;

// ── ARTNET CALLBACK ───────────────────────────
void onDmxFrame(uint16_t universe, uint16_t length,
                uint8_t sequence, uint8_t* data) {
  if (universe == 0) {
    memcpy(&dataDMX1[1], data, length > 512 ? 512 : length);
    newFrame1 = true;
  } else if (universe == 1) {
    memcpy(&dataDMX2[1], data, length > 512 ? 512 : length);
    newFrame2 = true;
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("Hotspot IP: ");
  Serial.println(WiFi.softAPIP());

  dmx_config_t config = DMX_CONFIG_DEFAULT;

  dmx_driver_install(dmxPort1, &config, 0);
  dmx_set_pin(dmxPort1, transmitPin1, receivePin1, enablePin1);
  memset(dataDMX1, 0, DMX_PACKET_SIZE);
  dmx_write(dmxPort1, dataDMX1, DMX_PACKET_SIZE);
  dmx_send(dmxPort1, DMX_PACKET_SIZE);
  dmx_wait_sent(dmxPort1, DMX_TIMEOUT_TICK);

  dmx_driver_install(dmxPort2, &config, 0);
  dmx_set_pin(dmxPort2, transmitPin2, receivePin2, enablePin2);
  memset(dataDMX2, 0, DMX_PACKET_SIZE);
  dmx_write(dmxPort2, dataDMX2, DMX_PACKET_SIZE);
  dmx_send(dmxPort2, DMX_PACKET_SIZE);
  dmx_wait_sent(dmxPort2, DMX_TIMEOUT_TICK);

  artnet.setArtDmxCallback(onDmxFrame);
  artnet.begin("ESP32-DMX-Node");

  Serial.println("Ready! WiFi: ESP32-DMX-Node | Password: dmxdmxdmx");
}

void loop() {
  artnet.read();

  if (newFrame1) {
    newFrame1 = false;
    dmx_write(dmxPort1, dataDMX1, DMX_PACKET_SIZE);
    dmx_send(dmxPort1, DMX_PACKET_SIZE);
    dmx_wait_sent(dmxPort1, DMX_TIMEOUT_TICK);
  }

  if (newFrame2) {
    newFrame2 = false;
    dmx_write(dmxPort2, dataDMX2, DMX_PACKET_SIZE);
    dmx_send(dmxPort2, DMX_PACKET_SIZE);
    dmx_wait_sent(dmxPort2, DMX_TIMEOUT_TICK);
  }
}
