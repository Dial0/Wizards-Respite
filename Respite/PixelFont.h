#pragma once
#include<vector>
#include<string>

struct fontVertexPos
{
	float x;
	float y;
	float u;
	float v;
};

struct tex_coord
{
	float x;
	float y;
	float width;
	float height;
};

struct pixel_font {
	//texture handle
	std::vector<tex_coord> glyph_tex_cord;
	std::vector<uint8_t> glyph_width;
	uint16_t textureWidth;
	uint16_t textureHeight;
	uint8_t glyph_height;
};


float ScreenSpaceWidth(uint16_t screenXRes, uint8_t fontPixelWidth, float scale)
{
	return (2.0f / screenXRes) * fontPixelWidth * scale;
}



void buildfontvb(std::string string, pixel_font font)
{
	std::vector<fontVertexPos> vertexbuff;

	uint16_t lineXPos = 0;

	for (std::string::size_type i = 0; i < string.size(); ++i) 
	{
		uint8_t	ascii_code = string[i];
		uint8_t idx = ascii_code - 32;

		uint8_t glyphwidth = font.glyph_width[idx];

		//0,0 is top left
		// x ---->
		// y
		// |
		// |
		// V

		//1----2
		//|   /
		//| /
		//3

		//     4
		//   / |
		// /   |
		//6----5

		fontVertexPos vert1;
		vert1.x = (float)lineXPos;
		vert1.y = 0.0f;

		vert1.u = (1.0f / font.textureWidth) * font.glyph_tex_cord[idx].x;
		vert1.v = (1.0f / font.textureHeight) * font.glyph_tex_cord[idx].y;


		fontVertexPos vert2;
		vert2.x = (float)glyphwidth + (float)lineXPos;
		vert2.y = 0.0f;

		vert2.u = (1.0f / font.textureWidth) * font.glyph_tex_cord[idx].x + font.glyph_tex_cord[idx].width;
		vert2.v = (1.0f / font.textureHeight) * font.glyph_tex_cord[idx].y;

		fontVertexPos vert3;
		vert3.x = (float)lineXPos;
		vert3.y = (float)font.glyph_height;

		vert3.u = (1.0f / font.textureWidth) * font.glyph_tex_cord[idx].x;
		vert3.v = (1.0f / font.textureHeight) * font.glyph_tex_cord[idx].y + font.glyph_tex_cord[idx].height;

		fontVertexPos vert4;
		vert4.x = (float)glyphwidth + (float)lineXPos;
		vert4.y = 0.0f;

		vert4.u = (1.0f / font.textureWidth) * font.glyph_tex_cord[idx].x + font.glyph_tex_cord[idx].width;;
		vert4.v = (1.0f / font.textureHeight) * font.glyph_tex_cord[idx].y;


		fontVertexPos vert5;
		vert5.x = (float)glyphwidth + (float)lineXPos;
		vert5.y = (float)font.glyph_height;

		vert5.u = (1.0f / font.textureWidth) * font.glyph_tex_cord[idx].x + font.glyph_tex_cord[idx].width;;
		vert5.v = (1.0f / font.textureHeight) * font.glyph_tex_cord[idx].y + font.glyph_tex_cord[idx].height;

		fontVertexPos vert6;
		vert6.x = (float)lineXPos;
		vert6.y = (float)font.glyph_height;

		vert6.u = (1.0f / font.textureWidth) * font.glyph_tex_cord[idx].x;
		vert6.v = (1.0f / font.textureHeight) * font.glyph_tex_cord[idx].y + font.glyph_tex_cord[idx].height;

		lineXPos += glyphwidth;

	}
}