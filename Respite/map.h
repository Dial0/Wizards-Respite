#pragma once

static const uint32_t chunksizex = 32;
static const uint32_t chunksizey = 32;
static const uint32_t chunksizez = 3;
static const uint32_t chunkMapsize = chunksizex * chunksizey * chunksizez;

enum class block_type { air, dirt };

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
int placeblock(MapChunk* Chunk, uint8_t x, uint8_t y, uint8_t z);
MapLoc to3D(uint32_t idx);
uint32_t to1D(uint8_t x, uint8_t y, uint8_t z);