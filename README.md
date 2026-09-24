# PEAK Tools

Standalone Windows tools for PEAK PCAN interfaces. The repository uses a
Visual Studio 2019 solution and keeps the bundled PCAN-Basic SDK in
`ThirdParty/PCANBasic`.

## Included applications

### PCAN Sniffer

A lightweight CAN/CAN-FD sniffer, log replayer, and converter for PEAK
adapters, built on the PCAN-Basic API. The executable is still named
`USB2PCAN.exe` for compatibility with the original version of the project.

### PCAN CANopen Emulator

`PCANCanopenEmulator.exe` emulates four CANopen/CiA 422 load units. It sends
process data and heartbeat frames, handles the implemented SDO requests, and
provides editable operating states and counters.

### PCAN J1939 Emulator

`PCANJ1939Emulator.exe` generates FMS/J1939 vehicle data, including engine,
vehicle, fuel, environment, driver, tell-tale, text, and TPMS information.

### PCAN J1939 DTC Emulator

`PCANJ1939DtcEmulator.exe` generates configurable DM1 and DM2 diagnostic
messages for four ECUs. It supports address claims, request handling, and
J1939 BAM transport for multi-packet diagnostic messages.

Each emulator is a standalone project with its own PCAN-Basic integration.
Its GUI enumerates attached PEAK channels, selects a Classic CAN bitrate, and
connects or disconnects without BUSMASTER or `Wrapper_CAN`.

## Requirements

- Windows,
- a PEAK PCAN-USB adapter and the PEAK device driver,
- PCAN-Basic; the required headers, libraries, and DLLs are included in
  `ThirdParty/PCANBasic`,
- Visual Studio 2019 or newer with support for the `v142` toolset.

## PCAN Sniffer usage

### Features

- captures CAN and CAN-FD frames in a `candump -L` compatible format,
- replays captured frames with original timing or at full speed,
- bridges CAN1 and CAN2 in both directions,
- converts column logs, PCAN-View TRC, GVRET CSV, and SavvyCAN files,
- provides both a native Windows GUI and a command-line interface.

### GUI

Run `USB2PCAN.exe` without arguments. The GUI provides `Sniff`, `Send`, and
`Convert` modes, displays the generated command line, and captures program
output in the `Console` panel.

To enable bridging, select `Sniff` and check `Bridge CAN1 <-> CAN2`. Both
channels are selected automatically. Frames received on CAN1 are forwarded
to CAN2 and frames received on CAN2 are forwarded to CAN1. Bridge mode cannot
be combined with `Listen only`.

Use `Extra parameters` to append command-line options that do not have a
dedicated GUI control, for example `--debug`, `--no-brs`, or `--device 1`.

### Command line

```text
USB2PCAN.exe sniff -s 500000 --data 2000000 --can both -f capture.log
USB2PCAN.exe sniff -s 500000 --can both --classic --bridge -f bridge.log
USB2PCAN.exe send -s 500000 --data 2000000 --can both -f capture.log
USB2PCAN.exe convert -i PCAN_Std.trc -f capture.log
USB2PCAN.exe --help
```

Common options:

- `-s`, `--speed` - nominal bitrate; default: 500000 bit/s,
- `--data` - CAN-FD data-phase bitrate; default: 2000000 bit/s,
- `--can 1|2|both` - channel selection,
- `--classic` - use Classic CAN 2.0 instead of CAN-FD,
- `--listen` - enable listen-only mode,
- `--bridge` - forward frames between CAN1 and CAN2,
- `--fast`, `--loop`, `--batch-send` - replay options,
- `--debug` - print frame and PCAN-Basic call diagnostics.

Run `USB2PCAN.exe --help` or press `Help` in the GUI for the complete option
list.

## Emulator usage

Run the selected emulator, choose an attached PEAK interface and Classic CAN
bitrate in the `PEAK PCAN connection` panel, then press `Connect`. The status
line shows the selected PCAN handle and live RX, TX, and error counters.

The protocol controls remain editable while disconnected. Closing the main
window disconnects the channel and exits the application cleanly.

## Building

Open `PEAKTools.sln`, select the `x64` or `Win32` platform and the `Debug` or
`Release` configuration, then build the solution. The post-build step copies
the matching `PCANBasic.dll` into the output directory, for example:

```text
bin\x64\Release\PCANSniffer\USB2PCAN.exe
bin\x64\Release\PCANCanopenEmulator\PCANCanopenEmulator.exe
bin\x64\Release\PCANJ1939Emulator\PCANJ1939Emulator.exe
bin\x64\Release\PCANJ1939DtcEmulator\PCANJ1939DtcEmulator.exe
bin\x64\Release\PCANSniffer\PCANBasic.dll
```

PEAK device instances can be selected with `--device`. For device index `0`,
CAN1 and CAN2 map to the first two PCAN-USB channels exposed by PCAN-Basic.
