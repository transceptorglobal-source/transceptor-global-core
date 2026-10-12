# Transceptor Global — Core Architecture & Software Engine (v3.4-Adaptive)

[![USCO Registered](https://img.shields.io/badge/USCO%20Copyright-Registered-blue.svg)](https://copyright.gov)
[![License](https://img.shields.io/badge/License-Proprietary-red.svg)](#legal-notice--copyright)
[![Architecture](https://img.shields.io/badge/Architecture-TEE%20%7C%20ZK--SNARKs%20%7C%20Edge%20AI-green.svg)](#architecture-overview)

## Legal Notice & Copyright

**Copyright (c) 2026 Yevhen Volovoy. All Rights Reserved.**

* **Author & Rights Holder:** Yevhen Volovoy (Евгений Воловой)
* **Registration:** Registered with the United States Copyright Office (eCO)
* **Date of Priority:** October 3, 2026
* **Institutional Contour:** Transceptor Global Ecosystem

All source code, software specifications, mathematical models, and architectural components contained within this repository are proprietary intellectual property. Unauthorized copying, distribution, modification, or commercial exploitation is strictly prohibited without express written permission from the rights holder.

---

## Architecture Overview

Transceptor Global is an institutional, multi-layered digital platform operating on the principles of **Internal Blindness (ZK-SNARKs)**, **Hardware Isolation (TEE/Secure Enclave)**, and **Adaptive Biometric Verification**.

### Key Modules & Components
* **Core Engine (`libcore.so`):** High-performance C++ implementation of the Trust Policy Engine, ZK-Location verification, and fiscal allocation algorithms.
* **Personal AI Companion Addendum (Appendix 3.4):**
  * **Local Passport OCR & 1-to-1 Face Match:** 100% on-device document parsing and biometric verification inside TEE with zeroization (`0x00`) of temporary memory buffers.
  * **Adaptive Biometric Anchor:** Deep landmark feature representation robust against aging and facial transformations.
  * **Multi-Factor Recovery Protocol:** Fallback authentication via passport NFC-chip reading, TEE PIN/fingerprint, and Shamir's Secret Sharing (Social Recovery).
* **Dual-Contour Fiscal Split:** Automated separation of institutional capital ($W_{effective} = 0$ for corporate entities) and individual civic participation.

---

## Repository Structure
├── .gitignore # Git exclusion rules for secrets and binaries

├── .env.example # Environment variables configuration template

├── CMakeLists.txt # Primary C++ build orchestration

├── Dockerfile # Container deployment recipe

├── README.md # Repository documentation and legal notice

└── src/ # Core C++ & Python source code

---

## Build & Local Setup

### Prerequisites
* GCC 11+ or Clang 13+ with C++20 support
* CMake 3.20+
* OpenSSL development headers
* Python 3.10+ & FastAPI

### Compilation
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)