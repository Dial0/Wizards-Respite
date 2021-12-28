#pragma once
#include<vector>
#include<string>
#include <bgfx/bgfx.h>

struct tex_coord
{
	uint16_t x;
	uint16_t y;
};

struct pixel_font {
	bgfx::TextureHandle texhandle;
	std::vector<tex_coord> glyph_tex_cord;
	std::vector<uint8_t> glyph_width;
	uint16_t textureWidth;
	uint16_t textureHeight;
	uint8_t glyph_height;
};


struct FontVertexData {
	float x;
	float y;
	float z;
	float s;
	float t;

	static void init() {
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
	};

	static bgfx::VertexDecl ms_decl;
};

void buildfontvb(std::vector<FontVertexData>& vertexbuff, std::string string, pixel_font& font, uint16_t screenXRes, uint16_t screenYRes, float scale);
void InitPixelFontVertexDecl();