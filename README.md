# 💔 Destruction II: Resurrection Project

<div align="center">

![Banner](https://img.shields.io/badge/Destruction%20II-Resurrection%20Project-red?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-In%20Development-yellow?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Windows%2010%2F11-blue?style=for-the-badge)
![Love](https://img.shields.io/badge/Made%20with-Love%20%26%20Memories-pink?style=for-the-badge)

### *"Some games live in our hearts forever..."*

**[🇷🇺 Русская версия](README_RU.md)**

</div>

---

## 📖 A Personal Story

My cousin and I absolutely loved this game when we were 10-15 years old. We spent endless hours battling each other, laughing at failed shots, and celebrating epic victories. Destruction II wasn't just a game for us — it was our tradition, our way of being together.

Even as we grew up, every time we met, we'd fire up this old game, returning to those carefree times. The pixelated tanks, destructible terrain, simple graphics — it was all part of our friendship.

**But life is unpredictable. My cousin is no longer with us...**

I can't bring him back to life, but I can bring our game and our memories back to life. Every line of code in this project is a tribute to those happy moments we shared together.

## 🎮 About This Project

**Destruction II** is a legendary turn-based tactical tank and artillery game created by Johan Torp (Nocturnal Development) in 2001-2003. Released as freeware, it captured the hearts of thousands of players worldwide.

### Why This Project Matters:

- 🕹️ **Nostalgia** — For many of us, this game is part of our childhood
- 💻 **Inaccessibility** — The game doesn't work on Windows 10/11
- 🎯 **Preserving Legacy** — Classic games shouldn't disappear
- ❤️ **Memory** — This is more than just code, these are memories

## 🤝 Who's Behind This Project?

I'm not a programmer. Not a game developer. Not an engineer.

**I'm just a person with memories and a dream.**

I've been using computers since I was 6 years old, but I've never written code professionally. This entire project is being created with the help of artificial intelligence:

- 🤖 **Claude 4.5 Sonnet** (Opus 4.1, Haiku 4.5) — my main assistant
- 💬 I just describe in plain language what needs to be done
- 🧠 AI transforms my words into working code
- ⚡ We work together when I have free time

**I'm not a "prompt engineer". I'm just someone who knows how to set tasks properly and has a dream.**

## 🚀 Development Roadmap

### 📊 Current Progress: **85%** ⬛⬛⬛⬛⬛⬛⬛⬛⬜⬜

**Last Updated**: October 24, 2025
**Status**: 🟢 BREAKTHROUGH! SDL rendering works! 🎉

📋 **Detailed progress tracking**: See [PROGRESS.md](PROGRESS.md)

### ✅ Completed:
- ✅ **GAME LAUNCHES!** SDL rendering successfully works!
- ✅ Full transition from DirectX 7 to SDL2
- ✅ Updated from C++98 to C++20 (all 327 files compile!)
- ✅ vcpkg package manager + CMake build system
- ✅ Git repository on GitHub: https://github.com/elchin92/destruction_II_hd
- ✅ SDL2_DirectDrawCompat and SDL2_DirectInputCompat compatibility layers
- ✅ All game bitmaps load and convert successfully
- ✅ Game loop runs, Flip() is called every frame

### 🔄 In Progress:
- 🔄 Fixing black backbuffer (game graphics not rendering yet)
- 🔄 Solving dual window issue (Win32 and SDL)
- ⏳ Porting sound to SDL2_mixer

### 📅 Phase 1: Resurrection (Current)
- ✅ Porting to Windows 10/11 x64
- 🔄 Replacing DirectX 7 with SDL2
- 🔄 Updating from C++98 to C++20
- ⏳ First working build

### 🎨 Phase 2: HD Remaster
- 📐 High resolution support (4K)
- 🎮 Improved controls
- 🎵 Remastered audio
- 💎 Steam release

### 📱 Phase 3: Mobile Era
- 📱 iOS version
- 🤖 Android version
- 🌐 Online multiplayer
- 🏆 Global leaderboards

### 🌟 Phase 4: Destruction III
If everything works out... If the community supports us...
Maybe we'll create a completely new game — **Destruction III**.
With your help. With your ideas. With your support.

## 💝 How You Can Help

I need help from people who understand:
- **This is a project of love**, not commerce
- **This is a tribute**, not just a port
- **This is a community**, not a solo project

### Help Needed:
- 🐛 **Testing** — Find bugs, help fix them
- 💻 **Code** — Improve, optimize, modernize
- 🎨 **Graphics** — HD textures, new sprites
- 🎵 **Sound** — Remaster sound effects
- 🌍 **Translation** — Localize to other languages
- 📢 **Promotion** — Tell your friends
- ⭐ **Support** — Just star this repository

## 🛠️ Technical Information

### Original Game (2003)
- DirectX 7 (DirectDraw, DirectInput, DirectSound)
- C++98, Visual C++ 6.0
- x86 (32-bit)
- Windows 98/XP

### Modernization (2025)
- SDL2 (cross-platform graphics)
- C++20, Visual Studio 2022
- x64 (64-bit)
- Windows 10/11

### How to Build:

```bash
# Clone the repository
git clone https://github.com/yourusername/DestructionII-Resurrection.git

# Install vcpkg (package manager)
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat

# Configure the project
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="[path to vcpkg]/scripts/buildsystems/vcpkg.cmake"

# Build
cmake --build build --config Release

# Run the game!
.\build\bin\Release\DestructionII.exe
```

## 📜 Version History

- **v0.1** (in development) - Initial port to modern systems
- **v1.0** (2003) - Original version by Johan Torp

## ❤️ Acknowledgments

- **Johan Torp / Nocturnal Development** — For creating this masterpiece
- **Claude AI (Anthropic)** — For turning my words into code
- **My Cousin** — For all those happy moments. Rest in peace.
- **Everyone Who Supports** — You make this possible

## 📬 Contact

If you want to help or just share your memories of the game:
- 🐙 GitHub Issues — For bugs and suggestions
- ⭐ Star — If this project touched your heart
- 🔄 Fork — If you want to contribute

---

<div align="center">

### *"This game is a bridge between past and future, between memory and dream"*

**Made with love and memories**

</div>

---

## 🏗️ Current Development Status

### ✅ Completed
- ✅ **GAME RUNS!** Window opens, SDL rendering functional
- ✅ CMake build system for x64, C++20
- ✅ vcpkg dependency management
- ✅ Full codebase compiles (327 files, 0 errors!)
- ✅ SDL2 DirectDraw compatibility layer (90% complete)
- ✅ SDL2 DirectInput compatibility layer (basic functionality)
- ✅ Fixed uninitialized pointer crashes
- ✅ Fixed bitmap path issues
- ✅ Emotional README in Russian and English
- ✅ GitHub repository: https://github.com/elchin92/destruction_II_hd

### 🔄 In Progress
- 🔄 Fixing black backbuffer (test graphics work, game graphics don't)
- 🔄 Solving dual window problem
- 🔄 DirectSound → SDL2_mixer migration

### ⏳ Next Steps
- Debug why BltFast() doesn't render game graphics
- Remove test graphics and fix actual game rendering
- Port sound system to SDL2_mixer
- Create installer for Windows 10/11

---

## 📝 License

Original game: Copyright © 2001-2003 Johan Torp / Nocturnal Development

This modernization project is created to preserve gaming heritage and is distributed with respect to the original creators.

---

<div align="center">

![GitHub stars](https://img.shields.io/github/stars/yourusername/DestructionII-Resurrection?style=social)
![GitHub forks](https://img.shields.io/github/forks/yourusername/DestructionII-Resurrection?style=social)
![GitHub watchers](https://img.shields.io/github/watchers/yourusername/DestructionII-Resurrection?style=social)

</div>

