# Simple TOTP

A super simple application that takes in a TOTP secret key and returns
the TOTP code.

This application is intended to be used in combination with other
utilities. A example is included in the file `dmenu_select.sh` which
will give you a dmenu prompt to select any of the files in
`~/.config/totp/` that will contain TOTP keys. It will then put the TOTP
token of said file in the clipboard.

# Install

```shell
git clone https://github.com/0xc1f1/totp
cd totp
make && make install
```

# Usage

```shell
totp -t [UNIX time] -d [digit] -i [interval] -h(print this message) [TOTP token]
```

Either the TOTP key can be supplied as a argument. If no argument is
supplied then by default it will read a TOTP key from stdin.

# Example(s)

```shell
totp JBSWY3DPEHPK3PXP
```

```shell
echo JBSWY3DPEHPK3PXP | totp
```
