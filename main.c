#include <stdio.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "nanovg/nanovg.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg/nanovg_gl.h"
#include "draw.h"
#include "view.h"
//#include "perf.h"

int    mouse_down;
double drag_x;
double drag_y;
double current_x;
double current_y;
int    current_mod;

static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    printf("Key event...\n");
}

static void mouse_event(GLFWwindow *window, int button, int action, int mod)
{
    if(action == GLFW_PRESS) {
        if(!mouse_down) {
            drag_x = current_x;
            drag_y = current_y;
        }
        mouse_down |= (1<<button);
    } else {
        mouse_down &= ~(1<<button);
    }

    current_mod = mod;

    printf("Mouse Event[%x](%d, %d, %d)...\n", mouse_down, button, action, mod);
}

static void mouse_hover(GLFWwindow *window, double x, double y)
{
    if(mouse_down) {
        //printf("Drag[%x,%d] (%f,%f)->(%f,%f) [%f,%f]\n", mouse_down, current_mod,
        //        drag_x, drag_y, x, y, x-current_x, y-current_y);
    }


    current_x = x;
    current_y = y;
    //printf("Mouse hover(%f, %f)\n",x,y);
}


int main()
{
    GLFWwindow* window;
    NVGcontext *vg;

    if(!glfwInit()) {
        fprintf(stderr, "Failed to start glfw\n");
        return -1;
    }


    //Set OpenGL Revision
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //Create Window
    window = glfwCreateWindow(1000, 600, "NanoVG", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to make window\n");
        glfwTerminate();
        return -1;
    }


    //Setup Context
    glfwSetKeyCallback(window, key);
    glfwSetMouseButtonCallback(window, mouse_event);
    glfwSetCursorPosCallback(window, mouse_hover);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to start GLEW\n");
        return -1;
    }

    vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    nvgCreateFont(vg, "sans", "Roboto-Regular.ttf");
    nvgCreateFont(vg, "icons", "entypo.ttf");

    glfwSwapInterval(0);

	glfwSetTime(0);
	long prevt = glfwGetTime();


	while (!glfwWindowShouldClose(window))
	{
		double mx, my;
		int winWidth, winHeight;
		int fbWidth, fbHeight;
		float pxRatio;

		glfwGetCursorPos(window, &mx, &my);
		glfwGetWindowSize(window, &winWidth, &winHeight);
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		// Calculate pixel ration for hi-dpi devices.
		pxRatio = (float)fbWidth / (float)winWidth;

		// Update and render
		glViewport(0, 0, fbWidth, fbHeight);
        //glClearColor(0x06/255.0, 0x27/255.0, 0x37/255.0, 1.0f);
        glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

//#define M_PI 3.14159
        nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
        //float in[100] = {0.0, 0.0, 0.5, 0.2, 0.3, 0.7,0.9,0.8,0.0,1.0};
        //drawHarmonicPlot(vg, in, 100, 0,0,winWidth, winHeight);
        drawPowButton(vg, 0,0,winWidth, winHeight);

	//nvgBeginPath(vg);
    //float cx = 30.0, cy = 30.0, h = 80;
    //nvgArc(vg, cx, cy, 0.4*h, 0, 1.0/2.0*M_PI, 1);
    //nvgArc(vg, cx, cy, 0.2*h, 1.0/2.0*M_PI, 0, 2);
    //nvgClosePath(vg);
	//nvgFillColor(vg, nvgRGBA(0x11,0x45,0x75,255));
	//nvgFill(vg);
    nvgEndFrame(vg);
//		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
//
//        //viewReverb(vg, 0,0, winWidth, winHeight);
//        viewModule(vg, "afilter", 0,0,winWidth, winHeight);
//        //viewLFO(vg, 0,0, winWidth, winHeight);
//        //viewFilterEnv(vg, 0,0, winWidth, winHeight);
//                       //y   x    y   x     y    x    y   x   y   x
//        //float in[10] = {0.0, 0.0, 0.5, 0.2, 0.3, 0.7,-0.9,0.8,0.0,1.0};
//        //drawEnvEdit(vg, in, 5, 3, 0, 0, winWidth, winHeight);
//
//#if 0
//        dial_t dial = {60, 0, 0, 100, "label"};
//        renderDial(vg, dial);
//        drawButton(vg, "banana", 100, 25, 100, 50);
//        drawOptButton(vg, "opt", 200, 25, 100, 50);
//        drawButtonGrid(vg, 4,4, 300, 0, 100, 100);
//        drawAltDial(vg, 400, 0, 100, 100);
//        drawGrid(vg, 8, 8, 500, 0, 200, 200);
//        drawSin(vg,  500, 0, 200, 200);
//#endif
//
//		nvgEndFrame(vg);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	nvgDeleteGL2(vg);
	glfwTerminate();
	return 0;
}
