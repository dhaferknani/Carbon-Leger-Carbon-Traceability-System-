// pzem_reader.h — Lecture du PZEM-004T via Modbus RTU
#pragma once
#include <HardwareSerial.h>
#include <PZEM004Tv30.h>  // lib: dvarrel/PZEM-004T-v30

struct PZEMData {
  float voltage_v;       // Tension RMS (V)
  float current_a;       // Courant RMS (A)
  float power_w;         // Puissance active (W)
  float energy_kwh;      // Énergie cumulée (kWh)
  float frequency_hz;    // Fréquence réseau (Hz)
  float power_factor;    // Facteur de puissance
  bool  valid;           // Lecture valide ?
};

class PZEMReader {
private:
  PZEM004Tv30* _pzem;
  uint8_t _rxPin, _txPin;
  uint32_t _errorCount = 0;
  PZEMData _lastValid;

public:
  PZEMReader(uint8_t rx, uint8_t tx) : _rxPin(rx), _txPin(tx) {}

  bool begin() {
    // UART2 @ 9600 baud (default PZEM)
    Serial2.begin(9600, SERIAL_8N1, _rxPin, _txPin);
    _pzem = new PZEM004Tv30(Serial2);
    
    // Test de communication
    float v = _pzem->voltage();
    return !isnan(v);
  }

  PZEMData read() {
    PZEMData d;
    d.voltage_v    = _pzem->voltage();
    d.current_a    = _pzem->current();
    d.power_w      = _pzem->power();
    d.energy_kwh   = _pzem->energy();
    d.frequency_hz = _pzem->frequency();
    d.power_factor = _pzem->pf();

    // Validation des valeurs
    d.valid = !isnan(d.voltage_v) &&
              !isnan(d.current_a) &&
              d.voltage_v > 180.0 &&  // Plage 180-250V
              d.voltage_v < 260.0 &&
              d.current_a >= 0.0  &&
              d.power_w   >= 0.0;

    if (d.valid) {
      _lastValid = d;
      _errorCount = 0;
    } else {
      _errorCount++;
      if (_errorCount > 5) {
        Serial.println("[WARN] PZEM: 5 erreurs consécutives");
      }
    }
    return d.valid ? d : _lastValid;
  }

  uint32_t getErrorCount() { return _errorCount; }
  
  void resetEnergy() {
    _pzem->resetEnergy();
    Serial.println("[PZEM] Compteur énergie remis à zéro");
  }
};