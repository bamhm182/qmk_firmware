# bamhm182's Keyboard Layout

Primarily Colemak-focused ZSA Planck EZ keymap.
There are diagrams in the keymap.c file.

## Building

```
nix shell nixpkgs#dfu-util nixpkgs#gnumake nixpkgs#qmk nixpkgs#gcc-arm-embedded-9
sudo make planck/ez/glow:bamhm182:flash
```
