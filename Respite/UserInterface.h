#pragma once
#include<vector>
#include<unordered_map>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include "AssetLoader.h"
#include "map.h"



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

UiRenderObjHandle Add_UiRenderObjs(UiRenderObjs& UiRenderObjs, bx::Vec3 pos, bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ibh, bgfx::TextureHandle texh, MatrixTransformStruct mtx);
void Remove_UiRenderObjs(UiRenderObjs& UiRenderObjs, UiRenderObjHandle Handle);

typedef uint16_t Ui3DRenderObjHandle;

struct Ui3DRenderObjs {

	std::vector<UiRenderObjHandle> idxToHandle;
	std::unordered_map< UiRenderObjHandle, uint32_t> handleToIdx;

	//Vertex Buffer Handle[]
	std::vector <bgfx::VertexBufferHandle> vbh;
	//Index Buffer Handle[]
	std::vector <bgfx::IndexBufferHandle> ibh;
	//Texture Handle[]
	std::vector <bgfx::TextureHandle> texh;
	//Matrix Transform[]
	std::vector<MatrixTransformStruct> perspectiveMatrixTransform; //how to render the 3d element to get the correct perspective
	std::vector<MatrixTransformStruct> matrixTransform_2D; //where to place the element in 2d ui space
};

Ui3DRenderObjHandle Add_Ui3DRenderObjs(Ui3DRenderObjs& Ui3DRenderObjs, bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ibh, bgfx::TextureHandle texh, MatrixTransformStruct proj, MatrixTransformStruct mtx);
void Remove_Ui3DRenderObjs(Ui3DRenderObjs& Ui3DRenderObjs, Ui3DRenderObjHandle Handle);


//void Label_UiRenderObjs(UiRenderObjHandle Handle, std::string label);
//UiRenderObjHandle HandleFromLabel_UiRenderObjs(std::string label);

//uint32_t idxFromHandle_UiRenderObjs(UiRenderObjHandle);
//UiRenderObjHandle handleFromIdx_UiRenderObjs(uint32_t);





struct UiWindow {
	std::vector <UiRenderObjHandle> UiHandles;
	std::vector <Ui3DRenderObjHandle> Ui3DHandles;
	std::vector < std::vector<UiVertexData>> VertexBuffers;
};




UiWindow Ui_BuildBlockSelectionUI(UiRenderObjs& uiRenderObjs, Ui3DRenderObjs& ui3DRenderObjs,
	std::unordered_map<std::string, texture>& TexturesMap, std::unordered_map<std::string, StaticProp> StaticPropMap, uint16_t screenXRes, uint16_t screenYRes);