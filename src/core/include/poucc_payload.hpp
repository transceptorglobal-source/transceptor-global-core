// ==============================================================================
// TRANSCEPTOR GLOBAL: PROOF-OF-USEFUL-COGNITIVE-COMPUTE (PoUCC) DATA STRUCTURES
// MODULE: CORE COMPUTE & ENTROPY DEDUCTION (CONTOUR 3 & 4)
// TIMESTAMP ANCHOR: 2026-09-20T13:18:39+03:00
// ==============================================================================

#ifndef TRANSCEPTOR_POUCC_PAYLOAD_HPP
#define TRANSCEPTOR_POUCC_PAYLOAD_HPP

#include <string>
#include <cstdint>

namespace Transceptor::Core {

// Тип вычислительной нагрузки: Кремниевая (Machine) vs Углеродная (Human Cognitive)
enum class ComputeLayerType {
    SILICON_MACHINE_FLOP = 0, // FLOPs дата-центров, TEE-инференс, ZK-генерация
    CARBON_HUMAN_RLHF    = 1  // RLHF-валидация, экспертный аудит, декомпиляция
};

// Структура данных когнитивно-вычислительной нагрузки
struct CognitiveComputePayload {
    std::string task_id;                 // Уникальный идентификатор задачи/аудита
    std::string author_did;              // DID-профиль исполнителя (анонимизированный ZK-хеш)
    ComputeLayerType layer_type;         // Кремний vs Углерод
    
    double raw_work_units;               // FLOPs для машин / Оценка сложности задачи для человека
    double entropy_delta_reduction;      // Полезный объем снижения системной энтропии (Delta S)
    
    double competence_coefficient;       // Исторический коэффициент компетенции (K_comp: 0.01 - 5.00)
    double outcome_verification_score;   // Коэффициент верификации результата (K_outcome: 0.00 - 1.00)
    
    bool zk_snark_verified;             // Флаг проверки ZK-доказательства
    std::string tee_attestation_hash;    // Аттестат изолированной памяти Enclave
};

// Результат расчета списания/начисления токенов и выплат
struct PoUCCExecutionResult {
    bool is_valid;                       // Успешность прохождения валидации
    double gross_load_tokens;            // Валовый объем сгенерированных токенов нагрузки
    double entropy_deduction_amount;     // Сумма автоматического отчисления в Фонд Регенерации
    double net_goods_token_issuance;     // Чистый выпуск Токенов Блага (T_net)
    std::string execution_log_marker;    // Маркер лога для TEE Ledger
};

} // namespace Transceptor::Core

#endif // TRANSCEPTOR_POUCC_PAYLOAD_HPP