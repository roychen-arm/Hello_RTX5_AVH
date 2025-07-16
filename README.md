# Blinky with Keil RTX5 Project

This is a simple CMSIS-RTOS2 example based on Keil RTX5 for the Arm *Cortex-M3 DesignStart* device using **Arm Virtual Hardware**.  
The project complies with the *Cortex Microcontroller Software Interface Standard (CMSIS)*.

## Functionality Overview

- **Clock Settings**
  - XTAL: 50 MHz  
  - CCLK: 25 MHz

- **LED Behavior**
  - Blinks at a fixed rate
  - Pauses while a push button is held

- **GLCD Display**
  - Initialization message
  - Button status
  - LED status

---

## Prerequisites

### Tools

- [vcpkg](https://github.com/microsoft/vcpkg?tab=readme-ov-file)  
  All required tools are defined in [`vcpkg-configuration.json`](./vcpkg-configuration.json).

  You can follow the [Arm Developer Hub guide](https://learn.arm.com/learning-paths/microcontrollers/vcpkg-tool-installation/installation/) for a quick installation:

  ```bash
  . <(curl https://aka.ms/vcpkg-init.sh -L)
  ```

- Compatible with [Keil MDK 6](https://www.keil.arm.com/keil-mdk/)

### CMSIS Packs

- Required packs are listed in [`Hello_rtos.cproject.yml`](./Hello_rtos.cproject.yml)

---

## Project Structure

The project is generated using [CMSIS-Toolbox](https://github.com/Open-CMSIS-Pack/cmsis-toolbox) and is defined using the `csolution` format:

- [`Hello_rtos.csolution.yml`](./Hello_rtos.csolution.yml): Defines compiler options and target types
- [`Hello_rtos.cproject.yml`](./Hello_rtos.cproject.yml): Lists source files, software components, and required CMSIS packs
- [`vcpkg-configuration.json`](./vcpkg-configuration.json): Lists the required tools for building

> **Note**: For more information on Arm tools available with vcpkg, see [Arm Tools in vcpkg](https://www.keil.arm.com/artifacts/)

---

## Build Instructions

1. **Initialize vcpkg**

   ```bash
   . ~/.vcpkg/vcpkg-init
   ```

2. **Activate vcpkg**  
   This will automatically download the tools defined in `vcpkg-configuration.json` and set up environment variables:

   ```bash
   vcpkg activate
   ```

3. **Build the project using `cbuild`**

   ```bash
   cbuild Hello_rtos.csolution.yml --context-set --update-rte --packs --context Hello_rtos+AVH
   ```

> **Note**: Required packs may be downloaded during the build process.  
> Refer to the [CMSIS Project Manager User Manual](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/build-overview.md) for advanced usage of `cpackget`, `csolution`, and `cbuild`.

---

## Run the Project

The project is configured to run on **Arm Virtual Hardware**, removing the need for physical hardware.

If you're using `VHT_MPS2_Cortex-M3`, execute with:

```bash
FVP_MPS2_Cortex-M3 -a out/Hello_rtos/AVH/Hello_rtos.axf
```

---

## Troubleshooting

If you encounter a license error like the following:

```
armclang: error: Failed to check out a license.
The license file could not be found. Check that ARMLMD_LICENSE_FILE is set correctly.
```

Try activating the Arm compiler license:

- [Keil MDK Community](https://www.keil.arm.com/mdk-community/)
- [Keil MDK Essential](https://www.arm.com/en/products/development-tools/embedded-and-software/keil-mdk)

---

## GitHub Workflow with Actions Runner

This repository includes a sample workflow [`led_check.yml`](./.github/workflows/led_check.yml) to demonstrate running the job on an Ubuntu-based GitHub Actions runner with vcpkg and license setup.

### Action: `vcpkg`

Activates a development environment based on `vcpkg-configuration.json`, following the [CMSIS Toolbox installation guide](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/installation.md#vcpkg---setup-using-cli):

```yaml
- name: Setup vcpkg environment
  uses: ARM-software/cmsis-actions/vcpkg@v1
  with:
    config: "./vcpkg-configuration.json"
    vcpkg-downloads: "${{ github.workspace }}/.vcpkg/downloads"
    cache: "-"
```

The environment is stored in `$GITHUB_PATH` and `$GITHUB_ENV` for use in subsequent steps.

### Action: `armlm`

Activates an [Arm user-based license](https://developer.arm.com/documentation/102516/1-2/Activate-and-deactivate-your-product-license/Activate-your-product-using-a-license-server?lang=en):

```yaml
- name: Activate Arm license
  uses: ARM-software/cmsis-actions/armlm@v1
  with:
    server: https://mdk-preview.keil.arm.com
    code: <personal product code>
    product: KEMDK-COM0
```

By default, this activates the free [Keil MDK v6 Community license](https://learn.arm.com/learning-paths/microcontrollers/vcpkg-tool-installation/licenseactivation/).
