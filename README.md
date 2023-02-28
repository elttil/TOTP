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
git clone --recurse-submodules https://github.com/0xc1f1/totp
cd totp
make && make install
```

# Usage

```shell
totp -t [UNIX time] -d [digit] -i [interval] -h(print this message) [file containing the secret]
```

Either a file containing the TOTP secret key can be given as the first
argument. If no argument is supplied then it will by default read from
stdin.

# Example(s)

```shell
totp ./secret
```

```shell
echo JBSWY3DPEHPK3PXP | totp
```
