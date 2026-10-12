// ==============================================================================
// TRANSCEPTOR GLOBAL: PoUCC CLEARING AND REGENERATION ENGINE
// MODULE: CORE COMPUTE ENGINE (CONTOUR 3 & 4)
// ==============================================================================

#ifndef TRANSCEPTOR_POUCC_ENGINE_HPP
#define TRANSCEPTOR_POUCC_ENGINE_HPP

#include "poucc_payload.hpp"
#include <algorithm>
#include <cmath>

namespace Transceptor::Core {

class PoUCCEngine {
public:
    // Конструктор принимающий баловый процент антиэнтропийных отчислений (по умолчанию 3.5%)
    explicit PoUCCEngine(double base_entropy_rate = 0.035) 
        : k_entropy_rate_(base_entropy_rate) {}

    // Вычисление чистой эмиссии T_net и взноса в Фонд Регенерации
    PoUCCExecutionResult process_compute_payload(const CognitiveComputePayload& payload) {
        PoUCCExecutionResult result;
        result.is_valid = false;

        // 1. Проверка аттестата TEE и ZK-доказательства
        if (!payload.zk_snark_verified || payload.tee_attestation_hash.empty()) {
            result.execution_log_marker = "[ERROR] Invalid ZK-Proof or TEE Attestation Hash.";
            return result;
        }

        // 2. Валидация диапазонов коэффициентов
        double clamped_comp = std::clamp(payload.competence_coefficient, 0.01, 5.0);
        double clamped_outcome = std::clamp(payload.outcome_verification_score, 0.0, 1.0);

        // 3. Вычисление валового объема нагрузки: Work * K_comp * K_outcome
        double gross_tokens = payload.entropy_delta_reduction * clamped_comp * clamped_outcome;

        // 4. Расчет отчисления в Фонд Регенерации (K_entropy)
        double entropy_deduction = gross_tokens * k_entropy_rate_;
        double net_issuance = gross_tokens - entropy_deduction;

        // 5. Заполнение структуры результатов
        result.is_valid = true;
        result.gross_load_tokens = gross_tokens;
        result.entropy_deduction_amount = entropy_deduction;
        result.net_goods_token_issuance = net_issuance;
        result.execution_log_marker = "[SUCCESS] PoUCC Verified. Entropy Fund Deducted successfully.";

        return result;
    }

    // Обновление базовой ставки антиэнтропии через решение Контура Управления
    void set_entropy_deduction_rate(double new_rate) {
        k_entropy_rate_ = std::clamp(new_rate, 0.01, 0.15); // Ограничение ставки в коридоре 1% - 15%
    }

    double get_current_entropy_rate() const {
        return k_entropy_rate_;
    }

private:
    double k_entropy_rate_; // Коэффициент K_entropy
};

} // namespace Transceptor::Core

#endif // TRANSCEPTOR_POUCC_ENGINE_HPP