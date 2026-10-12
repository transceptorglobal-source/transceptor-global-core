// ==============================================================================
// TRANSCEPTOR GLOBAL: INTEGRATION TEST FOR POUCC AND REGENERATION FUND DEDUCTIONS
// MODULE: TEE INFRASTRUCTURE TEST SUITE
// ==============================================================================

#include "../../../src/core/include/poucc_engine.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace Transceptor::Core;

// Функция сопоставления значений с плавающей точкой
bool is_close(double a, double b, double epsilon = 1e-5) {
    return std::fabs(a - b) < epsilon;
}

int main() {
    std::cout << "====================================================\n";
    std::cout << "[TEST SUITE] RUNNING POUCC & REGENERATION FUND TESTS\n";
    std::cout << "====================================================\n\n";

    PoUCCEngine engine(0.035); // Ставка антиэнтропии 3.5%

    // --------------------------------------------------------------------------
    // ТЕСТ 1: Углеродная когнитивная нагрузка (Human RLHF & Audit)
    // --------------------------------------------------------------------------
    std::cout << "[TEST 1] Verifying Carbon Cognitive Payload processing...\n";
    CognitiveComputePayload carbon_payload;
    carbon_payload.task_id = "TASK-AUDIT-2026-001";
    carbon_payload.author_did = "did:transceptor:zk_user_777";
    carbon_payload.layer_type = ComputeLayerType::CARBON_HUMAN_RLHF;
    carbon_payload.raw_work_units = 100.0;
    carbon_payload.entropy_delta_reduction = 500.0; // Delta S = 500
    carbon_payload.competence_coefficient = 1.5;   // K_comp = 1.5
    carbon_payload.outcome_verification_score = 0.9; // K_outcome = 0.9
    carbon_payload.zk_snark_verified = true;
    carbon_payload.tee_attestation_hash = "0x8f7a6b5c4d3e2f10987654321abcdef";

    // Вычисление:
    // Gross = 500.0 * 1.5 * 0.9 = 675.0
    // Deduction (3.5%) = 675.0 * 0.035 = 23.625
    // Net T_net = 675.0 - 23.625 = 651.375
    PoUCCExecutionResult res1 = engine.process_compute_payload(carbon_payload);

    assert(res1.is_valid == true);
    assert(is_close(res1.gross_load_tokens, 675.0));
    assert(is_close(res1.entropy_deduction_amount, 23.625));
    assert(is_close(res1.net_goods_token_issuance, 651.375));
    std::cout << "  -> PASSED: Gross=" << res1.gross_load_tokens 
              << ", Fund_Deduction=" << res1.entropy_deduction_amount 
              << ", Net_T_net=" << res1.net_goods_token_issuance << "\n\n";

    // --------------------------------------------------------------------------
    // ТЕСТ 2: Отбраковка при отсутствии ZK-доказательства
    // --------------------------------------------------------------------------
    std::cout << "[TEST 2] Verifying rejection of unverified ZK-proofs...\n";
    CognitiveComputePayload invalid_payload = carbon_payload;
    invalid_payload.zk_snark_verified = false; // Нарушение целостности

    PoUCCExecutionResult res2 = engine.process_compute_payload(invalid_payload);
    assert(res2.is_valid == false);
    std::cout << "  -> PASSED: Successfully rejected unverified payload.\n\n";

    // --------------------------------------------------------------------------
    // ТЕСТ 3: Корректность ограничения весов компетенций (std::clamp)
    // --------------------------------------------------------------------------
    std::cout << "[TEST 3] Verifying competence coefficient boundaries (clamp)...\n";
    CognitiveComputePayload clamped_payload = carbon_payload;
    clamped_payload.competence_coefficient = 10.0; // Попытка завысить K_comp выше 5.0

    PoUCCExecutionResult res3 = engine.process_compute_payload(clamped_payload);
    // K_comp обрезается до 5.0 -> Gross = 500.0 * 5.0 * 0.9 = 2250.0
    assert(res3.is_valid == true);
    assert(is_close(res3.gross_load_tokens, 2250.0));
    std::cout << "  -> PASSED: K_comp safely clamped to 5.0 max limit.\n\n";

    std::cout << "====================================================\n";
    std::cout << "[ALL TESTS PASSED SUCCESSFULLY] ALL INVARIANTS SECURED.\n";
    std::cout << "====================================================\n";

    return 0;
}