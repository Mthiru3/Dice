# ARM7 Digital Dice Roller Game (NXP LPC2129)

A bare-metal embedded C firmware application developed for the **NXP LPC2129 (ARM7TDMI-S)** microcontroller. This project implements an interactive digital dice roller game by combining high-speed hardware timers, external vector interrupts, custom character design, and a wide **20x4 character LCD display**.

---

## 🚀 Features

* **True Pseudo-Randomness:** Leverages unpredictable human reaction times by sampling a free-running hardware timer (`T0TC`) at the exact millisecond an external interrupt fires.
* **Custom CGRAM Character Bitmaps:** Implements custom graphics generation to bypass standard alphanumeric LCD limitations, rendering custom dice borders and face dots.
* **20x4 Layout Optimization:** Fully utilizes the 20-character width layout for clear user instructions (`"RELEASE TO PICK !!!"`) and perfectly aligned animated rolling sequences.
* **Efficient Interrupt-Driven Architecture:** The CPU remains free to cycle through rolling graphics while the crucial randomization tracking is isolated to a hardware ISR (Interrupt Service Routine).

---

## 🛠️ Hardware Requirements

* **Microcontroller:** NXP LPC2129 Development Board (ARM7 core).
* **Display:** 20x4 Character LCD (Hitachi HD44780 compliant or equivalent parallel interface).
* **Input Switch:** 1x Push Button mapped to `EINT0` (External Interrupt 0 / Pin `P0.16`).

---

## ⚙️ How It Works

### 1. Custom Graphics Generation
Standard character LCDs only possess standard alphanumeric blocks. By sending the custom pixel mapping array (`cgram[]`) to the LCD's **CGRAM (Character Generator RAM)** space via `LCD_CGRAM(48);`, we generate 6 custom $5 \times 8$ dot-matrix dice characters.



### 2. The Randomization Loop
True algorithmic randomness is computationally difficult to simulate cleanly on lightweight microcontrollers. This project resolves that using a real-time sampling strategy:
1. **Timer 0** runs continuously at millions of clock cycles per second.
2. The user initiates a roll by holding down the hardware button.
3. An edge-triggered interrupt (`EINT0`) instantly trips an ISR execution block.
4. The exact state of the counter register (`T0TC`) is captured and converted to a safe index scope: 
   $$\text{count} = (T0TC \pmod 6) + 1$$
5. This yields a perfectly fair, randomized output range from **1 to 6**.

### 3. State Sequence
```text
[Idle/Rolling State] ──> [Button Pressed / ISR Fires] ──> [Release Prompt Alert] ──> [Display Dice Selection]
