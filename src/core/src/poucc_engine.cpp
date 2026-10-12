// ==============================================================================
// TRANSCEPTOR GLOBAL: PoUCC CLEARING IMPLEMENTATION
// MODULE: CORE COMPUTE ENGINE (CONTOUR 3 & 4)
// ==============================================================================

#include "../include/poucc_engine.hpp"
#include <iostream>

namespace Transceptor::Core {

// Вспомогательный служебный метод для логирования аттестаций TEE
void log_poucc_execution(const PoUCCExecutionResult& result) {
    std::cout << "[TEE_LOG] Status: " << (result.is_valid ? "VALID" : "INVALID") << "\n"
              << "[TEE_LOG] Gross Load Tokens: " << result.gross_load_tokens << "\n"
              << "[TEE_LOG] Entropy Fund (Regeneration) Deduction: " << result.entropy_deduction_amount << "\n"
              << "[TEE_LOG] Net Goods Token (T_net) Issued: " << result.net_goods_token_issuance << "\n"
              << "[TEE_LOG] Marker: " << result.execution_log_marker << std::endl;
}

} // namespace Transceptor::Core