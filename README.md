# 🗡️ Tactical Dungeon Crawler

A procedurally-generated roguelike dungeon crawler built in C++ showcasing game development fundamentals and advanced programming techniques.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## 🎮 Overview

A turn-based tactical game featuring procedural dungeon generation, polymorphic enemy AI, and a robust ability system. Built to demonstrate production-quality game architecture suitable for AAA game development.

## ✨ Features

### Core Systems
- **Entity Component System (ECS)** architecture for scalable game objects
- **Procedural Generation** using BSP-inspired algorithms
- **Polymorphic AI** with unique behavior patterns per enemy type
- **Ability System** with cooldowns and resource management
- **Progressive Difficulty** scaling across procedurally generated floors

### Gameplay
- 🎯 Tactical turn-based combat
- 🐉 Multiple enemy types (Goblins, Orcs, Dragons)
- ⚔️ Player abilities: Cleave, Heal, Fire Blast
- 📈 Experience and leveling system
- 🗺️ Randomly generated dungeons with rooms and corridors

## 🛠️ Technical Highlights

### Object-Oriented Design
- **Inheritance & Polymorphism**: Enemy class hierarchy with virtual functions
- **Encapsulation**: Clean separation of game systems
- **RAII & Smart Pointers**: Proper memory management using `std::unique_ptr`

### Algorithms & Data Structures
- Procedural dungeon generation
- Simple pathfinding for enemy AI
- State management patterns
- STL containers (vectors, maps, algorithms)

### C++ Best Practices
- C++17 standard features
- Const correctness
- Move semantics
- Modern memory management

## 🚀 Building & Running

### Option 1: Using CMake (Recommended)
```bash
mkdir build
cd build
cmake ..
make
./dungeon_crawler
```

### Option 2: Using Make
```bash
make
./dungeon_crawler
```

### Option 3: Direct Compilation
```bash
g++ -std=c++17 -O2 -o dungeon_crawler main.cpp
./dungeon_crawler
```

## 🎯 Controls

- **W/A/S/D**: Move character
- **F**: Attack nearby enemy
- **1/2/3**: Use abilities
- **Q**: Quit game

## 🏗️ Architecture
```
Game
├── Player (Entity)
│   ├── Ability System
│   └── Progression System
├── Enemies (Entity)
│   ├── Goblin (aggressive melee)
│   ├── Orc (heavy hitter)
│   └── Dragon (ranged breath attack)
└── DungeonMap
    └── Procedural Generation
```

## 📋 System Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10+ (optional)
- Standard C++ library

## 🎓 Learning Outcomes

This project demonstrates:
- Game loop architecture
- Entity management systems
- AI behavior programming
- Procedural content generation
- Resource management in C++
- Object-oriented game design patterns

## 🔮 Future Enhancements

- [ ] Save/Load system
- [ ] Item and inventory system
- [ ] More complex AI (A* pathfinding)
- [ ] Multiple character classes
- [ ] Sound effects and music
- [ ] Network multiplayer

## 📝 License

MIT License - feel free to use for learning and portfolio purposes

## 👤 Author

[Your Name]
- GitHub: [@yourusername](https://github.com/yourusername)
- LinkedIn: [Your Profile](https://linkedin.com/in/yourprofile)

---

*Built as a portfolio project to demonstrate C++ game development skills for AAA game studio applications.*
```

4. Commit with message: `Update README with comprehensive documentation`

### Step 7: Add Topics (Keywords for Discoverability)

1. On your repo's main page, look for **"About"** section (top right)
2. Click the **gear icon** ⚙️
3. In the **"Topics"** field, add these one by one:
   - `cpp`
   - `game-development`
   - `roguelike`
   - `procedural-generation`
   - `game-engine`
   - `portfolio-project`
   - `dungeon-crawler`
   - `oop`
4. Click **"Save changes"**

### Step 8: Pin This Repository

1. Go to **your GitHub profile** (click your avatar → Your profile)
2. Scroll to repositories section
3. Click **"Customize your pins"**
4. Check the box next to **tactical-dungeon-crawler**
5. Click **"Save pins"**

### Step 9: Create a Release

1. In your repo, click **"Releases"** (right sidebar)
2. Click **"Create a new release"**
3. Fill in:
   - **Tag version**: `v1.0.0`
   - **Release title**: `Initial Release - Tactical Dungeon Crawler v1.0`
   - **Description**: 
```
   First stable release featuring:
   - Complete turn-based combat system
   - Procedural dungeon generation
   - Three enemy types with unique AI
   - Player progression and abilities
   - Multi-floor gameplay
