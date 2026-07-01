#pragma once
#include <cstdint>

// ── Collision filter categories (Box2D b2Filter::categoryBits / maskBits) ──────
// Two fixtures collide only when  (A.category & B.mask) != 0
//                              AND (B.category & A.mask) != 0
namespace CollisionFilter {
    inline constexpr uint16_t PROJECTILE = 0x0001;
    inline constexpr uint16_t BLOCK      = 0x0002;
    inline constexpr uint16_t TARGET     = 0x0004;
    inline constexpr uint16_t WALL       = 0x0008;   // invisible boundary walls
    inline constexpr uint16_t GROUND     = 0x0010;
    inline constexpr uint16_t ALL        = 0xFFFF;
}

namespace Physics {
    inline constexpr float PPM = 30.0f;
    inline constexpr float GRAVITY = 9.8f;
    inline constexpr int VELOCITY_ITERATIONS = 6;
    inline constexpr int POSITION_ITERATIONS = 2;
    inline constexpr float DAMAGE_THRESHOLD        = 2.0f;
    inline constexpr float REST_VELOCITY_THRESHOLD = 0.5f;
    inline constexpr float REST_DURATION           = 2.5f;
}

namespace Window_Settings {
    inline constexpr unsigned int WIDTH              = 1024;
    inline constexpr unsigned int HEIGHT             = 768;
    inline constexpr unsigned int FPS               = 60;

    inline constexpr unsigned int BG_R              = 135;
    inline constexpr unsigned int BG_G              = 206;
    inline constexpr unsigned int BG_B              = 235;

    inline constexpr float        GROUND_VISUAL_DEPTH = 3000.0f;
}

namespace Slingshot_Settings {
    inline constexpr float MAX_PULL_DISTANCE = 100.0f;
    inline constexpr float POWER_MULTIPLIER = 0.2f;
    inline constexpr float GRAB_RADIUS = 30.0f;
    inline constexpr float FORK_HEIGHT_RATIO = 0.38f;
    inline constexpr float SPAWN_X = 200.0f;
    inline constexpr float SPAWN_Y_OFFSET = 150.0f;
}

namespace Projectile_Settings {
    inline constexpr float DENSITY = 1.0f;
    inline constexpr float FRICTION = 0.3f;
    inline constexpr float RESTITUTION = 0.4f;
    inline constexpr float BOOST_IMPULSE = 25.0f;
    inline constexpr float RADIUS = 15.0f;
}

namespace Gameplay {
    inline constexpr float TRANSITION_DELAY        = 3.0f;

    // Out-of-bounds limits (tighter than the level width)
    inline constexpr float OOB_RIGHT               = 2500.0f;
    inline constexpr float OOB_LEFT                = -50.0f;
    inline constexpr float OOB_BOTTOM              = static_cast<float>(Window_Settings::HEIGHT);

    // Anti-rolling timeout: destroy projectile if it crawls too long
    inline constexpr float ROLL_VELOCITY_THRESHOLD = 1.5f;   // px/s below which we consider "rolling"
    inline constexpr float ROLL_TIMEOUT            = 1.5f;   // seconds at low velocity before kill
    inline constexpr float PROJECTILE_MAX_LIFETIME = 7.0f;   // hard cap on total flight time
}

namespace Materials {
    namespace Wood {
        inline constexpr float DENSITY = 0.8f;
        inline constexpr float FRICTION = 0.5f;
        inline constexpr float RESTITUTION = 0.1f;
        inline constexpr float HEALTH = 20.0f;
        inline constexpr int SCORE = 10;
    }
    namespace Ice {
        inline constexpr float DENSITY = 0.5f;
        inline constexpr float FRICTION = 0.1f;
        inline constexpr float RESTITUTION = 0.3f;
        inline constexpr float HEALTH = 8.0f;
        inline constexpr int SCORE = 15;
    }
    namespace Stone {
        inline constexpr float DENSITY = 2.0f;
        inline constexpr float FRICTION = 0.7f;
        inline constexpr float RESTITUTION = 0.05f;
        inline constexpr float HEALTH = 50.0f;
        inline constexpr int SCORE = 25;
    }
}

