#pragma once
#include<vector>
#include<string>
#include <bgfx/bgfx.h>
#include "PixelFont.h"

bgfx::VertexDecl FontVertexData::ms_decl;

void InitPixelFontVertexDecl()
{
	FontVertexData::init();
}

void buildfontvb(std::vector<FontVertexData>& vertexbuff, std::string string, pixel_font& font, uint16_t screenXRes, uint16_t screenYRes, float scale)
{

	float lineXPos = 0;

	for (std::string::size_type i = 0; i < string.size(); ++i)
	{
		uint8_t	ascii_code = string[i];
		uint8_t idx = ascii_code - 32;

		uint8_t glyphwidth = font.glyph_width[idx];

		float ss_glyphwidth = (2.0f / screenXRes) * glyphwidth * scale;
		float ss_glyphheight = (2.0f / screenYRes) * font.glyph_height * scale;

		float half_pixel_x = (2.0f / screenXRes) / 2.0f;
		float half_pixel_y = (2.0f / screenYRes) / 2.0f;

		//0,0 is bottom left
		// ^
		// |
		// |
		// y
		// x ---->

		//1----2
		//|   /
		//| /
		//3

		//     4
		//   / |
		// /   |
		//6----5

		FontVertexData vert1;
		vert1.x = lineXPos + half_pixel_x;
		vert1.y = ss_glyphheight + half_pixel_y;
		vert1.z = 0.0f;

		vert1.s = (1.0f / font.textureWidth) * font.glyph_tex_cord[idx].x;
		vert1.t = (1.0f / font.textureHeight) * font.glyph_tex_cord[idx].y;

		vertexbuff.push_back(vert1);


		FontVertexData vert2;
		vert2.x = ss_glyphwidth + lineXPos + half_pixel_x;
		vert2.y = ss_glyphheight + half_pixel_y;
		vert2.z = 0.0f;

		vert2.s = (1.0f / font.textureWidth) * (font.glyph_tex_cord[idx].x + font.glyph_width[idx]);
		vert2.t = (1.0f / font.textureHeight) * font.glyph_tex_cord[idx].y;

		vertexbuff.push_back(vert2);

		FontVertexData vert3;
		vert3.x = lineXPos + half_pixel_x;
		vert3.y = 0.0f + half_pixel_y;
		vert3.z = 0.0f;

		vert3.s = (1.0f / font.textureWidth) * font.glyph_tex_cord[idx].x;
		vert3.t = (1.0f / font.textureHeight) * (font.glyph_tex_cord[idx].y + font.glyph_height);

		vertexbuff.push_back(vert3);


		vertexbuff.push_back(vert2); //vert 4


		//fontVertexPos vert4;
		//vert4.x = (float)glyphwidth + (float)lineXPos;
		//vert4.y = 0.0f;

		//vert4.u = (1.0f / font.textureWidth) * font.glyph_tex_cord[idx].x + font.glyph_tex_cord[idx].width;;
		//vert4.v = (1.0f / font.textureHeight) * font.glyph_tex_cord[idx].y;


		FontVertexData vert5;
		vert5.x = ss_glyphwidth + lineXPos + half_pixel_x;
		vert5.y = 0.0f + half_pixel_y;
		vert5.z = 0.0f;

		vert5.s = (1.0f / font.textureWidth) * (font.glyph_tex_cord[idx].x + font.glyph_width[idx]);
		vert5.t = (1.0f / font.textureHeight) * (font.glyph_tex_cord[idx].y + font.glyph_height);

		vertexbuff.push_back(vert5);

		vertexbuff.push_back(vert3); //vert 6

		//fontVertexPos vert6;
		//vert6.x = (float)lineXPos;
		//vert6.y = (float)font.glyph_height;

		//vert6.u = (1.0f / font.textureWidth) * font.glyph_tex_cord[idx].x;
		//vert6.v = (1.0f / font.textureHeight) * font.glyph_tex_cord[idx].y + font.glyph_tex_cord[idx].height;

		lineXPos += ss_glyphwidth;

	}
}