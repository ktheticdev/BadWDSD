# BadWDSD

This is a hardware modchip for Sony PlayStation 3. By using Raspberry Pi Pico (RP2040), It is possible for non-CFW compatible models to boot qCFW.

# Supported models

All **CECH-2500**

All **CECH-3000**

**CECH-4x00** with **NOR** flash

<img width="284" height="370" alt="firefox_Z4WaABYPQH" src="https://github.com/user-attachments/assets/7066c760-a097-45ba-9697-6022c9cf1e07" />

**CECH-4x00** with **eMMC** flash is **NOT** supported

<img width="220" height="285" alt="firefox_LGBpLg82NH" src="https://github.com/user-attachments/assets/6592b99e-f80f-4319-a450-10a894aa5164" />

# What is qCFW?

You still can't install CFW PUP, so new variant of CFW must be made. This is called **quasi-CFW**.

It is heavily based from **Evilnat 4.92.2 PEX CFW**. And will support every feature except one: **Dumping eid_root_key**.

Cobra must be active at all times or some feature will not work properly.

# qCFW quirks

For some unknown reason, When you turn on the console using wireless controller it won't sync. You must power cycle the controller for it to sync.

# Note on DEX mode

DEX mode is fully supported. But any kind of firmware installation or update is not possible while in this mode.

This means if you somehow need to reinstall the firmware such as corrupted HDD, you are stuck.

To recover, use **BANKSEL** pin on the modchip to go back to OFW.
