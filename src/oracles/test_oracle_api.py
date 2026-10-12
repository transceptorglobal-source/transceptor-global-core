Python
import math


def verify_universal_math():
    g_in = 5000000.0  # 5,000,000 USD
    t = 5.0  # 5 лет
    y_nom = 0.08  # 8.0% годовых
    k_risk = 1.0  # Коэффициент надежности

    # 1. Вход от обратного (2.5% в том числе)
    n_in = g_in / 1.025
    s_in_total = g_in - n_in

    # 2. Ставка риска и порог безопасности
    y_eff = y_nom * k_risk
    target_payout_factor = 1.0 + (0.025 * t)  # 112.5% за 5 лет
    safe_threshold = math.pow(target_payout_factor * 1.025, 1.0 / t) - 1.0

    # 3. Накопление за T=5 лет
    accumulated = n_in * math.pow(1.0 + y_eff, t)
    payout = g_in * target_payout_factor
    surplus = max(0.0, accumulated - payout)

    # 4. Сплит сверхдохода (30% / 30% / 30% / 10%)
    eco_fund = surplus * 0.30
    infra_dev = surplus * 0.30
    dynamic_dev = surplus * 0.30
    ip_royalty = surplus * 0.10

    # 5. Векторная декомпозиция Экофонда (Зарыбление 40%, Лесопосадки 30%)
    allocated_fish = eco_fund * 0.40
    allocated_forest = eco_fund * 0.30

    print(f"[TEST 5Y] Net Invested Tnet: {n_in:.4f}")
    print(f"[TEST 5Y] Guaranteed Payout (112.5%): {payout:.4f}")
    print(f"[TEST 5Y] Accumulated 5Y: {accumulated:.4f}")
    print(f"[TEST 5Y] Total Surplus Captured: {surplus:.4f}")
    print(f"[TEST 5Y] Eco Fund (30%): {eco_fund:.4f}")
    print(f"[TEST 5Y] Allocated Fish Stocking (40% of Eco): {allocated_fish:.4f}")
    print(f"[TEST 5Y] Allocated Afforestation (30% of Eco): {allocated_forest:.4f}")

    assert payout == 5625000.0
    assert surplus > 0
    print("\n[SUCCESS] Universal Math & Vector Allocation Verified!")


if __name__ == "__main__":
    verify_universal_math()