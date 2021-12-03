#include <SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <SDL_syswm.h>
#include <bx/math.h>
#include <bimg/bimg.h>
#include <random>
#include "loadMesh.h"
#include "Animation.h"
#include "Model.h"
#include "terrain.h"
#include "actor.h"
#include "ActorComponets.h"

#include "ShaderLoader.h"

#include "GenerateMap.h"

#include "Render.h"
#include "AssetLoader.h"

#include "map.h"



const int WIDTH = 1024;
const int HEIGHT = 768;

//object occlusion
//
//draw a rectangle behind the controlled character, the same height and width of the character
//
//get the near plane co-ords of this rect.
//
//create bounding box from the near and far rect, then test this agains the other object boudning boxes to determine which objects to dissolve.


static void* sdlNativeWindowHandle(SDL_Window* _window)
{
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(_window, &wmi))
	{
		return NULL;
	}

#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
	wl_egl_window* win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
	if (!win_impl)
	{
		int width, height;
		SDL_GetWindowSize(_window, &width, &height);
		struct wl_surface* surface = wmi.info.wl.surface;
		if (!surface)
			return nullptr;
		win_impl = wl_egl_window_create(surface, width, height);
		SDL_SetWindowData(_window, "wl_egl_window", win_impl);
	}
	return (void*)(uintptr_t)win_impl;
#		else
	return (void*)wmi.info.x11.window;
#		endif
#	elif BX_PLATFORM_OSX
	return wmi.info.cocoa.window;
#	elif BX_PLATFORM_WINDOWS
	return wmi.info.win.window;
#	elif BX_PLATFORM_STEAMLINK
	return wmi.info.vivante.window;
#	endif // BX_PLATFORM_
}

inline bool sdlSetWindow(SDL_Window* _window)
{
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(_window, &wmi))
	{
		return false;
	}

	bgfx::PlatformData pd;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
	pd.ndt = wmi.info.wl.display;
#		else
	pd.ndt = wmi.info.x11.display;
#		endif
#	elif BX_PLATFORM_OSX
	pd.ndt = NULL;
#	elif BX_PLATFORM_WINDOWS
	pd.ndt = NULL;
#	elif BX_PLATFORM_STEAMLINK
	pd.ndt = wmi.info.vivante.display;
#	endif // BX_PLATFORM_
	pd.nwh = sdlNativeWindowHandle(_window);

	pd.context = NULL;
	pd.backBuffer = NULL;
	pd.backBufferDS = NULL;
	bgfx::setPlatformData(pd);

	return true;
}





bool intersectPlane(const bx::Vec3& n, const bx::Vec3& p0, const bx::Vec3& l0, const bx::Vec3& l, float& t)
{
	// assuming vectors are all normalized
	float denom = bx::dot(l, n);
	if (denom > 1e-6) {
		bx::Vec3 p0l0 = bx::sub(p0, l0);
		t = bx::dot(p0l0, n) / denom;
		return (t >= 0);
	}

	return false;
}

void updateactor(actor& actor, double dT)
{
	float theta = bx::atan2(actor.pos.x - actor.target.x, actor.pos.y - actor.target.y);
	if (theta < 0.0)
		theta += bx::kPi2;
	//update heading
	actor.heading = theta;
	bx::Vec3 difference = bx::sub(actor.target, actor.pos);
	if (bx::distance(difference, bx::Vec3(0, 0, 0)))
	{
		actor.pos = bx::add((bx::mul(bx::normalize(difference), actor.speed)), actor.pos);
	}
}






void* load(const char* filename, size_t& size) {
	/// Opens a file and returns a bgfx::Memory of the raw data. The lifetime of the data is controlled by bgfx
	std::ifstream fs(filename, std::ios::in | std::ios::binary);
	if (!fs.is_open()) {
		return NULL;
	}
	fs.seekg(0, std::ios::end);
	const size_t LEN = fs.tellg();
	fs.seekg(0, std::ios::beg);
	size = LEN;
	void* mem = malloc(LEN);
	fs.read((char*)mem, LEN);
	fs.close();
	return mem;
}

