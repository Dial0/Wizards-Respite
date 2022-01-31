#pragma once
#include <vector>
#include <string>
enum block_type { air, terrain_grass_dirt_both_sides, terrain_grass_dirt_center, terrain_grass_dirt_center_four_internal, terrain_grass_dirt_center_one_internal, terrain_grass_dirt_center_three_internal, terrain_grass_dirt_center_two_internal, terrain_grass_dirt_center_two_internal_opposite, terrain_grass_dirt_corner, terrain_grass_dirt_corner_double, terrain_grass_dirt_side, terrain_grass_dirt_side_internal_left, terrain_grass_dirt_side_internal_right, terrain_grass_dirt_side_two_internal, terrain_grass_dirt_single, terrain_grass_dirt_single_end, };
static const std::vector<std::string> block_str = { "Air", "Terrain Grass Dirt Both Sides", "Terrain Grass Dirt Center", "Terrain Grass Dirt Center Four Internal", "Terrain Grass Dirt Center One Internal", "Terrain Grass Dirt Center Three Internal", "Terrain Grass Dirt Center Two Internal", "Terrain Grass Dirt Center Two Internal Opposite", "Terrain Grass Dirt Corner", "Terrain Grass Dirt Corner Double", "Terrain Grass Dirt Side", "Terrain Grass Dirt Side Internal Left", "Terrain Grass Dirt Side Internal Right", "Terrain Grass Dirt Side Two Internal", "Terrain Grass Dirt Single", "Terrain Grass Dirt Single End", };
static const std::vector<block_type> block_emums = { 
block_type::air,
block_type::terrain_grass_dirt_both_sides,
block_type::terrain_grass_dirt_center,
block_type::terrain_grass_dirt_center_four_internal,
block_type::terrain_grass_dirt_center_one_internal,
block_type::terrain_grass_dirt_center_three_internal,
block_type::terrain_grass_dirt_center_two_internal,
block_type::terrain_grass_dirt_center_two_internal_opposite,
block_type::terrain_grass_dirt_corner,
block_type::terrain_grass_dirt_corner_double,
block_type::terrain_grass_dirt_side,
block_type::terrain_grass_dirt_side_internal_left,
block_type::terrain_grass_dirt_side_internal_right,
block_type::terrain_grass_dirt_side_two_internal,
block_type::terrain_grass_dirt_single,
block_type::terrain_grass_dirt_single_end, };