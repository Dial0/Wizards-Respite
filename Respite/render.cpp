#include "Render.h"
#include <SDL.h>

bgfx::VertexDecl PosTexCoord0Vertex::ms_decl;
// Utility function to draw a screen space quad for deferred rendering
void screenSpaceQuad(float _textureWidth, float _textureHeight, float _texelHalf, bool _originBottomLeft, float _width = 1.0f, float _height = 1.0f)
{
	if (3 == bgfx::getAvailTransientVertexBuffer(3, PosTexCoord0Vertex::ms_decl))
	{
		bgfx::TransientVertexBuffer vb;
		bgfx::allocTransientVertexBuffer(&vb, 3, PosTexCoord0Vertex::ms_decl);
		PosTexCoord0Vertex* vertex = (PosTexCoord0Vertex*)vb.data;

		const float minx = -_width;
		const float maxx = _width;
		const float miny = 0.0f;
		const float maxy = _height * 2.0f;

		const float texelHalfW = _texelHalf / _textureWidth;
		const float texelHalfH = _texelHalf / _textureHeight;
		const float minu = -1.0f + texelHalfW;
		const float maxu = 1.0f + texelHalfH;

		const float zz = 0.0f;

		float minv = texelHalfH;
		float maxv = 2.0f + texelHalfH;

		if (_originBottomLeft)
		{
			float temp = minv;
			minv = maxv;
			maxv = temp;

			minv -= 1.0f;
			maxv -= 1.0f;
		}

		vertex[0].m_x = minx;
		vertex[0].m_y = miny;
		vertex[0].m_z = zz;
		vertex[0].m_u = minu;
		vertex[0].m_v = minv;

		vertex[1].m_x = maxx;
		vertex[1].m_y = miny;
		vertex[1].m_z = zz;
		vertex[1].m_u = maxu;
		vertex[1].m_v = minv;

		vertex[2].m_x = maxx;
		vertex[2].m_y = maxy;
		vertex[2].m_z = zz;
		vertex[2].m_u = maxu;
		vertex[2].m_v = maxv;

		bgfx::setVertexBuffer(0, &vb);
	}
}





void loadRenderResources(RenderResources& ResourceHandles, screen screen)
{


	PosTexCoord0Vertex::init();

}


