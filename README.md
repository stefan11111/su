# su
Minimal su without pam dependency

This app is called su to make is easier to replace GNU/su.
All this app does is change the current shell into a root shell, provided you know root's password.
This app is very minimal and has no bloat.

# Installation

You can clone and build su with the following set of commands (as root if necessary):

```sh
git clone https://github.com/stefan11111/su.git
cd su
make
make install
```

To uninstall (run as root if necessary):

```sh
make uninstall
```

### Usage

```sh
su
```
