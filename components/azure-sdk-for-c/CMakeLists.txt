set(MIDDLEWARE_ROOT ${CMAKE_SOURCE_DIR}/azure-iot-middleware-freertos)
set(AZURE_SDK_FOR_C_PATH ${MIDDLEWARE_ROOT}/libraries/azure-sdk-for-c/sdk)

file(GLOB_RECURSE COMPONENT_SOURCES
    ${AZURE_SDK_FOR_C_PATH}/src/azure/*.c
)

list(FILTER COMPONENT_SOURCES EXCLUDE REGEX ".*(curl|win32).*")

set(COMPONENT_INCLUDE_DIRS
    ${AZURE_SDK_FOR_C_PATH}/inc
)

idf_component_register(
    SRCS ${COMPONENT_SOURCES}
    INCLUDE_DIRS ${COMPONENT_INCLUDE_DIRS}
)
