#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>
#include <queue>
#include <cmath>

// ============================================================================
// CORE GAME ARCHITECTURE
// ============================================================================

// Entity Component System Base
class Entity {
protected:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int x, y;
    char symbol;
    
public:
    Entity(const std::string& n, int h, int a, int d, char s)
        : name(n), health(h), maxHealth(h), attack(a), defense(d), x(0), y(0), symbol(s) {}
    
    virtual ~Entity() = default;
    
    virtual void takeDamage(int damage) {
        int actualDamage = std::max(1, damage - defense);
        health -= actualDamage;
        if (health < 0) health = 0;
    }
    
    virtual int dealDamage() const { return attack; }
    virtual void update() {}
    
    bool isAlive() const { return health > 0; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    std::string getName() const { return name; }
    char getSymbol() const { return symbol; }
    void setPosition(int px, int py) { x = px; y = py; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    
    void heal(int amount) {
        health = std::min(maxHealth, health + amount);
    }
};

// ============================================================================
// ABILITY SYSTEM
// ============================================================================

enum class AbilityType { NONE, CLEAVE, HEAL, FIRE_BLAST, SHIELD_BASH };

struct Ability {
    std::string name;
    AbilityType type;
    int cooldown;
    int currentCooldown;
    int manaCost;
    
    Ability(const std::string& n, AbilityType t, int cd, int cost)
        : name(n), type(t), cooldown(cd), currentCooldown(0), manaCost(cost) {}
    
    bool isReady() const { return currentCooldown == 0; }
    void use() { currentCooldown = cooldown; }
    void tick() { if (currentCooldown > 0) currentCooldown--; }
};

// ============================================================================
// PLAYER CLASS
// ============================================================================

class Player : public Entity {
private:
    int mana;
    int maxMana;
    int experience;
    int level;
    std::vector<Ability> abilities;
    
public:
    Player() : Entity("Hero", 100, 15, 5, '@'), mana(50), maxMana(50), experience(0), level(1) {
        abilities.emplace_back("Cleave", AbilityType::CLEAVE, 3, 15);
        abilities.emplace_back("Heal", AbilityType::HEAL, 5, 20);
        abilities.emplace_back("Fire Blast", AbilityType::FIRE_BLAST, 4, 25);
    }
    
    void gainExperience(int exp) {
        experience += exp;
        while (experience >= level * 100) {
            experience -= level * 100;
            levelUp();
        }
    }
    
    void levelUp() {
        level++;
        maxHealth += 20;
        health = maxHealth;
        attack += 3;
        defense += 2;
        maxMana += 10;
        mana = maxMana;
        std::cout << "\n*** LEVEL UP! You are now level " << level << " ***\n";
    }
    
    int getMana() const { return mana; }
    int getMaxMana() const { return maxMana; }
    int getLevel() const { return level; }
    int getExperience() const { return experience; }
    
    bool useMana(int amount) {
        if (mana >= amount) {
            mana -= amount;
            return true;
        }
        return false;
    }
    
    void restoreMana(int amount) {
        mana = std::min(maxMana, mana + amount);
    }
    
    std::vector<Ability>& getAbilities() { return abilities; }
    
    void update() override {
        for (auto& ability : abilities) {
            ability.tick();
        }
    }
};

// ============================================================================
// ENEMY TYPES (Polymorphism & Inheritance)
// ============================================================================

class Enemy : public Entity {
protected:
    int expValue;
    
public:
    Enemy(const std::string& n, int h, int a, int d, char s, int exp)
        : Entity(n, h, a, d, s), expValue(exp) {}
    
    int getExpValue() const { return expValue; }
    virtual void aiAction(Player& player, int distance) = 0;
};

class Goblin : public Enemy {
public:
    Goblin() : Enemy("Goblin", 30, 8, 2, 'g', 25) {}
    
    void aiAction(Player& player, int distance) override {
        // Goblins are aggressive and charge
        if (distance <= 1 && rand() % 100 < 70) {
            player.takeDamage(dealDamage());
            std::cout << name << " attacks for " << attack << " damage!\n";
        }
    }
};

class Orc : public Enemy {
public:
    Orc() : Enemy("Orc", 50, 12, 4, 'O', 40) {}
    
    void aiAction(Player& player, int distance) override {
        // Orcs hit harder but less frequently
        if (distance <= 1 && rand() % 100 < 60) {
            int damage = dealDamage() + 5;
            player.takeDamage(damage);
            std::cout << name << " smashes for " << damage << " damage!\n";
        }
    }
};

class Dragon : public Enemy {
private:
    int breathCooldown;
    
public:
    Dragon() : Enemy("Dragon", 120, 20, 8, 'D', 100), breathCooldown(0) {}
    
    void aiAction(Player& player, int distance) override {
        if (breathCooldown > 0) {
            breathCooldown--;
        }
        
        if (distance <= 3 && breathCooldown == 0 && rand() % 100 < 40) {
            int damage = 25;
            player.takeDamage(damage);
            std::cout << name << " breathes fire for " << damage << " damage!\n";
            breathCooldown = 3;
        } else if (distance <= 1) {
            player.takeDamage(dealDamage());
            std::cout << name << " claws you for " << attack << " damage!\n";
        }
    }
};

// ============================================================================
// DUNGEON MAP SYSTEM
// ============================================================================

class DungeonMap {
private:
    static const int WIDTH = 40;
    static const int HEIGHT = 20;
    char tiles[HEIGHT][WIDTH];
    
    struct Room {
        int x, y, width, height;
    };
    
    std::vector<Room> rooms;
    
public:
    DungeonMap() {
        generateDungeon();
    }
    
    void generateDungeon() {
        // Fill with walls
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                tiles[i][j] = '#';
            }
        }
        
        // Generate rooms using BSP (Binary Space Partitioning) concept
        int numRooms = 5 + rand() % 4;
        for (int i = 0; i < numRooms; i++) {
            int roomWidth = 5 + rand() % 8;
            int roomHeight = 4 + rand() % 6;
            int roomX = 1 + rand() % (WIDTH - roomWidth - 2);
            int roomY = 1 + rand() % (HEIGHT - roomHeight - 2);
            
            // Carve room
            for (int y = roomY; y < roomY + roomHeight; y++) {
                for (int x = roomX; x < roomX + roomWidth; x++) {
                    tiles[y][x] = '.';
                }
            }
            
            rooms.push_back({roomX, roomY, roomWidth, roomHeight});
            
            // Connect to previous room
            if (i > 0) {
                int prevCenterX = rooms[i-1].x + rooms[i-1].width / 2;
                int prevCenterY = rooms[i-1].y + rooms[i-1].height / 2;
                int currCenterX = roomX + roomWidth / 2;
                int currCenterY = roomY + roomHeight / 2;
                
                // Horizontal corridor
                for (int x = std::min(prevCenterX, currCenterX); x <= std::max(prevCenterX, currCenterX); x++) {
                    tiles[prevCenterY][x] = '.';
                }
                
                // Vertical corridor
                for (int y = std::min(prevCenterY, currCenterY); y <= std::max(prevCenterY, currCenterY); y++) {
                    tiles[y][currCenterX] = '.';
                }
            }
        }
    }
    
