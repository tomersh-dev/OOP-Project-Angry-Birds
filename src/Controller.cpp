#include "Controller.h"
#include "ObjectFactory.h"
#include "Constants.h"
#include <optional>
#include <string>
#include <fstream>
#include <cmath>
#include <algorithm>

Controller::Controller()
    : window(sf::VideoMode({ Window_Settings::WIDTH, Window_Settings::HEIGHT }), "OOP Game Project"),
    world(b2Vec2(0.0f, Physics::GRAVITY)),
    camera(Window_Settings::WIDTH, Window_Settings::HEIGHT) {

    window.setFramerateLimit(Window_Settings::FPS);
    world.SetContactListener(&contactListener);

    InitAssets();
    InitGround();
    InitButtons();
}

Controller::~Controller() {
    for (auto block : blocks) {
        block->DestroyPhysics(&world);
        delete block;
    }
    for (auto target : targets) {
        target->DestroyPhysics(&world);
        delete target;
    }
    if (activeProjectile) {
        activeProjectile->DestroyPhysics(&world);
        delete activeProjectile;
    }
}

void Controller::InitAssets() {
    AssetManager& assets = AssetManager::GetInstance();
    assets.LoadTexture("wood",      "assets/wood.png");
    assets.LoadTexture("pig",       "assets/pig.png");
    assets.LoadTexture("red_bird",  "assets/red_bird.png");
    assets.LoadTexture("slingshot", "assets/slingshot.png");

    auto tryLoad = [&](const std::string& name, const std::string& file, const std::string& fallback) {
        try { assets.LoadTexture(name, file); }
        catch (...) { assets.LoadTexture(name, fallback); }
    };

    tryLoad("ice",        "assets/ice.png",        "assets/wood.png");
    tryLoad("stone",      "assets/stone.png",      "assets/wood.png");
    tryLoad("boost_bird", "assets/boost_bird.png", "assets/red_bird.png");

    hudFontLoaded = hudFont.openFromFile("assets/font.ttf");
}

void Controller::InitButtons() {
    const float bx     = UI::MENU_CENTER_X - UI::BUTTON_WIDTH / 2.0f;
    const sf::Vector2f bSize { UI::BUTTON_WIDTH, UI::BUTTON_HEIGHT };

    // Main menu
    btnStart = std::make_unique<Button>(
        sf::Vector2f(bx, UI::MENU_FIRST_BTN_Y),
        bSize, "Start", hudFont, UI::BUTTON_FONT_SIZE);

    btnInstructions = std::make_unique<Button>(
        sf::Vector2f(bx, UI::MENU_FIRST_BTN_Y + UI::BUTTON_SPACING),
        bSize, "Instructions", hudFont, UI::BUTTON_FONT_SIZE);

    btnExit = std::make_unique<Button>(
        sf::Vector2f(bx, UI::MENU_FIRST_BTN_Y + 2.0f * UI::BUTTON_SPACING),
        bSize, "Exit", hudFont, UI::BUTTON_FONT_SIZE);

    btnBack = std::make_unique<Button>(
        sf::Vector2f(bx, UI::INSTR_BACK_BTN_Y),
        bSize, "Back", hudFont, UI::BUTTON_FONT_SIZE);

    // Game over screen
    btnNextLevel = std::make_unique<Button>(
        sf::Vector2f(bx, UI::GAMEOVER_FIRST_BTN_Y),
        bSize, "Next Level", hudFont, UI::BUTTON_FONT_SIZE);

    btnRestart = std::make_unique<Button>(
        sf::Vector2f(bx, UI::GAMEOVER_FIRST_BTN_Y),
        bSize, "Restart", hudFont, UI::BUTTON_FONT_SIZE);

    btnMainMenu = std::make_unique<Button>(
        sf::Vector2f(bx, UI::GAMEOVER_FIRST_BTN_Y + UI::BUTTON_SPACING),
        bSize, "Main Menu", hudFont, UI::BUTTON_FONT_SIZE);
}

void Controller::StartNewGame() {
    score = 0;
    currentLevel = 1;
    InitLevel("levels/level" + std::to_string(currentLevel) + ".txt");
}

