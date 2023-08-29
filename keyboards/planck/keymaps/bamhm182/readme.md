# bamhm182's Keyboard Layout

Basic Colemak keyboard with custom layers. I put a mouse on Lower because why not?

## Building

```
nix shell nixpkgs#dfu-util nixpkgs#gnumake nixpkgs#qmk nixpkgs#gcc-arm-embedded-9
sudo make planck/ez/glow:bamhm182:flash
```
