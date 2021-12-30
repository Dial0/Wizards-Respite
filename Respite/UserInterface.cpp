#pragma once
#include<vector>
#include<unordered_map>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include "UserInterface.h"

bgfx::VertexDecl UiVertexData::ms_decl;



UiRenderObjHandle getLowestAvalHandle(const std::vector<UiRenderObjHandle>& arr)
{
	std::vector<uint8_t> visited;
	visited.resize(arr.size(), 1);

	for (size_t i = 0; i < arr.size(); i++)
	{
		if (arr[i] - 1 < arr.size() && arr[arr[i] - 1] > 0)
		{
			visited[arr[i] - 1] = 0;
		}

	}

	// Return the first index value at which is positive
	for (size_t i = 0; i < visited.size(); i++)
	{
		if (visited[i] > 0)
		{
			// 1 is added because indexes start from 0
			return i + 1;
		}

	}

	return arr.size() + 1;
}

UiRenderObjHandle Add_UiRenderObjs(UiRenderObjs& UiRenderObjs, bx::Vec3 pos, bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ibh, bgfx::TextureHandle texh, MatrixTransformStruct mtx)
{

	//get new handle
	UiRenderObjHandle NewHandle = getLowestAvalHandle(UiRenderObjs.idxToHandle);

	//get index
	//map idx to handle
	UiRenderObjs.idxToHandle.push_back(NewHandle);
	uint16_t idx = UiRenderObjs.idxToHandle.size() - 1;

	//map handle to index
	UiRenderObjs.handleToIdx[NewHandle] = idx;

	//push back all arrays with inputs

	//do we want to check all arrays are the same length?
	UiRenderObjs.pos.push_back(pos);
	UiRenderObjs.vbh.push_back(vbh);
	UiRenderObjs.ibh.push_back(ibh);
	UiRenderObjs.texh.push_back(texh);
	UiRenderObjs.matrixTransform.push_back(mtx);

	return NewHandle;
}

void Remove_UiRenderObjs(UiRenderObjs& UiRenderObjs, UiRenderObjHandle Rem_Handle) {

	//get index from input handle
	uint16_t rem_idx = UiRenderObjs.handleToIdx[Rem_Handle];
	uint16_t end_idx = UiRenderObjs.idxToHandle.size() - 1;

	if (rem_idx != end_idx) //check if we are removing the last element
	{
		//get the handle of the last element
		UiRenderObjHandle lastElementHandle = UiRenderObjs.idxToHandle[end_idx];

		//swap element to be removed with the last element for all arrays
		std::swap(UiRenderObjs.idxToHandle[rem_idx], UiRenderObjs.idxToHandle[end_idx]);
		std::swap(UiRenderObjs.pos[rem_idx], UiRenderObjs.pos[end_idx]);
		std::swap(UiRenderObjs.vbh[rem_idx], UiRenderObjs.vbh[end_idx]);
		std::swap(UiRenderObjs.ibh[rem_idx], UiRenderObjs.ibh[end_idx]);
		std::swap(UiRenderObjs.texh[rem_idx], UiRenderObjs.texh[end_idx]);
		std::swap(UiRenderObjs.matrixTransform[rem_idx], UiRenderObjs.matrixTransform[end_idx]);

		//update index for swapped handle
		UiRenderObjs.handleToIdx[lastElementHandle] = rem_idx;
	}

	//pop last element for all arrays
	UiRenderObjs.idxToHandle.pop_back();
	UiRenderObjs.pos.pop_back();
	UiRenderObjs.vbh.pop_back();
	UiRenderObjs.ibh.pop_back();
	UiRenderObjs.texh.pop_back();
	UiRenderObjs.matrixTransform.pop_back();
	
	//remove input handle from handletoidx map
	UiRenderObjs.handleToIdx.erase(Rem_Handle);
}



