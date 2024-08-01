# Wayland Protocols

This folder contains the generated protocol files for `xdg-shell` which are required for Wayland support in this project.

## Files

- `xdg-shell-client-protocol.h`: The header file generated from the `xdg-shell.xml` protocol definition.
- `xdg-shell-protocol.c`: The source file generated from the `xdg-shell.xml` protocol definition.
- `readme.md`: This file.
- `license.md`: The license for the `xdg-shell` protocol files.

## Generation

These files were generated using the `wayland-scanner` tool from the `xdg-shell.xml` file, which is part of the Wayland protocols repository.

## Source

`xdg-shell.xml` can be aquired from the official gitlab repository, or by installing `wayland-protocols` package on your computer.

### Packages

By default, the `xdg-shell.xml` can be found in `/usr/share/wayland-protocols/stable/xdg-shell/` folder.

##### Debian-based systems

```sh
sudo apt-get install libwayland-dev wayland-protocols
```

##### Fedora-based systems

```sh
sudo dnf install wayland-devel wayland-protocols-devel
```

##### Arch-based systems

```sh
sudo pacman -S wayland wayland-protocols
```

### Gitlab

These files are generated from the `xdg-shell.xml` file, which can be found in the Wayland protocols repository:
[Wayland Protocols Repository](https://gitlab.freedesktop.org/wayland/wayland-protocols)

[xdg-shell.xml file in Wayland Protocols Repository](https://gitlab.freedesktop.org/wayland/wayland-protocols/-/blob/main/stable/xdg-shell/xdg-shell.xml)

## Commands Used

```sh
wayland-scanner client-header <path to xdg-shell.xml> xdg-shell-client-protocol.h
wayland-scanner code <path to xdg-shell.xml> xdg-shell-protocol.c
```

