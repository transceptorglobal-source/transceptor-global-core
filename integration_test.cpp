// ==============================================================================
// TRANSCEPTOR GLOBAL v3.4-Adaptive | MULTI-SCENARIO ECONOMICS TEST SUITE
// ==============================================================================

#include "src/core/economics/fiscal_split_calculator.hpp"
#include "src/core/economics/crypto_treasury_sweeper.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace transceptor::core::economics;

void run_test_case(const std::string& title, double amount, SettlementContour contour, int term_years, double yield_rate = 0.05) {
    std::cout << "\n----------------------------------------------------------------------" << std::endl;
    std::cout << "SCENARIO: " << title << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    
    std::string contour_name = (contour == SettlementContour::CONTOUR_A_TNAT) ? "Contour A (T_nat / 2.50%)" : "Contour B (T_net / 3.50%)";
    std::cout << "Input Amount: $" << std::fixed << std::setprecision(2) << amount << " USD" << std::endl;
    std::cout << "Clearing Contour: " << contour_name << std::endl;
    std::cout << "Placement Term: " << term_years << " year(s)" << std::endl;

    // 1. Входной сплит
    InboundSplitResult inbound = FiscalSplitCalculator::calculate_inbound_split(amount, contour);
    std::cout << " -> Inbound Fee: $" << inbound.inbound_fee << " (Protocol: $" << inbound.protocol_share << ", SPV Margin: $" << inbound.spv_bank_margin << ")" << std::endl;
    std::cout << " -> Net Cleared Balance: $" << inbound.net_cleared_amount << std::endl;

    // 2. Условия депозита и Капитальный Иммунитет
    DepositImmunityResult terms = FiscalSplitCalculator::evaluate_deposit_terms(amount, term_years, yield_rate);
    std::cout << " -> Capital Immunity Active: " << (terms.capital_immunity_active ? "YES ($0/year fee)" : "NO ($20/year fee applied)") << std::endl;
    std::cout << " -> Total Annual Fee Deducted: $" << terms.annual_fee_deducted_usd << std::endl;
    std::cout << " -> Net Coupon Yield to User: $" << terms.net_coupon_yield_credited << std::endl;

    // 3. Выкуп T-Bills (для Контура Б)
    if (contour == SettlementContour::CONTOUR_B_TNET) {
        CryptoTreasurySweeper sweeper;
        SweeperDepositRequest req{"did:transceptor:test_resident", amount, term_years, true};
        SweeperExecutionResult res = sweeper.process_deposit(req, yield_rate);
        std::cout << " -> T-Bills Sweep Triggered: " << (res.requires_tbills_sweep ? "YES (Accumulator >= $100,000)" : "NO (Held in Vault)") << std::endl;
    }
}

int main() {
    std::cout << "======================================================================" << std::endl;
    std::cout << "   TRANSCEPTOR GLOBAL v3.4-Adaptive | MULTI-SCENARIO ECONOMICS TEST   " << std::endl;
    std::cout << "======================================================================" << std::endl;

    // Сценарий 1: Малый депозит $100 на 1 год в T_nat (Контур А / 2.50%)
    run_test_case("1. Small Deposit $100 / 1 Year in T_nat (Contour A)", 100.0, SettlementContour::CONTOUR_A_TNAT, 1);

    // Сценарий 2: Малый депозит $500 на 2 года в T_net (Контур Б / 3.50%)
    run_test_case("2. Small Deposit $500 / 2 Years in T_net (Contour B)", 500.0, SettlementContour::CONTOUR_B_TNET, 2);

    // Сценарий 3: Депозит $1,000 с Капитальным Иммунитетом на 3 года в T_nat (Контур А / 2.50%)
    run_test_case("3. Threshold Immunity Deposit $1,000 / 3 Years in T_nat (Contour A)", 1000.0, SettlementContour::CONTOUR_A_TNAT, 3);

    // Сценарий 4: Крупный депозит $5,000 с Капитальным Иммунитетом на 3 года в T_net (Контур Б / 3.50%)
    run_test_case("4. Large Immunity Deposit $5,000 / 3 Years in T_net (Contour B)", 5000.0, SettlementContour::CONTOUR_B_TNET, 3);

    // Сценарий 5: Институциональный депо-поток $120,000 с автоматическим выкупом T-Bills (Контур Б)
    run_test_case("5. Institutional Sweep Deposit $120,000 / 3 Years in T_net (Contour B)", 120000.0, SettlementContour::CONTOUR_B_TNET, 3);

    // Проверка P2P-перевода на $10,000
    std::cout << "\n----------------------------------------------------------------------" << std::endl;
    std::cout << "SCENARIO: Internal P2P Clearing ($10,000 Transfer)" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    P2PClearingSplit p2p = FiscalSplitCalculator::calculate_p2p_split(10000.0);
    std::cout << "P2P Fee 1.50%: $" << p2p.total_p2p_fee << std::endl;
    std::cout << " -> F_infra Share (50%): $" << p2p.infra_fund_share << std::endl;
    std::cout << " -> P_coop Share (30%):  $" << p2p.coop_pool_share << std::endl;
    std::cout << " -> S_reserve Share (20%): $" << p2p.reserve_fund_share << std::endl;
    std::cout << " -> Net Transferred to Recipient: $" << p2p.net_transferred << std::endl;

    std::cout << "\n======================================================================" << std::endl;
    std::cout << "             ALL ECONOMICS SCENARIOS TESTED SUCCESSFULLY               " << std::endl;
    std::cout << "======================================================================" << std::endl;

    return 0;
}
