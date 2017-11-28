#ifndef __PLAYER_H__
#define __PLAYER_H__

typedef struct
{
    int x;                  // X Position of player
    int y;                  // Y Position of player
    int width;              // Width of sprite for player
    int height;             // Height of sprite for player
    int angle;              // Rotated angle of sprite
    int speed;              // Speed of player
    int animation;          // Animation player is using.
    int depth;              // display depth in world
    int health;             // Player health
    int weapon;             // Weapon player is using
    int ammo;               // Player ammo level

    int weapon1_ammo;        // Grenades
    int weapon2_ammo;        // Pistol ammo (placeholder)
    int weapon3_ammo;        // Shotgun Ammo
    int weapon4_ammo;        // Uzi Ammo
    int weapon5_ammo;        // Assault Rifle ammo
    int weapon6_ammo;        // flame thrower fuel ammo
    int weapon7_ammo;        // RPG ammo
    int riotshield_value;
    int riotcount;
    int shotcount;

    int alarm0;

    int bullet_speed;        // bullet speed
    int bullet_offset_x;     // bullet offset x axis in relation to player graphic
    int bullet_offset_y;     // bullet offset x axis in relation to player graphic

    bool moving;             // Flag for movement of player
    bool have_weapon1;       // Grenade
    bool have_weapon2;       // pistol
    bool have_weapon3;       // shotgun
    bool have_weapon4;       // uzi
    bool have_weapon5;       // m4a1
    bool have_weapon6;       // flamethrower
    bool have_weapon7;       // rpg
    bool have_item1;         // ARTIFACT 1 (KEYCARD, DISK, ITEM ECT)
    bool have_item2;         // ARTIFACT 1 (KEYCARD, DISK, ITEM ECT)
    bool have_item3;         // ARTIFACT 1 (KEYCARD, DISK, ITEM ECT)
    bool have_riotshield;
    bool riotactive;
    bool ok2shoot;
    bool shooting;

} _player;

extern _player player;
#endif
