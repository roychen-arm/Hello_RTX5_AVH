# Blinky with FreeRTOS project

This is the simple CMSIS-RTOS2 FreeRTOS based example for
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

- [Arm Virtual Hardware](https://arm-software.github.io/AVH/main/overview/html/index.html)
  - [CMSIS-Toolbox 1.0.0](https://github.com/Open-CMSIS-Pack/devtools/releases) or higher
  - [Arm Compiler 6.18](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded) or higher
  - Virtual Hardware Target for MPS2_Cortex-Mx


### Packs

- Required packs are listed in the file [`Hello_freertos.csolution.yml`](./Hello_freertos.csolution.yml) and [`Hello_freertos.cproject.yml`](./Hello_freertos.cproject.yml)

## Project Structure

The project is generated using the [CMSIS-Toolbox](https://github.com/Open-CMSIS-Pack/devtools/blob/main/tools/projmgr/docs/Manual/Overview.md) and is defined in [`csolution`](https://github.com/Open-CMSIS-Pack/devtools/blob/main/tools/projmgr/docs/Manual/YML-Input-Format.md) format:


- [`Hello_freertos.csolution.yml`](./Hello_freertos.csolution.yml) lists the required packs and defines the compiler option and target-types (along with the compiler).
- [`Hello_freertos.cproject.yml`](./Hello_freertos.cproject.yml) defines the source files and the software components.

## Generated Project 

1. Download required packs (not required when the packs are already available):

   ```bash
   $ csolution list packs -s Hello_freertos.csolution.yml -m > packs.txt
   $ cpackget update-index 
   $ cpackget add -f packs.txt
   ```

2. Use the `csolution` command to create `.cprj` project file.

   ```bash
   $ csolution convert -s Hello_freertos.csolution.yml
   ```

3. Use the `cbuild` command to an executable file.

    If you are using CMSIS-Toolbox 1.4.0 or higher
   ```bash
   $ cbuild -p --update-rte Hello_freertos+AVH.cprj      
   ```

    If you are using CMSIS-Toolbox 1.3.0 or lower
   ```bash
   $ cbuild -p Hello_freertos+AVH.cprj      
   ```

>**Note:** During the build process required packs may be downloaded.

You can refer [`CMSIS Project Manager - Users Manual`](https://github.com/Open-CMSIS-Pack/devtools/blob/main/tools/projmgr/docs/Manual/Overview.md) for more detail usage of cpackget/csolution/cbuild.

## Execute Project

The project is configured for execution on Arm Virtual Hardware which removes the requirement for a physical hardware board.  

- When using VHT_MPS2_Cortex-M3, you may execute it with the command:

  ```bash
  VHT_MPS2_Cortex-M3 -a ./Objects/Hello_freertos+AVH.axf
  ```

>**Note:** Arm Virtual Hardware models are also available on AWS Marketplace.