namespace Target_Settings {
    inline constexpr float DENSITY = 1.0f;
    inline constexpr float FRICTION = 0.4f;
    inline constexpr float RESTITUTION = 0.2f;
    inline constexpr float HEALTH = 10.0f;
    inline constexpr int SCORE = 500;
}

namespace Scores {
    inline constexpr int REMAINING_PROJECTILE_BONUS = 1000;
}

namespace HUD {
    inline constexpr float MAGAZINE_X = 20.0f;
    inline constexpr float MAGAZINE_Y = 700.0f;
    inline constexpr float MAGAZINE_SPRITE_SIZE = 40.0f;
    inline constexpr float MAGAZINE_SPACING = 50.0f;
    inline constexpr unsigned int SCORE_FONT_SIZE = 28;
    inline constexpr float SCORE_X = 20.0f;
    inline constexpr float SCORE_Y = 20.0f;
}

namespace Level_Bounds {
    inline constexpr float LEFT_WALL_X  = 400.0f;    // px — invisible wall just past the catapult
    inline constexpr float RIGHT_WALL_X = 2000.0f;   // px — invisible wall at level end
    inline constexpr float WALL_HALF_W  = 5.0f;      // px — half-width of each wall (invisible)
    inline constexpr float WALL_HALF_H  = 1000.0f;   // px — half-height (2000 px tall total)
}

namespace Camera_Settings {
    inline constexpr float ZOOM_SPEED = 0.1f;   // fraction of current size per scroll tick
    inline constexpr float MIN_ZOOM   = 0.3f;   // minimum size factor relative to initial view
    inline constexpr float MAX_ZOOM   = 3.0f;   // maximum size factor relative to initial view
}

namespace Trajectory {
    inline constexpr int   NUM_DOTS       = 18;
    inline constexpr float TIME_STEP      = 0.1f;
    inline constexpr float MAX_DOT_RADIUS = 5.0f;
}

namespace UI {
    inline constexpr float        BUTTON_WIDTH         = 250.0f;
    inline constexpr float        BUTTON_HEIGHT        =  60.0f;
    inline constexpr float        BUTTON_SPACING       =  90.0f;
    inline constexpr float        MENU_CENTER_X        = 512.0f;   // window width / 2
    inline constexpr float        MENU_TITLE_Y         = 140.0f;
    inline constexpr float        MENU_FIRST_BTN_Y     = 320.0f;
    inline constexpr float        INSTR_TITLE_Y        =  70.0f;
    inline constexpr float        INSTR_TEXT_X         = 100.0f;
    inline constexpr float        INSTR_TEXT_Y         = 140.0f;
    inline constexpr float        INSTR_BACK_BTN_Y     = 660.0f;
    inline constexpr unsigned int BUTTON_FONT_SIZE     =  26;
    inline constexpr unsigned int TITLE_FONT_SIZE      =  64;
    inline constexpr unsigned int INSTR_FONT_SIZE      =  20;
    // GameOver overlay
    inline constexpr float        GAMEOVER_TITLE_Y     = 260.0f;
    inline constexpr float        GAMEOVER_SCORE_Y     = 355.0f;
    inline constexpr float        GAMEOVER_FIRST_BTN_Y = 440.0f;
    inline constexpr uint8_t      OVERLAY_ALPHA        = 160u;
}

namespace FloatingText_Settings {
    inline constexpr unsigned int FONT_SIZE  = 28;
    inline constexpr float        LIFESPAN   = 1.5f;   // seconds until fully transparent
    inline constexpr float        VELOCITY_Y = -80.0f; // pixels per second (upward)
}

namespace Default_Materials {
    inline constexpr float BIRD_DENSITY = 1.0f;
    inline constexpr float BIRD_FRICTION = 0.3f;
    inline constexpr float BIRD_RESTITUTION = 0.4f;

    inline constexpr float BLOCK_DENSITY = 1.0f;
    inline constexpr float BLOCK_FRICTION = 0.5f;
    inline constexpr float BLOCK_RESTITUTION = 0.1f;

    inline constexpr float TARGET_DENSITY = 1.0f;
    inline constexpr float TARGET_FRICTION = 0.4f;
    inline constexpr float TARGET_RESTITUTION = 0.2f;
}
