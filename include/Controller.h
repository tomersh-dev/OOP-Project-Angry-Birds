#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <queue>
#include <memory>
#include <string>
#include "AssetManager.h"
#include "ContactListener.h"
#include "Block.h"
#include "Target.h"
#include "Projectile.h"
#include "Camera.h"
#include "Slingshot.h"
#include "Board.h"
#include "Button.h"
#include "FloatingText.h"

enum class GameState { MainMenu, Instructions, Playing, LevelTransition, GameOver };

class Controller {
public:
    Controller();
    ~Controller();

    void Run();

private:
    void InitAssets();
    void InitGround();
    void InitButtons();
    void InitLevel(const std::string& levelFile);
    void StartNewGame();

    void ProcessEvents();
    void Update(float dt);
    void Render();
    void RenderHUD();
    void RenderMainMenu();
    void RenderInstructions();

    void CleanUpDeadObjects();
    void LoadNextProjectile();
    void CheckRestState(float dt);
    void CheckOutOfBounds();
    void TransitionToNextTurn();
    void TransitionToNextLevel();

    sf::Vector2f GetMouseWorldPosition() const;

    sf::RenderWindow    window;
    sf::RectangleShape  groundShape;
    b2World             world;
    ContactListener     contactListener;
    Camera              camera;
    Board               board;

    std::unique_ptr<Slingshot> slingshot;

    std::vector<Block*>   blocks;
    std::vector<Target*>  targets;
    Projectile*           activeProjectile = nullptr;

    std::queue<std::string>     magazineQueue;
    std::vector<FloatingText>   floatingTexts;

    sf::Font hudFont;
    bool     hudFontLoaded = false;
    int      score         = 0;
    int      currentLevel  = 1;
    std::string currentLevelFile;

    float restTimer           = 0.0f;
    float rollTimer           = 0.0f;
    float projectileLifeTimer = 0.0f;
    bool  projectileFired     = false;
    bool  isWin               = false;

    GameState gameState = GameState::MainMenu;

    // Main menu & instructions
    std::unique_ptr<Button> btnStart;
    std::unique_ptr<Button> btnInstructions;
    std::unique_ptr<Button> btnExit;
    std::unique_ptr<Button> btnBack;

    // Game over screen
    std::unique_ptr<Button> btnNextLevel;
    std::unique_ptr<Button> btnRestart;
    std::unique_ptr<Button> btnMainMenu;
};
