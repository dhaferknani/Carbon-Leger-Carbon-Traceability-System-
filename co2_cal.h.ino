// co2_calc.h — Calcul des émissions CO₂ (Scope 2)
#pragma once

// Facteurs d'émission par pays (kgCO2/kWh) - Source: IEA 2022
const float FE_TUNISIE  = 0.553;   // STEG 2022
const float FE_FRANCE   = 0.059;   // RTE 2022 (nucléaire)
const float FE_MAROC    = 0.684;   // ONE 2022
const float FE_MONDE    = 0.491;   // IEA World avg

class CO2Calculator {
private:
  double _totalKwh = 0.0;
  double _totalCO2Kg = 0.0;
  float  _fe;  // Facteur d'émission actif

public:
  CO2Calculator(float fe = FE_TUNISIE) : _fe(fe) {}

  // Calcule CO₂ pour un delta d'énergie
  float compute(float deltaKwh, float fe = -1.0) {
    if (fe < 0) fe = _fe;
    float co2Delta = deltaKwh * fe;  // kg CO₂
    _totalKwh  += deltaKwh;
    _totalCO2Kg += co2Delta;
    return co2Delta;
  }

  // Getters
  double getTotalKwh()   { return _totalKwh; }
  double getTotalCO2Kg() { return _totalCO2Kg; }
  float  getEmissionFactor() { return _fe; }
  
  void setEmissionFactor(float newFE) {
    Serial.printf("[CO2] FE mis à jour: %.3f → %.3f kgCO2/kWh\n", _fe, newFE);
    _fe = newFE;
  }

  // Calcule le débit CO₂ instantané
  float getRateGPerMin(float powerW) {
    // P(W) → kWh/min → kgCO2/min → gCO2/min
    return (powerW / 60000.0) * _fe * 1000.0;
  }

  void reset() { _totalKwh = 0.0; _totalCO2Kg = 0.0; }

  // Vérification plausibilité (sanity check)
  bool isSane(float deltaKwh) {
    // Max 10 kW sur 5s = 0.0139 kWh max raisonnable
    return deltaKwh >= 0.0 && deltaKwh < 0.05;
  }
};