void Controller::InitGround() {
    // ── Ground ──────────────────────────────────────────────────────────────
    b2BodyDef groundDef;
    groundDef.position.Set((Window_Settings::WIDTH / 2.0f) / Physics::PPM,
                           (Window_Settings::HEIGHT - 20.0f) / Physics::PPM);
    b2Body* groundBody = world.CreateBody(&groundDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox((Window_Settings::WIDTH * 5.0f) / Physics::PPM, 20.0f / Physics::PPM);

    b2FixtureDef groundFixDef;
    groundFixDef.shape              = &groundBox;
    groundFixDef.density            = 0.0f;
    groundFixDef.filter.categoryBits = CollisionFilter::GROUND;
    groundFixDef.filter.maskBits     = CollisionFilter::ALL;
    groundBody->CreateFixture(&groundFixDef);

    groundShape.setSize({ Window_Settings::WIDTH * 10.0f, Window_Settings::GROUND_VISUAL_DEPTH });
    groundShape.setOrigin({ (Window_Settings::WIDTH * 10.0f) / 2.0f, 20.0f });
    groundShape.setPosition({ Window_Settings::WIDTH / 2.0f, Window_Settings::HEIGHT - 20.0f });
    groundShape.setFillColor(sf::Color(34, 139, 34));

    // ── Invisible boundary walls (physics only, no SFML shape) ──────────────
    // Projectiles pass through; blocks and targets are contained.
    b2PolygonShape wallBox;
    wallBox.SetAsBox(Level_Bounds::WALL_HALF_W / Physics::PPM,
                     Level_Bounds::WALL_HALF_H / Physics::PPM);

    b2FixtureDef wallFixDef;
    wallFixDef.shape              = &wallBox;
    wallFixDef.density            = 0.0f;
    wallFixDef.filter.categoryBits = CollisionFilter::WALL;
    wallFixDef.filter.maskBits     = CollisionFilter::BLOCK | CollisionFilter::TARGET;

    const float wallCenterY = (Window_Settings::HEIGHT / 2.0f) / Physics::PPM;

    b2BodyDef leftWallDef;
    leftWallDef.position.Set(Level_Bounds::LEFT_WALL_X / Physics::PPM, wallCenterY);
    world.CreateBody(&leftWallDef)->CreateFixture(&wallFixDef);

    b2BodyDef rightWallDef;
    rightWallDef.position.Set(Level_Bounds::RIGHT_WALL_X / Physics::PPM, wallCenterY);
    world.CreateBody(&rightWallDef)->CreateFixture(&wallFixDef);
}

void Controller::InitLevel(const std::string& levelFile) {
    currentLevelFile = levelFile;
    floatingTexts.clear();
    isWin = false;

    for (auto block : blocks) { block->DestroyPhysics(&world); delete block; }
    blocks.clear();
    for (auto target : targets) { target->DestroyPhysics(&world); delete target; }
    targets.clear();
    if (activeProjectile) { activeProjectile->DestroyPhysics(&world); delete activeProjectile; activeProjectile = nullptr; }
    while (!magazineQueue.empty()) magazineQueue.pop();

    projectileFired = false;
    restTimer = 0.0f;
    gameState = GameState::Playing;
    contactListener.SetDamageEnabled(false);  // blocks settle without taking damage

    const float slingshotY = Window_Settings::HEIGHT - Slingshot_Settings::SPAWN_Y_OFFSET;
    slingshot = std::make_unique<Slingshot>(
        Slingshot_Settings::SPAWN_X, slingshotY, 50.0f, 150.0f,
        AssetManager::GetInstance().GetTexture("slingshot"));

    // Default bounds used when the level file has no BOUNDS line
    sf::FloatRect levelBounds({ 0.0f, -200.0f },
                              { 2000.0f, static_cast<float>(Window_Settings::HEIGHT) + 400.0f });
    board.LoadLevel(levelFile, world, blocks, targets, magazineQueue, levelBounds);
    camera.SetBounds(levelBounds);

    LoadNextProjectile();
}

void Controller::LoadNextProjectile() {
    if (magazineQueue.empty()) return;

    const std::string typeStr = magazineQueue.front();
    magazineQueue.pop();

    const float slingshotY = Window_Settings::HEIGHT - Slingshot_Settings::SPAWN_Y_OFFSET;
    activeProjectile = ObjectFactory::CreateProjectile(
        typeStr,
        Slingshot_Settings::SPAWN_X, slingshotY,
        Projectile_Settings::RADIUS,
        world);
    slingshot->AttachProjectile(activeProjectile);

    projectileFired       = false;
    restTimer             = 0.0f;
    rollTimer             = 0.0f;
    projectileLifeTimer   = 0.0f;
    camera.ClearTarget();
}

sf::Vector2f Controller::GetMouseWorldPosition() const {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    return window.mapPixelToCoords(pixelPos, camera.GetView());
}

void Controller::Run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        ProcessEvents();
        Update(dt);
        CheckRestState(dt);
        CheckOutOfBounds();
        CleanUpDeadObjects();
        Render();
    }
}

