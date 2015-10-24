#include "GPass.h"

void RenderEngine::GPass::init()
{
	auto &a = AssetLibrary::AssetManager::instance();
	auto &w = AssetLibrary::Window::instance();

	const char* gpasstexturenames[] = { "GPassAlbedo", "GPassPosition", "GPassNormal", "GPassSpecular" };
	const unsigned gpassdepths[] = { GL_RGB8, GL_RGB32F, GL_RGB32F, GL_RGBA32F };
	a.buildFBO("GPassFrameBuffer", w.getWidth(), w.getHeight(), 4, gpasstexturenames, gpassdepths, true);

	fbo = "GPassFrameBuffer";
}

void RenderEngine::GPass::prep()
{

}

void RenderEngine::GPass::post()
{

}