const bgfx::Memory* readTexture(const char* filename) {
	/// Opens a file and returns a bgfx::Memory of the raw data. The lifetime of the data is controlled by bgfx
	std::ifstream fs(filename, std::ios::in | std::ios::binary);
	if (!fs.is_open()) {
		return NULL;
	}
	fs.seekg(0, std::ios::end);
	const size_t LEN = fs.tellg();
	fs.seekg(0, std::ios::beg);

	const bgfx::Memory* mem = bgfx::alloc(LEN);
	fs.read((char*)mem->data, LEN);
	fs.close();
	return mem;
}

int addStaticRenderObj(StaticProp newProp, StaticRenderObjs& staticRenderObjs)
{
	Position pos;
	pos.position.x = 0;
	pos.position.y = 0;
	pos.position.z = 0;

	staticRenderObjs.pos.push_back(pos);


	float* mtx = new float[16];

	bx::mtxRotateX(mtx, 0.0f);

	// position x,y,z
	//mtx[12] = 0;
	//mtx[13] = 0;
	//mtx[14] = 0;

	float scalemtx[16];
	bx::mtxScale(scalemtx, 1.0);


	MatrixTransformStruct test;
	//bx::mtxRotateZ(mtx, bx::kPi);
	bx::mtxMul(test.mtx, scalemtx, mtx);



	staticRenderObjs.matrixTransform.push_back(test);
	staticRenderObjs.vbh.push_back(newProp.p_Meshcontainer.vbh);
	staticRenderObjs.ibh.push_back(newProp.p_Meshcontainer.ibh);
	staticRenderObjs.texh.push_back(newProp.p_texture.texh);

	return 1;
}