void Controller::ProcessEvents() {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f uiPos    = window.mapPixelToCoords(pixelPos, window.getDefaultView());
    sf::Vector2f worldPos = GetMouseWorldPosition();

    bool leftJustPressed = false;

    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mb->button == sf::Mouse::Button::Left) {
                leftJustPressed = true;
                if (gameState == GameState::Playing && projectileFired &&
                    activeProjectile && !activeProjectile->IsDead()) {
                    activeProjectile->ActivateAbility();
                }
            }
            if (mb->button == sf::Mouse::Button::Right && gameState == GameState::Playing) {
                camera.StartPan(worldPos);
            }
        }

        if (const auto* mb = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mb->button == sf::Mouse::Button::Right) {
                camera.StopPan();
            }
        }

        if (const auto* mw = event->getIf<sf::Event::MouseWheelScrolled>()) {
            if (mw->wheel == sf::Mouse::Wheel::Vertical && gameState == GameState::Playing) {
                camera.Zoom(mw->delta);
            }
        }
    }

    switch (gameState) {
        case GameState::MainMenu:
            if (btnStart)        btnStart->Update(uiPos);
            if (btnInstructions) btnInstructions->Update(uiPos);
            if (btnExit)         btnExit->Update(uiPos);
            if (btnStart        && btnStart->IsClicked(uiPos, leftJustPressed))        StartNewGame();
            if (btnInstructions && btnInstructions->IsClicked(uiPos, leftJustPressed)) gameState = GameState::Instructions;
            if (btnExit         && btnExit->IsClicked(uiPos, leftJustPressed))         window.close();
            break;

        case GameState::Instructions:
            if (btnBack) btnBack->Update(uiPos);
            if (btnBack && btnBack->IsClicked(uiPos, leftJustPressed)) gameState = GameState::MainMenu;
            break;

        case GameState::Playing: {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                camera.UpdatePan(worldPos);
            }
            if (slingshot) {
                bool isLeftHeld = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
                slingshot->Update(worldPos, isLeftHeld);

                if (!projectileFired && slingshot->GetFiredProjectile()) {
                    projectileFired = true;
                    contactListener.SetDamageEnabled(true);
                }
            }
            break;
        }

        case GameState::GameOver: {
            if (isWin) {
                if (btnNextLevel) {
                    btnNextLevel->Update(uiPos);
                    if (btnNextLevel->IsClicked(uiPos, leftJustPressed))
                        TransitionToNextLevel();
                }
            }
            else {
                if (btnRestart) {
                    btnRestart->Update(uiPos);
                    if (btnRestart->IsClicked(uiPos, leftJustPressed))
                        InitLevel(currentLevelFile);
                }
            }
            if (btnMainMenu) {
                btnMainMenu->Update(uiPos);
                if (btnMainMenu->IsClicked(uiPos, leftJustPressed))
                    gameState = GameState::MainMenu;
            }
            break;
        }

        default: break;
    }
}

