# ReSize

Simple program that resizes the selected window when a hotkey is pressed. I like certain windows to be certain sizes. This helped me scratch that itch. Being able to resize windows without leaving my keyboard feels pretty good too.

## Prerequisites

- Visual Studio 2022 or greater
- Latest Windows SDK (included with Visual Studio)

## Setup Instructions

1. Clone or download this repository to your local machine.
2. Open `ReSize.sln` with Visual Studio.
3. Ensure the necessary Windows SDK and standard libraries are installed.
4. Build the project by selecting `Build > Build Solution` or pressing `CTRL + SHIFT + B`.

## Usage

1. Run `ReSize.exe` (see output directory).
2. Resize/move selected windows using one of six hotkeys.
3. Close ReSize by right-clicking the tray icon and selecting the `Exit` option.

## Hotkeys

| Hotkey                | Description                           |
| :----:                | :----:                                |
| `CTRL + 1`            | Decrease window size                  |
| `CTRL + 2`            | Increase window size                  |
| `CTRL + 3`            | Center window                         |
| `CTRL + SHIFT + 1`    | Center window and resize (small)      |
| `CTRL + SHIFT + 2`    | Center window and resize (medium)     |
| `CTRL + SHIFT + 3`    | Center window and resize (large)      |
