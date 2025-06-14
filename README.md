# EduWord

EduWord is a Microsoft Word-like word processor application for Linux, built using C++ and Qt.

## Features

- Rich text editing
- Document formatting (bold, italic, underline)
- Text alignment (left, center, right, justify)
- Font selection and sizing
- Text coloring
- File operations (new, open, save, save as)
- Printing support

## Prerequisites

- C++ compiler (GCC or Clang)
- CMake (version 3.10 or higher)
- Qt5 development libraries

### Installing Prerequisites on Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential cmake qtbase5-dev qtbase5-dev-tools libqt5printsupport5-dev
```

### Installing Prerequisites on Fedora

```bash
sudo dnf install gcc-c++ cmake qt5-qtbase-devel qt5-qtbase
```

### Installing Prerequisites on Arch Linux

```bash
sudo pacman -S base-devel cmake qt5-base
```

## Building from Source

You can use the provided build script:

```bash
./build.sh
```

Or manually build the project:

```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```

## Installation

You can use the provided installation script:

```bash
./install.sh
```

Or manually install the application:

```bash
cd build
sudo make install
```

## Usage

After installation, you can run EduWord from your applications menu or by typing `eduword` in the terminal.

### Basic Operations

- **New Document**: Create a new document (Ctrl+N)
- **Open Document**: Open an existing document (Ctrl+O)
- **Save Document**: Save the current document (Ctrl+S)
- **Save As**: Save the document with a new name (Ctrl+Shift+S)
- **Print**: Print the current document (Ctrl+P)
- **Exit**: Close the application (Ctrl+Q)

### Formatting

- **Bold**: Make selected text bold (Ctrl+B)
- **Italic**: Make selected text italic (Ctrl+I)
- **Underline**: Underline selected text (Ctrl+U)
- **Text Color**: Change the color of selected text
- **Font**: Change the font of selected text
- **Font Size**: Change the size of selected text
- **Alignment**: Left, center, right, or justify text

## License

This project is licensed under the MIT License - see the LICENSE file for details.