#pragma once
#include <bgfx/bgfx.h>
#include <bx/math.h>
#include "ShaderLoader.h"
#include "Model.h"


//object occlusion
//
//draw a rectangle behind the controlled character, the same height and width of the character
//
//get the near plane co-ords of this rect.
//
//create bounding box from the near and far rect, then test this agains the other object boudning boxes to determine which objects to dissolve.


// Vertex layout for our screen space quad (used in deferred rendering)
struct PosTexCoord0Vertex
{
	float m_x;
	float m_y;
	float m_z;
	float m_u;
	float m_v;

	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
	};

	static bgfx::VertexDecl ms_decl;
};


struct camera {
	float* view; //[16] 4x4 matrix
	float* proj; //[16] 4x4 matrix
	float* proj2; //[16] 4x4 matrix non homogenus
};

struct screen {
	unsigned int WIDTH;
	unsigned int HEIGHT;
};



inline float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}


inline void vec2Set(float* _v, float _x, float _y)
{
	_v[0] = _x;
	_v[1] = _y;
}

inline void vec4Set(float* _v, float _x, float _y, float _z, float _w)
{
	_v[0] = _x;
	_v[1] = _y;
	_v[2] = _z;
	_v[3] = _w;
}

inline void vec4iSet(int32_t* _v, int32_t _x, int32_t _y, int32_t _z, int32_t _w)
{
	_v[0] = _x;
	_v[1] = _y;
	_v[2] = _z;
	_v[3] = _w;
}

static const int32_t cMaxBlurPassCount = 6;

struct RenderResources {

	bgfx::ProgramHandle BasicProgram;

	bgfx::UniformHandle TexColorUniform;

	float   m_view[16];
	float   m_proj[16];
	float   m_proj2[16];
};


void loadRenderResources(RenderResources& ResourceHandles, screen screen);
void renderFrame(camera cam, screen screen, StaticRenderObjs& staticRenderObjs, RenderResources RenResources);