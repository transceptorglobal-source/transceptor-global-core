// ==============================================================================
// TRANSCEPTOR GLOBAL: DID COMPETENCE PASSPORT & GOVERNANCE KERNEL
// MODULE: COMPETENCE PASSPORT MANAGER ($I_cp$, $V^2$, SHAMIR RECOVERY)
// TIMESTAMP ACCORDING TO JURISDICTIONAL COMPLIANCE: 2026-10-03T11:58:21+03:00
// ==============================================================================

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>

namespace transceptor::governance {

struct CompetenceProfile {
    std::string did_hash;
    double electoral_weight = 0.01; // Начальный квант (коридор [0.01; 5.0])
    double sprint_accumulated_delta = 0.0; // Максимум +0.05 за спринт
    bool is_sanctioned = false;
};

class CompetencePassportManager {
private:
    constexpr static double MIN_ELECTORAL_WEIGHT = 0.01;
    constexpr static double MAX_ELECTORAL_WEIGHT = 5.00;
    constexpr static double MAX_SPRINT_INCREMENT = 0.05; // Жесткий лимит std::clamp

public:
    CompetencePassportManager() = default;

    /**
     * @brief Динамическое начисление кванта вклада за подтвержденные ORCID/Scopus/GitHub аттестаты
     */
    void add_contribution_quantum(CompetenceProfile& profile, double quantum_delta) {
        if (profile.is_sanctioned || quantum_delta <= 0.0) return;

        // Ограничение электоральной инфляции std::clamp на уровне +0.05 за спринт
        double allowed_delta = std::clamp(quantum_delta, 0.0, MAX_SPRINT_INCREMENT - profile.sprint_accumulated_delta);
        profile.sprint_accumulated_delta += allowed_delta;

        profile.electoral_weight = std::clamp(
            profile.electoral_weight + allowed_delta, 
            MIN_ELECTORAL_WEIGHT, 
            MAX_ELECTORAL_WEIGHT
        );
    }

    /**
     * @brief Протокол санкций: Мгновенное обрушение веса голоса при обнаружении фрода
     */
    void apply_fraud_sanction(CompetenceProfile& profile) {
        profile.electoral_weight = MIN_ELECTORAL_WEIGHT;
        profile.is_sanctioned = true;
        std::cout << "[SANCTION_APPLIED] DID electoral weight collapsed to 0.01 due to fraud detection." << std::endl;
    }

    /**
     * @brief Расчет стоимости голосования V^2 (Квадратичное подавление лоббизма)
     * Стоимость закупки V голосов = V^2
     */
    [[nodiscard]] double calculate_quadratic_voting_cost(uint32_t votes_count) const {
        return std::pow(static_cast<double>(votes_count), 2.0);
    }
};

} // namespace transceptor::governance