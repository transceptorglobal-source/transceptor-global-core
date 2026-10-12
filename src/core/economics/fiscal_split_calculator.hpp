// ==============================================================================
// TRANSCEPTOR GLOBAL: FISCAL SPLIT & TREASURY CLEARING CALCULATOR
// MODULE: CORE ECONOMICS (CONTOUR 1 & CONTOUR 3)
// REVISION: v3.4-Adaptive-Production
// JURISDICTIONAL COMPLIANCE ANCHOR: 2026-10-08T17:43:41+03:00
// ==============================================================================

#ifndef TRANSCEPTOR_CORE_ECONOMICS_FISCAL_SPLIT_CALCULATOR_HPP
#define TRANSCEPTOR_CORE_ECONOMICS_FISCAL_SPLIT_CALCULATOR_HPP

#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>

namespace transceptor {
namespace core {
namespace economics {

enum class SettlementContour {
    CONTOUR_A_TNAT, // B2B / Региональный фиатный клиринг (2.50%)
    CONTOUR_B_TNET  // Глобальный институциональный DLT-клиринг (3.50%)
};

struct InboundSplitResult {
    double total_amount;
    double inbound_fee;
    double protocol_share;
    double spv_bank_margin;
    double net_cleared_amount;
};

struct P2PClearingSplit {
    double total_p2p_fee;       // 1.50% от объема внутреннего перевода
    double infra_fund_share;    // 50% от комиссии (0.75%) -> F_infra (амортизация TEE/GPU, PoUCC)
    double coop_pool_share;     // 30% от комиссии (0.45%) -> P_coop (АХД и ликвидность МПК)
    double reserve_fund_share;  // 20% от комиссии (0.30%) -> S_reserve (Резервный фонд регенерации)
    double net_transferred;     // Чистая сумма получателю
};

struct DepositImmunityResult {
    double initial_deposit_usd;
    int placement_term_years;        // Ограничение: от 1 до 3 лет
    bool capital_immunity_active;   // true при депозите >= $1000
    double annual_fee_deducted_usd; // $0/год при иммунитете, иначе $20/год
    double entry_fee_usd;
    double net_coupon_yield_credited;
    bool is_valid_term;              // Ограничение срока (максимум 3 года)
};

class FiscalSplitCalculator {
public:
    // --- ВЪЕЗДНЫЕ СПЛИТЫ (v3.4-Adaptive) ---
    static constexpr double INBOUND_FEE_CONTOUR_A = 0.0250;   // 2.50% (Контур А / T_nat)
    static constexpr double PROTOCOL_SHARE_CONTOUR_A = 0.0200;// 2.00% Протокол
    static constexpr double SPV_MARGIN_CONTOUR_A = 0.0050;   // 0.50% Маржа банка SPV 51/49

    static constexpr double INBOUND_FEE_CONTOUR_B = 0.0350;   // 3.50% (Контур Б / T_net)
    static constexpr double PROTOCOL_SHARE_CONTOUR_B = 0.0300;// 3.00% Протокол и RWA-буфер
    static constexpr double SPV_MARGIN_CONTOUR_B = 0.0050;   // 0.50% Маржа банка SPV 51/49

    // --- ВНУТРЕННИЙ P2P-КЛИРИНГ (1.50%) ---
    static constexpr double P2P_FEE_RATE = 0.0150;           // 1.50%
    static constexpr double P2P_INFRA_RATIO = 0.50;          // 50% в F_infra
    static constexpr double P2P_COOP_RATIO = 0.30;           // 30% в P_coop
    static constexpr double P2P_RESERVE_RATIO = 0.20;        // 20% в S_reserve

    // --- ЗАЛОГОВАЯ МОДЕЛЬ И КАПИТАЛЬНЫЙ ИММУНИТЕТ ---
    static constexpr double SOULBOUND_ANNUAL_FEE_USD = 20.0;
    static constexpr double CAPITAL_IMMUNITY_THRESHOLD_USD = 1000.0;
    static constexpr int MIN_PLACEMENT_TERM_YEARS = 1;
    static constexpr int MAX_PLACEMENT_TERM_YEARS = 3;       // Ограничение риска 3 годами

    // Расчет въездного фискального сплита
    static InboundSplitResult calculate_inbound_split(double amount, SettlementContour contour) {
        InboundSplitResult result{};
        result.total_amount = amount;

        if (contour == SettlementContour::CONTOUR_A_TNAT) {
            result.inbound_fee = amount * INBOUND_FEE_CONTOUR_A;
            result.protocol_share = amount * PROTOCOL_SHARE_CONTOUR_A;
            result.spv_bank_margin = amount * SPV_MARGIN_CONTOUR_A;
        } else {
            result.inbound_fee = amount * INBOUND_FEE_CONTOUR_B;
            result.protocol_share = amount * PROTOCOL_SHARE_CONTOUR_B;
            result.spv_bank_margin = amount * SPV_MARGIN_CONTOUR_B;
        }

        result.net_cleared_amount = amount - result.inbound_fee;
        return result;
    }

    // Расчет P2P расщепления комиссии 1.50% (50/30/20)
    static P2PClearingSplit calculate_p2p_split(double transfer_amount) {
        P2PClearingSplit split{};
        split.total_p2p_fee = transfer_amount * P2P_FEE_RATE;
        split.infra_fund_share = split.total_p2p_fee * P2P_INFRA_RATIO;
        split.coop_pool_share = split.total_p2p_fee * P2P_COOP_RATIO;
        split.reserve_fund_share = split.total_p2p_fee * P2P_RESERVE_RATIO;
        split.net_transferred = transfer_amount - split.total_p2p_fee;
        return split;
    }

    // Расчет условий RWA-депонирования и Капитального Иммунитета (срок от 1 до 3 лет)
    static DepositImmunityResult evaluate_deposit_terms(double deposit_usd, int term_years, double expected_annual_yield_rate) {
        DepositImmunityResult res{};
        res.initial_deposit_usd = deposit_usd;
        
        // Ограничение срока размещения рамками 1-3 лет
        res.is_valid_term = (term_years >= MIN_PLACEMENT_TERM_YEARS && term_years <= MAX_PLACEMENT_TERM_YEARS);
        res.placement_term_years = std::clamp(term_years, MIN_PLACEMENT_TERM_YEARS, MAX_PLACEMENT_TERM_YEARS);

        // Проверка Капитального Иммунитета (Депозиты от $1000)
        if (deposit_usd >= CAPITAL_IMMUNITY_THRESHOLD_USD) {
            res.capital_immunity_active = true;
            res.annual_fee_deducted_usd = 0.0; // Плата $20/год НЕ удерживается
        } else {
            res.capital_immunity_active = false;
            res.annual_fee_deducted_usd = SOULBOUND_ANNUAL_FEE_USD * res.placement_term_years; // $20 списывается за каждый год
        }

        // Въездной сплит Контура Б
        InboundSplitResult inbound = calculate_inbound_split(deposit_usd, SettlementContour::CONTOUR_B_TNET);
        res.entry_fee_usd = inbound.inbound_fee;

        // Расчет купонной доходности RWA-портфеля
        double total_gross_yield = inbound.net_cleared_amount * expected_annual_yield_rate * res.placement_term_years;
        res.net_coupon_yield_credited = std::max(0.0, total_gross_yield - res.annual_fee_deducted_usd);

        return res;
    }
};

} // namespace economics
} // namespace core
} // namespace transceptor

#endif // TRANSCEPTOR_CORE_ECONOMICS_FISCAL_SPLIT_CALCULATOR_HPP