void Ui_BuildBox(std::vector<UiVertexData>& vbo, uint16_t screenXRes, uint16_t screenYRes)
{
	static const uint16_t largeUIboxsize = 64; //64 x 64 pixels
	static const uint16_t smallUIboxsize = 48;

	float ss_largeUIboxsizex = (2.0f / screenXRes) * largeUIboxsize;
	float ss_largeUIboxsizey = (2.0f / screenYRes) * largeUIboxsize;

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

	UiVertexData vert1;
	vert1.x = 0.0f + half_pixel_x;
	vert1.y = ss_largeUIboxsizey + half_pixel_y;
	vert1.z = 0.0f;

	vert1.s = 0.0f;
	vert1.t = 0.0f;

	UiVertexData vert2;
	vert2.x = ss_largeUIboxsizex + half_pixel_x;
	vert2.y = ss_largeUIboxsizey + half_pixel_y;
	vert2.z = 0.0f;

	vert2.s = 1.0f;
	vert2.t = 0.0f;

	UiVertexData vert3;
	vert3.x = 0.0f + half_pixel_x;
	vert3.y = 0.0f + half_pixel_y;
	vert3.z = 0.0f;

	vert3.s = 0.0f;
	vert3.t = 1.0f;

	UiVertexData vert4; //position 5 in diagram
	vert4.x = ss_largeUIboxsizex + half_pixel_x;
	vert4.y = 0.0f + half_pixel_y;
	vert4.z = 0.0f;

	vert4.s = 1.0f;
	vert4.t = 1.0f;


	vbo.push_back(vert1);
	vbo.push_back(vert2);
	vbo.push_back(vert3);

	vbo.push_back(vert2);
	vbo.push_back(vert4);
	vbo.push_back(vert3);

}

std::vector <UiRenderObjHandle> Ui_BuildBlockSelectionUI(std::vector<UiVertexData>& vbo, UiRenderObjs& uiRenderObjs, std::unordered_map<std::string, texture>& TexturesMap, uint16_t screenXRes, uint16_t screenYRes)
{

	std::vector <UiRenderObjHandle> BbsHandles;
	Ui_BuildBox(vbo, screenXRes, screenYRes);

	size_t datasize = sizeof(UiVertexData);
	bgfx::VertexBufferHandle box = bgfx::createVertexBuffer(bgfx::makeRef(&vbo[0], datasize * vbo.size()), UiVertexData::ms_decl);

	MatrixTransformStruct mtx;
	bx::mtxTranslate(mtx.mtx, -0.95f, 0.0f, 0.0f);
	BbsHandles.push_back(Add_UiRenderObjs(uiRenderObjs, bx::Vec3(), box, bgfx::IndexBufferHandle(), TexturesMap["ui_box2.png"].texh, mtx));

	bx::mtxTranslate(mtx.mtx, -0.95f, 0.2f, 0.0f);
	BbsHandles.push_back(Add_UiRenderObjs(uiRenderObjs, bx::Vec3(), box, bgfx::IndexBufferHandle(), TexturesMap["ui_box2.png"].texh, mtx));

	bx::mtxTranslate(mtx.mtx, -0.95f, -0.2f, 0.0f);
	BbsHandles.push_back(Add_UiRenderObjs(uiRenderObjs, bx::Vec3(), box, bgfx::IndexBufferHandle(), TexturesMap["ui_box2.png"].texh, mtx));

	bx::mtxTranslate(mtx.mtx, -0.95f, 0.4f, 0.0f);
	BbsHandles.push_back(Add_UiRenderObjs(uiRenderObjs, bx::Vec3(), box, bgfx::IndexBufferHandle(), TexturesMap["ui_box2.png"].texh, mtx));

	bx::mtxTranslate(mtx.mtx, -0.95f, -0.4f, 0.0f);
	BbsHandles.push_back(Add_UiRenderObjs(uiRenderObjs, bx::Vec3(), box, bgfx::IndexBufferHandle(), TexturesMap["ui_box2.png"].texh, mtx));

	return BbsHandles;

}

