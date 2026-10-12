#pragma once
#include <cstdint>
#include <string>

namespace transceptor::core::engine::trust_policy {

/**
 * @brief Структура оценки инициативы оракулом премодерации Агоры.
 */
struct ProposalEvaluationResult {
    bool is_passed{false};
    double law_score{0.0};
    double fin_score{0.0};
    double eco_score{0.0};
    bool deepfake_fraud_detected{false};
    std::string evaluation_summary;
};

/**
 * @brief C++ фильтр премодерации гражданских инициатив и проектов Agora AI Oracle.
 * Проверяет нормативные пороги и защищает платформу от фрода и дипфейков.
 */
class AgoraAiOracleFilter {
public:
    static constexpr double LAW_THRESHOLD = 0.85; // Нормативно-правовой порог
    static constexpr double FIN_THRESHOLD = 0.80; // Финансово-экономический порог
    static constexpr double ECO_THRESHOLD = 0.75; // Экологический/технологический порог

    AgoraAiOracleFilter() = default;
    ~AgoraAiOracleFilter() = default;

    /**
     * @brief Проверка параметров и текстового вектора инициативы.
     * @param proposal_id Хэш идентификатора инициативы.
     * @param proposal_text_raw Текст предложения или сметы ТЭО.
     * @param author_did_hash Хэш DID автора предложения.
     * @return Результат премодерации ProposalEvaluationResult.
     */
    static ProposalEvaluationResult evaluate_proposal(
        uint64_t proposal_id,
        const std::string& proposal_text_raw,
        uint64_t author_did_hash
    ) noexcept;
};

} // namespace transceptor::core::engine::trust_policy