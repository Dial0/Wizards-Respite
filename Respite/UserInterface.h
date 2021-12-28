#pragma once
#include<vector>
#include<unordered_map>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include "AssetLoader.h"

struct UiVertexData {
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

typedef uint16_t UiRenderObjHandle;

struct UiRenderObjs {

	std::vector<UiRenderObjHandle> idxToHandle;
	std::unordered_map< UiRenderObjHandle, uint32_t> handleToIdx;

	//Position[] in screen space
	std::vector<bx::Vec3> pos;
	//Vertex Buffer Handle[]
	std::vector <bgfx::VertexBufferHandle> vbh;
	//Index Buffer Handle[]
	std::vector <bgfx::IndexBufferHandle> ibh;
	//Texture Handle[]
	std::vector <bgfx::TextureHandle> texh;
	//Matrix Transform[]
	std::vector<MatrixTransformStruct> matrixTransform;
};

UiRenderObjHandle Add_UiRenderObjs(UiRenderObjs UiRenderObjs, bx::Vec3 pos, bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ibh, bgfx::TextureHandle texh, MatrixTransformStruct mtx);
//get handle
//get index
//map handle to index
//map idx to handle
//push back all arrays with inputs

//void Label_UiRenderObjs(UiRenderObjHandle Handle, std::string label);
//UiRenderObjHandle HandleFromLabel_UiRenderObjs(std::string label);

uint32_t idxFromHandle_UiRenderObjs(UiRenderObjHandle);
UiRenderObjHandle handleFromIdx_UiRenderObjs(uint32_t);

void Remove_UiRenderObjs(UiRenderObjs UiRenderObjs, UiRenderObjHandle Handle);








void Ui_BuildBlockSelectionUI(std::vector<UiVertexData>& vbo, UiRenderObjs& uiRenderObjs, std::unordered_map<std::string, texture>& TexturesMap, uint16_t screenXRes, uint16_t screenYRes);