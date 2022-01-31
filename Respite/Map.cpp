#include <stdint.h>
#include "map.h"

uint32_t to1D(uint8_t x, uint8_t y, uint8_t z) 
{
    return (x * chunksizez * chunksizey) + (y * chunksizez) + z;
}

MapLoc to3D(uint32_t idx)
{
    MapLoc result;
    result.x = idx / (chunksizez * chunksizey);
    idx -= result.x * chunksizez * chunksizey;
    result.y = idx / chunksizez;
    result.z = idx % chunksizez;
    return result;
}

int placeblock(MapChunk* Chunk, block_type place_type, uint8_t rotation, uint8_t x, uint8_t y, uint8_t z)
{
    if (x>=0 && x< Chunk->sizex && 
        y>=0 && y < Chunk->sizey&&
        z>=0 && z < Chunk->sizez)
    {
        uint32_t idx = to1D(x, y, z);
        Chunk->BlockMap[idx] = place_type;
        Chunk->rot[idx] = rotation;
    }
    return 0;
}

int buildmap(MapChunk* Chunk)
{
    static const uint32_t height = 10;

    for (size_t x = 0; x < chunksizex; x++)
    {
        for (size_t y = 0; y < chunksizey; y++)
        {
            for (size_t z = 0; z < chunksizez; z++)
            {
                uint32_t idx = to1D(x, y, z);
                Chunk->BlockMap[idx] = block_type::air;
            }
        }
    }

    return 0;
}