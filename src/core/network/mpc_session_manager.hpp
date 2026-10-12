#pragma once
#include <cstdint>
#include <vector>
#include <array>

namespace transceptor::core::network {

/**
 * @brief Структура состояния сессии многосторонних вычислений (MPC).
 */
struct MpcSessionState {
    uint64_t session_id{0};
    uint32_t active_nodes_count{0};
    uint32_t required_threshold{3}; // Схема порога 3 из 5
    bool is_established{false};
    int64_t session_start_timestamp_ms{0};
};

/**
 * @brief Контроллер MPC-сессий между узлами-майнерами для пороговой подписи транзакций.
 */
class MpcSessionManager {
public:
    static constexpr uint32_t MIN_NODES = 3;
    static constexpr uint32_t TOTAL_NODES = 5;

    MpcSessionManager() = default;
    ~MpcSessionManager() = default;

    /**
     * @brief Инициализация новой MPC-сессии вычислений.
     * @param session_id Уникальный 64-битный ID сессии.
     * @param node_did_hashes Вектор DID-хэшей участвующих узлов.
     * @param current_timestamp_ms Текущий таймштамп.
     * @return Заполненная структура состояния MpcSessionState.
     */
    static MpcSessionState initialize_session(
        uint64_t session_id,
        const std::vector<uint64_t>& node_did_hashes,
        int64_t current_timestamp_ms
    ) noexcept {
        MpcSessionState state{};
        state.session_id = session_id;
        state.session_start_timestamp_ms = current_timestamp_ms;
        state.active_nodes_count = static_cast<uint32_t>(node_did_hashes.size());

        if (state.active_nodes_count >= MIN_NODES) {
            state.is_established = true;
        } else {
            state.is_established = false;
        }

        return state;
    }
};

} // namespace transceptor::core::network 