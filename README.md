# PEAK Tools

Standalone Windows tools for PEAK PCAN interfaces. The repository uses a
Visual Studio 2017 solution and keeps the bundled PCAN-Basic SDK in
`ThirdParty/PCANBasic`.

## Included applications

### PCAN Sniffer

A lightweight CAN/CAN-FD sniffer, log replayer, and converter for PEAK
adapters, built on the PCAN-Basic API. The executable is still named
`USB2PCAN.exe` for compatibility with the original version of the project.

## Features

- captures CAN and CAN-FD frames in a `candump -L` compatible format,
- replays captured frames with original timing or at full speed,
- bridges CAN1 and CAN2 in both directions,
- converts column logs, PCAN-View TRC, GVRET CSV, and SavvyCAN files,
- provides both a native Windows GUI and a command-line interface.

## Requirements

- Windows,
- a PEAK PCAN-USB adapter and the PEAK device driver,
- PCAN-Basic; the required headers, libraries, and DLLs are included in
  `ThirdParty/PCANBasic`,
- Visual Studio 2017 or newer with support for the `v141` toolset.

## GUI

Run `USB2PCAN.exe` without arguments. The GUI provides `Sniff`, `Send`, and
`Convert` modes, displays the generated command line, and captures program
output in the `Console` panel.

To enable bridging, select `Sniff` and check `Bridge CAN1 <-> CAN2`. Both
channels are selected automatically. Frames received on CAN1 are forwarded
to CAN2 and frames received on CAN2 are forwarded to CAN1. Bridge mode cannot
be combined with `Listen only`.

Use `Extra parameters` to append command-line options that do not have a
dedicated GUI control, for example `--debug`, `--no-brs`, or `--device 1`.

## Command Line

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

## Building

Open `PEAKTools.sln`, select the `x64` or `Win32` platform and the `Debug` or
`Release` configuration, then build the solution. The post-build step copies
the matching `PCANBasic.dll` into the output directory, for example:

```text
x64\Release\USB2PCAN.exe
x64\Release\PCANBasic.dll
```

PEAK device instances can be selected with `--device`. For device index `0`,
CAN1 and CAN2 map to the first two PCAN-USB channels exposed by PCAN-Basic.
