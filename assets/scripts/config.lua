-- screen config
name = "SkillSwitch"
screen_width = 1024
screen_height = 768

-- general physic config
box2d_scale = 1 / 500
gravity = 9.8

-- level config
aspect_ratio = screen_width/screen_height
view_width = aspect_ratio * 20 * 64
view_height = 20 * 64
level_speed = 1
level_file = "assets/levels/level0.tmx"

-- player config
jump_impulse = -0.4
player_friction = 0
player_density = 1
flying_gravity_scale = 0.2

immortal = 3
