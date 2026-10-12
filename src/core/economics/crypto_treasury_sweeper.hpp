// ==============================================================================
// TRANSCEPTOR GLOBAL: CRYPTO TREASURY SWEEPER & RWA VAULT ENGINE
// MODULE: CORE ECONOMICS (TSB SETTLEMENT BANK)
// REVISION: v3.4-Adaptive-Production
// JURISDICTIONAL COMPLIANCE ANCHOR: 2026-10-08T17:48:44+03:00
// ==============================================================================

#ifndef TRANSCEPTOR_CORE_ECONOMICS_CRYPTO_TREASURY_SWEEPER_HPP
#define TRANSCEPTOR_CORE_ECONOMICS_CRYPTO_TREASURY_SWEEPER_HPP

#include "fiscal_split_calculator.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

namespace transceptor {
namespace core {
namespace economics {

struct SweeperDepositRequest {
    std::string did_hash;          // W3C DID хэш резидента
    double deposit_amount_usdt;    // Сумма депозита в USDT/USD
    int term_years;                // Срок размещения (от 1 до 3 лет)
    bool has_valid_did;            // Бинарная проверка W3C DID
};

struct SweeperExecutionResult {
    bool success;
    std::string error_code;
    std::string status_message;
    double gross_deposit_usd;
    double net_cleared_usd;
    double inbound_protocol_fee;
    double spv_bank_margin;
    double annual_fee_deducted;
    bool capital_immunity_applied;
    bool requires_tbills_sweep;    // Флаг выкупа казначейских векселей при накопителе >= $100,000
    double accumulative_vault_balance;
};

class CryptoTreasurySweeper {
private:
    double current_vault_balance_usd;
    double total_tbills_purchased_usd;

public:
    // Порог автоматического выкупа суверенных казначейских векселей (T-Bills)
    static constexpr double TBILLS_SWEEP_THRESHOLD_USD = 100000.0;

    CryptoTreasurySweeper() 
        : current_vault_balance_usd(0.0), total_tbills_purchased_usd(0.0) {}

    // Обработка депозита с занулением нулевого доверия ("Нет DID — Нет Токенов")
    SweeperExecutionResult process_deposit(const SweeperDepositRequest& request, double expected_annual_yield = 0.048) {
        SweeperExecutionResult res{};
        res.gross_deposit_usd = request.deposit_amount_usdt;

        // 1. Нулевое доверие (Zero-Trust Guard): "Нет DID — Нет Токенов"
        if (!request.has_valid_did || request.did_hash.empty()) {
            res.success = false;
            res.error_code = "AUTH_ERR_NO_DID";
            res.status_message = "CRITICAL: Zero-Trust Policy Enforced. Deposit rejected: Missing or unverified W3C DID profile.";
            return res;
        }

        // 2. Расчет въездного сплита Контура Б (3.50%)
        InboundSplitResult inbound = FiscalSplitCalculator::calculate_inbound_split(
            request.deposit_amount_usdt, 
            SettlementContour::CONTOUR_B_TNET
        );

        res.net_cleared_usd = inbound.net_cleared_amount;
        res.inbound_protocol_fee = inbound.protocol_share;
        res.spv_bank_margin = inbound.spv_bank_margin;

        // 3. Расчет Капитального Иммунитета и годовой подписки ($20/год vs $0/год)
        DepositImmunityResult deposit_terms = FiscalSplitCalculator::evaluate_deposit_terms(
            request.deposit_amount_usdt,
            request.term_years,
            expected_annual_yield
        );

        res.capital_immunity_applied = deposit_terms.capital_immunity_active;
        res.annual_fee_deducted = deposit_terms.annual_fee_deducted_usd;

        // 4. Пополнение казначейского накопителя TSB
        current_vault_balance_usd += res.net_cleared_usd;
        res.accumulative_vault_balance = current_vault_balance_usd;

        // 5. Пороговый триггер выкупа T-Bills ($100,000)
        if (current_vault_balance_usd >= TBILLS_SWEEP_THRESHOLD_USD) {
            res.requires_tbills_sweep = true;
            total_tbills_purchased_usd += current_vault_balance_usd;
            res.status_message = "SUCCESS: Deposit cleared. T-Bills Sweep Triggered (" + 
                                 std::to_string(current_vault_balance_usd) + " USD -> Sovereign Bonds).";
            current_vault_balance_usd = 0.0; // Сброс накопителя после выкупа RWA
        } else {
            res.requires_tbills_sweep = false;
            res.status_message = "SUCCESS: Deposit cleared. Capital held in TSB Escrow Vault.";
        }

        res.success = true;
        res.error_code = "OK";
        return res;
    }

    double get_current_vault_balance() const { return current_vault_balance_usd; }
    double get_total_tbills_purchased() const { return total_tbills_purchased_usd; }
};

} // namespace economics
} // namespace core
} // namespace transceptor

#endif // TRANSCEPTOR_CORE_ECONOMICS_CRYPTO_TREASURY_SWEEPER_HPP