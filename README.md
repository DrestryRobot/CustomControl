# CustomControl

STM32F103 firmware (Keil MDK-ARM project `Robot_Control`) that initializes the
MCU peripherals and drives an MPU6050 IMU through the InvenSense DMP.

## Overview

- **HAL skeleton** — `Src/` contains the CubeMX/HAL-generated initialization:
  clocks, GPIO, I2C, USART, DMA, interrupt handlers and `system_stm32f1xx.c`,
  with the matching headers in `Inc/`.
- **IMU support code** — `User/` holds the device-specific layer:
  `IIC.c/.h` (soft I²C driver), `mpu6050.c/.h`, the InvenSense motion driver
  (`inv_mpu.c`, `inv_mpu_dmp_motion_driver.c`) and its DMP firmware data tables
  (`dmpKey.h`, `dmpmap.h`).
- **Toolchain setup** — `MDK-ARM/Robot_Control.uvprojx` is the Keil project,
  together with `JLinkSettings.ini`, a debug configuration for the
  `STM32F103C8` target and the CMSIS startup assembly
  (`startup_stm32f103xb.s`).

## Repository layout

| Path | Purpose |
| --- | --- |
| `Inc/`, `Src/` | STM32Cube HAL initialization code and headers |
| `User/` | I²C driver, MPU6050 driver, InvenSense DMP motion driver and firmware tables |
| `MDK-ARM/Robot_Control.uvprojx` | Keil uVision project |
| `MDK-ARM/Robot_Control/` | Build output: `.axf`, `.hex`, `.map`, object files |
| `MDK-ARM/startup_stm32f103xb.s` | CMSIS startup file for the STM32F103xB family |

## Requirements

- Keil MDK-ARM (uVision 5) with the STM32F1 device family pack
- An STM32F103C8 board and a J-Link (or compatible) debug probe — the J-Link
  settings in the project are configured for that probe

## Build

Open `MDK-ARM/Robot_Control.uvprojx` in uVision, build the `Robot_Control`
target and flash it. The repository already contains the build output
(`MDK-ARM/Robot_Control/Robot_Control.hex` and `.axf`), so a prebuilt image is
available if you only need to flash a board.

## Notes

- Compiler output, listing files, J-Link logs and a `.bak` copy of the uVision
  GUI layout are committed under `MDK-ARM/`; only the `Inc/`, `Src/` and `User/`
  trees are hand-written source.
- The project has no external dependencies beyond the STM32Cube HAL and the
  InvenSense motion driver that is vendored in `User/`.

## License

No license file is present in this repository. Contact the repository owner for
licensing terms. The STM32 HAL portions carry ST's BSD-3-Clause licence header.
