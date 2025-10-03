# Copyright (c) Microsoft Corporation. All rights reserved.
# SPDX-License-Identifier: MIT

set(ROOT_PATH ${CMAKE_SOURCE_DIR})
set(SAMPLE_ROOT ${ROOT_PATH}/demos)

if(CONFIG_AZURE_SAMPLE_USE_PLUG_AND_PLAY)
    file(GLOB_RECURSE COMPONENT_SOURCES
        ${SAMPLE_ROOT}/sample_azure_iot_pnp/*.c
    )
else()
    file(GLOB_RECURSE COMPONENT_SOURCES
        ${SAMPLE_ROOT}/sample_azure_iot/*.c
    )
endif()

if(EXISTS "${CMAKE_BINARY_DIR}/config/sdkconfig.h")
    file(READ "${CMAKE_BINARY_DIR}/config/sdkconfig.h" config_header)
    string(REPLACE "\\n" "n" client_certificate ${config_header})
    file(WRITE "${CMAKE_BINARY_DIR}/config/sdkconfig.h" "${client_certificate}")
endif()

idf_component_get_property(MBEDTLS_DIR mbedtls COMPONENT_DIR)

list(APPEND COMPONENT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/backoff_algorithm.c
    ${CMAKE_CURRENT_LIST_DIR}/transport_tls_esp32.c
    ${CMAKE_CURRENT_LIST_DIR}/crypto_esp32.c
)

set(COMMON_ROOT ${ROOT_PATH}/demos/common)

set(COMPONENT_INCLUDE_DIRS
    ${ROOT_PATH}/config
    ${CMAKE_CURRENT_LIST_DIR}
    ${MBEDTLS_DIR}/mbedtls/include
    ${COMMON_ROOT}/transport
    ${COMMON_ROOT}/utilities
    ${COMMON_ROOT}/connection
)

if(CONFIG_AZURE_SAMPLE_USE_PLUG_AND_PLAY)
    list(APPEND COMPONENT_INCLUDE_DIRS
        ${SAMPLE_ROOT}/sample_azure_iot_pnp
    )
endif()

if(DEFINED CONFIG_ESP_TLS_USE_SECURE_ELEMENT)
    idf_component_register(
        SRCS ${COMPONENT_SOURCES}
        INCLUDE_DIRS ${COMPONENT_INCLUDE_DIRS}
        REQUIRES mbedtls tcp_transport esp-cryptoauthlib coreMQTT azure-sdk-for-c azure-iot-middleware-freertos
    )
else()
    idf_component_register(
        SRCS ${COMPONENT_SOURCES}
        INCLUDE_DIRS ${COMPONENT_INCLUDE_DIRS}
        REQUIRES mbedtls tcp_transport coreMQTT azure-sdk-for-c azure-iot-middleware-freertos
    )
endif()
