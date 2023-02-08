# su
Minimal su without pam dependency

This app is called su to make is easier to replace GNU/su.
All this app does is change the current shell into another user's shell, provided you know his's password.
If you don't specify a user, it defaults to root.
This app is very minimal and has no bloat.

# Configuration

All configuration for this app is done by editing the source code.
You can chose what shell to spawn if the user has no shell by editing the SHELL macro in su.h.
After editing su.h, the app must be recompiled.
Doing things this way keeps su fast, secure and simple.

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
su [user]
```
