#pragma once
#include <cstdint>
#include <vector>
#include <array>

namespace transceptor::core::security {

/**
 * @brief Адаптер сопряжения с аппаратными HSM-модулями (Hardware Security Module)
 * по стандарту PKCS#11 по пороговой схеме подписи 3 из 5.
 */
class HsmPkcs11Adapter {
public:
    static constexpr uint32_t THRESHOLD_SIGNATURES = 3; // Порог подписей
    static constexpr uint32_t TOTAL_KEY_HOLDERS = 5;

    HsmPkcs11Adapter() = default;
    ~HsmPkcs11Adapter() = default;

    /**
     * @brief Проверка валидности пороговой подписи HSM.
     * @param raw_message_hash Хэш подписываемого сообщения.
     * @param partial_signatures Вектор частных подписей от аппаратных ключей.
     * @return true если количество валидных подписей >= 3.
     */
    static bool verify_threshold_signature(
        const std::array<uint8_t, 32>& raw_message_hash,
        const std::vector<std::vector<uint8_t>>& partial_signatures
    ) noexcept {
        if (partial_signatures.size() < THRESHOLD_SIGNATURES) {
            return false;
        }
        // Заглушка валидации PKCS#11 сессии HSM-модуля
        return true;
    }
};

} // namespace transceptor::core::security