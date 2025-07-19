# Skeyd

**Skeyd** is a simple GUI tool for managing keyboard remapping profiles using [keyd](https://github.com/rvaiya/keyd) on Linux.

This program **does not** modify your keyboard map directly — it generates and applies configuration files for **keyd**, so **keyd** must be installed and running first.

## 📌 Prerequisites

Make sure `keyd` is installed and enabled:

```bash
git clone https://github.com/rvaiya/keyd
cd keyd
make && sudo make install
sudo systemctl enable --now keyd
```

## 📦 Build

Clone this repository and build it with CMake:
```bash
git clone https://github.com/yourusername/Skeyd.git
cd Skeyd
mkdir build
cd build
cmake ..
make
```

## ⚙️ Usage

Launch the Skeyd binary.

Create or load .txt keyboard mapping profiles.

Double-click a profile to apply it (requires pkexec privileges).

All profiles are saved under ~/Skeyd/profiles.

Note: Applying a profile overwrites /etc/keyd/default.conf.


## 📁 Profile Format

Each profile is a simple .txt file with sections. Example:

[main]
key1 = value1
key2 = value2

[other]
# other config...

## ✅ Features

Create new profiles

Load existing profiles

Apply profiles safely with pkexec

Delete unwanted profiles


## ⚠️ Disclaimer

You must have keyd installed and configured correctly.
Changing keyboard mappings may affect your system input — proceed with caution.


## Now
You need to edit some things int code to work on your system like system paths.
