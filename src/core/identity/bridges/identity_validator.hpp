// ==============================================================================
// TRANSCEPTOR GLOBAL: IDENTITY & ACCESS CONTROL MODULE
// MODULE: IDENTITY VALIDATOR (IDENTITY LOCK: "NO DID - NO TOKENS")
// TIMESTAMP ACCORDING TO JURISDICTIONAL COMPLIANCE: 2026-10-03T09:16:41+03:00
// ==============================================================================

#ifndef TRANSCEPTOR_IDENTITY_VALIDATOR_HPP
#define TRANSCEPTOR_IDENTITY_VALIDATOR_HPP

#include <string>
#include <unordered_set>

namespace transceptor::identity {

/**
 * @brief Класс валидации децентрализованной идентичности (DID)
 * Реализует аппаратный инвариант "Нет DID — Нет Токенов"
 */
class IdentityValidator {
private:
    std::unordered_set<std::string> verified_dids_;

public:
    IdentityValidator() = default;

    /**
     * @brief Регистрация верифицированного DID
     */
    void register_did(const std::string& did_hash) {
        if (!did_hash.empty()) {
            verified_dids_.insert(did_hash);
        }
    }

    /**
     * @brief Проверка валидности DID субъекта
     * @return true, если DID привязан и верифицирован в анклаве
     */
    [[nodiscard]] bool is_valid_did(const std::string& did_hash) const {
        if (did_hash.empty()) {
            return false;
        }
        return verified_dids_.find(did_hash) != verified_dids_.end();
    }
};

} // namespace transceptor::identity

#endif // TRANSCEPTOR_IDENTITY_VALIDATOR_HPP