void Controller::Update(float dt) {
    // Floating texts fade in all game states (they may linger into GameOver)
    for (auto& ft : floatingTexts) ft.Update(dt);
    floatingTexts.erase(
        std::remove_if(floatingTexts.begin(), floatingTexts.end(),
                       [](const FloatingText& ft) { return ft.IsExpired(); }),
        floatingTexts.end());

    if (gameState != GameState::Playing) return;

    world.Step(1.0f / Window_Settings::FPS, Physics::VELOCITY_ITERATIONS, Physics::POSITION_ITERATIONS);

    for (auto block : blocks) block->Update();
    for (auto target : targets) target->Update();
    if (activeProjectile) activeProjectile->Update();

    if (projectileFired && activeProjectile && !activeProjectile->IsDead()) {
        camera.SetTarget(activeProjectile);
    }

    camera.Update();
}

void Controller::CheckOutOfBounds() {
    if (!activeProjectile || !projectileFired) return;

    sf::Vector2f pos = activeProjectile->GetPosition();
    if (pos.x > Gameplay::OOB_RIGHT ||
        pos.x < Gameplay::OOB_LEFT  ||
        pos.y > Gameplay::OOB_BOTTOM) {
        activeProjectile->MarkForDestruction();
    }
}

void Controller::CheckRestState(float dt) {
    if (gameState != GameState::Playing || !projectileFired) return;

    // Anti-rolling timeout: kill the projectile if it crawls or lives too long
    if (activeProjectile && !activeProjectile->IsDead()) {
        projectileLifeTimer += dt;

        sf::Vector2f vel = activeProjectile->GetLinearVelocity();
        float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);

        if (speed < Gameplay::ROLL_VELOCITY_THRESHOLD)
            rollTimer += dt;
        else
            rollTimer = 0.0f;

        if (rollTimer             >= Gameplay::ROLL_TIMEOUT ||
            projectileLifeTimer   >= Gameplay::PROJECTILE_MAX_LIFETIME) {
            activeProjectile->MarkForDestruction();
        }
    }

    // Turn-transition timer: count up once the world has settled
    if (!activeProjectile || activeProjectile->IsDead()) {
        restTimer += dt;
    }
    else {
        bool worldSettled = true;
        for (auto block : blocks) {
            sf::Vector2f bvel = block->GetLinearVelocity();
            if (std::abs(bvel.x) > Physics::REST_VELOCITY_THRESHOLD ||
                std::abs(bvel.y) > Physics::REST_VELOCITY_THRESHOLD) {
                worldSettled = false;
                break;
            }
        }

        sf::Vector2f projVel = activeProjectile->GetLinearVelocity();
        bool projSettled = std::abs(projVel.x) <= Physics::REST_VELOCITY_THRESHOLD &&
                           std::abs(projVel.y) <= Physics::REST_VELOCITY_THRESHOLD;

        if (worldSettled && projSettled)
            restTimer += dt;
        else
            restTimer = 0.0f;
    }

    if (restTimer >= Physics::REST_DURATION) {
        TransitionToNextTurn();
    }
}

void Controller::TransitionToNextTurn() {
    restTimer = 0.0f;

    if (targets.empty()) {
        isWin = true;
        score += static_cast<int>(magazineQueue.size()) * Scores::REMAINING_PROJECTILE_BONUS;
        // Safety: clean up projectile if still alive when last target was destroyed
        if (activeProjectile) {
            slingshot->ClearFiredProjectile();
            camera.ClearTarget();
            activeProjectile->DestroyPhysics(&world);
            delete activeProjectile;
            activeProjectile = nullptr;
        }
        camera.ResetView();
        gameState = GameState::GameOver;
        return;
    }

    if (magazineQueue.empty()) {
        isWin = false;
        camera.ResetView();
        gameState = GameState::GameOver;
        return;
    }

    if (activeProjectile) {
        slingshot->ClearFiredProjectile();
        camera.ClearTarget();
        camera.ResetView();
        activeProjectile->DestroyPhysics(&world);
        delete activeProjectile;
        activeProjectile = nullptr;
    }

    LoadNextProjectile();
}

void Controller::TransitionToNextLevel() {
    currentLevel++;
    std::string nextFile = "levels/level" + std::to_string(currentLevel) + ".txt";
    std::ifstream test(nextFile);
    if (test.good()) {
        test.close();
        InitLevel(nextFile);
    }
    else {
        // All levels complete — return to main menu
        gameState = GameState::MainMenu;
    }
}