void renderFrame(camera cam, screen screen, StaticRenderObjs& staticRenderObjs,UiRenderObjs& uiRenderObjs, Ui3DRenderObjs& ui3DRenderObjs, RenderResources RenResources, bgfx::VertexBufferHandle Font_vbh, bgfx::TextureHandle font_th )
//void renderFrame(camera cam, screen screen, StaticRenderObjs& staticRenderObjs, RenderResources RenResources, bgfx::VertexBufferHandle Font_vbh, bgfx::TextureHandle font_th)
{

	uint16_t RENDER_SCENE_PASS = 0;
	bgfx::setViewRect(RENDER_SCENE_PASS, 0, 0, screen.WIDTH, screen.HEIGHT);
	bgfx::setViewTransform(RENDER_SCENE_PASS, cam.view, cam.proj);
	bgfx::setViewName(RENDER_SCENE_PASS, "Scene");

	bgfx::touch(0);

	bgfx::setViewClear(RENDER_SCENE_PASS
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff, 1.0f, 0
	);
	{
		unsigned long long state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_MSAA
			| BGFX_STATE_CULL_CW
			;

		// Set render states.
		bgfx::setState(state);
	}

	for (size_t i = 0; i < staticRenderObjs.pos.size(); i++)
	{

		// Set model matrix for rendering.
		bgfx::setTransform(staticRenderObjs.matrixTransform[i].mtx);

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(0, staticRenderObjs.vbh[i]);
		//bgfx::setIndexBuffer(staticRenderObjs.ibh[i]);

		bgfx::setTexture(0, RenResources.TexColorUniform, staticRenderObjs.texh[i]);



		// Submit primitive for rendering to view 0.
		bgfx::submit(RENDER_SCENE_PASS, RenResources.BasicProgram);

	}




	const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
	const bx::Vec3 eye = { 0.0f, -45.0, 45.0 };


	const bx::Vec3 up = { 0.0f, 0.0f, 1.0f };

	// Set view and projection matrix for view 0.
	//bx::Handness::Right
	float view[16];
	bx::mtxLookAt(view, eye, at, up, bx::Handness::Right);


	uint16_t RENDER_UI_PASS = 1;
	bgfx::setViewRect(RENDER_UI_PASS, 0, 0, screen.WIDTH, screen.HEIGHT);
	bgfx::setViewTransform(RENDER_UI_PASS, view, cam.proj);
	bgfx::setViewName(RENDER_UI_PASS, "UserInterface");

	bgfx::touch(0);

	bgfx::setViewClear(RENDER_UI_PASS
		, BGFX_CLEAR_DEPTH
		, 0x00000000, 1.0f, 0
	);

	for (size_t i = 0; i < uiRenderObjs.vbh.size(); i++)
	{
		bgfx::setVertexBuffer(0, uiRenderObjs.vbh[i]);
		bgfx::setTexture(0, RenResources.TexColorUniform, uiRenderObjs.texh[i]);
		bgfx::setTransform(uiRenderObjs.matrixTransform[i].mtx);

		unsigned long long state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_MSAA
			| BGFX_STATE_BLEND_ALPHA
			;

		// Set render states.
		bgfx::setState(state);

		// Submit primitive for rendering to view 0.
		bgfx::submit(RENDER_UI_PASS, RenResources.FontProgram);
	}

	for (size_t i = 1; i < ui3DRenderObjs.idxToHandle.size(); i++)
	{
		float testmtx[32];
		float sinetest = bx::sin(bx::kPi2 * SDL_GetTicks() / 3000.0f) + 1.0f;
		bx::mtxRotateZ(testmtx, sinetest);
		memcpy(&testmtx[16], ui3DRenderObjs.perspectiveMatrixTransform[i].mtx, 16 * sizeof(float));

		bgfx::setTransform(testmtx, 2);
		bgfx::setVertexBuffer(0, ui3DRenderObjs.vbh[i]);
		bgfx::setTexture(0, RenResources.TexColorUniform, ui3DRenderObjs.texh[i]);

		unsigned long long state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_MSAA
			| BGFX_STATE_CULL_CW
			;

		// Set render states.
		bgfx::setState(state);

		bgfx::submit(RENDER_UI_PASS, RenResources.Basic3DUiProgram);
	}

	//float testmtx[16];
	//bx::mtxRotateZ(testmtx, 0.0f);

	//memcpy(RenResources.testmodeldata, testmtx, 16 * sizeof(float));
	//


	////RENDER_UI_PASS = 2;
	//bgfx::setViewRect(RENDER_UI_PASS, 0, 0, screen.WIDTH, screen.HEIGHT);
	//bgfx::setViewTransform(RENDER_UI_PASS, view, cam.proj);
	//bgfx::setViewName(RENDER_UI_PASS, "UserInterface2");

	////bgfx::setViewClear(RENDER_UI_PASS
	////	, BGFX_CLEAR_DEPTH
	////	, 0x00000000, 1.0f, 0
	////);
	//
	//// Set model matrix for rendering.
	//bgfx::setTransform(RenResources.testmodeldata,2);
	////bgfx::setTransform(RenResources.testmodeldata,2);
	//// Set vertex and index buffer.
	//bgfx::setVertexBuffer(0, staticRenderObjs.vbh[0]);
	////bgfx::setIndexBuffer(staticRenderObjs.ibh[i]);

	//bgfx::setTexture(0, RenResources.TexColorUniform, staticRenderObjs.texh[0]);

	//unsigned long long state = 0
	//	| BGFX_STATE_WRITE_RGB
	//	| BGFX_STATE_WRITE_A
	//	| BGFX_STATE_WRITE_Z
	//	| BGFX_STATE_DEPTH_TEST_LESS
	//	| BGFX_STATE_MSAA
	//	| BGFX_STATE_CULL_CW
	//	;

	//// Set render states.
	//bgfx::setState(state);

	//// Submit primitive for rendering to view 0.
	//

	//bgfx::submit(RENDER_UI_PASS, RenResources.Basic3DUiProgram);




	bgfx::frame();
}
