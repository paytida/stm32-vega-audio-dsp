# Realtime Wireless Audio Streaming and Digital Signal Processing

A low-latency wireless audio streaming system built using **STM32 microcontrollers**, **NRF24L01+ 2.4 GHz transceivers**, and the **VEGA development board**. The project demonstrates reliable real-time audio transmission between embedded nodes while performing digital signal processing (DSP) with minimal latency and efficient resource utilization.

---

## Overview

This project implements a complete embedded wireless audio communication pipeline capable of:

- Capturing audio from an input source
- Processing audio samples in real time
- Packetizing and transmitting audio wirelessly
- Detecting transmission errors
- Recovering from packet loss through retransmission
- Receiving and reconstructing the audio stream
- Playing back audio with minimal latency

The primary objective is to build a robust wireless audio link suitable for real-time embedded applications while operating under the resource constraints of microcontrollers.

---

## Features

- Real-time wireless audio streaming
- Low-latency audio transmission
- Digital signal processing pipeline
- SPI-based communication with NRF24L01+
- Circular buffering for continuous streaming
- Custom packet framing protocol
- Packet sequence numbering
- Error detection using checksum/CRC
- Retransmission mechanism for lost packets
- Optimized memory utilization
- Interrupt-driven firmware architecture
- Modular embedded software design

---

# Hardware

## Transmitter

- STM32 Development Board
- NRF24L01+ Wireless Module
- Audio input source
  - Microphone module / ADC input / Line input
- Power Supply

## Receiver

- STM32 Development Board
- NRF24L01+ Wireless Module
- Audio DAC / PWM Output
- Speaker / Audio Amplifier

## DSP Platform

- VEGA Development Board
- Audio peripherals
- DSP processing modules

---

# Software Stack

- Embedded C
- STM32 HAL Libraries
- SPI Driver
- Timer Interrupts
- DMA (if enabled)
- GPIO
- UART Debug Interface

Development Environment:

- STM32CubeIDE
- GCC ARM Toolchain

---

# System Architecture

```
                Audio Input
                     │
                     ▼
             Audio Sampling
                     │
                     ▼
          Digital Signal Processing
                     │
                     ▼
             Packet Formation
                     │
                     ▼
              SPI Interface
                     │
                     ▼
             NRF24L01 Transmitter
                     │
         2.4 GHz Wireless Channel
                     │
             NRF24L01 Receiver
                     │
                     ▼
              Packet Validation
                     │
                     ▼
          Error Detection & Recovery
                     │
                     ▼
            Audio Reconstruction
                     │
                     ▼
              Audio Playback
```

---

# Firmware Architecture

```
main()

│

├── System Initialization

│      ├── Clock

│      ├── GPIO

│      ├── SPI

│      ├── UART

│      ├── Timers

│      └── Audio Interface

│

├── Audio Capture

│

├── DSP Processing

│

├── Packet Formation

│

├── Wireless Transmission

│

├── Wireless Reception

│

├── Packet Validation

│

├── Playback Buffer

│

└── Audio Output
```

---

# Communication Pipeline

## Audio Acquisition

The transmitter continuously samples audio using the onboard ADC/audio interface.

Each frame consists of a fixed number of audio samples.

Example:

```
ADC

↓

128 Samples

↓

Processing Buffer
```

---

## DSP Processing

The sampled audio undergoes processing before transmission.

Typical operations include:

- Gain adjustment
- Filtering
- Noise suppression
- Sample normalization
- Frame buffering

The DSP stage is optimized for real-time execution without interrupting continuous acquisition.

---

## Packet Formation

Each packet contains

```
+------------+------------+-----------+-----------+
| Header     | Sequence   | Payload   | CRC       |
+------------+------------+-----------+-----------+
```

Fields include

- Packet ID
- Sequence Number
- Audio Samples
- Error Detection Code

---

## Wireless Transmission

Communication uses the NRF24L01+ transceiver over SPI.

Workflow:

```
Audio Buffer

↓

Packet Builder

↓

SPI Driver

↓

NRF24L01+

↓

Wireless Link
```

---

## Packet Reception

Incoming packets are verified before playback.

Receiver performs:

- Header validation
- Sequence verification
- CRC verification
- Packet ordering

---

## Error Handling

Wireless channels are susceptible to packet losses.

The firmware includes:

- Packet numbering
- Lost packet detection
- Retransmission requests
- Duplicate packet rejection

This improves robustness without introducing excessive latency.

---

## Playback

Validated packets are stored inside a playback buffer.

The audio output module continuously reads this buffer and sends samples to the DAC/PWM output.

A circular buffer prevents:

- Audio glitches
- Buffer underruns
- Playback discontinuities

---

# Memory Management

The firmware uses multiple buffers:

```
ADC Buffer

↓

Processing Buffer

↓

Transmit Buffer

↓

Wireless Packet

↓

Receive Buffer

↓

Playback Buffer
```

Circular buffering minimizes memory copies and allows concurrent processing.

---

# Optimization Techniques

Several optimizations were implemented to maintain real-time performance.

### Efficient SPI Communication

- Reduced communication overhead
- Optimized packet transfers
- Hardware SPI utilization

### Interrupt Driven Design

- Non-blocking execution
- Fast ISR routines
- Reduced CPU idle time

### Memory Optimization

- Static memory allocation
- Circular buffers
- Minimal dynamic memory usage

### CPU Optimization

- Lightweight DSP routines
- Fixed-size packet processing
- Efficient buffer management

---

# Project Structure

```
├── Core
│   ├── Inc
│   ├── Src
│
├── Drivers
│
├── Middleware
│
├── DSP
│   ├── Filters
│   ├── Audio Processing
│
├── NRF24L01
│   ├── Driver
│   ├── SPI Interface
│
├── VEGA
│   ├── DSP Modules
│
├── Docs
│
├── Images
│
└── README.md
```

---

# Performance

| Parameter | Value |
|-----------|-------|
| Wireless Interface | NRF24L01+ |
| MCU | STM32 |
| DSP Platform | VEGA |
| Communication | SPI |
| Streaming | Real-time |
| Packet Loss Recovery | Supported |
| Processing | Embedded DSP |
| Buffering | Circular Buffer |

---

# Applications

- Wireless microphones
- Embedded intercom systems
- IoT audio devices
- Smart speakers
- Industrial monitoring
- Wireless sensor networks
- Digital audio communication
- Educational DSP platform

---

# Future Improvements

- Adaptive bitrate streaming
- Audio compression (ADPCM)
- Forward Error Correction (FEC)
- Stereo audio support
- I²S codec integration
- Bluetooth Low Energy comparison
- AES encryption
- Dynamic channel selection
- Mesh networking
- RTOS-based scheduling

---

# Technologies Used

- STM32
- VEGA Board
- NRF24L01+
- Embedded C
- SPI
- UART
- ADC
- PWM/DAC
- Digital Signal Processing
- Real-Time Embedded Systems

---

# Learning Outcomes

Through this project, the following concepts were explored:

- Embedded firmware architecture
- Wireless communication protocols
- Low-latency system design
- Digital signal processing
- Interrupt-driven programming
- Buffer management
- Embedded optimization techniques
- SPI peripheral programming
- Error detection and recovery
- Real-time software development

---

## Contributors

Developed as an embedded systems project focusing on **real-time wireless audio streaming**, **digital signal processing**, and **low-latency embedded communication** using STM32 and VEGA platforms.
