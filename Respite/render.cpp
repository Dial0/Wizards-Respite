#include "Render.h"


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


void renderFrame(camera cam, screen screen, StaticRenderObjs& staticRenderObjs, RenderResources RenResources)
{

	uint16_t RENDER_SCENE_PASS_ID = 0;

	bgfx::setViewRect(RENDER_SCENE_PASS_ID, 0, 0, screen.WIDTH, screen.HEIGHT);
	bgfx::setViewTransform(RENDER_SCENE_PASS_ID, cam.view, cam.proj);
	//bgfx::setViewFrameBuffer(RENDER_SCENE_PASS_ID, RenResources.m_gbuffer);
	bgfx::setViewName(RENDER_SCENE_PASS_ID, "Gbuffer");

	bgfx::touch(0);

	bgfx::setViewClear(RENDER_SCENE_PASS_ID
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff, 1.0f, 0
	);


	for (size_t i = 0; i < staticRenderObjs.pos.size(); i++)
	{
		// Set model matrix for rendering.
		bgfx::setTransform(staticRenderObjs.matrixTransform[i].mtx);

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(0, staticRenderObjs.vbh[i]);
		bgfx::setIndexBuffer(staticRenderObjs.ibh[i]);

		bgfx::setTexture(0, RenResources.TexColorUniform, staticRenderObjs.texh[i]);

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

		// Submit primitive for rendering to view 0.
		bgfx::submit(RENDER_SCENE_PASS_ID, RenResources.BasicProgram);

	}


	bgfx::frame();
}