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

- This repository is compatible with [Keil MDK 6](https://www.keil.arm.com/keil-mdk/).

### CMSIS Packs

- Required packs are listed in the file [`Hello_rtos.cproject.yml`](./Hello_rtos.cproject.yml).

## Project Structure

The project is generated using the [CMSIS-Toolbox](https://github.com/Open-CMSIS-Pack/devtools/blob/main/tools/projmgr/docs/Manual/Overview.md) and is defined in [`csolution`](https://github.com/Open-CMSIS-Pack/devtools/blob/main/tools/projmgr/docs/Manual/YML-Input-Format.md) format:


- [`Hello_rtos.csolution.yml`](./Hello_rtos.csolution.yml) : defines the compiler option and target-types (along with the compiler).
- [`Hello_rtos.cproject.yml`](./Hello_rtos.cproject.yml) : lists the source files, software components and the required CMSIS packs. 
-  [`vcpkg-configuration.json`](./vcpkg-configuration.json) : defines the required tools for building the project.
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
   cbuild Hello_rtos.csolution.yml --context-set --update-rte --packs --context Hello_rtos+AVH
   ```


>**Note:** During the build process required packs may be downloaded.

You can refer [`CMSIS Project Manager - Users Manual`](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/build-overview.md) for more detail usage of cpackget/csolution/cbuild.

## Execute Project

The project is configured for execution on Arm Virtual Hardware which removes the requirement for a physical hardware board.  

- When using VHT_MPS2_Cortex-M3, you may execute it with the command:

  ```bash
  FVP_MPS2_Cortex-M3 -a out/Hello_rtos/AVH/Hello_rtos.axf
  ```

## Trouble Shooting

1. If you get some compiler error such as the below, Pleasd try to activate Arm compiler license
> armclang: error: Failed to check out a license.
> The license file could not be found. Check that ARMLMD_LICENSE_FILE is set correctly.

- Keil MDK-Community : https://www.keil.arm.com/mdk-community/ 
- Keil MDK-Essential : https://www.arm.com/en/products/development-tools/embedded-and-software/keil-mdk

## GitHub workflow with GitHub Actions Runners
This repository use [`led_check.yml`](./.github/workflows/led_check.yml) as a sample workflow yaml.
This samlpe yaml run workflow job with delpoying `vcpkg` as enviroment in Ubuntu based GitHub Actions Runners.
vcpkg delpoyment and Arm UBL license activation refer CMSIS-Toolbox
[`CMSIS Toolbox installation refernece`](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/installation.md#github-actions)

### Action: vcpkg
This action can be used to activate a development environment based on a `vcpkg-configuration.json` file as described in the [`CMSIS-Toolbox installation guide`](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/installation.md#vcpkg---setup-using-cli).

```yml
- name: Setup vcpkg environment
  uses: ARM-software/cmsis-actions/vcpkg@v1
  with:
    config: "./vcpkg-configuration.json"
    vcpkg-downloads: "${{ github.workspace }}/.vcpkg/downloads"
    cache: "-"
```
The activated environment is preserved into `$GITHUB_PATH` and `$GITHUB_ENV` so that it can be used by subsequent steps.

### Action: armlm
This action can be used to activate an [`Arm user based license`](https://developer.arm.com/documentation/102516/1-2/Activate-and-deactivate-your-product-license/Activate-your-product-using-a-license-server?lang=en) using `armlm` license manager.
```yml
- name: Activate Arm license
  uses: ARM-software/cmsis-actions/armlm@v1
  with:
    server: https://mdk-preview.keil.arm.com
    server: <custom license server>
    code: <personal product code>
    product: KEMDK-COM0
```
By default, the action activates the free [`Keil MDK v6 Community license`](https://learn.arm.com/learning-paths/microcontrollers/vcpkg-tool-installation/licenseactivation/).
