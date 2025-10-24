# Contributing to Destruction II HD

First off, thank you for considering contributing to Destruction II HD! This project is a labor of love to bring a classic game to modern systems, and every contribution helps make it better.

## 🎮 Ways to Contribute

### 1. Report Bugs
Found a bug? Please help us fix it!
- Check if the bug has already been reported in [Issues](https://github.com/elchin92/destruction_II_hd/issues)
- If not, [open a new issue](https://github.com/elchin92/destruction_II_hd/issues/new)
- Use a clear title and description
- Include steps to reproduce
- Add your system information (Windows version, hardware specs)
- Attach screenshots if relevant

### 2. Suggest Features
Have ideas for improvements?
- Check [Issues](https://github.com/elchin92/destruction_II_hd/issues) for similar suggestions
- Open a new issue with the `enhancement` label
- Describe your idea clearly
- Explain why it would benefit the game

### 3. Code Contributions
Ready to code? Awesome!

#### First Time Setup
```bash
# Fork the repository
# Clone your fork
git clone https://github.com/YOUR_USERNAME/destruction_II_hd.git
cd destruction_II_hd

# Install vcpkg (if not already installed)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg integrate install

# Install dependencies
./vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-mixer:x64-windows

# Build the project
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Debug
```

#### Development Workflow
1. Create a new branch for your feature: `git checkout -b feature/amazing-feature`
2. Make your changes
3. Test thoroughly - the game should still run!
4. Commit with meaningful messages: `git commit -m 'Add amazing feature'`
5. Push to your fork: `git push origin feature/amazing-feature`
6. Open a Pull Request

#### Code Style Guidelines
- Use consistent indentation (4 spaces)
- Follow the existing code style in the file you're editing
- Comment complex logic
- Keep functions focused and small when possible
- Update relevant documentation

### 4. Areas Needing Help

We especially need help with:

#### 🎨 Graphics & UI
- Improving aspect ratio handling for modern displays
- Creating high-resolution textures
- UI scaling for different resolutions
- Menu improvements and modernization

#### 🔊 Audio
- Implementing SDL2_mixer for sound effects
- Adding music support
- Volume controls and audio settings

#### 🎮 Gameplay
- Controller support via SDL2
- Network multiplayer improvements
- AI enhancements
- New weapons or game modes

#### 📚 Documentation
- Translating README to other languages
- Creating gameplay guides
- Documenting code architecture
- Writing tutorials

#### 🧪 Testing
- Testing on different Windows versions
- Performance testing and optimization
- Finding and reporting bugs
- Creating automated tests

### 5. Good First Issues

Look for issues labeled [`good first issue`](https://github.com/elchin92/destruction_II_hd/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22) - these are perfect for newcomers!

## 📋 Pull Request Process

1. **Before submitting:**
   - Test your changes thoroughly
   - Update documentation if needed
   - Add comments for complex code
   - Check that the game still compiles and runs

2. **In your PR:**
   - Reference any related issues
   - Describe what you changed and why
   - Include screenshots for visual changes
   - List any breaking changes

3. **After submitting:**
   - Be responsive to feedback
   - Make requested changes promptly
   - Be patient - reviews take time

## 🏗️ Project Structure

```
/
├── Code/                 # Main game source code
│   ├── *.cpp            # Implementation files
│   ├── *.h              # Header files
│   └── SDL2_*Compat.*   # SDL2 compatibility layers
├── Bitmaps/             # Game graphics
├── RunDir/              # Runtime resources
│   ├── Sound/          # Audio files
│   ├── Data/           # Configuration
│   └── Save/           # Save games
├── media/               # Screenshots and promotional materials
└── build/               # Build output (generated)
```

## 🔧 Technical Details

### Key Technologies
- **Language:** C++20
- **Graphics:** SDL2 (replacing DirectDraw)
- **Input:** SDL2 (replacing DirectInput)
- **Audio:** SDL2_mixer (replacing DirectSound)
- **Build:** CMake + vcpkg

### Important Files
- `CLAUDE.md` - AI assistant instructions and architecture notes
- `PROGRESS.md` - Current development status
- `CMakeLists.txt` - Build configuration
- `vcpkg.json` - Dependency management

### Architecture Notes
The project uses compatibility layers to translate old DirectX calls to SDL2:
- `SDL2_DirectDrawCompat.*` - Graphics translation
- `SDL2_DirectInputCompat.*` - Input translation
- Future: `SDL2_DirectSoundCompat.*` - Audio translation

This approach minimizes changes to the original game code while enabling modern system support.

## 🤝 Code of Conduct

This project adheres to a Code of Conduct that all contributors are expected to follow. Please read the full text so that you can understand what actions will and will not be tolerated.

📖 **[Read the full Code of Conduct](CODE_OF_CONDUCT.md)**

In summary:
- Be welcoming to newcomers
- Accept constructive criticism gracefully
- Focus on what's best for the project
- Show empathy towards other contributors

## 📬 Contact

- **GitHub Issues:** Best for bugs and features
- **Email:** [Your email here if you want]
- **Discord:** [If you create a Discord server]

## 🙏 Recognition

Contributors will be added to the README.md credits section. Your work helps keep classic games alive!

## 📜 License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for helping make Destruction II HD better! Every contribution, no matter how small, is valuable.** ⭐