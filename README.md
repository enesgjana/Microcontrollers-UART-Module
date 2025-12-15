# TM4C1294 LED Control via UART

## Overview
This embedded C program provides a command-line interface for controlling LEDs on a Texas Instruments TM4C1294NCPDT microcontroller via UART2 communication. Users can send simple text commands through a serial terminal to turn individual LEDs on or off.

## Hardware Requirements
- **Microcontroller**: TM4C1294NCPDT
- **LEDs**: Connected to Port M pins PM0-PM3 (4 LEDs total)
- **UART Interface**: UART2 on Port D
  - RX: PD4
  - TX: PD5
- **Baud Rate**: 115200 bps, 8 data bits, 1 stop bit, no parity

## Features
1. **LED Control**: Turn on/off any of 4 LEDs individually
2. **Command Validation**: Strict syntax checking with graceful error handling
3. **Interactive Prompt**: Visual command prompt for user interaction
4. **Echo Display**: Received commands are echoed back via printf()
5. **Continuous Operation**: Infinite loop for repeated command processing

## Command Syntax
Commands follow a strict 6-character format:

```
led<action><number>
```

- **led**: Command prefix (required, lowercase)
- **action**: 
  - `+` to turn LED on
  - `-` to turn LED off
- **number**: LED identifier (0, 1, 2, or 3)

### Valid Command Examples
- `led+0` - Turn on LED 0
- `led-0` - Turn off LED 0
- `led+1` - Turn on LED 1
- `led-3` - Turn off LED 3

### Invalid Commands
Any deviation from the syntax is ignored:
- `LED+0` (uppercase)
- `led+4` (LED number out of range)
- `ledon0` (invalid action character)
- `led+0 ` (extra spaces or characters)

## How to Use

### 1. Hardware Setup
- Connect LEDs to Port M pins PM0-PM3 with appropriate current-limiting resistors
- Connect UART2 pins (PD4/PD5) to a USB-to-serial adapter or debugger
- Power the TM4C1294 board

### 2. Terminal Configuration
Configure your serial terminal (PuTTY, Tera Term, minicom, etc.) with:
- **Port**: Check device manager for COM port
- **Baud Rate**: 115200
- **Data Bits**: 8
- **Stop Bits**: 1
- **Parity**: None
- **Flow Control**: None

### 3. Running the Program
1. Compile and flash the program to the microcontroller
2. Open your serial terminal
3. Wait for the prompt: `>` (preceded by carriage return and line feed)
4. Type a command and press Enter
5. The command will be echoed back
6. The corresponding LED will turn on or off
7. A new prompt appears for the next command

### Example Session
```
>led+0
led+0
>led+1
led+1
>led-0
led-0
>led+2
led+2
```

## Code Structure

### Main Function (`main`)
- Initializes Port M (LEDs) and Port D (UART)
- Configures UART2 for 115200 baud communication
- Enters infinite loop:
  - Sends prompt
  - Receives command string (up to 50 characters)
  - Echoes command via printf()
  - Decodes and executes command

### Port Initialization (`init_port_m`)
- Enables clock for Port M
- Configures PM0-PM3 as digital outputs
- Initializes all LEDs to off state

### Command Processing (`decode_command`)
- Validates command syntax character by character
- Extracts action (on/off) and LED number
- Controls GPIO pins via bit manipulation
- Aborts silently on syntax violations

### Prompt Display (`send_prompt`)
- Sends carriage return (0x0D)
- Sends line feed (0x0A)
- Sends '>' character
- Waits for UART transmit buffer availability

## Technical Details

### UART Configuration
- **Clock Frequency**: 120 MHz (assumed system clock)
- **Baud Rate**: 115200
- **Integer Baud Rate Divisor (IBRD)**: 8
- **Fractional Baud Rate Divisor (FBRD)**: 44
- **Frame Format**: 8-N-1 (8 data bits, no parity, 1 stop bit)

### GPIO Pin Mapping
| LED | Port M Pin | Bit Position |
|-----|------------|--------------|
| 0   | PM0        | Bit 0        |
| 1   | PM1        | Bit 1        |
| 2   | PM2        | Bit 2        |
| 3   | PM3        | Bit 3        |

### Buffer Management
- Maximum input buffer size: 50 characters
- Commands terminated by Enter key (0x0D)
- Buffer automatically null-terminated
- Overflow protection included

## Error Handling
The program implements silent error handling:
- Invalid commands are ignored without error messages
- No LED state change occurs on invalid input
- System continues waiting for next valid command

## Limitations
- Commands must be typed exactly as specified (case-sensitive)
- No feedback for invalid commands
- No command history or editing capabilities
- Maximum command length: 49 characters + null terminator
- Only controls 4 LEDs (expandable by modifying code)

## Troubleshooting

**LEDs don't respond:**
- Verify LED connections to PM0-PM3
- Check LED polarity and current-limiting resistors
- Confirm power supply to microcontroller

**No prompt appears:**
- Check UART connections (TX/RX may be swapped)
- Verify terminal baud rate settings (115200)
- Ensure UART2 pins are properly configured

**Commands don't work:**
- Use lowercase only: `led+0` not `LED+0`
- Ensure no spaces: `led+0` not `led + 0`
- Press Enter after typing command
- Verify LED number is 0-3

## Extension Ideas
- Add status display showing current LED states
- Implement error messages for invalid commands
- Support multiple LED control in one command
- Add LED blinking or pattern capabilities
- Include help command to display syntax

## License
This code is provided for educational purposes. Modify and use as needed for your projects.
