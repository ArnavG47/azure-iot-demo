#pragma once
#include <stdint.h>

// Updated by the GPIO task; read by the Azure task.
extern volatile uint32_t g_button_count;
