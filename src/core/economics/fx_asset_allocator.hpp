// ==============================================================================
// TRANSCEPTOR GLOBAL: DUAL-CONTOUR FX & SOVEREIGN ALLOCATOR
// MODULE: FX ASSET ALLOCATOR WITH STRATEGIC FORK (1.50% CROSS-BORDER SPLIT)
// TIMESTAMP ACCORDING TO JURISDICTIONAL COMPLIANCE: Oct 03, 2026
// ==============================================================================

#ifndef TRANSCEPTOR_FX_ASSET_ALLOCATOR_HPP
#define TRANSCEPTOR_FX_ASSET_ALLOCATOR_HPP

#include <iostream>
#include <string>
#include <cmath>

namespace transceptor::economics {

enum class SovereignAssetClass {
    LOCAL_SOVEREIGN_BONDS,   // Локальные облигации (Только 2.50% сплит, без 1.50%)
    GLOBAL_US_TREASURIES,   // Глобальные бумаги US Treasuries (2.50% + 1.50% сплит)
    SWISS_DLT_RESERVE       // Глобальный резерв Швейцарии (2.50% + 1.50% сплит)
};

struct DualContourClearingResult {
    std::string source_currency;
    double gross_inbound_fiat;
    double fx_rate;
    double converted_gross_tnat;   // Валовый T_nat
    
    double inbound_split_2_5;      // Въездной сплит 2.5%
    double net_tnat_balance;       // Баланс T_nat (Gross / 1.025)
    
    bool is_global_conversion;     // Требуется ли конвертация T_nat -> T_net
    double cross_border_split_1_5; // Кросс-сплит 1.5% (если T_net)
    double global_s_reserve_share; // 1.00% в Глобальный S_reserve
    double author_ip_royalty_share;// 0.50% в Авторский пул IP Royalty
    
    double final_invested_tnet;    // Итоговый баланс T_net (при глобальном контуре)
    SovereignAssetClass target_asset;
    bool is_cleared;
    std::string status_message;
};

class FxAssetAllocator {
public:
    constexpr static double INBOUND_VAT_RATE = 0.0250;     // 2.5% въездной
    constexpr static double CROSS_BORDER_VAT_RATE = 0.0150;// 1.5% кросс-контурный

    static DualContourClearingResult allocate_capital(
        const std::string& source_currency,
        double gross_inbound_fiat,
        double fx_rate,
        SovereignAssetClass target_asset,
        bool sender_has_valid_did
    ) {
        DualContourClearingResult res{};
        res.source_currency = source_currency;
        res.gross_inbound_fiat = gross_inbound_fiat;
        res.fx_rate = fx_rate;
        res.target_asset = target_asset;

        if (!sender_has_valid_did || gross_inbound_fiat <= 0.0 || fx_rate <= 0.0) {
            res.is_cleared = false;
            res.status_message = "ERR_INVALID_INPUT_OR_DID";
            return res;
        }

        // 1. Конвертация фиата в валовый T_nat
        res.converted_gross_tnat = gross_inbound_fiat * fx_rate;

        // 2. Извлечение въездного сплита 2.5% "от обратного"
        res.net_tnat_balance = res.converted_gross_tnat / (1.0 + INBOUND_VAT_RATE);
        res.inbound_split_2_5 = res.converted_gross_tnat - res.net_tnat_balance;

        // 3. Проверка необходимости глобальной конвертации T_nat -> T_net
        if (target_asset == SovereignAssetClass::LOCAL_SOVEREIGN_BONDS) {
            // Локальный контур: 1.5% НЕ взимается
            res.is_global_conversion = false;
            res.cross_border_split_1_5 = 0.0;
            res.global_s_reserve_share = 0.0;
            res.author_ip_royalty_share = 0.0;
            res.final_invested_tnet = res.net_tnat_balance; // Остается в T_nat
            res.status_message = "SUCCESS_LOCAL_CONTOUR: 2.5% fee applied, 1.5% global fee EXEMPTED.";
        } else {
            // Глобальный контур: Применяется сбор 1.5% T_nat -> T_net "от обратного"
            res.is_global_conversion = true;
            res.final_invested_tnet = res.net_tnat_balance / (1.0 + CROSS_BORDER_VAT_RATE);
            res.cross_border_split_1_5 = res.net_tnat_balance - res.final_invested_tnet;

            res.global_s_reserve_share  = res.cross_border_split_1_5 * (1.0 / 1.5); // 1.00%
            res.author_ip_royalty_share = res.cross_border_split_1_5 * (0.5 / 1.5); // 0.50%
            res.status_message = "SUCCESS_GLOBAL_CONTOUR: 2.5% entry + 1.5% global cross-border fee applied.";
        }

        res.is_cleared = true;
        return res;
    }
};

} // namespace transceptor::economics

#endif // TRANSCEPTOR_FX_ASSET_ALLOCATOR_HPP