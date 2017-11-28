/*
2D Saturn Shooter engine. 
*/

#include <jo/jo.h>
#include "globals.h"
#include "player.h"

_player   player;   // alias to the struct for the player


// DRAW BULLET AND MOVE IT IN DIRECTION IT WAS SHOT.
// -------------------------------------------------

inline void draw_player_bullet_list(jo_node *node)
{
    // draw the bullet
    jo_sprite_draw3D_and_rotate(obj_bullet, node->data.coord.x, node->data.coord.y, 300, node->data.coord.angle );

    // INCREMENT POSITION FOR BULLET BASED ON DIRECTION
    switch(node->data.coord.angle)
    {
      case 270: { node->data.coord.y -=player.bullet_speed; break; }
      case 90:  { node->data.coord.y +=player.bullet_speed; break; }
      case 180: { node->data.coord.x -=player.bullet_speed; break; }
      case 0:   { node->data.coord.x +=player.bullet_speed; break; }
      case 315:   { node->data.coord.x +=player.bullet_speed; node->data.coord.y -=player.bullet_speed; break; } // RIGHT/UP DIAG
      case 45+1:  { node->data.coord.x +=player.bullet_speed; node->data.coord.y +=player.bullet_speed; break; } // RIGHT/DN DIAG
      case 225+1: { node->data.coord.x -=player.bullet_speed; node->data.coord.y -=player.bullet_speed; break; } // LEFT/UP DIAG
      case 135+1: { node->data.coord.x -=player.bullet_speed; node->data.coord.y +=player.bullet_speed; break; } // LEFT/DN DIAG
    }

    // do collision checks...

    // kill object if off screen
		if (node->data.coord.y < -JO_TV_HEIGHT_2 || node->data.coord.y > JO_TV_HEIGHT_2 ) { jo_list_remove(&player_bullet_list, node); }
    if (node->data.coord.x < -JO_TV_WIDTH_2  || node->data.coord.x > JO_TV_WIDTH_2  ) { jo_list_remove(&player_bullet_list, node); }
}

// **************************************
// DRAWING ROUTINE
// **************************************

