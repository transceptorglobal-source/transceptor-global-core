#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace transceptor::core::engine::trust_policy {

/**
 * @brief Пакет аудита финансовой транзакции клиринга для TPE-анклава.
 */
struct TransactionAuditPayload {
    uint64_t sender_did_hash{0};
    uint64_t recipient_did_hash{0};
    double amount_usd{0.0};
    double k_entropy{0.0};
    bool has_valid_did{false};
    int64_t timestamp_ms{0};
};

/**
 * @brief Результат аудита политики доверия и проверки метрики Psi_TPE.
 */
struct PolicyAuditResult {
    bool is_approved{false};
    double psi_tpe_score{0.0};
    bool policy_breach_detected{false};
    bool zeroization_triggered{false};
    std::string audit_message;
};

/**
 * @brief Аппаратно-изолированный модуль доверия (Trust Policy Engine — TPE).
 * Осуществляет потоковый аудит транзакций и контроль экстренного зануления памяти.
 */
class TrustPolicyEngine {
public:
    TrustPolicyEngine() = default;
    ~TrustPolicyEngine() = default;

    /**
     * @brief Оценка транзакции на соответствие инвариантам Psi_TPE.
     */
    PolicyAuditResult evaluate_transaction(const TransactionAuditPayload& payload) noexcept;

    /**
     * @brief Экстренный вызов протокола зануления ОЗУ при аппаратном или сетевом сбое.
     */
    void initiate_zeroization_protocol() noexcept;

    /**
     * @brief Проверка текущего состояния TEE-анклава.
     */
    bool is_enclave_active() const noexcept;

private:
    bool enclave_active_{true};
};

} // namespace transceptor::core::engine::trust_policy