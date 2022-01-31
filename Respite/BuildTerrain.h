#pragma once
#include "block_ids.h"
struct BlockTypeStruct {
	block_type block_type;
	int rotation;
};


BlockTypeStruct get_block_type(uint8_t blocks_around);