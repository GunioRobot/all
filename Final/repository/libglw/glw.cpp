#include <std/algo>
#include <std/assert>
#include <std/containers>

#include "glw.hpp"

#include "glfw3.h"
#include "GL3/gl3w.h"

using namespace glw;

////////////////////////

static GLFWwindow instance = 0;

static vector<Listener*> eListeners;
static vector<Listener*> kListeners;
static vector<Listener*> mListeners;
static vector<Listener*> wListeners;

////////////////////////

static void error_callback(int error, const char* msg) {
	for (size_t i = 0; i < eListeners.size(); ++i) eListeners[i]->onError(error, msg);
}
static void key_callback(void*, int key, int state) {
	for (size_t i = 0; i < kListeners.size(); ++i) kListeners[i]->onKey(key, state);
}
static void mouse_click_callback(void*, int index, int state) {
	for (size_t i = 0; i < mListeners.size(); ++i) mListeners[i]->onMouseClick(index, state);
}
static void mouse_move_callback(void*, int x, int y) {
	for (size_t i = 0; i < mListeners.size(); ++i) mListeners[i]->onMouseMove(x, y);
}
static void mouse_scroll_callback(void*, int, int dir) {
	for (size_t i = 0; i < mListeners.size(); ++i) mListeners[i]->onMouseScroll(dir);
}
static int window_close_callback(void*) {
	for (size_t i = 0; i < wListeners.size(); ++i) wListeners[i]->onWindowClose();
	return false;
}
static void window_resize_callback(void*, int width, int height) {
	for (size_t i = 0; i < wListeners.size(); ++i) wListeners[i]->onWindowResize(width, height);
}

////////////////////////

static void setDefaultCallbacks() {
	glfwSetErrorCallback(error_callback);

	glfwSetKeyCallback(key_callback);

	glfwSetMouseButtonCallback(mouse_click_callback);
	glfwSetMousePosCallback(mouse_move_callback);
	glfwSetScrollCallback(mouse_scroll_callback);

	glfwSetWindowCloseCallback(window_close_callback);
	glfwSetWindowSizeCallback(window_resize_callback);
}

////////////////////////

static void glfwCreateWindow(const char* title, uint width, uint height, bool fullscreen) {
	assert(instance == 0);
	assert(glfwInit() == true);

	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, 1);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	instance = glfwOpenWindow((int)width, (int)height, (fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOWED), title, 0);
	assert(instance != 0);
	glfwSwapInterval(0);
	setDefaultCallbacks();

	assert(glfwGetError() == false);
	assert(gl3wInit() == true);
	assert(gl3wIsSupported(3,3) == true);
	assert(glGetError() == false);
}

////////////////////////

InputManager::InputManager() {
	assert(glfwInit() == true);
}
InputManager::~InputManager() {
	eListeners.clear();
	kListeners.clear();
	mListeners.clear();
	wListeners.clear();
}

void InputManager::addErrorListener(Listener& listener) { eListeners.push_back(&listener); }
void InputManager::addKeyListener(Listener& listener) { kListeners.push_back(&listener); }
void InputManager::addMouseListener(Listener& listener) { mListeners.push_back(&listener); }
void InputManager::addWindowListener(Listener& listener) { wListeners.push_back(&listener); }

void InputManager::getMousePosition(int& x, int& y) const { assert(instance); glfwGetMousePos(instance, &x, &y); }
void InputManager::hideCursor(bool hide) {
	assert(instance);
	if (hide) glfwDisable(instance, GLFW_MOUSE_CURSOR);
	else glfwEnable(instance, GLFW_MOUSE_CURSOR);
}

void InputManager::removeErrorListener(Listener& listener) { eListeners.erase(std::remove(eListeners.begin(), eListeners.end(), &listener), eListeners.end()); }
void InputManager::removeKeyListener(Listener& listener) { kListeners.erase(std::remove(kListeners.begin(), kListeners.end(), &listener), kListeners.end()); }
void InputManager::removeMouseListener(Listener& listener) { mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), &listener), mListeners.end()); }
void InputManager::removeWindowListener(Listener& listener) { wListeners.erase(std::remove(wListeners.begin(), wListeners.end(), &listener), wListeners.end()); }

void InputManager::setMousePosition(int x, int y) { assert(instance); glfwSetMousePos(instance, x, y); }
void InputManager::update() { glfwPollEvents(); }

////////////////////////

Window::Window(const char* title, uint width, uint height, bool fullscreen) :
	m_title(title),
	m_fullscreen(fullscreen)
{
	glfwCreateWindow(title, width, height, fullscreen);
}

Window::~Window() {
	assert(instance != 0);
	glfwCloseWindow(instance);
	instance = 0;

	glfwTerminate();
}

////////////////////////

const char* Window::getTitle() const { return m_title; }
void Window::getWindowSize(uint& width, uint& height) const {
	int w, h;
	glfwGetWindowSize(instance, &w, &h);
	width = (uint) w;
	height = (uint) h;
}

bool Window::isFocused() const { return glfwGetWindowParam(instance, GLFW_ACTIVE); }
bool Window::isFullscreen() const { return m_fullscreen; }
bool Window::isMinimized() const { return glfwGetWindowParam(instance, GLFW_ICONIFIED); }

void Window::setTitle(const char* title) {
	m_title = title;
	glfwSetWindowTitle(instance, m_title);
}
void Window::setWindowSize(uint width, uint height) { glfwSetWindowSize(instance, width, height); }
void Window::swapBuffers() { glfwSwapBuffers(); }

////////////////////////

double getTime() { assert(glfwInit() == true); return glfwGetTime(); }
