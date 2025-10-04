/*
 * Lightweight logging shim for the Azure IoT samples.  This maps the
 * LogError/LogWarn/LogInfo/LogDebug macros used by the shared transport
 * code onto ESP-IDF logging so we avoid pulling in the original AWS
 * helper implementation.
 */

#ifndef LOGGING_STACK_H_
#define LOGGING_STACK_H_

#include "esp_log.h"
#include "logging_levels.h"

#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME    "AzureSample"
#endif

#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL   LOG_ERROR
#endif

#define AZ_LOG_E(...)    ESP_LOGE( LIBRARY_LOG_NAME, __VA_ARGS__ )
#define AZ_LOG_W(...)    ESP_LOGW( LIBRARY_LOG_NAME, __VA_ARGS__ )
#define AZ_LOG_I(...)    ESP_LOGI( LIBRARY_LOG_NAME, __VA_ARGS__ )
#define AZ_LOG_D(...)    ESP_LOGD( LIBRARY_LOG_NAME, __VA_ARGS__ )

#define AZ_LOG_IF(level_macro, log_macro, message) \
    do {                                             \
        if( ( LIBRARY_LOG_LEVEL ) >= ( level_macro ) )\
        {                                            \
            log_macro message;                       \
        }                                            \
    } while( 0 )

#define LogError( message )    AZ_LOG_IF( LOG_ERROR, AZ_LOG_E, message )
#define LogWarn( message )     AZ_LOG_IF( LOG_WARN,  AZ_LOG_W, message )
#define LogInfo( message )     AZ_LOG_IF( LOG_INFO,  AZ_LOG_I, message )
#define LogDebug( message )    AZ_LOG_IF( LOG_DEBUG, AZ_LOG_D, message )

#endif /* LOGGING_STACK_H_ */
