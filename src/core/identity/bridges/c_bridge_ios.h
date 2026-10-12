#ifndef TRANSCEPTOR_CORE_C_BRIDGE_IOS_H
#define TRANSCEPTOR_CORE_C_BRIDGE_IOS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief C-интерфейс для сопряжения нативной биометрии iOS (Swift / LocalAuthentication) 
 * с C++ ядром и проверкой Secure Enclave.
 */

typedef struct {
    bool is_valid;
    uint64_t did_hash;
    uint32_t attestation_level;
    bool is_hardware_enclave_attested;
} iOSDidValidationResult;

/**
 * @brief Функция нативной проверки DID подписи для iOS Swift.
 * @param did_hash 64-битный хэш DID пользователя.
 * @param signature_bytes Указатель на подпись Secure Enclave.
 * @param signature_len Длина подписи в байтах.
 * @return iOSDidValidationResult Результат проверки правила "Нет DID — Нет Токенов".
 */
iOSDidValidationResult validate_did_ios_native(
    uint64_t did_hash,
    const uint8_t* signature_bytes,
    size_t signature_len
);

#ifdef __cplusplus
}
#endif

#endif // TRANSCEPTOR_CORE_C_BRIDGE_IOS_H