# Copyright (c) Microsoft Corporation. All rights reserved.
# SPDX-License-Identifier: MIT

set(ROOT_PATH ${CMAKE_SOURCE_DIR})

# Unescape any escaped newlines in sdkconfig.h (for certs entered as single-line strings)
if(EXISTS "${CMAKE_BINARY_DIR}/config/sdkconfig.h")
    file(READ "${CMAKE_BINARY_DIR}/config/sdkconfig.h" config_header)
    string(REPLACE "\\n" "n" client_certificate ${config_header})
    file(WRITE "${CMAKE_BINARY_DIR}/config/sdkconfig.h" "${client_certificate}")
endif()

idf_component_get_property(MBEDTLS_DIR mbedtls COMPONENT_DIR)

if(EXISTS "${ROOT_PATH}/config/threading_alt.h")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/config")
    configure_file(
        ${ROOT_PATH}/config/threading_alt.h
        ${CMAKE_BINARY_DIR}/config/threading_alt.h
        COPYONLY)
endif()

set(COMPONENT_SOURCES
    ${ROOT_PATH}/main/sample_azure_iot.c
    ${CMAKE_CURRENT_LIST_DIR}/backoff_algorithm.c
    ${CMAKE_CURRENT_LIST_DIR}/transport_tls_esp32.c
    ${CMAKE_CURRENT_LIST_DIR}/crypto_esp32.c
    ${CMAKE_CURRENT_LIST_DIR}/common/transport/transport_socket.c
    ${CMAKE_CURRENT_LIST_DIR}/common/transport/transport_tls_socket_using_mbedtls.c
    ${CMAKE_CURRENT_LIST_DIR}/common/transport/sockets_wrapper_lwip.c
    ${CMAKE_CURRENT_LIST_DIR}/common/utilities/azure_sample_crypto_mbedtls.c
    ${CMAKE_CURRENT_LIST_DIR}/common/utilities/mbedtls_freertos_port.c
    ${CMAKE_CURRENT_LIST_DIR}/common/logging/logging_compat.c
)

set(COMPONENT_INCLUDE_DIRS
    ${ROOT_PATH}/config
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/common/transport
    ${CMAKE_CURRENT_LIST_DIR}/common/utilities
    ${CMAKE_CURRENT_LIST_DIR}/common/logging
    ${CMAKE_CURRENT_LIST_DIR}/common/connection
    ${MBEDTLS_DIR}/mbedtls/include
    ${MBEDTLS_DIR}/mbedtls/include/mbedtls
    ${MBEDTLS_DIR}/port/include
    ${MBEDTLS_DIR}/port/mbedtls_rom
)

if(DEFINED CONFIG_ESP_TLS_USE_SECURE_ELEMENT)
    idf_component_register(
        SRCS ${COMPONENT_SOURCES}
        INCLUDE_DIRS ${COMPONENT_INCLUDE_DIRS}
        REQUIRES mbedtls tcp_transport lwip esp-cryptoauthlib coreMQTT azure-sdk-for-c azure-iot-middleware-freertos
    )
else()
    idf_component_register(
        SRCS ${COMPONENT_SOURCES}
        INCLUDE_DIRS ${COMPONENT_INCLUDE_DIRS}
        REQUIRES mbedtls tcp_transport lwip coreMQTT azure-sdk-for-c azure-iot-middleware-freertos
    )
endif()
