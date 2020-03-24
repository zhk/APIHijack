// Direct3D-Hook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "apihijack.h"
#include "GraphicsHook.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//-------------------------------------------------------------------------
// OpenGL
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")

#include <gl\gl.h>
#include <gl\glu.h>

// Function pointer types.
typedef void ( APIENTRY* glBegin_t )( GLenum );
typedef void ( APIENTRY* glEnd_t )( void );
typedef void ( APIENTRY* glClear_t )( GLbitfield );
typedef void ( APIENTRY* glVertex3fv_t )( const GLfloat* v );
typedef void ( APIENTRY* glVertex3f_t )( GLfloat x,  GLfloat y,  GLfloat z );
typedef void ( APIENTRY* glEnable_t )( GLenum );

// Function prototype.
void APIENTRY Hooked_glBegin( GLenum mode );
void APIENTRY Hooked_glEnd( void );
void APIENTRY Hooked_glVertex3fv( GLfloat* v );
void APIENTRY Hooked_glVertex3f ( GLfloat x,  GLfloat y,  GLfloat z );
void APIENTRY Hooked_glClear( GLbitfield mask );
void APIENTRY Hooked_glEnable (GLenum cap);

// Hook structure.
SDLLHook OpenGL_Hook = 
{
	"opengl32.dll",
	false, NULL,		// Default hook disabled, NULL function pointer.
	{
		{ "glBegin", Hooked_glBegin},
		{ "glEnd", Hooked_glEnd},
		{ "glVertex3fv", Hooked_glVertex3fv},
		{ "glVertex3f", Hooked_glVertex3f},
		{ "glClear", Hooked_glClear},
		{ "glEnable", Hooked_glEnable},
		{ NULL, NULL }
	}
};

// Table index
enum
{
	GLFN_glBegin = 0
	, GLFN_glEnd
	, GLFN_glVertex3fv
	, GLFN_glVertex3f
	, GLFN_glClear
	, GLFN_glEnable
};


bool wallhack = false;

void APIENTRY Hooked_glBegin( GLenum mode )
{
	static bool s_bFirstTime = true;
	if (s_bFirstTime)
	{
		MessageBox_Debug("First time, glBegin");
		s_bFirstTime = false;
	}
	 
	if (GetAsyncKeyState(VK_F1) & 1)
		wallhack = !wallhack;

	if (wallhack)
	{
		if(mode == GL_TRIANGLES || mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN)
			glDepthRange(0, 0.5);
		else
			glDepthRange(0.5, 1);
	}

	glBegin_t old_func = (glBegin_t) OpenGL_Hook.Functions[GLFN_glBegin].OrigFn;
	old_func(mode);
}

void APIENTRY Hooked_glEnd( void )
{
	glEnd_t old_func = (glEnd_t) OpenGL_Hook.Functions[GLFN_glEnd].OrigFn;
	old_func();
}

void APIENTRY Hooked_glVertex3fv( GLfloat* v )
{
	glVertex3fv_t old_func = (glVertex3fv_t) OpenGL_Hook.Functions[GLFN_glVertex3fv].OrigFn;
	old_func(v);
}

void APIENTRY Hooked_glVertex3f ( GLfloat x,  GLfloat y,  GLfloat z )
{
	glVertex3f_t old_func = (glVertex3f_t) OpenGL_Hook.Functions[GLFN_glVertex3f].OrigFn;
	old_func(x, y, z);
}

void APIENTRY Hooked_glClear( GLbitfield mask )
{
	glClear_t old_func = (glClear_t) OpenGL_Hook.Functions[GLFN_glClear].OrigFn;
	old_func(mask);
}

void DrawBox(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
	glColor3f(0.9f, 0.1f, 0.1f);

	glBegin(GL_QUADS);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x, y + height, z);
	glEnd();
}

void APIENTRY Hooked_glEnable (GLenum cap)
{
	if (GetAsyncKeyState(VK_F2))
		DrawBox(200, 100, 0, 50, 30);

	glEnable_t old_func = (glEnable_t) OpenGL_Hook.Functions[GLFN_glEnable].OrigFn;
	old_func(cap);
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

