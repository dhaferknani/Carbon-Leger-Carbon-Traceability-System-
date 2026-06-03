// ═══════════════════════════════════════════════════════
// Carbon Ledger — main.cpp
// ESP32 · Firmware v1.0.3
// Polytechnique Sousse
// ═══════════════════════════════════════════════════════

#include "config.h"
#include "pzem_reader.h"
#include "co2_calc.h"
#include "crypto_sign.h"
#include "mqtt_client.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>  // NVS (flash sécurisée)

// ── Objets globaux ──────────────────────────────────
PZEMReader  pzem(16, 17);      // RX2=16, TX2=17
CO2Calculator co2Calc;
CryptoSigner  signer;
MQTTClient   mqttClient;
Preferences  prefs;

float totalEnergyKwh = 0.0;
float totalCO2Kg = 0.0;
unsigned long lastSampleMs = 0;

// ── setup() ─────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println("═══ Carbon Ledger v1.0.3 ═══");

  // Charger config depuis NVS
  prefs.begin("clconfig", false);
  
  // Init PZEM-004T
  if (!pzem.begin()) {
    Serial.println("[ERR] PZEM-004T non détecté!");
    while(1) delay(1000); // Halt
  }

  // Wi-Fi
  connectWiFi();

  // Charger clé ECDSA depuis NVS
  if (!signer.loadKey(prefs)) {
    Serial.println("[CRYPTO] Génération nouvelle clé...");
    signer.generateKey(prefs);
  }

  // MQTT
  mqttClient.begin(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(onMQTTMessage);
  mqttClient.subscribe("carbon/v1/config");
  mqttClient.subscribe("carbon/v1/reset");

  Serial.println("[OK] Système prêt. Échantillonnage toutes " 
               + String(SAMPLE_INTERVAL_MS/1000) + "s");
}

// ── loop() ──────────────────────────────────────────
void loop() {
  mqttClient.loop();  // Maintenir connexion MQTT

  if (millis() - lastSampleMs >= SAMPLE_INTERVAL_MS) {
    lastSampleMs = millis();
    takeMeasurement();
  }
}

// ── takeMeasurement() ────────────────────────────────
void takeMeasurement() {
  // 1. Lire le capteur
  PZEMData data = pzem.read();
  if (!data.valid) {
    Serial.println("[WARN] Lecture PZEM invalide, skip");
    return;
  }

  // 2. Accumuler énergie
  totalEnergyKwh += (data.power_w / 3600000.0) * SAMPLE_INTERVAL_MS;

  // 3. Calculer CO₂
  float co2Delta = co2Calc.compute(data.energy_kwh, EMISSION_FACTOR);
  totalCO2Kg += co2Delta;

  // 4. Construire payload JSON
  StaticJsonDocument<512> doc;
  doc["device_id"]  = DEVICE_ID;
  doc["timestamp"]  = getISO8601();
  doc["voltage_v"]  = data.voltage_v;
  doc["current_a"]  = data.current_a;
  doc["power_w"]    = data.power_w;
  doc["energy_kwh"] = totalEnergyKwh;
  doc["co2_kg"]     = totalCO2Kg;
  doc["pf"]         = data.power_factor;
  doc["freq_hz"]   = data.frequency_hz;

  // 5. Signer le hash
  String payload; serializeJson(doc, payload);
  String hash = signer.sha256(payload);
  String sig  = signer.sign(hash);
  doc["hash_sha256"] = hash;
  doc["signature"]  = sig;
  doc["fw_version"] = FW_VERSION;

  // 6. Publier MQTT
  String finalPayload; serializeJson(doc, finalPayload);
  mqttClient.publish("carbon/v1/measures", finalPayload, true);

  Serial.printf("[OK] V=%.2fV I=%.3fA P=%.1fW CO2=%.3fg\n",
    data.voltage_v, data.current_a, data.power_w, totalCO2Kg * 1000);
}