void my_draw()
{
	// clear text from screen
	jo_clear_screen();

  // draw map
	jo_map_draw(WORLD_MAP_ID, MAP_X, MAP_Y);

	// NOW SET PLAYER ANIMATION BASED ON WHAT WEAPON VARIABLE SAYS...
	switch(player.weapon)
	{
		case 1:  { player.animation = PWALK_ANIM; break; }
		case 2:  { if(player.riotactive == true) { player.animation = PRIOT_ANIM; break; } else { player.animation = PGUN2_ANIM; break; } }
		case 3:  { player.animation = PGUN3_ANIM; break; }
		case 4:  { if(player.riotactive == true) { player.animation = PRIOT_ANIM; break; } else { player.animation = PGUN4_ANIM; break; } }
		case 5:  { player.animation = PGUN5_ANIM; break; }
		case 6:  { player.animation = PGUN6_ANIM; break; }
		case 7:  { player.animation = PGUN7_ANIM; break; }
		default: { player.animation = PWALK_ANIM; }
	}

	// draw player
	jo_sprite_draw3D_and_rotate(jo_get_anim_sprite(player.animation), 0, 0, player.depth, player.angle);
  // start animations for player based on value set
	if (player.moving == true) { jo_start_sprite_anim_loop(player.animation); } else { jo_stop_sprite_anim(player.animation); jo_set_sprite_anim_frame(player.animation,2); }

  // -------------------------------
  // draw enemies, sprites, ect here
	// -------------------------------

	// Draw Player Bullets/Projectiles
	jo_list_foreach(&player_bullet_list, draw_player_bullet_list); // DRAW BULLETS THAT WERE ADDED TO THE LIST


  // Draw GUI
	jo_sprite_enable_half_transparency();				// SET HALF TRANSPARENCY
	jo_sprite_draw3D2(obj_gui, 0, 10, 150);			// DRAW GUI PANEL
	jo_sprite_disable_half_transparency();      // UN-SET HALF TRANSPARENCY
  jo_sprite_draw3D2(obj_target_icon, 6, 22-6, 100);	  // DRAW TARGET GUI ICON

	// DRAW ICONS (BULLETS/SHELLS/GAS)
  if (player.weapon == 1 || player.weapon == 7) { jo_sprite_draw3D2(obj_grenade_icon,5,48-6,100); }
	if (player.weapon == 2 || player.weapon == 4 || player.weapon == 5) { jo_sprite_draw3D2(obj_ammo_icon,6,47-6,100); }
	if (player.weapon == 3) { jo_sprite_draw3D2(obj_shells_icon,7,47-6,100); }
	if (player.weapon == 6) { jo_sprite_draw3D2(obj_gas_icon,6,47-6,100); }

  // DRAW AMMO NEXT TO ICON

	if (player.weapon == 1) { player.ammo = player.weapon1_ammo; }
	if (player.weapon == 2) { jo_printf_with_color(5, 6, JO_COLOR_INDEX_White, "-"); }
	if (player.weapon == 3) { player.ammo = player.weapon3_ammo; }
	if (player.weapon == 4) { player.ammo = player.weapon4_ammo; }
	if (player.weapon == 5) { player.ammo = player.weapon5_ammo; }
	if (player.weapon == 6) { player.ammo = player.weapon6_ammo; }
	if (player.weapon == 7) { player.ammo = player.weapon7_ammo; }

	if (player.weapon != 2) {
	if (player.ammo >= 100) { jo_printf_with_color(3, 6, JO_COLOR_INDEX_White, "%d", player.ammo); }
	if (player.ammo < 100 && player.ammo > 9) { jo_printf_with_color(4, 6, JO_COLOR_INDEX_White, "%d", player.ammo); }
	if (player.ammo < 100 && player.ammo < 10) { jo_printf_with_color(5, 6, JO_COLOR_INDEX_Red, "%d", player.ammo); } }

  // DRAW HEALTH
	jo_sprite_draw3D2(obj_health_icon,6,72-6,100);
	if(player.health == 100) { jo_printf_with_color(3, 9, JO_COLOR_INDEX_White, "%d%%", player.health); }
	if(player.health < 100 && player.health > 9) { jo_printf_with_color(5, 9, JO_COLOR_INDEX_White, "%d%%", player.health); }
	if(player.health < 100 && player.health < 10) { jo_printf_with_color(6, 9, JO_COLOR_INDEX_Red, "%d%%", player.health); }
	// DRAW WEAPON ICONS AND SELECTOR BOX
	if (player.have_weapon1 == true) { jo_sprite_draw3D2(obj_grenade_icon,24,94-6,100); } else { jo_sprite_enable_half_transparency(); jo_sprite_draw3D2(obj_grenade_icon,24,94-6,100); jo_sprite_disable_half_transparency(); }
	if (player.have_weapon2 == true) { jo_sprite_draw3D2(obj_pistol_icon,20,114-6,100); } else { jo_sprite_enable_half_transparency(); jo_sprite_draw3D2(obj_pistol_icon,20,114-6,100); jo_sprite_disable_half_transparency(); }
	if (player.have_weapon3 == true) { jo_sprite_draw3D2(obj_shotgun_icon,16,134-6,100); } else { jo_sprite_enable_half_transparency(); jo_sprite_draw3D2(obj_shotgun_icon,16,134-6,100); jo_sprite_disable_half_transparency(); }
	if (player.have_weapon4 == true) { jo_sprite_draw3D2(obj_uzi_icon,20,154-6,100); } else { jo_sprite_enable_half_transparency(); jo_sprite_draw3D2(obj_uzi_icon,20,154-6,100); jo_sprite_disable_half_transparency(); }
	if (player.have_weapon5 == true) { jo_sprite_draw3D2(obj_m4a1_icon,16,174-6,100); } else { jo_sprite_enable_half_transparency(); jo_sprite_draw3D2(obj_m4a1_icon,16,174-6,100); jo_sprite_disable_half_transparency(); }
	if (player.have_weapon6 == true) { jo_sprite_draw3D2(obj_flamethrower_icon,16,194-6,100); } else { jo_sprite_enable_half_transparency(); jo_sprite_draw3D2(obj_flamethrower_icon,16,194-6,100); jo_sprite_disable_half_transparency(); }
	if (player.have_weapon7 == true) { jo_sprite_draw3D2(obj_rpg_icon,16,214-6,100); } else { jo_sprite_enable_half_transparency(); jo_sprite_draw3D2(obj_rpg_icon,16,214-6,100); jo_sprite_disable_half_transparency(); }
	jo_sprite_draw3D2(obj_selector,0,68+(20*player.weapon),100);

	// DRAW RIOTSHIELD TEXT VALUE IF ACTIVE
 	if (player.riotactive == true) { jo_printf_with_color(14, 8, JO_COLOR_INDEX_Yellow,"RiotShield: %d%%" ,player.riotshield_value); }

	jo_printf_with_color(13, 0, JO_COLOR_INDEX_Red,"Sprite Memory Usage: %d%%" ,jo_sprite_usage_percent());
  	jo_printf_with_color(13, 1, JO_COLOR_INDEX_Red,"Memory Usage: %d%%" ,jo_memory_usage_percent());
  	jo_printf_with_color(13, 2, JO_COLOR_INDEX_Red,"FPS:%d/60" , (60/JO_FRAMERATE));
	jo_printf_with_color(13, 4, JO_COLOR_INDEX_Red,"player.ok2shoot: %d", player.ok2shoot);
}