void Controller::CleanUpDeadObjects() {
    if (gameState != GameState::Playing) return;

    for (auto it = blocks.begin(); it != blocks.end(); ) {
        if ((*it)->IsDead()) {
            int val = (*it)->GetScoreValue();
            score += val;
            if (hudFontLoaded) {
                floatingTexts.emplace_back(
                    hudFont, "+" + std::to_string(val),
                    (*it)->GetPosition(),
                    sf::Vector2f(0.0f, FloatingText_Settings::VELOCITY_Y),
                    FloatingText_Settings::LIFESPAN);
            }
            (*it)->DestroyPhysics(&world);
            delete *it;
            it = blocks.erase(it);
        }
        else { ++it; }
    }

    for (auto it = targets.begin(); it != targets.end(); ) {
        if ((*it)->IsDead()) {
            score += Target_Settings::SCORE;
            if (hudFontLoaded) {
                floatingTexts.emplace_back(
                    hudFont, "+" + std::to_string(Target_Settings::SCORE),
                    (*it)->GetPosition(),
                    sf::Vector2f(0.0f, FloatingText_Settings::VELOCITY_Y),
                    FloatingText_Settings::LIFESPAN);
            }
            (*it)->DestroyPhysics(&world);
            delete *it;
            it = targets.erase(it);
        }
        else { ++it; }
    }

    if (activeProjectile && activeProjectile->IsDead()) {
        slingshot->ClearFiredProjectile();
        camera.ClearTarget();
        camera.ResetView();
        activeProjectile->DestroyPhysics(&world);
        delete activeProjectile;
        activeProjectile = nullptr;
    }
}

void Controller::Render() {
    window.clear(sf::Color(Window_Settings::BG_R, Window_Settings::BG_G, Window_Settings::BG_B));

    switch (gameState) {
        case GameState::MainMenu:
            RenderMainMenu();
            break;

        case GameState::Instructions:
            RenderInstructions();
            break;

        default:
            window.setView(camera.GetView());
            window.draw(groundShape);
            for (auto block   : blocks)   block->Render(window);
            for (auto target  : targets)  target->Render(window);
            if (activeProjectile) activeProjectile->Render(window);
            if (slingshot)        slingshot->Render(window);
            for (auto& ft : floatingTexts) ft.Render(window);
            window.setView(window.getDefaultView());
            RenderHUD();
            break;
    }

    window.display();
}

void Controller::RenderHUD() {
    if (gameState == GameState::GameOver) {
        // Dim overlay over the world
        sf::RectangleShape overlay(
            sf::Vector2f(Window_Settings::WIDTH, Window_Settings::HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, UI::OVERLAY_ALPHA));
        window.draw(overlay);

        if (hudFontLoaded) {
            // Win/Lose title
            const std::string titleStr = isWin ? "Level Complete!" : "Game Over!";
            const sf::Color   titleCol = isWin ? sf::Color(255, 220,   0)
                                               : sf::Color(255,  80,  80);

            sf::Text title(hudFont, titleStr, UI::TITLE_FONT_SIZE);
            title.setFillColor(titleCol);
            title.setOutlineColor(sf::Color::Black);
            title.setOutlineThickness(3.0f);
            sf::FloatRect tb = title.getLocalBounds();
            title.setOrigin({ tb.position.x + tb.size.x / 2.0f,
                              tb.position.y + tb.size.y / 2.0f });
            title.setPosition({ UI::MENU_CENTER_X, UI::GAMEOVER_TITLE_Y });
            window.draw(title);

            // Final score
            sf::Text scoreDisp(hudFont, "Score: " + std::to_string(score), UI::BUTTON_FONT_SIZE);
            scoreDisp.setFillColor(sf::Color::White);
            scoreDisp.setOutlineColor(sf::Color::Black);
            scoreDisp.setOutlineThickness(2.0f);
            sf::FloatRect sb = scoreDisp.getLocalBounds();
            scoreDisp.setOrigin({ sb.position.x + sb.size.x / 2.0f,
                                  sb.position.y + sb.size.y / 2.0f });
            scoreDisp.setPosition({ UI::MENU_CENTER_X, UI::GAMEOVER_SCORE_Y });
            window.draw(scoreDisp);
        }

        if (isWin) { if (btnNextLevel) btnNextLevel->Render(window); }
        else        { if (btnRestart)   btnRestart->Render(window); }
        if (btnMainMenu) btnMainMenu->Render(window);
        return;
    }

    // ── Playing HUD ──────────────────────────────────────────────────────────
    if (hudFontLoaded) {
        sf::Text scoreText(hudFont, "Score: " + std::to_string(score), HUD::SCORE_FONT_SIZE);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setOutlineColor(sf::Color::Black);
        scoreText.setOutlineThickness(2.0f);
        scoreText.setPosition({ HUD::SCORE_X, HUD::SCORE_Y });
        window.draw(scoreText);
    }

    // Magazine icons — sprite-only, no font required
    AssetManager& assets = AssetManager::GetInstance();
    std::queue<std::string> tempQueue = magazineQueue;
    float drawX = HUD::MAGAZINE_X;

    while (!tempQueue.empty()) {
        const std::string typeStr = tempQueue.front();
        tempQueue.pop();

        const std::string texName = (typeStr == "BOOST") ? "boost_bird" : "red_bird";
        sf::Sprite hudSprite(assets.GetTexture(texName));
        sf::Vector2u texSize = assets.GetTexture(texName).getSize();
        float scale = HUD::MAGAZINE_SPRITE_SIZE / static_cast<float>(texSize.x);
        hudSprite.setScale({ scale, scale });
        hudSprite.setPosition({ drawX, HUD::MAGAZINE_Y });
        window.draw(hudSprite);

        drawX += HUD::MAGAZINE_SPACING;
    }
}

