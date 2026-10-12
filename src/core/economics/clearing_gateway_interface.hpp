#pragma once
#include <cstdint>

namespace transceptor::core::economics {

// Опережающее объявление структуры распределения фиатных сплитов
struct SplitAllocations;

/**
 * @brief Абстрактный интерфейс фискального клирингового шлюза.
 * Определяет контракт для входящего фиатного минта, экологического вычета K_entropy
 * и институционального белого списка (B2B-Whitelisting).
 */
class IClearingGateway {
public:
    virtual ~IClearingGateway() = default;

    /**
     * @brief Обработка входящего фиатного транша и расчет атомарного сплита.
     * @param fiat_amount_usd Сумма пополнения в USD.
     * @param initiator_did_hash Уникальный 64-битный хэш DID инициатора.
     * @param has_valid_did Флаг наличия валидной DID-аттестации ("Нет DID — Нет Токенов").
     * @param current_timestamp_ms Текущий таймштамп в миллисекундах.
     * @param k_entropy Индекс экологической/антропогенной энтропии (по умолчанию 0.0).
     * @return Структура SplitAllocations с детальным распределением средств.
     */
    virtual SplitAllocations process_incoming_fiat_mint(
        double fiat_amount_usd,
        uint64_t initiator_did_hash,
        bool has_valid_did,
        int64_t current_timestamp_ms,
        double k_entropy = 0.0) = 0;

    /**
     * @brief Установка институционального статуса Whitelist для B2B-партнеров.
     * @param target_did_hash Хэш DID целевого субъекта.
     * @param status Статус (true — снятие лимита в 1,000,000 USD, false — стандартный контур).
     */
    virtual void set_institutional_whitelist_status(uint64_t target_did_hash, bool status) = 0;
};

} // namespace transceptor::core::economics