#include "BuildTerrain.h"


//Starting at the north block and moving clockwise each bit stores if there is a terrain block in that direction
//    N  NE  E  SE  S  SW  W  NW
// 0b 1  1   1  1   1  1   1  1


static const uint8_t north = 0b10000000;
static const uint8_t north_east = 0b01000000;
static const uint8_t east = 0b00100000;
static const uint8_t south_east = 0b00010000;
static const uint8_t south = 0b00001000;
static const uint8_t south_west = 0b00000100;
static const uint8_t west = 0b00000010;
static const uint8_t north_west = 0b00000001;

uint8_t blocks_around;


int num_cardinals(uint8_t blocks_around)
{
	return ((blocks_around & north) >> 7)
		+ ((blocks_around & east) >> 5)
		+ ((blocks_around & south) >> 3)
		+ ((blocks_around & west) >> 1);
}

int num_ordinals(uint8_t blocks_around)
{
	return ((blocks_around & north_east) >> 6)
		+ ((blocks_around & south_east) >> 4)
		+ ((blocks_around & south_west) >> 2)
		+ ((blocks_around & north_west));
}


BlockTypeStruct get_block_type(uint8_t blocks_around)
{
	int num_cardinal_sides = num_cardinals(blocks_around);
	uint8_t block_cardinals = blocks_around & (north | east | south | west);

	BlockTypeStruct block_type_ret;
	block_type_ret.block_type = air;
	block_type_ret.rotation = -1;

	switch (num_cardinal_sides)
	{
	case 0:
	{
		//Draw Single Block
		block_type_ret.block_type = terrain_grass_dirt_single;
		block_type_ret.rotation = 0;
		break;
	}
	case 1:
	{
		//calc block_type_ret.rotation
		switch (block_cardinals)
		{
		case north:
		{
			block_type_ret.rotation = 0;
			break;
		}
		case east:
		{
			block_type_ret.rotation = 90;
			break;
		}
		case south:
		{
			block_type_ret.rotation = 180;
			break;
		}
		case west:
		{
			block_type_ret.rotation = 270;
			break;
		}
		}
		//Draw Single End
		block_type_ret.block_type = terrain_grass_dirt_single_end;
		break;
	}
	case 2:
	{
		bool opposing_cardinals = false;

		if (block_cardinals == (north | south) || block_cardinals == (east | west))
		{
			opposing_cardinals = true;
		}



		if (opposing_cardinals) //double flat sided block
		{
			if (block_cardinals == (east | west))
			{
				block_type_ret.rotation = 90;
			}
			else
			{
				block_type_ret.rotation = 0;
			}
			//Draw Double Sided Block
			block_type_ret.block_type = terrain_grass_dirt_both_sides;
		}
		else //Corner Block
		{
			bool ordinal_between_cardinals = false;
			switch (block_cardinals)
			{
			case (north | east):
			{
				block_type_ret.rotation = 0;
				if ((blocks_around & north_east) > 0)
				{
					ordinal_between_cardinals = true;
				}
				break;
			}
			case (south | east):
			{
				if ((blocks_around & south_east) > 0)
				{
					ordinal_between_cardinals = true;
				}
				block_type_ret.rotation = 90;
				break;
			}
			case (south | west):
			{
				if ((blocks_around & south_west) > 0)
				{
					ordinal_between_cardinals = true;
				}
				block_type_ret.rotation = 180;
				break;
			}
			case (north | west):
			{
				if ((blocks_around & north_west) > 0)
				{
					ordinal_between_cardinals = true;
				}
				block_type_ret.rotation = 270;
				break;
			}
			}


			if (ordinal_between_cardinals)
			{
				//Draw outside corner
				block_type_ret.block_type = terrain_grass_dirt_corner;
			}
			else
			{
				//Draw outside + inside corner double
				block_type_ret.block_type = terrain_grass_dirt_corner_double;
			}
		}

		break;
	}
	case 3:
	{
		int Num_ordinal_between_cardinal = 0;
		bool right_internal = true;
		//calc block_type_ret.rotation
		switch (block_cardinals)
		{
		case (west | north | east):
		{
			block_type_ret.rotation = 0;
			if ((blocks_around & north_east) > 0)
			{
				right_internal = false;
				Num_ordinal_between_cardinal++;
			}

			if ((blocks_around & north_west) > 0)
			{
				Num_ordinal_between_cardinal++;
			}

			break;
		}
		case (north | east | south):
		{
			if ((blocks_around & south_east) > 0)
			{
				right_internal = false;
				Num_ordinal_between_cardinal++;
			}

			if ((blocks_around & north_east) > 0)
			{
				Num_ordinal_between_cardinal++;
			}
			block_type_ret.rotation = 90;
			break;
		}
		case (east | south | west):
		{
			if ((blocks_around & south_west) > 0)
			{
				right_internal = false;
				Num_ordinal_between_cardinal++;
			}

			if ((blocks_around & south_east) > 0)
			{
				Num_ordinal_between_cardinal++;
			}
			block_type_ret.rotation = 180;
			break;
		}
		case (south | west | north):
		{
			if ((blocks_around & north_west) > 0)
			{
				right_internal = false;
				Num_ordinal_between_cardinal++;
			}
			if ((blocks_around & south_west) > 0)
			{
				Num_ordinal_between_cardinal++;
			}
			block_type_ret.rotation = 270;
			break;
		}
		}


		switch (Num_ordinal_between_cardinal)
		{
		case 0:
		{
			//Draw Side with 2 internals
			block_type_ret.block_type = terrain_grass_dirt_side_two_internal;
			break;
		}
		case 1:
		{
			if (right_internal)
			{
				//Draw Side with right internal
				block_type_ret.block_type = terrain_grass_dirt_side_internal_right;
			}
			else
			{
				//Draw Side with left internal
				block_type_ret.block_type = terrain_grass_dirt_side_internal_left;
			}
			break;
		}
		case 2:
		{
			//Draw Side (no internals)
			block_type_ret.block_type = terrain_grass_dirt_side;
			break;
		}
		}
		break;
	}
	case 4:
	{

		switch (num_ordinals(blocks_around))
		{
		case 0:
		{
			//draw center 4 internals
			block_type_ret.block_type = terrain_grass_dirt_center_four_internal;
			block_type_ret.rotation = 0;
			break;
		}
		case 1:
		{
			uint8_t block_ordinals = blocks_around & (north_east | south_east | south_west | north_west);
			//calc block_type_ret.rotation
			switch (block_ordinals)
			{
			case north_east:
			{
				block_type_ret.rotation = 0;
				break;
			}
			case south_east:
			{
				block_type_ret.rotation = 90;
				break;
			}
			case south_west:
			{
				block_type_ret.rotation = 180;
				break;
			}
			case north_west:
			{
				block_type_ret.rotation = 270;
				break;
			}
			}
			//draw center 3 internals
			block_type_ret.block_type = terrain_grass_dirt_center_three_internal;
			break;
		}
		case 2:
		{
			uint8_t block_ordinals = blocks_around & (north_east | south_east | south_west | north_west);
			bool ordinals_same_side = false;
			switch (block_ordinals)
			{
			case (north_west | north_east):
			{
				ordinals_same_side = true;
				block_type_ret.rotation = 0;
				break;
			}
			case (north_east | south_east):
			{
				ordinals_same_side = true;
				block_type_ret.rotation = 90;
				break;
			}
			case (south_east | south_west):
			{
				ordinals_same_side = true;
				block_type_ret.rotation = 180;
				break;
			}
			case (south_west | north_west):
			{
				ordinals_same_side = true;
				block_type_ret.rotation = 270;
				break;
			}
			case (north_east | south_west):
			{
				ordinals_same_side = false;
				block_type_ret.rotation = 0;
				break;
			}
			case (south_east | north_west):
			{
				ordinals_same_side = false;
				block_type_ret.rotation = 90;
				break;
			}
			}

			if (ordinals_same_side)
			{

				//draw center 2 internals same side
				block_type_ret.block_type = terrain_grass_dirt_center_two_internal;

			}
			else
			{
				//draw center 2 internals opposite side
				block_type_ret.block_type = terrain_grass_dirt_center_two_internal_opposite;
			}
			break;
		}
		case 3:
		{
			//calc block_type_ret.rotation
			uint8_t block_ordinals = blocks_around & (north_east | south_east | south_west | north_west);
			switch (block_ordinals)
			{
			case (south_east | south_west | north_west): // Not north_east
			{
				block_type_ret.rotation = 270;
				break;
			}
			case (north_east | south_west | north_west): // Not south_east
			{
				block_type_ret.rotation = 0;
				break;
			}
			case  (north_east | south_east | north_west): // Not south_west
			{
				block_type_ret.rotation = 90;
				break;
			}
			case (north_east | south_east | south_west): // Not north_west
			{
				block_type_ret.rotation = 180;
				break;
			}
			}
			//draw center 1 internal corner
			block_type_ret.block_type = terrain_grass_dirt_center_one_internal;
			break;
		}
		case 4:
		{
			//Draw center (no internal corners)
			block_type_ret.block_type = terrain_grass_dirt_center;
			block_type_ret.rotation = 0;
			break;
		}
		}
	}
	}
	return block_type_ret;
}