    void display(const Player& player, const std::vector<std::unique_ptr<Enemy>>& enemies) const {
        std::cout << "\n";
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (player.getX() == j && player.getY() == i) {
                    std::cout << player.getSymbol();
                } else {
                    bool enemyHere = false;
                    for (const auto& enemy : enemies) {
                        if (enemy->isAlive() && enemy->getX() == j && enemy->getY() == i) {
                            std::cout << enemy->getSymbol();
                            enemyHere = true;
                            break;
                        }
                    }
                    if (!enemyHere) {
                        std::cout << tiles[i][j];
                    }
                }
            }
            std::cout << "\n";
        }
    }
    
    bool isWalkable(int x, int y) const {
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return false;
        return tiles[y][x] == '.';
    }
    
    std::pair<int, int> getRandomFloor() const {
        while (true) {
            int x = rand() % WIDTH;
            int y = rand() % HEIGHT;
            if (tiles[y][x] == '.') {
                return {x, y};
            }
        }
    }
    
    static int getWidth() { return WIDTH; }
    static int getHeight() { return HEIGHT; }
};

// ============================================================================
// GAME STATE MANAGER
// ============================================================================

class Game {
private:
    Player player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    DungeonMap dungeon;
    int floor;
    bool running;
    
    void spawnEnemies() {
        int numEnemies = 3 + floor;
        for (int i = 0; i < numEnemies; i++) {
            auto pos = dungeon.getRandomFloor();
            
            std::unique_ptr<Enemy> enemy;
            int roll = rand() % 100;
            
            if (floor >= 3 && roll < 15) {
                enemy = std::make_unique<Dragon>();
            } else if (roll < 50) {
                enemy = std::make_unique<Orc>();
            } else {
                enemy = std::make_unique<Goblin>();
            }
            
            enemy->setPosition(pos.first, pos.second);
            enemies.push_back(std::move(enemy));
        }
    }
    
