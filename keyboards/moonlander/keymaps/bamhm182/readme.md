# bamhm182's Keyboard Layout

Primarily Colemak-focused Moonlander keymap.
There are diagrams in the keymap.c file.

## Building

```
nix shell nixpkgs#dfu-util nixpkgs#gnumake nixpkgs#qmk nixpkgs#gcc-arm-embedded-9
sudo make moonlander:bamhm182:flash
```
