#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

typedef struct mbedtls_threading_mutex_t {
    SemaphoreHandle_t mutex;
    char is_valid;
} mbedtls_threading_mutex_t;

#ifdef __cplusplus
}
#endif
