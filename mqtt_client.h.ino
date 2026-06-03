// config.h — Configuration centrale du firmware
#pragma once

// ── Identité du dispositif ───────────────────────────
#define DEVICE_ID          "ESP32-CLedger-A3F7B291"
#define FW_VERSION         "1.0.3"
#define PROJECT_NAME       "Carbon Ledger"

// ── Wi-Fi ────────────────────────────────────────────
#define WIFI_SSID          "Carbon-Lab"
#define WIFI_PASSWORD      "*****"  // À changer!
#define WIFI_TIMEOUT_MS    15000

// ── MQTT ─────────────────────────────────────────────
#define MQTT_BROKER        "broker.hivemq.com"
#define MQTT_PORT          8883  // TLS
#define MQTT_USER          "carbonledger"
#define MQTT_PASS          "*****"

// ── Topics MQTT ──────────────────────────────────────
#define TOPIC_MEASURES     "carbon/v1/measures"
#define TOPIC_STATUS       "carbon/v1/status"
#define TOPIC_CONFIG       "carbon/v1/config"
#define TOPIC_RESET        "carbon/v1/reset"
#define TOPIC_ACK          "carbon/v1/ack"

// ── Capteur ──────────────────────────────────────────
#define PZEM_RX_PIN        16   // GPIO16 → UART2 RX
#define PZEM_TX_PIN        17   // GPIO17 → UART2 TX
#define PZEM_BAUD          9600

// ── Mesures ──────────────────────────────────────────
#define SAMPLE_INTERVAL_MS 5000    // Intervalle (ms)
#define VOLTAGE_MIN        180.0f  // Seuil bas (V)
#define VOLTAGE_MAX        260.0f  // Seuil haut (V)
#define MAX_POWER_W        10000   // Max 10kW

// ── CO₂ (IEA 2022) ──────────────────────────────────
#define EMISSION_FACTOR    0.553f  // kgCO₂/kWh — Tunisie STEG

// ── LED status ───────────────────────────────────────
#define LED_PIN            2
// LED: OFF=erreur, blink lent=WiFi conn, blink rapide=mesure

// ── Bibliothèques requises (platformio.ini) ──────────
// lib_deps =
//   dvarrel/PZEM-004T-v30@^1.1.2
//   knolleary/PubSubClient@^2.8.0
//   bblanchon/ArduinoJson@^7.0.0
//   adafru