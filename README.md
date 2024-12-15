<div align="center">

# Terminal Wrap
🧑‍💻 Which command do you use the most in the terminal? Let's find out the fun way! 

<h3>
Terminal wrap tool written in <code>👑 C++ 👑</code>
</h3>
<br>

![License](https://img.shields.io/github/license/xeyossr/terminalwrap?style=for-the-badge)
![Commits](https://img.shields.io/github/commit-activity/m/xeyossr/terminalwrap?style=for-the-badge)
![GitHub Repo stars](https://img.shields.io/github/stars/xeyossr/terminalwrap?style=for-the-badge)
![GitHub issues](https://img.shields.io/github/issues/xeyossr/terminalwrap?style=for-the-badge)
![GitHub pull requests](https://img.shields.io/github/issues-pr/xeyossr/terminalwrap?style=for-the-badge)

![GitHub release (latest by date)](https://img.shields.io/github/v/release/xeyossr/terminalwrap?style=for-the-badge)

</div>

##  💫 Features
- Supports all popular shells!
  - bash
  - zsh
  - fish
  - csh

- Shows you motivational messages according to the command you use most. Show me your most used command and I'll tell you who you are 😀
- Yo, what more are you waiting for? lol

## 💻 Installing
---
### Installation 

There are 2 ways to install **TerminalWrap**:
1. Compile it on your own system (don't worry, this is much easier than you think)
2. Download the compiled version (not recommended)

If you don't want to compile it, all you need to do is install TerminalWrap from the [releases](https://github.com/xeyossr/terminalwrap/releases) page and then add it to PATH.
But if you want to compile it (recommended method) follow the steps below:

#### Compile yourself

1. Clone the repository:

```bash
$ git clone https://github.com/xeyossr/terminalwrap.git
$ cd terminalwrap
```

2. Dependencies:

You are expected to install the dependencies required to build TerminalWrap:

```bash
# Debian/Ubuntu
$ sudo apt install g++ libstdc++-dev

# RHEL/CentOs/Fedora
$ sudo dnf install gcc-c++ libstdc++-devel

# Arch
$ sudo pacman -S gcc
```

3. Compile the source code:

```bash
$ g++ -std=c++17 -o dist/terminalwrap terminalwrap.cpp
```

If you want the tool to run faster, you can also compile it with the following flags (because if your history file is too big, this may cause the tool to run slowly; recommended):

```bash
$ g++ -O3 -march=native -flto -funroll-loops -ftree-vectorize -std=c++17 -o dist/terminalwrap terminalwrap.cpp
```

4. Move the compiled file to PATH

To use TerminalWrap you need to add the compiled file to PATH (`/usr/bin/` on Linux systems):

```bash
$ sudo cp dist/terminalwrap /usr/bin/terminalwrap
```

5. Now you can run TerminalWrap with a single command!
```bash
$ terminalwrap
```

### Installation on Arch-based distros
You can easily install TerminalWrap via [AUR](https://aur.archlinux.org/packages/terminalwrap) for Arch Linux-based systems.

1. Install TerminalWrap via `yay`.

```bash
$ yay -S terminalwrap
```

2. Now you can easily run TerminalWrap with a simple command!
```bash
$ terminalwrap
```

## Usage

```bash
Usage: terminalwrap [OPTION]

Options:
  --help, -h       Show this help message.
  --fish           Force using Fish
  --zsh            Force using ZSH
  --csh            Force using CSH
  --bash           Force using BASH
  (default)        Detect the shell from the environment.
```

## License

This project is licensed under the GNU General Public License v3.0. You are free to use, modify, and distribute the software under the terms of this license. For more details, please refer to the [LICENSE](LICENSE) file.


## Thanks for idea & example 💬
- [islemci/cliwrap](https://github.com/islemci/cliwrap)