int main(int argc, char* argv[])
{

	SDL_Init(0);
	SDL_Window* window = SDL_CreateWindow("Respite",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	sdlSetWindow(window);

	// Render an empty frame
	bgfx::renderFrame();

	// Initialize bgfx
	bgfx::init();
	VertexData::init();

	MapChunk* TestChunk = new MapChunk;
	buildmap(TestChunk);

	std::unordered_map<std::string, texture> TexturesMap;
	std::unordered_map<std::string, staticMesh> StaticMeshMap;
	std::unordered_map<std::string, StaticProp> StaticPropMap;

	LoadPropsFile("Assets\\Prop\\PropList.csv",TexturesMap, StaticMeshMap, StaticPropMap);



	StaticRenderObjs staticRenderObjs;


	StaticProp TestCube = StaticPropMap["TestCube"];
	for (size_t i = 0; i < chunkMapsize; i++)
	{
		bool check = TestChunk->BlockMap[i] == block_type::dirt;
		if (check)
		{
			MapLoc chunkloc = to3D(i);

			Position pos;
			pos.position.x = chunkloc.x;
			pos.position.y = chunkloc.y;
			pos.position.z = chunkloc.z;

			staticRenderObjs.pos.push_back(pos);


			float* mtx = new float[16];

			bx::mtxRotateX(mtx, 0.0f);

			// position x,y,z
			mtx[12] = pos.position.x * 2;
			mtx[13] = pos.position.y * 2;
			mtx[14] = pos.position.z * 2;

			float scalemtx[16];
			bx::mtxScale(scalemtx, 1.0);


			MatrixTransformStruct test;
			//bx::mtxRotateZ(mtx, bx::kPi);
			bx::mtxMul(test.mtx, scalemtx, mtx);
			staticRenderObjs.matrixTransform.push_back(test);
			staticRenderObjs.vbh.push_back(TestCube.p_Meshcontainer.vbh);
			staticRenderObjs.ibh.push_back(TestCube.p_Meshcontainer.ibh);
			staticRenderObjs.texh.push_back(TestCube.p_texture.texh);
		}
	}


	//for (const auto& StaticProp : StaticPropMap)
	//{

	//	addStaticRenderObj(StaticProp.second, staticRenderObjs);

	//}



	bgfx::UniformHandle s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

	//bgfx::ProgramHandle m_progShadow = loadProgram("vs_sms_shadow.bin", "fs_sms_shadow.bin");
	bgfx::ProgramHandle m_progMesh = loadProgram("G:\\Users\\Ethan\\Documents\\bgfx\\bgfx\\scripts\\shaders\\dx11\\vs_basic.bin", "G:\\Users\\Ethan\\Documents\\bgfx\\bgfx\\scripts\\shaders\\dx11\\fs_basic.bin");
	//bgfx::ProgramHandle m_progssao = loadProgram("J:\\Users\\Ethan\\Documents\\bgfx\\bgfx\\scripts\\shaders\\dx11\\vs_ssao.bin", "J:\\Users\\Ethan\\Documents\\bgfx\\bgfx\\scripts\\shaders\\dx11\\fs_ssao_2.bin");
	//bgfx::ProgramHandle m_progssaoblurmerge = loadProgram("J:\\Users\\Ethan\\Documents\\bgfx\\bgfx\\scripts\\shaders\\dx11\\vs_ssaoblurmerge.bin", "J:\\Users\\Ethan\\Documents\\bgfx\\bgfx\\scripts\\shaders\\dx11\\fs_ssaoblurmerge.bin");

	// Reset window
	bgfx::reset(WIDTH, HEIGHT, BGFX_RESET_VSYNC);

	// Enable debug text.
	//bgfx::setDebug(BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS);

	// Set view rectangle for 0th view
	bgfx::setViewRect(0, 0, 0, uint16_t(WIDTH), uint16_t(HEIGHT));

	// Clear the view rect
	bgfx::setViewClear(0,
		BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
		0x443355FF, 1.0f, 0);


	//// Set empty primitive on screen
	bgfx::touch(0);

	bgfx::frame();

	// Poll for events and wait till user closes window
	bool quit = false;
	SDL_Event currentEvent;
	const Uint8* state = SDL_GetKeyboardState(NULL);
	float z_rot = 0.0f;
	float x_trans = 0.0f;
	float y_trans = 0.0f;
	bx::Vec3 camera_heading = { 0.0f,0.0f,0.0f };
	float camera_height = 30.0f;
	float camera_distance = 30.0f;

	float animation_index = 0;

	Uint32 lastUpdate = SDL_GetTicks();
	Uint32 current = SDL_GetTicks();


	actor testplayer;
	testplayer.speed = 0.5;
	testplayer.pos.x = 0.0f;
	testplayer.pos.y = 0.0f;
	testplayer.pos.z = 3.0f;
	testplayer.heading = 0;

	testplayer.target.x = 0.0f;
	testplayer.target.y = 0.0f;
	testplayer.target.z = 3.0f;






	screen RenScreen;
	RenScreen.HEIGHT = HEIGHT;
	RenScreen.WIDTH = WIDTH;

	RenderResources RenResHandles;
	loadRenderResources(RenResHandles, RenScreen);
	RenResHandles.BasicProgram = m_progMesh;
	//RenResHandles.ShadowProgram = m_progShadow;
	RenResHandles.TexColorUniform = s_texColor;
	//RenResHandles.m_progssaoblurmerge = m_progssaoblurmerge;

	while (!quit)
	{
		lastUpdate = current;
		current = SDL_GetTicks();
		float dT = (current - lastUpdate) / 1000.0f;

		animation_index += dT;
		if (animation_index > 1)
		{
			animation_index = 0;
		}

		camera_heading.x = bx::cos(z_rot);
		camera_heading.y = bx::sin(z_rot);

		const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
		const bx::Vec3 eye = { 0.0f, camera_distance, camera_height };


		const bx::Vec3 up = { 0.0f, 0.0f, 1.0f };

		// Set view and projection matrix for view 0.
		//bx::Handness::Right
		float view[16];
		bx::mtxLookAt(view, eye, at, up, bx::Handness::Right);

		float r_mtx[16];
		bx::mtxRotateZ(r_mtx, z_rot);

		float t_mtx[16];
		bx::mtxTranslate(t_mtx, x_trans, y_trans, 0);

		float new_view[16];
		bx::mtxMul(new_view, r_mtx, view);
		bx::mtxMul(new_view, t_mtx, new_view);

		float proj[16];
		float proj2[16];
		bx::mtxProj(proj,
			30.0f,
			float(WIDTH) / float(HEIGHT),
			0.1f, 1000.0f,
			bgfx::getCaps()->homogeneousDepth, bx::Handness::Right);



		bx::mtxProj(proj2,
			30.0f,
			float(WIDTH) / float(HEIGHT),
			0.1f, 1000.0f,
			false);



		

		for (size_t i = 0; i < 16; i++)
		{
			RenResHandles.m_proj[i] = proj[i];
			RenResHandles.m_proj2[i] = proj2[i];
			RenResHandles.m_view[i] = new_view[i];
		}





		while (SDL_PollEvent(&currentEvent) != 0) // only use this to update flags or switch to push events?
		{

			if (currentEvent.type == SDL_QUIT) {
				quit = true;
			}

			if (currentEvent.type == SDL_MOUSEWHEEL)
			{
				if (currentEvent.wheel.y > 0) // scroll up
				{
					camera_distance--;
					camera_height -= 2;
				}
				else if (currentEvent.wheel.y < 0) // scroll down
				{
					camera_distance++;
					camera_height += 2;
				}
			}
		}






		int mx, my;
		if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{

			//shouldnt put all this picking code in here, just return the coords and a flag?

			SDL_Log("Mouse Button 1 (left) is pressed.");
			float viewProj[16];
			bx::mtxMul(viewProj, new_view, proj);

			float invViewProj[16];
			bx::mtxInverse(invViewProj, viewProj);

			float mouseXNDC = (mx / (float)WIDTH) * 2.0f - 1.0f;
			float mouseYNDC = ((HEIGHT - my) / (float)HEIGHT) * 2.0f - 1.0f;

			const bx::Vec3 pickEye = bx::mulH({ mouseXNDC, mouseYNDC, 0.0f }, invViewProj);
			const bx::Vec3 pickAt = bx::mulH({ mouseXNDC, mouseYNDC, 1.0f }, invViewProj);

			bx::Vec3 dist = bx::sub(pickEye, pickAt);
			float d_x = dist.x / dist.z;
			float d_y = dist.y / dist.z;

			float pos_x = (d_x * -pickEye.z) + pickEye.x;
			float pos_y = (d_y * -pickEye.z) + pickEye.y;

		}


		if (state[SDL_SCANCODE_A])
		{
			y_trans += camera_heading.y;
			x_trans += camera_heading.x;
		}
		if (state[SDL_SCANCODE_D])
		{
			y_trans -= camera_heading.y;
			x_trans -= camera_heading.x;
		}

		if (state[SDL_SCANCODE_W])
		{
			y_trans -= camera_heading.x;
			x_trans += camera_heading.y;
		}
		if (state[SDL_SCANCODE_S])
		{
			y_trans += camera_heading.x;
			x_trans -= camera_heading.y;
		}

		if (state[SDL_SCANCODE_Q])
		{
			z_rot += 0.03;
		}
		if (state[SDL_SCANCODE_E])
		{
			z_rot -= 0.03;
		}

		if (state[SDL_SCANCODE_I])
		{
			animation_index += 0.1;
			if (animation_index > 1)
			{
				animation_index = 0;
			}
		}



		camera RenCam;

		RenCam.proj = proj;
		RenCam.proj2 = proj2;
		RenCam.view = new_view;

		float testmat[16] = {2.79904, 0.00, 0.00, 0.00,
			0.00, 3.73205, 0.00, 0.00,
			0.00, 0.00, 1.0001, 1.00,
			0.00, 0.00, -0.10001, 0.00};



		float vec[4] = { 0.6, 0.7, 60.0, 1.0 };
		float result[4];
		bx::vec4MulMtx(result,vec, testmat);



		renderFrame(RenCam, RenScreen, staticRenderObjs, RenResHandles);



	}

	bgfx::shutdown();
	// Free up window
	SDL_DestroyWindow(window);
	// Shutdown SDL
	SDL_Quit();

	return 0;
}