void Controller::RenderMainMenu() {
    if (hudFontLoaded) {
        sf::Text title(hudFont, "OOP Game", UI::TITLE_FONT_SIZE);
        title.setFillColor(sf::Color::White);
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(3.0f);
        sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin({ tb.position.x + tb.size.x / 2.0f,
                          tb.position.y + tb.size.y / 2.0f });
        title.setPosition({ UI::MENU_CENTER_X, UI::MENU_TITLE_Y });
        window.draw(title);

        sf::Text sub(hudFont, "A Physics-Based Puzzle Game", 22u);
        sub.setFillColor(sf::Color(200, 220, 255));
        sf::FloatRect sb = sub.getLocalBounds();
        sub.setOrigin({ sb.position.x + sb.size.x / 2.0f,
                        sb.position.y + sb.size.y / 2.0f });
        sub.setPosition({ UI::MENU_CENTER_X, UI::MENU_TITLE_Y + 75.0f });
        window.draw(sub);
    }

    if (btnStart)        btnStart->Render(window);
    if (btnInstructions) btnInstructions->Render(window);
    if (btnExit)         btnExit->Render(window);
}

void Controller::RenderInstructions() {
    if (hudFontLoaded) {
        sf::Text title(hudFont, "Instructions", static_cast<unsigned int>(UI::TITLE_FONT_SIZE * 0.75f));
        title.setFillColor(sf::Color::White);
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(2.0f);
        sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin({ tb.position.x + tb.size.x / 2.0f,
                          tb.position.y + tb.size.y / 2.0f });
        title.setPosition({ UI::MENU_CENTER_X, UI::INSTR_TITLE_Y });
        window.draw(title);

        const std::string instr =
            "AIM & FIRE\n"
            "  Click and drag the projectile on the slingshot,\n"
            "  then release to launch.\n\n"
            "ABILITIES  (left-click while projectile is in flight)\n"
            "  Normal Projectile  -  No special ability.\n"
            "  Boost Projectile   -  Bursts forward horizontally.\n\n"
            "CAMERA\n"
            "  Right-click and drag to pan the view.\n\n"
            "OBJECTIVE\n"
            "  Destroy all the targets to complete the level!\n"
            "  Leftover projectiles earn bonus score.";

        sf::Text instrText(hudFont, instr, UI::INSTR_FONT_SIZE);
        instrText.setFillColor(sf::Color::White);
        instrText.setPosition({ UI::INSTR_TEXT_X, UI::INSTR_TEXT_Y });
        window.draw(instrText);
    }

    if (btnBack) btnBack->Render(window);
}
