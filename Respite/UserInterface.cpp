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

Ui3DRenderObjHandle Add_Ui3DRenderObjs(Ui3DRenderObjs& Ui3DRenderObjs, bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ibh, bgfx::TextureHandle texh, MatrixTransformStruct proj, MatrixTransformStruct mtx)
{

	//get new handle
	Ui3DRenderObjHandle NewHandle = getLowestAvalHandle(Ui3DRenderObjs.idxToHandle);

	//get index
	//map idx to handle
	Ui3DRenderObjs.idxToHandle.push_back(NewHandle);
	uint16_t idx = Ui3DRenderObjs.idxToHandle.size() - 1;

	//map handle to index
	Ui3DRenderObjs.handleToIdx[NewHandle] = idx;

	//push back all arrays with inputs

	//do we want to check all arrays are the same length?
	Ui3DRenderObjs.vbh.push_back(vbh);
	Ui3DRenderObjs.ibh.push_back(ibh);
	Ui3DRenderObjs.texh.push_back(texh);
	Ui3DRenderObjs.perspectiveMatrixTransform.push_back(proj);
	Ui3DRenderObjs.matrixTransform_2D.push_back(mtx);

	return NewHandle;
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

void Ui_BuildBoxCursor(std::vector<UiVertexData>& vbo, uint16_t screenXRes, uint16_t screenYRes)
{
	static const uint16_t largeUICursorSize = 82; //64 x 64 pixels

	float ss_largeUIboxsizex = (2.0f / screenXRes) * largeUICursorSize;
	float ss_largeUIboxsizey = (2.0f / screenYRes) * largeUICursorSize;

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

void TestmtxProjXYWH(float* _result, float _x, float _y, float _width, float _height, float _near, float _far, bool _homogeneousNdc, bx::Handness::Enum _handness)
{
	const float diff = _far - _near;
	const float aa = _homogeneousNdc ? (_far + _near) / diff : _far / diff;
	const float bb = _homogeneousNdc ? (2.0f * _far * _near) / diff : _near * aa;

	bx::memSet(_result, 0, sizeof(float) * 16);
	_result[0] = _width;
	_result[5] = _height;
	_result[8] = (bx::Handness::Right == _handness) ? _x : -_x;
	_result[9] = (bx::Handness::Right == _handness) ? _y : -_y;
	_result[10] = (bx::Handness::Right == _handness) ? -aa : aa;
	_result[11] = (bx::Handness::Right == _handness) ? -1.0f : 1.0f;
	_result[14] = -bb;
}

UiWindow Ui_BuildBlockSelectionUI(UiRenderObjs& uiRenderObjs, Ui3DRenderObjs& ui3DRenderObjs, const uint32_t cursor_enum_vec_idx,
	std::unordered_map<std::string, texture>& TexturesMap, std::unordered_map<std::string, StaticProp> StaticPropMap, const uint16_t screenXRes, const uint16_t screenYRes)
{
	const uint16_t base_res_x = 1280;
	const uint16_t base_res_y = 720;

	const float scale_ratio_x = float(base_res_x) / float(screenXRes);
	const float scale_ratio_y = float(base_res_y) / float(screenYRes);

	const float pixel_sz_x = (2.0f / screenXRes);
	const float pixel_sz_y = (2.0f / screenYRes);

	UiWindow BBSWindow;
	BBSWindow.VertexBuffers.push_back(std::vector< UiVertexData>());
	Ui_BuildBox(BBSWindow.VertexBuffers[0], screenXRes, screenYRes);
	BBSWindow.VertexBuffers.push_back(std::vector< UiVertexData>());
	Ui_BuildBoxCursor(BBSWindow.VertexBuffers[1], screenXRes, screenYRes);

	const float left_side_offset = -1 + (pixel_sz_x * 32);

	const float ss_box_width = BBSWindow.VertexBuffers[0][1].x - BBSWindow.VertexBuffers[0][0].x;
	const float ss_box_height = BBSWindow.VertexBuffers[0][0].y - BBSWindow.VertexBuffers[0][2].y;

	const float vertical_box_spacing = ss_box_height + (pixel_sz_y * 16);

	size_t datasize = sizeof(UiVertexData);
	bgfx::VertexBufferHandle box = bgfx::createVertexBuffer(bgfx::makeRef(&BBSWindow.VertexBuffers[0][0], datasize * BBSWindow.VertexBuffers[0].size()), UiVertexData::ms_decl);
	bgfx::VertexBufferHandle boxCursor = bgfx::createVertexBuffer(bgfx::makeRef(&BBSWindow.VertexBuffers[1][0], datasize * BBSWindow.VertexBuffers[1].size()), UiVertexData::ms_decl);

	float height = 1.0f / tan(bx::toRad(30.0f) * 0.5f);
	float width = height * 1.0f / (float(screenXRes) / float(screenYRes));
	height *= scale_ratio_y;
	width *= scale_ratio_y;

	const uint32_t num_aval_blocks = block_emums.size();
	const uint32_t max_shown_blocks = 5;
	const uint32_t show_blocks = std::min(num_aval_blocks, max_shown_blocks);


	const int start = (show_blocks - 1) / 2;
	const float start_pos_y = vertical_box_spacing * start;
	const float start3D_pos_y = (vertical_box_spacing * start) - (vertical_box_spacing * 2);

	for (size_t i = 0; i < show_blocks; i++)
	{
		MatrixTransformStruct mtx;

		float posy = start_pos_y - vertical_box_spacing * i;

		bx::mtxTranslate(mtx.mtx, left_side_offset, posy, 0.9f);
		BBSWindow.UiHandles.push_back(Add_UiRenderObjs(uiRenderObjs, bx::Vec3(), box, bgfx::IndexBufferHandle(), TexturesMap["ui_box2.png"].texh, mtx));

		if (i == cursor_enum_vec_idx)
		{
			float offset_x = 9 * pixel_sz_x;
			float offset_y = 9 * pixel_sz_y;
			bx::mtxTranslate(mtx.mtx, left_side_offset - offset_x, posy - offset_y, 0.91f);
			BBSWindow.UiHandles.push_back(Add_UiRenderObjs(uiRenderObjs, bx::Vec3(), boxCursor, bgfx::IndexBufferHandle(), TexturesMap["cursor.png"].texh, mtx));
		}

		MatrixTransformStruct proj;

		float pos_x = -(left_side_offset + (ss_box_width / 2));
		float pos_y = (vertical_box_spacing * i) - (ss_box_height / 2) - (vertical_box_spacing * 2);
		pos_y -= start3D_pos_y;
		TestmtxProjXYWH(proj.mtx, pos_x, pos_y, width, height, 60.0f, 70.0f, bgfx::getCaps()->homogeneousDepth, bx::Handness::Right);

		MatrixTransformStruct mtx3D;
		bx::mtxTranslate(mtx3D.mtx, 0.0, 0.0f, 0.0f);
		BBSWindow.Ui3DHandles.push_back(Add_Ui3DRenderObjs(ui3DRenderObjs,
			StaticPropMap[block_str[i]].p_Meshcontainer.vbh,
			StaticPropMap[block_str[i]].p_Meshcontainer.ibh,
			StaticPropMap[block_str[i]].p_texture.texh,
			proj,
			mtx));
	}


	return BBSWindow;

}

