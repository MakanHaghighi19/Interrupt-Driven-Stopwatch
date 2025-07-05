EGEC 450 Lab 3 – Interrupt-Driven Stopwatch

This embedded systems project implements a stopwatch with millisecond resolution using the Tiva-C microcontroller and a time-multiplexed seven-segment display. The system is fully interrupt-driven and was developed in C using Code Composer Studio. It was designed as part of the EGEC 450 Embedded Systems course at California State University, Fullerton.

---

Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Build & Flash Instructions](#build--flash-instructions)
- [Code Overview](#code-overview)
  - [Seven-Segment Display Driver](#seven-segment-display-driver)
  - [Stopwatch System](#stopwatch-system)
- [Interrupt Usage](#interrupt-usage)
- [Button Configuration](#button-configuration)
- [Timer Setup](#timer-setup)
- [Known Issues / Limitations](#known-issues--limitations)
- [Demo Instructions](#demo-instructions)
- [References](#references)
- [License](#license)

---

Project Overview

Overview

The lab is divided into two parts:

1. Seven-Segment Display Driver
   - Drives a multi-digit seven-segment display using SPI.
   - Uses time-multiplexing to refresh digits.
   - Operates with timer-based interrupts.

2. Stopwatch System
   - Implements a stopwatch with a 1 ms resolution.
   - Controlled by three buttons (Start/Stop, Increment, Reset).
   - Stopwatch updates are interrupt-driven.

## Features

- Time-multiplexed seven-segment display
- Real-time stopwatch functionality with 1 ms resolution
- Button-based control using GPIO interrupts
- Display refresh and debounce logic using timers

## Hardware Requirements

- Tiva-C LaunchPad (TM4C123GH6PM)
- EduBase-V2 Trainer Board or compatible 7-segment SPI display
- Three push-buttons (external recommended)
- Breadboard and jumper wires

Author: Makan Haghighi
