# Real-time IoT + Blockchain carbon traceability system — ESP32 · Hyperledger Fabric · FastAPI · React-
🔍 The Problem Current carbon accounting methods rely on estimates from electricity bills rather than real-time sensor data — making them imprecise and easily falsifiable. Carbon Ledger addresses this directly.
# 🌿 Carbon Ledger

> **Real-time IoT + Blockchain Carbon Traceability System**  
> End-to-end cryptographic proof of industrial CO₂ emissions — from physical sensor to immutable ledger.

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://www.espressif.com/)
[![Blockchain: Hyperledger Fabric](https://img.shields.io/badge/Blockchain-Hyperledger%20Fabric-purple.svg)](https://hyperledger-fabric.readthedocs.io/)
[![Backend: FastAPI](https://img.shields.io/badge/Backend-FastAPI-teal.svg)](https://fastapi.tiangolo.com/)
[![Frontend: React](https://img.shields.io/badge/Frontend-React-61DAFB.svg)](https://reactjs.org/)
[![Made in Tunisia](https://img.shields.io/badge/Made%20in-Tunisia-E70013.svg)](https://en.wikipedia.org/wiki/Tunisia)
## 📋 Table of Contents

- [Overview](#-overview)
- [Problem Statement](#-problem-statement)
- [Architecture](#-architecture)
- [Tech Stack](#-tech-stack)
- [Key Results](#-key-results)
- [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
  - [Prerequisites](#prerequisites)
  - [1 — Deploy the Infrastructure](#1--deploy-the-infrastructure-docker)
  - [2 — Run the Backend](#2--run-the-backend)
  - [3 — Run the Frontend](#3--run-the-frontend)
  - [4 — Flash the ESP32 Firmware](#4--flash-the-esp32-firmware)
- [API Reference](#-api-reference)
- [Security Model](#-security-model)
- [Hardware Setup](#-hardware-setup)
- [ESG Compliance](#-esg-compliance)
- [Roadmap](#-roadmap)
- [Academic Context](#-academic-context)
- [License](#-license)
- ## 🌍 Overview

**Carbon Ledger** is a full-stack system that measures the real-time electricity consumption of industrial equipment, automatically calculates the corresponding CO₂ emissions, and records a cryptographic proof of every measurement on a private **Hyperledger Fabric** blockchain — making carbon data tamper-proof and auditable by any third party.

The system addresses a critical gap in current ESG practices: emissions data is often estimated from utility bills rather than measured directly, making it imprecise and unverifiable. Carbon Ledger replaces estimates with hardware-grade measurements and blockchain-grade proof.

```
Physical Measurement → ECDSA Signature → MQTT/TLS → FastAPI Backend
        → InfluxDB + PostgreSQL + Hyperledger Fabric → React Dashboard
```

---
## ❓ Problem Statement

> *"How can we guarantee the measurement, integrity, and tamper-proof traceability of industrial carbon emissions — from the physical source to the digital proof?"*

Current carbon accounting tools:
- Rely on **theoretical estimates** from electricity bills (not real-time measurement)
- Store data in **centralized systems** that can be manipulated
- Are **not natively auditable** by external parties
- Produce emissions reports for **Scope 2** that are difficult to verify

Carbon Ledger solves all four problems simultaneously.

---
## 🏗️ Architecture

The system is built on **4 independent, interoperable layers**:

```
┌─────────────────────────────────────────────────────────────────┐
│  Layer 4 — Application                                          │
│  React Dashboard  ·  REST API (FastAPI)  ·  ESG PDF Reports     │
├─────────────────────────────────────────────────────────────────┤
│  Layer 3 — Backend & Blockchain                                  │
│  FastAPI Worker  ·  InfluxDB  ·  PostgreSQL  ·  Hyperledger     │
├─────────────────────────────────────────────────────────────────┤
│  Layer 2 — Edge (ESP32)                                         │
│  CO₂ Calculation  ·  ECDSA Signing  ·  MQTT/TLS Publish        │
├─────────────────────────────────────────────────────────────────┤
│  Layer 1 — Sensors                                              │
│  PZEM-004T  →  Voltage · Current · Power · Energy (kWh)        │
└─────────────────────────────────────────────────────────────────┘
```

### Data Flow (End-to-End)

```
1. PZEM-004T measures energy every 5 seconds
2. ESP32 calculates CO₂ = energy_kWh × 0.553 kgCO₂/kWh (IEA 2022, Tunisia)
3. ESP32 signs the payload with ECDSA (secp256k1) + SHA-256
4. Signed message published on MQTT topic carbon/measures over TLS 1.2
5. FastAPI worker validates ECDSA signature
6. Data written to InfluxDB (time series) and PostgreSQL (relational)
7. SHA-256 hash recorded on Hyperledger Fabric (RecordMeasure chaincode)
8. React dashboard polls REST API every 5s for live display
9. Any auditor can verify any hash via GET /blockchain/verify/{hash}
```

---
## 🛠️ Tech Stack

| Layer | Technology | Role |
|---|---|---|
| Microcontroller | ESP32 (Espressif) | IoT edge node, Wi-Fi, dual-core |
| Power Sensor | PZEM-004T | Voltage, current, power, energy (±1%) |
| Protocol | MQTT + TLS 1.2 | Secure IoT messaging |
| Cryptography | ECDSA secp256k1 + SHA-256 | Per-measurement digital signature |
| Firmware | C++ / ESP-IDF / FreeRTOS | Embedded real-time OS |
| Blockchain | Hyperledger Fabric 2.x | Permissioned ledger, 847 TPS measured |
| Smart Contract | Go (Chaincode) | RecordMeasure / VerifyMeasure |
| Backend | Python / FastAPI | Async REST API, MQTT worker |
| Time Series DB | InfluxDB | Sensor measurements |
| Relational DB | PostgreSQL | Users, devices, sessions |
| Auth | JWT (RS256) | Stateless access + refresh tokens |
| Frontend | React 18 + Recharts | Real-time dashboard |
| Containerization | Docker / Docker Compose | Full infrastructure orchestration |
| ESG Reports | ReportLab (PDF) | ISO 14064-compliant PDF generation |

---

## 📊 Key Results

| Metric | Result | Target |
|---|---|---|
| Measurement accuracy | **< ±1%** | ±1% |
| End-to-end latency (avg) | **2.4 s** | < 5 s |
| End-to-end latency (max) | **4.1 s** | < 5 s |
| System availability (48h) | **99.7%** | > 99% |
| Forged MQTT rejection | **100%** | 100% |
| Replay attack detection | **100%** | 100% |
| Blockchain hash consistency | **100%** | 100% |
| MQTT packet loss (10,000 msgs) | **0** | 0 |
| Blockchain TPS (local infra) | **847 TPS** | — |

---
---

## 🚀 Getting Started

### Prerequisites

| Tool | Version | Install |
|---|---|---|
| Docker Desktop | 24+ | [docs.docker.com](https://docs.docker.com/get-docker/) |
| Node.js | 18+ | [nodejs.org](https://nodejs.org/) |
| Python | 3.11+ | [python.org](https://www.python.org/) |
| PlatformIO | Latest | [platformio.org](https://platformio.org/) |
| Git | Any | [git-scm.com](https://git-scm.com/) |

---

### 1 — Deploy the Infrastructure (Docker)

This single command starts Hyperledger Fabric, PostgreSQL, InfluxDB, and the Mosquitto MQTT broker:

```bash
git clone https://github.com/YOUR_USERNAME/carbon-ledger.git
cd carbon-ledger
docker-compose up -d
```

Initialize the blockchain network and deploy the chaincode:

```bash
cd blockchain/scripts
./init-network.sh        # Creates the Fabric network
./create-channel.sh      # Creates carbonchannel
./deploy-chaincode.sh    # Installs and instantiates the chaincode
```

> ⏱️ First launch takes ~3–5 minutes. Subsequent starts are under 30 seconds.

Verify everything is running:

```bash
docker-compose ps
# All services should show "Up"
```

---

### 2 — Run the Backend

```bash
cd backend
pip install -r requirements.txt
uvicorn app.main:app --reload --host 0.0.0.0 --port 8000
```

The API is now live at `http://localhost:8000`  
Interactive Swagger docs: `http://localhost:8000/docs`

The MQTT worker starts automatically with the backend and subscribes to `carbon/measures`.

---

### 3 — Run the Frontend

```bash
cd frontend
npm install
npm run dev
```

Dashboard available at `http://localhost:5173`

---

### 4 — Flash the ESP32 Firmware

```bash
cd firmware
# Edit config.h with your Wi-Fi credentials and MQTT broker IP
pio run --target upload
pio device monitor --baud 115200   # Monitor serial output
```

Expected serial output on successful startup:
```
[INFO] PZEM-004T initialized — UART2
[INFO] ECDSA key pair loaded from secure flash
[INFO] Wi-Fi connected — IP: 192.168.x.x
[INFO] MQTT connected to broker
[INFO] Publishing first measurement...
[INFO] Measurement signed and published ✓
```

---

## 📡 API Reference

| Method | Endpoint | Description |
|---|---|---|
| `GET` | `/measures` | Fetch measurements with filters (device_id, period, aggregation) |
| `GET` | `/co2/summary` | Aggregated CO₂ by period (day / week / month / year) |
| `GET` | `/blockchain/verify/{hash}` | Returns `VERIFIED` / `ALTERED` / `UNKNOWN` |
| `GET` | `/reports/esg` | Generate ISO 14064-compliant PDF report |
| `POST` | `/auth/token` | JWT authentication (returns access + refresh token) |
| `POST` | `/devices/register` | Register a new IoT device with its ECDSA public key |

Full interactive documentation: `http://localhost:8000/docs`

---

## 🔐 Security Model

Carbon Ledger applies **defense-in-depth** across all layers:

```
Device Level
  └── ECDSA key pair generated at first boot (secp256k1)
  └── Private key stored in ESP32 encrypted flash (eFuse)
  └── Every measurement individually signed

Transport Level
  └── MQTT over TLS 1.2
  └── X.509 device certificates

Backend Level
  └── ECDSA signature validation before any storage
  └── JWT authentication (RS256, access + refresh tokens)
  └── Rate limiting + CORS
  └── Full audit logs

Blockchain Level
  └── SHA-256 hash of every measurement recorded on Hyperledger Fabric
  └── Immutable ledger — modifications are cryptographically impossible
  └── Public VerifyMeasure() function for external auditors
```

**Security tests performed:**
- ✅ Forged MQTT injection → 100% rejected (ECDSA validation failure)
- ✅ Replay attack → 100% detected (hash uniqueness on Fabric)
- ✅ Hash falsification → 100% detected (`ALTERED` response)
- ✅ JWT brute force → 0 successful intrusions

---

## ⚡ Hardware Setup

### Components

| Component | Specification |
|---|---|
| Microcontroller | ESP32 DevKit (Espressif), dual-core 240 MHz, 520 KB RAM |
| Power Sensor | PZEM-004T v3.0 — measures V, A, W, kWh, PF via UART |
| Connection | UART2: ESP32 GPIO16 (RX) / GPIO17 (TX) |
| Level Shifting | 10kΩ / 20kΩ voltage divider (5V PZEM → 3.3V ESP32) |
| Power Supply | USB 5V / 500 mA (ESP32 draws ~150 mA @ 3.3V) |

### Wiring Diagram

```
PZEM-004T                          ESP32
─────────                          ─────
  TX  ──── 10kΩ ─┬──────────────── GPIO16 (UART2 RX)
                  └── 20kΩ ──── GND
  RX  ──────────────────────────── GPIO17 (UART2 TX)
  VCC ──────────────────────────── 5V
  GND ──────────────────────────── GND

AC Current Transformer → clamp on LINE wire of load
```

> ⚠️ **Safety:** The PZEM-004T operates at mains voltage on the AC side. Follow all applicable electrical safety standards. This device is for measurement only — never modify the mains wiring unless qualified.

---

## 📄 ESG Compliance

Carbon Ledger is designed with ESG regulatory requirements in mind:

- **CO₂ Emission Factor:** Tunisia national grid factor — **0.553 kgCO₂/kWh** (IEA 2022)
  - Configurable via NVS for other countries/grids
- **Standard alignment:** ISO 14064-1:2018 (GHG quantification and reporting)
- **Scope coverage:** Scope 2 emissions (purchased electricity consumption)
- **Report format:** PDF reports generated via `/reports/esg` endpoint
- **Audit trail:** Every measurement hash independently verifiable via blockchain

> The emission factor can be updated in firmware NVS without reflashing — just update the `FE_FACTOR` key via the provisioning endpoint.

---

## 🗺️ Roadmap

- [ ] **Multi-sensor deployment** — aggregate multiple ESP32 nodes per site
- [ ] **Cloud deployment** — Kubernetes on AWS / Azure / OVHcloud
- [ ] **ML anomaly detection** — detect abnormal energy patterns in time series
- [ ] **Multi-org blockchain** — extend Fabric network to include auditors and regulators
- [ ] **Mobile app** — React Native dashboard with push alerts
- [ ] **Official certification** — ISO 14064-1 and GHG Protocol Scope 2 validation

---

## 🎓 Academic Context

This project was developed as a **3_Year Engineering Project (Projet de Fin d'Études)** at:

**[Polytechnique Sousse](https://www.polytechnique.tn/)** — Electrical Engineering & Automation Department  
Academic Year: 2025–2026  
Academic Supervisor: Mr. Mohamed Amine Bouzidi  
Jury: Mr. Anis Sellami & Mr. Nadhem Sayari  
Defense Date: April 14, 2026

**Author:** Dhafer Knani — 3rd Year Electrical Engineering Student,Polytechnique Sousse

---

## 📜 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

You are free to use, modify, and distribute this project with attribution.

---

<div align="center">

**Carbon Ledger** — Making carbon data trustworthy, one hash at a time.

⭐ If this project helped you, consider giving it a star!

[Report a Bug](../../issues) · [Request a Feature](../../issues) · [Connect on LinkedIn](https://www.linkedin.com/in/dhafer-knani-b363b72a6/)

</div>
