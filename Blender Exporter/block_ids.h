#pragma once
enum block_type { air, dirt_block, stone_block, grass_dirt_block, grass_ramp, grass_ramp_corner, grass_block, tree_trunk, tree_ramp_corner, tree_top, tree_ramp, tree_stump, tree_block, };
static const std::vector<std::string> block_str = { "Air", "Dirt Block", "Stone Block", "Grass Dirt Block", "Grass Ramp", "Grass Ramp Corner", "Grass Block", "Tree Trunk", "Tree Ramp Corner", "Tree Top", "Tree Ramp", "Tree Stump", "Tree Block", };
static const std::vector<block_type> block_emums = {block_type::air, block_type::dirt_block, block_type::stone_block, block_type::grass_dirt_block, block_type::grass_ramp, block_type::grass_ramp_corner, block_type::grass_block, block_type::tree_trunk, block_type::tree_ramp_corner, block_type::tree_top, block_type::tree_ramp, block_type::tree_stump, block_type::tree_block, };