// set offset where bullets spawn when a bullet is Created from player shooting
// ----------------------------------------------------------------------------

void set_bullet_offset()
{
    // MAYBE SET THESE TO SWITCH CASE?
    
    if (player.weapon == 2) // pistol
    {
      if(player.angle == 270)  { player.bullet_offset_x = player.x; player.bullet_offset_y = player.y-20; return; } // up
      if(player.angle == 90 )  { player.bullet_offset_x = player.x; player.bullet_offset_y = player.y+20; return; } // down
      if(player.angle == 180)  { player.bullet_offset_x = player.x-20; player.bullet_offset_y = player.y; return; } // left
      if(player.angle == 0)    { player.bullet_offset_x = player.x+20; player.bullet_offset_y = player.y; return; } // right
      if(player.angle == 315)  { player.bullet_offset_x = player.x+12; player.bullet_offset_y = player.y-12; return; } // up+right
      if(player.angle == 45+1) { player.bullet_offset_x = player.x+12; player.bullet_offset_y = player.y+12; return; } // down+right
      if(player.angle == 135+1){ player.bullet_offset_x = player.x-13; player.bullet_offset_y = player.y+12; return; } // down+Left
      if(player.angle == 225+1){ player.bullet_offset_x = player.x-11; player.bullet_offset_y = player.y-12; return; } // up+left
    }

    if (player.weapon == 4) // uzi
    {
      if(player.angle == 270)  { player.bullet_offset_x = player.x+8; player.bullet_offset_y = player.y-20; return; } // up
      if(player.angle == 90 )  { player.bullet_offset_x = player.x-8; player.bullet_offset_y = player.y+20; return; } // down
      if(player.angle == 180)  { player.bullet_offset_x = player.x-20; player.bullet_offset_y = player.y-8; return; } // left
      if(player.angle == 0)    { player.bullet_offset_x = player.x+20; player.bullet_offset_y = player.y+7; return; } // right
      if(player.angle == 315)  { player.bullet_offset_x = player.x+20; player.bullet_offset_y = player.y-10; return; } // up+right
      if(player.angle == 45+1) { player.bullet_offset_x = player.x+10; player.bullet_offset_y = player.y+20; return; } // down+right
      if(player.angle == 135+1){ player.bullet_offset_x = player.x-20; player.bullet_offset_y = player.y+10; return; } // down+Left
      if(player.angle == 225+1){ player.bullet_offset_x = player.x-8; player.bullet_offset_y = player.y-18; return; } // up+left
    }


    return;
}


