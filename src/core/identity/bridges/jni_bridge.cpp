#include <jni.h>
#include <cstdint>
#include "core/identity/bridges/clean_identity_validator.hpp"

using namespace transceptor::core::identity;

extern "C" {

JNIEXPORT jboolean JNICALL
Java_dev_transceptor_mobile_NativeBridge_validateDidLocalToken(
    JNIEnv* env,
    jobject /* this */,
    jlong did_hash,
    jbyteArray signature_bytes
) {
    if (did_hash <= 0 || signature_bytes == NULL) {
        return JNI_FALSE;
    }

    // Изолированная передача вызова в C++ ядро
    uint64_t native_did = static_cast<uint64_t>(did_hash);
    
    // Проверка правила: «Нет DID — Нет Токенов»
    if (native_did == 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

} // extern "C"