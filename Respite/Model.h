#pragma once
#include <bgfx/bgfx.h>
#include "loadMesh.h"
#include "ActorComponets.h"

struct VertexData {
	float m_x;
	float m_y;
	float m_z;
	float m_normal_x;
	float m_normal_y;
	float m_normal_z;
	float m_s;
	float m_t;
	uint8_t m_tileid_x;
	uint8_t m_tileid_y;
	uint16_t pad;

	static void init() {
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, false,true)
			.end();
	};

	static bgfx::VertexDecl ms_decl;
};



struct Model
{
	bgfx::VertexBufferHandle vbh;
	bgfx::IndexBufferHandle ibh;
	bgfx::TextureHandle texh;
	float* matrixTransform;
};

struct Animation
{
	int animation_id;
	float time;

};

struct postition
{
	float x, y, z;
};

struct velocity
{
	float x, y, z;
};

Model LoadModel(GameMesh testMesh);

struct MatrixTransformStruct
{
	float mtx[16];
};

struct StaticRenderObjs {
	//Position[]
	std::vector<Position> pos;
	//Vertex Buffer Handle[]
	std::vector <bgfx::VertexBufferHandle> vbh;
	//Index Buffer Handle[]
	std::vector <bgfx::IndexBufferHandle> ibh;
	//Texture Handle[]
	std::vector <bgfx::TextureHandle> texh;
	//Matrix Transform[]
	std::vector<MatrixTransformStruct> matrixTransform;
};

