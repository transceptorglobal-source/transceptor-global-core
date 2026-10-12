#pragma once
#include <cstdint>
#include <algorithm>

namespace transceptor::core::network {

/**
 * @brief Результат проверки задержки сети и дрифта времени.
 */
struct LatencyCheckResult {
    bool is_valid{false};
    uint32_t latency_ms{0};
    int64_t clock_drift_ms{0};
    bool latency_cap_exceeded{false};
};

/**
 * @brief Фильтр контроля сетевых задержек (Latency Cap <= 100 мс).
 * Отфильтровывает узлы с высокой задержкой и дрифтом времени для поддержания частоты сетевого пульса.
 */
class LatencyFilter {
public:
    static constexpr uint32_t MAX_LATENCY_CAP_MS = 100; // Порог Latency Cap = 100 мс
    static constexpr int64_t MAX_ALLOWED_DRIFT_MS = 1000; // Лимит дрифта часов = 1 с

    /**
     * @brief Оценка сетевой задержки между узлами.
     * @param sent_timestamp_ms Таймштамп отправки пакета.
     * @param received_timestamp_ms Таймштамп получения пакета.
     * @param node_reported_time_ms Время, заявленное удаленным узлом.
     * @return Результат проверки LatencyCheckResult.
     */
    static LatencyCheckResult evaluate_latency(
        int64_t sent_timestamp_ms,
        int64_t received_timestamp_ms,
        int64_t node_reported_time_ms
    ) noexcept {
        LatencyCheckResult res{};
        if (received_timestamp_ms < sent_timestamp_ms) {
            res.is_valid = false;
            return res;
        }

        res.latency_ms = static_cast<uint32_t>(received_timestamp_ms - sent_timestamp_ms);
        res.clock_drift_ms = std::abs(received_timestamp_ms - node_reported_time_ms);

        if (res.latency_ms > MAX_LATENCY_CAP_MS) {
            res.latency_cap_exceeded = true;
            res.is_valid = false;
        } else if (res.clock_drift_ms > MAX_ALLOWED_DRIFT_MS) {
            res.is_valid = false;
        } else {
            res.is_valid = true;
        }

        return res;
    }
};

} // namespace transceptor::core::network