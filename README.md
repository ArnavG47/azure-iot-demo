# Azure IoT ESP32 Button Telemetry Demo

This project adapts Microsoft's **Azure IoT middleware for FreeRTOS** sample to run on an ESP32 development board.
In addition to the stock Wi-Fi, SNTP, and Azure IoT Hub connectivity flow, it adds a dedicated FreeRTOS task that
monitors a hardware push-button, toggles an LED for user feedback, and streams button click counts to the cloud.

## What this sample demonstrates

- Connecting an ESP32 to Wi-Fi, synchronizing time with SNTP, and creating a secure MQTT connection to Azure IoT Hub.
- Reading a GPIO button from a FreeRTOS task (`button_detect_task`) that debounces the input, lights an LED on
  GPIO5, and increments the shared `g_button_count` counter whenever the button wired to GPIO21 is pressed.
- Publishing telemetry that includes both the JSON payload (`{"deviceId": "<device>", "buttonCount": <n>}`)
  and a custom application property named `Clicks` whose value mirrors the running button count. This allows you to
  visualize presses directly in IoT Hub metrics, Event Hub consumers, or downstream services.

The result is an end-to-end path from a physical interaction on the ESP32 to Azure IoT Hub telemetry.

## Repository layout

- `main/azure_iot_freertos_esp32_main.c` – ESP-IDF entry point that brings up networking, initializes SNTP, and launches
  both the Azure IoT demo and the button monitoring FreeRTOS task.
- `main/sample_azure_iot.c` – Upstream Microsoft sample that handles Azure IoT Hub provisioning, subscriptions, and
  telemetry publishing using the shared `g_button_count` value.
- `config/gpio_vars.h` – Declares the global button counter shared between the FreeRTOS GPIO task and the Azure telemetry
  layer.
- `components/sample-azure-iot` – Kconfig options and component sources provided by Microsoft for Azure IoT Hub
  connectivity.

## Hardware requirements

- ESP32 development board supported by ESP-IDF (e.g., ESP32-DevKitC).
- Momentary push-button connected between GPIO21 and GND. The firmware enables the internal pull-up resistor, so no
  external pull-up is required.
- (Optional) LED on GPIO5 with an appropriate resistor to visualize button activity. You can also rely solely on the
  serial log statements if you do not attach an LED.
- USB cable for flashing and serial monitoring.

## Configure Wi-Fi and Azure IoT Hub credentials

1. Install [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) and set up the
   environment.
2. From the project root run `idf.py menuconfig`.
3. Navigate to **Azure IoT middleware for FreeRTOS Sample Configuration** to set your Wi-Fi SSID, password, scan
   preferences, and connection policy.
4. Open **Azure IoT middleware for FreeRTOS Main Task Configuration** to enter your Azure IoT Hub host name, device ID,
   and authentication method (symmetric key or X.509). Paste the corresponding credentials from the Azure portal.
5. (Optional) Enable the Device Provisioning Service (DPS) section if you plan to onboard devices dynamically instead of
   using a pre-created IoT Hub device identity.

## Build, flash, and monitor

```bash
idf.py set-target esp32        # Or another supported target
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

Replace `/dev/ttyUSB0` with the serial port that matches your development board. Press `Ctrl+]` to exit the monitor.

On boot the firmware connects to Wi-Fi, synchronizes time, and starts the Azure IoT Hub client. The log stream will show
entries such as `button clicked` whenever the button task detects a press. Each press also updates the LED (if present)
and increments the telemetry counter sent to the cloud.

## Observing telemetry in Azure

- Open the **IoT Hub** resource in the Azure portal and use the **Message to Device** or built-in metrics to confirm the
  device is connected.
- Use the **Event Hub-compatible endpoint** or the Azure CLI to watch telemetry. Each message payload includes the JSON
  document with the `buttonCount` field, and the message properties include the `Clicks` key for easy filtering.
- Because the counter is stored in `g_button_count`, the value persists across telemetry sends during a session. Reset
  the board to start over at zero.

## Adapting or extending the demo

- Change `button_pin` or `led_pin` in `main/azure_iot_freertos_esp32_main.c` if your hardware uses different GPIOs.
- Add additional properties or reported properties in `main/sample_azure_iot.c` to track more device state.
- Integrate downstream Azure services—such as Azure Functions, Stream Analytics, or Time Series Insights—to act on the
  `Clicks` telemetry and build richer scenarios.

## Troubleshooting tips

- Ensure the device clock is synchronized; Azure IoT Hub requires valid TLS certificates and SAS tokens. The sample
  automatically invokes SNTP, but lack of internet connectivity will prevent authentication.
- If you see Wi-Fi connection issues, revisit the menuconfig Wi-Fi settings and confirm the SSID/password are correct.
- For TLS or authentication errors, verify that the device ID and symmetric key (or certificate) in menuconfig match the
  registered device in your IoT Hub.

## Attribution

This project is based on Microsoft's Azure IoT middleware for FreeRTOS ESP32 sample and retains the upstream licensing.
The new button telemetry task, GPIO wiring guidance, and README documentation were added to demonstrate how to extend the
sample for custom device interactions.
