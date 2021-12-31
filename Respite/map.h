#pragma once
#include <vector>
#include <string>

static const uint32_t chunksizex = 32;
static const uint32_t chunksizey = 32;
static const uint32_t chunksizez = 3;
static const uint32_t chunkMapsize = chunksizex * chunksizey * chunksizez;

enum block_type { air, dirt, grass_drit, stone };
static const std::vector<std::string> block_str = { "Air", "Dirt", "Grassy Dirt", "Stone"};
static const std::vector<block_type> block_emums = {block_type::air,block_type::dirt,block_type::grass_drit,block_type::stone};


struct MapChunk {
    uint32_t sizex = chunksizex;
    uint32_t sizey = chunksizey;
    uint32_t sizez = chunksizez;
    block_type BlockMap[chunkMapsize];
};

struct MapLoc {
    uint8_t x;
    uint8_t y;
    uint8_t z;
};


int buildmap(MapChunk* Chunk);
int placeblock(MapChunk* Chunk, block_type place_type, uint8_t x, uint8_t y, uint8_t z);
MapLoc to3D(uint32_t idx);
uint32_t to1D(uint8_t x, uint8_t y, uint8_t z);