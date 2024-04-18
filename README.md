# Blinky with Keil RTX5 project

This is the simple CMSIS-RTOS2 Keil RTX5 based example for
ARM 'Cortex-M3 DesignStart' device using Arm Virtual Hardware.
Compliant to Cortex Microcontroller Software Interface Standard (CMSIS).

Example functionality:
 - Clock Settings:
   - XTAL  =  50 MHz
   - CCLK  =  25 MHz

 - LEDs is blinking with fixed speed.
 - blinking is paused while holding down Push buttons.
 - colour GLCD display shows
   - init message
   - Button status
   - LED status

## Prerequisites

### Tools:

- [vcpkg](https://github.com/microsoft/vcpkg?tab=readme-ov-file) 
  (All required tools define in [`vcpkg-configuration.json`](./vcpkg-configuration.json)

You can refer [Arm Developer Hub guide](https://learn.arm.com/learning-paths/microcontrollers/vcpkg-tool-installation/installation/) to quick install vpkg.
   ```bash
   . <(curl https://aka.ms/vcpkg-init.sh -L)
   ```

- This repository is compatible with [Keil MDK 6](https://www.keil.arm.com/keil-mdk/)

### Packs

- Required packs are listed in the file [`Hello_rtos.csolution.yml`](./Hello_rtos.csolution.yml) and [`Hello_rtos.cproject.yml`](./Hello_rtos.cproject.yml)

## Project Structure

The project is generated using the [CMSIS-Toolbox](https://github.com/Open-CMSIS-Pack/devtools/blob/main/tools/projmgr/docs/Manual/Overview.md) and is defined in [`csolution`](https://github.com/Open-CMSIS-Pack/devtools/blob/main/tools/projmgr/docs/Manual/YML-Input-Format.md) format:


- [`Hello_rtos.csolution.yml`](./Hello_rtos.csolution.yml) lists the required packs and defines the compiler option and target-types (along with the compiler).
- [`Hello_rtos.cproject.yml`](./Hello_rtos.cproject.yml) defines the source files and the software components.
-  [`vcpkg-configuration.json`](./vcpkg-configuration.json) defines the required tools for building the project.
>**Note:** See more detial about the Arm tools available with vcpkg : [Arm Tools Available in vcpkg](https://www.keil.arm.com/artifacts/)

## Build Project 

1. Initialize vcpkg :

   ```bash
   . ~/.vcpkg/vcpkg-init
   ```

2. vcpkg activate. It will automatically download the requred tools defined in  [`vcpkg-configuration.json`](./vcpkg-configuration.json) and setup the enviroment variable.

   ```bash
   vcpkg activate
   ```

3. Use the `cbuild` command to build the project.

   ```bash
   cbuild -p --update-rte Hello_rtos.csolution.yml    
   ```


>**Note:** During the build process required packs may be downloaded.

You can refer [`CMSIS Project Manager - Users Manual`](https://github.com/Open-CMSIS-Pack/devtools/blob/main/tools/projmgr/docs/Manual/Overview.md) for more detail usage of cpackget/csolution/cbuild.

## Execute Project

The project is configured for execution on Arm Virtual Hardware which removes the requirement for a physical hardware board.  

- When using VHT_MPS2_Cortex-M3, you may execute it with the command:

  ```bash
  FVP_MPS2_Cortex-M3 -a out/Hello_rtos/AVH/Hello_rtos.axf
  ```

## Trouble Shooting

1. If you get some compiler error such as the below, Pleasd try to activate Arm compiler license : https://www.keil.arm.com/mdk-community/
  ```comment
armclang: error: Failed to check out a license.
The license file could not be found. Check that ARMLMD_LICENSE_FILE is set correctly.
  ```