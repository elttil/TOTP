#!/bin/sh
selection="$HOME/.config/totp/"$(ls -1 ~/.config/totp/ | dmenu)
[ -e $selection ] || exit
./totp "$selection" | xclip -selection c