// **************************************
// GAMEPAD INPUT
// **************************************

void my_gamepad(void)
{
	if (!jo_is_pad1_available()) { return; } // if no joypad connected.. bail out!
  player.moving = false; // unset moving flag (joypad will set it again if we are moving...)
	//-------------------------------------------------------------------------------------------
  // GENERAL MOVEMENT (UDLR + DIAGONALS)
  if (jo_is_pad1_key_pressed(JO_KEY_UP))    { MAP_Y -=player.speed; player.moving = true; player.angle = 270; }
  if (jo_is_pad1_key_pressed(JO_KEY_DOWN))  { MAP_Y +=player.speed; player.moving = true; player.angle = 90; }
  if (jo_is_pad1_key_pressed(JO_KEY_LEFT))  { MAP_X -=player.speed; player.moving = true; player.angle = 180; }
  if (jo_is_pad1_key_pressed(JO_KEY_RIGHT)) { MAP_X +=player.speed; player.moving = true; player.angle = 0; }
  if (jo_is_pad1_key_pressed(JO_KEY_UP)    && jo_is_pad1_key_pressed(JO_KEY_RIGHT)) { player.angle = 315; }
  if (jo_is_pad1_key_pressed(JO_KEY_RIGHT) && jo_is_pad1_key_pressed(JO_KEY_DOWN))  { player.angle = 45+1; }
  if (jo_is_pad1_key_pressed(JO_KEY_DOWN)  && jo_is_pad1_key_pressed(JO_KEY_LEFT))  { player.angle = 135+1; }
  if (jo_is_pad1_key_pressed(JO_KEY_LEFT)  && jo_is_pad1_key_pressed(JO_KEY_UP))    { player.angle = 225+1; }
	//-------------------------------------------------------------------------------------------
  // STRAFING LEFT & RIGHT (while facing up or down only)
  if (player.angle == 270 || player.angle == 90) {
  if (jo_is_pad1_key_pressed(JO_KEY_L)) { player.moving = true; MAP_X -=player.speed; }  
  if (jo_is_pad1_key_pressed(JO_KEY_R)) { player.moving = true; MAP_X +=player.speed; } }
  // STRAFING UP & DOWN (while facing left)
  if (player.angle == 180) {
	if (jo_is_pad1_key_pressed(JO_KEY_L)) { player.moving = true; MAP_Y +=player.speed; }
	if (jo_is_pad1_key_pressed(JO_KEY_R)) { player.moving = true; MAP_Y -=player.speed; }
  } else if (player.angle == 0) {
	if (jo_is_pad1_key_pressed(JO_KEY_L)) { player.moving = true; MAP_Y -=player.speed; }
	if (jo_is_pad1_key_pressed(JO_KEY_R)) { player.moving = true; MAP_Y +=player.speed; } }
	//-------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------
	// SHOOTING - work in progress
	// ------------------------------------------------------------------------------------------

if (player.alarm0 > 0 && player.shotcount != player.alarm0) { player.shotcount +=1; }
if (player.shotcount >= player.alarm0) { player.shotcount = 0; player.alarm0 = 0; player.ok2shoot = true; }

	if (jo_is_pad1_key_pressed(JO_KEY_B) && player.ok2shoot == true)
	{

		if(player.weapon == 2) // pistol
		{
  			player.ok2shoot = false;
  			player.alarm0 = 16;
  			jo_list_data    bullet;
              		set_bullet_offset();
  			bullet.coord.x = player.bullet_offset_x;
  			bullet.coord.y = player.bullet_offset_y;
              		bullet.coord.angle = player.angle;
  			jo_list_add(&player_bullet_list, bullet);
		}

          	if(player.weapon == 4) // UZI
		{
			player.ok2shoot = false;
			player.alarm0 = 4;
			jo_list_data    bullet;
              		set_bullet_offset();
              		bullet.coord.x = player.bullet_offset_x;
  			bullet.coord.y = player.bullet_offset_y;
              		bullet.coord.angle = player.angle;
			jo_list_add(&player_bullet_list, bullet);
		}

  }
	//-------------------------------------------------------------------------------------------

	// WEAPON SELECT
  	if (jo_is_pad1_key_down(JO_KEY_Z))
	{
		if(player.riotactive == true) { player.riotactive = false; }
    		player.shotcount = 0;
		if (player.weapon == 1) { player.weapon = 7;} else { player.weapon -=1; }
  	}

	if (jo_is_pad1_key_down(JO_KEY_C))
	{
	  if(player.riotactive == true) { player.riotactive = false; }
	  player.shotcount = 0;
	  if (player.weapon == 7) { player.weapon = 1;} else { player.weapon +=1; }
  	}

	//-------------------------------------------------------------------------------------------
	// RIOT SHIELD ON/OFF (only usable with pistol or uzi)
	if (jo_is_pad1_key_down(JO_KEY_Y) && player.have_riotshield == true && player.riotshield_value > 0) {
  	if (player.riotactive == false) { player.riotactive = true; } else { player.riotactive = false; }
	if (player.weapon <=2) { player.weapon = 2; }
	if (player.weapon > 2 && player.have_weapon4 == true && player.weapon4_ammo > 0) { player.weapon = 4; } else { player.weapon = 2; } }
	if (player.riotactive == true && player.riotshield_value != 0) { player.riotcount +=1; if (player.riotcount == 60) { player.riotshield_value -=1; player.riotcount = 0; } }
	if (player.riotactive == true && player.riotshield_value == 0) { player.riotactive = false; }
  //-------------------------------------------------------------------------------------------
}