    void displayHUD() {
        std::cout << "\n=== FLOOR " << floor << " ===\n";
        std::cout << "Level " << player.getLevel() << " | ";
        std::cout << "HP: " << player.getHealth() << "/" << player.getMaxHealth() << " | ";
        std::cout << "MP: " << player.getMana() << "/" << player.getMaxMana() << " | ";
        std::cout << "XP: " << player.getExperience() << "/" << (player.getLevel() * 100) << "\n";
        std::cout << "ATK: " << player.getAttack() << " | DEF: " << player.getDefense() << "\n";
    }
    
    void handleCombat() {
        for (auto& enemy : enemies) {
            if (!enemy->isAlive()) continue;
            
            int dx = player.getX() - enemy->getX();
            int dy = player.getY() - enemy->getY();
            int distance = std::abs(dx) + std::abs(dy);
            
            // Enemy AI
            enemy->aiAction(player, distance);
            
            // Move towards player using simple pathfinding
            if (distance > 1 && rand() % 100 < 70) {
                int newX = enemy->getX();
                int newY = enemy->getY();
                
                if (std::abs(dx) > std::abs(dy)) {
                    newX += (dx > 0) ? 1 : -1;
                } else {
                    newY += (dy > 0) ? 1 : -1;
                }
                
                if (dungeon.isWalkable(newX, newY)) {
                    bool blocked = false;
                    for (const auto& other : enemies) {
                        if (other->isAlive() && other->getX() == newX && other->getY() == newY) {
                            blocked = true;
                            break;
                        }
                    }
                    if (!blocked && !(newX == player.getX() && newY == player.getY())) {
                        enemy->setPosition(newX, newY);
                    }
                }
            }
        }
    }
    
    void playerAttack() {
        for (auto& enemy : enemies) {
            if (!enemy->isAlive()) continue;
            
            int dx = std::abs(player.getX() - enemy->getX());
            int dy = std::abs(player.getY() - enemy->getY());
            
            if (dx <= 1 && dy <= 1) {
                enemy->takeDamage(player.dealDamage());
                std::cout << "You attack " << enemy->getName() << " for " << player.getAttack() << " damage!\n";
                
                if (!enemy->isAlive()) {
                    std::cout << enemy->getName() << " has been defeated!\n";
                    player.gainExperience(enemy->getExpValue());
                }
                return;
            }
        }
        std::cout << "No enemy in range!\n";
    }
    
