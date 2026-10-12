# Contributing Guidelines — Transceptor Global Core

Thank you for your interest in contributing to the **Transceptor Global** core ecosystem. As an institutional-grade platform operating on Confidential Computing (TEE), Zero-Knowledge Proofs (ZK-SNARKs), and Edge AI, we maintain rigorous standards for code quality, architectural security, and legal compliance.

---

## 1. Intellectual Property & Legal Notice

* **Copyright & Ownership:** All rights to the Transceptor Global core architecture, mathematical models, and software specifications belong exclusively to **Yevhen Volovoy** (Registered with the United States Copyright Office, eCO Priority Date: October 3, 2026).
* **Contributor License Agreement (CLA):** By submitting a Pull Request (PR) or issue, you agree that your contribution is provided in compliance with the repository's intellectual property framework and does not violate third-party patents or copyrights.

---

## 2. Security & Zero-Trust Mandates

All submitted code must strictly adhere to our core security invariants:

* **No Hardcoded Secrets:** Never commit API keys, private keys, certificates, or `.env` files. All configuration variables must be injected via environment variables or encrypted TEE containers.
* **Internal Blindness Protocol:** Any code handling User PII or biometric vectors must execute 100% locally inside hardware-isolated Enclaves (Intel SGX, AMD SEV-SNP, ARM TrustZone) with atomic memory zeroization (`0x00`).
* **Cryptographic Proofs Verification:** All state changes in the value or governance contours must be backed by non-interactive zero-knowledge proofs (ZK-SNARKs) or hardware remote attestation reports.

---

## 3. Technology Stack & Coding Standards

### C++ Core (`src/core/`)
* **Standard:** C++20 / C++17 (`-std=c++20`).
* **Guidelines:**
  * Use modern RAII, `std::option`, and explicit memory management.
  * Avoid raw pointers; prefer `std::unique_ptr` and `std::shared_ptr`.
  * Ensure all cryptographic operations operate in constant-time to prevent Side-Channel timing attacks.

### Python Services (`src/oracles/`)
* **Runtime:** Python 3.10+ with FastAPI and ONNX Runtime.
* **Guidelines:**
  * Use strict type hinting (`pydantic`, `typing`).
  * Ensure asynchronous handlers (`async/await`) are used for non-blocking I/O.

---

## 4. Development & PR Workflow

1. **Fork & Branch:** Create a feature branch from `main`:
   ```bash
   git checkout -b feature/your-feature-name
Local Testing: Ensure all integration tests pass before submitting:
mkdir -p build && cd build
cmake ..
make -j$(nproc)
./integration_test
Commit Messages: Follow clean, descriptive commit formatting:
feat(tee): add remote attestation validator
fix(clearing): resolve floating point drift in fiscal split
docs(api): update OpenAPI schemas for semantic oracles
Submit Pull Request: Push your branch and open a PR against main. Provide a clear description of changes, architectural impact, and test coverage.
5. Reporting Security Vulnerabilities
If you discover a security flaw, timing vulnerability, or memory leak within the TEE enclave boundary, do not open a public issue. Please contact security directly at: security@transceptor.org.

---

### Команды для отправки в GitHub

После создания файла **`CONTRIBUTING.md`** откройте **PowerShell** в папке проекта и выполните 3 команды:

```powershell
git add CONTRIBUTING.md
git commit -m "Add CONTRIBUTING.md guidelines for developer ecosystem"
git push origin main