// **************************************
// INITIALIZE GAME
// **************************************
void Setup_Game()
{
	// SET MAP X/Y POS & LOAD MAP GFX AND MAP ITSELF
  MAP_X = 180;
  MAP_Y = 60;

  jo_sprite_add_image_pack("STG2", "TILE.TEX", JO_COLOR_Black); // STAGE 2 TILES
  jo_map_load_from_file(WORLD_MAP_ID, 450, "MAP", "S2L1.MAP");

  // lOAD UI GRAPHICS
	obj_gui = 							jo_sprite_add_tga("SPR", "GUI.TGA", JO_COLOR_Black); // LOAD GUI PANEL
	obj_target_icon = 			jo_sprite_add_tga("SPR", "TARGET.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_ammo_icon = 				jo_sprite_add_tga("SPR", "AMMO.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_shells_icon = 			jo_sprite_add_tga("SPR", "SHELLS.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_gas_icon = 					jo_sprite_add_tga("SPR", "GAS.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_health_icon = 			jo_sprite_add_tga("SPR", "HEALTH.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_grenade_icon = 			jo_sprite_add_tga("SPR", "GRENADE.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_pistol_icon = 			jo_sprite_add_tga("SPR", "PISTOL.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_shotgun_icon = 			jo_sprite_add_tga("SPR", "SHOTGUN.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_uzi_icon = 					jo_sprite_add_tga("SPR", "UZI.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_m4a1_icon = 				jo_sprite_add_tga("SPR", "M4A1.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_flamethrower_icon = jo_sprite_add_tga("SPR", "FLAME.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_rpg_icon = 					jo_sprite_add_tga("SPR", "RPG.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON
	obj_selector = 					jo_sprite_add_tga("SPR", "SELECT.TGA", JO_COLOR_Black); // LOAD TARGET GUI ICON

	// lOAD PLAYER ANIMATION GRAPHICS
	int ANIM_ID; // TEMP VARIABLE
	ANIM_ID    = jo_sprite_add_image_pack("SPR", "PWLK.ANM", JO_COLOR_Black); // PLAYER WALKING GRAPHICS
  	PWALK_ANIM = jo_create_sprite_anim(ANIM_ID, 6, 4);
	ANIM_ID    = jo_sprite_add_image_pack("SPR", "PSTL.ANM", JO_COLOR_Black); // PLAYER WALKING GRAPHICS
  	PGUN2_ANIM = jo_create_sprite_anim(ANIM_ID, 6, 4);
	ANIM_ID    = jo_sprite_add_image_pack("SPR", "SHTG.ANM", JO_COLOR_Black); // PLAYER WALKING GRAPHICS
  	PGUN3_ANIM = jo_create_sprite_anim(ANIM_ID, 5, 4);
	ANIM_ID    = jo_sprite_add_image_pack("SPR", "UZI.ANM", JO_COLOR_Black); // PLAYER WALKING GRAPHICS
  	PGUN4_ANIM = jo_create_sprite_anim(ANIM_ID, 5, 4);
	ANIM_ID    = jo_sprite_add_image_pack("SPR", "M4A1.ANM", JO_COLOR_Black); // PLAYER WALKING GRAPHICS
 	PGUN5_ANIM = jo_create_sprite_anim(ANIM_ID, 5, 4);
	ANIM_ID    = jo_sprite_add_image_pack("SPR", "FLME.ANM", JO_COLOR_Black); // PLAYER WALKING GRAPHICS
	PGUN6_ANIM = jo_create_sprite_anim(ANIM_ID, 5, 4);
	ANIM_ID    = jo_sprite_add_image_pack("SPR", "RPG.ANM", JO_COLOR_Black); // PLAYER WALKING GRAPHICS
  	PGUN7_ANIM = jo_create_sprite_anim(ANIM_ID, 5, 4);
	ANIM_ID    = jo_sprite_add_image_pack("SPR", "RIOT.ANM", JO_COLOR_Black); // PLAYER WALKING GRAPHICS
  	PRIOT_ANIM = jo_create_sprite_anim(ANIM_ID, 5, 4);

	// LOAD ITEMS and Projectile art
	obj_bullet = jo_sprite_add_tga("SPR", "BULLET.TGA", JO_COLOR_Black); // LOAD BULLET GRAPHICS

  	// initialize lists
	jo_list_init(&player_bullet_list);

  	// SET PLAYER VALUES
	player.health = 100;
	player.weapon = 2;
	player.speed = 2;					  // SPEED`
	player.depth = 400;					// SET VISUAL DEPTH ON SCREEN FOR PLAYER (450 IS MAP LEVEL DEPTH)
  	player.angle = 0;						// PLAYER ANGLE`
  	player.moving = false;      // MOVING FLAG
	player.have_weapon1 = true;
	player.have_weapon2 = true;
	player.have_weapon3 = false;
	player.have_weapon4 = true;
	player.have_weapon5 = false;
	player.have_weapon6 = false;
	player.have_weapon7 = false;
	player.weapon1_ammo = 5;
	player.weapon2_ammo = 100; // UNLIMITED?!!? (PISTOL)
	player.weapon3_ammo = 18;
	player.weapon4_ammo = 134;
	player.weapon5_ammo = 247;
	player.weapon6_ammo = 75;
	player.weapon7_ammo = 3;
  	player.bullet_speed = 12; // 12 = default
  	player.have_riotshield = true;
  	player.riotshield_value = 100;
	player.ok2shoot = true;
  	player.animation = PWALK_ANIM;
}

/* ************************************* */
void jo_main(void)
{
	jo_core_init(JO_COLOR_Black);
	jo_printf_with_color(15, 14, JO_COLOR_INDEX_White, "Loading...");
 	Setup_Game();
	jo_core_add_callback(my_gamepad);
  	jo_core_add_callback(my_draw);
	jo_core_run();
}
/* ************************************* */