    void useAbility(int index) {
        if (index < 0 || index >= player.getAbilities().size()) {
            std::cout << "Invalid ability!\n";
            return;
        }
        
        Ability& ability = player.getAbilities()[index];
        
        if (!ability.isReady()) {
            std::cout << ability.name << " is on cooldown (" << ability.currentCooldown << " turns)\n";
            return;
        }
        
        if (!player.useMana(ability.manaCost)) {
            std::cout << "Not enough mana!\n";
            return;
        }
        
        ability.use();
        
        switch (ability.type) {
            case AbilityType::CLEAVE: {
                int count = 0;
                for (auto& enemy : enemies) {
                    if (!enemy->isAlive()) continue;
                    int dx = std::abs(player.getX() - enemy->getX());
                    int dy = std::abs(player.getY() - enemy->getY());
                    if (dx <= 2 && dy <= 2) {
                        enemy->takeDamage(player.getAttack() * 2);
                        count++;
                        if (!enemy->isAlive()) {
                            player.gainExperience(enemy->getExpValue());
                        }
                    }
                }
                std::cout << "You cleave through " << count << " enemies!\n";
                break;
            }
            case AbilityType::HEAL:
                player.heal(40);
                std::cout << "You heal for 40 HP!\n";
                break;
            case AbilityType::FIRE_BLAST: {
                for (auto& enemy : enemies) {
                    if (!enemy->isAlive()) continue;
                    int dx = std::abs(player.getX() - enemy->getX());
                    int dy = std::abs(player.getY() - enemy->getY());
                    if (dx <= 3 && dy <= 3) {
                        enemy->takeDamage(30);
                        if (!enemy->isAlive()) {
                            player.gainExperience(enemy->getExpValue());
                        }
                    }
                }
                std::cout << "Fire engulfs the area!\n";
                break;
            }
            default:
                break;
        }
    }
    
public:
    Game() : floor(1), running(true) {
        srand(static_cast<unsigned>(time(nullptr)));
        auto pos = dungeon.getRandomFloor();
        player.setPosition(pos.first, pos.second);
        spawnEnemies();
    }
    
    void run() {
        std::cout << "=== TACTICAL DUNGEON CRAWLER ===\n";
        std::cout << "Survive the dungeon and defeat all enemies!\n";
        
        while (running) {
            displayHUD();
            dungeon.display(player, enemies);
            
            // Check win condition
            bool allDead = true;
            for (const auto& enemy : enemies) {
                if (enemy->isAlive()) {
                    allDead = false;
                    break;
                }
            }
            
            if (allDead) {
                std::cout << "\n*** Floor " << floor << " cleared! ***\n";
                floor++;
                enemies.clear();
                dungeon = DungeonMap();
                auto pos = dungeon.getRandomFloor();
                player.setPosition(pos.first, pos.second);
                spawnEnemies();
                player.heal(30);
                player.restoreMana(20);
                continue;
            }
            
            std::cout << "\nActions: (w/a/s/d) move | (f) attack | (1-3) abilities | (q) quit\n> ";
            
            char input;
            std::cin >> input;
            
            bool playerMoved = false;
            int newX = player.getX();
            int newY = player.getY();
            
            switch (input) {
                case 'w': newY--; playerMoved = true; break;
                case 's': newY++; playerMoved = true; break;
                case 'a': newX--; playerMoved = true; break;
                case 'd': newX++; playerMoved = true; break;
                case 'f': playerAttack(); break;
                case '1': useAbility(0); break;
                case '2': useAbility(1); break;
                case '3': useAbility(2); break;
                case 'q': running = false; continue;
                default: std::cout << "Invalid input!\n"; continue;
            }
            
            if (playerMoved) {
                if (dungeon.isWalkable(newX, newY)) {
                    bool blocked = false;
                    for (const auto& enemy : enemies) {
                        if (enemy->isAlive() && enemy->getX() == newX && enemy->getY() == newY) {
                            blocked = true;
                            break;
                        }
                    }
                    if (!blocked) {
                        player.setPosition(newX, newY);
                    } else {
                        std::cout << "An enemy blocks your path!\n";
                    }
                } else {
                    std::cout << "You can't walk through walls!\n";
                }
            }
            
            player.update();
            handleCombat();
            player.restoreMana(2);
            
            if (!player.isAlive()) {
                std::cout << "\n*** GAME OVER ***\n";
                std::cout << "You reached floor " << floor << " at level " << player.getLevel() << "\n";
                running = false;
            }
        }
    }
};

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================

int main() {
    Game game;
    game.run();
    return 0;
}
