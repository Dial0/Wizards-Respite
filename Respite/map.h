#pragma once

static const uint32_t chunksizex = 3;
static const uint32_t chunksizey = 3;
static const uint32_t chunksizez = 3;
static const uint32_t chunkMapsize = chunksizex * chunksizey * chunksizez;

enum class block_type { air, dirt };

struct MapChunk {
    block_type BlockMap[chunkMapsize];
};

struct MapLoc {
    uint8_t x;
    uint8_t y;
    uint8_t z;
};


int buildmap(MapChunk* Chunk);
MapLoc to3D(uint32_t idx);
uint32_t to1D(uint8_t x, uint8_t y, uint8_t z);