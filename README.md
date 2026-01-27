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

It is heavily based from **Evilnat 4.92.2 PEX CFW**. And will support every feature except: **Dumping eid_root_key and everything that needed it**.

Cobra must be active at all times or some feature will not work properly.

# qCFW quirks

For some unknown reason, When you turn on the console using wireless controller it won't sync. You must power cycle the controller for it to sync.

# Note on DEX mode

DEX mode is fully supported. But any kind of firmware installation or update is not possible while in this mode.

This means if you somehow need to reinstall the firmware such as corrupted HDD, you are stuck.

To recover, use **BANKSEL** pin on the modchip to go back to OFW.

# Installation (Hardware)

Currently, **Raspberry Pi Pico (RP2040)** and **RP2040-Zero** is supported.

Only install modchip after Stagex is installed to flash from above section. Otherwise it won't boot, if you already installed the modchip. You can use HOLD pin to temporary disable the modchip without unsoldering it.

<details>
  <summary> <b>Pico</b> </summary>
<p>
<img width="1100" height="800" alt="raspberry_pi_pico_pinout - Copy" src="https://github.com/user-attachments/assets/e1393136-d60f-4822-a818-f27cf2b1456b" />
</p>
</details>

<details>
  <summary> <b>RP2040-Zero</b> </summary>
<p>
<img src="https://github.com/user-attachments/assets/8304c258-386b-4f2c-84ee-5fd5f6f90217" />
</p>
</details>

<details>
  <summary> <b>4x00</b> </summary>
<p>
<img src="https://github.com/user-attachments/assets/9910be97-5c85-4b48-9edb-c2d7a4ecabd9" />

<img width="481" height="384" alt="firefox_hjbEN8ZhUV" src="https://github.com/user-attachments/assets/fb19f60f-76ee-4e76-a164-83b988cdf286" />

<img width="501" height="400" alt="firefox_ybeL3zep1j" src="https://github.com/user-attachments/assets/71580063-2a03-4b6e-8433-e5f99e925e89" />
</p>
</details>

Exclude power and ground, you only need to solder 4 wires that marked red (CMD, CLK, SC_RX, SC_TX). Other pin is optional.

It is possible to power the modchip using external power as long as it is active during standby

# Pin description

**Signal pin:**

**CLK** - XDR CLK signal

**CMD** - XDR CMD signal

**SC_TX/SC_RX** - Syscon UART signal

**DEBUG** - Optional modchip UART signal, for debugging and accessing syscon **(baud 576000, NOT 57600!)**


**Config pin:**

Short to ground to activate

**HOLD** - Disable the modchip without needing to remove power to unsolder

**LITE** - TODO

**BANKSEL** - Go back to OFW forcefully. It is equal to syscon command **w 1224 00**. Only use when absolutely needed.

    
