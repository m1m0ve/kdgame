/*
Title: kontent
Copyright (c) 2004 Martin John Baker
Author: Imran Ahmed khan

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

For information about the GNU General Public License see http://www.gnu.org/

To discuss this program http://sourceforge.net/forum/forum.php?forum_id=422259
also see website http://www.euclideanspace.com/software/games/twod/

*/

#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <gl\glaux.h>	
#include <math.h>	

#define CAMERASPEED	0.03f	

HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;		
HINSTANCE	hInstance;		

bool	keys[256];			
bool	active=TRUE;		
bool	fullscreen=TRUE;	

typedef struct tVector3				
{			
	tVector3() {}	
	tVector3 (float new_x, float new_y, float new_z) 
	{x = new_x; y = new_y; z = new_z;}
	
	tVector3 operator+(tVector3 vVector)
	{return tVector3(vVector.x+x, vVector.y+y, vVector.z+z);}
	
	tVector3 operator-(tVector3 vVector) 
	{return tVector3(x-vVector.x, y-vVector.y, z-vVector.z);}
	
	tVector3 operator*(float number)	
	{return tVector3(x*number, y*number, z*number);}
	
	tVector3 operator/(float number)
	{return tVector3(x/number, y/number, z/number);}
	
	float x, y, z;						
}tVector3;

tVector3 mPos;	
tVector3 mView;		
tVector3 mUp;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void Position_Camera(float pos_x,  float pos_y,  float pos_z,
							  float view_x, float view_y, float view_z,
							  float up_x,   float up_y,   float up_z)
{
	mPos	= tVector3(pos_x,  pos_y,  pos_z ); // set position
	mView	= tVector3(view_x, view_y, view_z); // set view
	mUp		= tVector3(up_x,   up_y,   up_z  ); // set the up vector	
}


void Move_Camera(float speed)
{
	tVector3 vVector = mView - mPos;	
	
	
	mPos.x  = mPos.x  + vVector.x * speed;
	mPos.z  = mPos.z  + vVector.z * speed;
	mView.x = mView.x + vVector.x * speed;
	mView.z = mView.z + vVector.z * speed;
}

void Rotate_View(float speed)
{
	tVector3 vVector = mView - mPos;	

	mView.z = (float)(mPos.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
	mView.x = (float)(mPos.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
}


void Keyboard_Input()
{
	if((GetKeyState(VK_UP) & 0x80))
	{	
	Move_Camera( CAMERASPEED);
	}

	if((GetKeyState(VK_DOWN) & 0x80))
	{
		Move_Camera(-CAMERASPEED);
	}

	if((GetKeyState(VK_LEFT) & 0x80))
	{	
	Rotate_View(-CAMERASPEED);
	}

	if((GetKeyState(VK_RIGHT) & 0x80))
	{
	Rotate_View( CAMERASPEED);
	}
}


void Mouse_Move(int wndWidth, int wndHeight)
{
	POINT mousePos;	
	int mid_x = wndWidth  >> 1;	
	int mid_y = wndHeight >> 1;	
	float angle_y  = 0.0f;				
	float angle_z  = 0.0f;							
	
	GetCursorPos(&mousePos);
	


	SetCursorPos(mid_x, mid_y);	

	
	angle_y = (float)( (mid_x - mousePos.x) ) / 1000;		
	angle_z = (float)( (mid_y - mousePos.y) ) / 1000;

	
	mView.y += angle_z * 2;

	
	if((mView.y - mPos.y) > 8)  mView.y = mPos.y + 8;
	if((mView.y - mPos.y) <-8)  mView.y = mPos.y - 8;
	
	Rotate_View(-angle_y); 
}


GLvoid ReSizeGLScene(GLsizei width, GLsizei height)	
{

	glViewport(0,0,width,height);					

	glMatrixMode(GL_PROJECTION);					
	glLoadIdentity();								

	
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);			
	glLoadIdentity();					
}

int InitGL(GLvoid)										
{
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	Position_Camera(0, 2.5f, 5,	0, 2.5f, 0,   0, 1, 0);
	
	return TRUE;										
}



int DrawGLScene(GLvoid) 
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glLoadIdentity(); 

gluLookAt(mPos.x,  mPos.y,  mPos.z,	
			  mView.x, mView.y, mView.z,	
			  mUp.x,   mUp.y,   mUp.z);



glTranslatef(-1.5f,0.0f,-6.0f);
glBegin(GL_TRIANGLES); 

/* Front side with Red Color */

glColor3f(1.0f,0.0f,0.0f);
glVertex3f( 0.0f, 1.0f, 0.0f); 
glVertex3f(-1.0f,-1.0f, 1.0f);
glVertex3f( 1.0f,-1.0f, 1.0f);


/* Right side with Blue Color */

glColor3f(0.0f,0.0f,1.0f);

glVertex3f( 0.0f, 1.0f, 0.0f); 
glVertex3f( 1.0f,-1.0f, 1.0f); 
glVertex3f( 1.0f,-1.0f, -1.0f); 

/* Back side with Red Color */

glColor3f(1.0f,0.0f,0.0f); 
glVertex3f( 0.0f, 1.0f, 0.0f); 
glVertex3f( 1.0f,-1.0f, -1.0f); 
glVertex3f(-1.0f,-1.0f, -1.0f);

/* Left Side with Green Color */

glColor3f(0.0f,1.0f,0.0f); 
glVertex3f( 0.0f, 1.0f, 0.0f); 
glVertex3f(-1.0f,-1.0f,-1.0f);
glVertex3f(-1.0f,-1.0f, 1.0f); 
glEnd(); 					

	glLoadIdentity();	
	
	gluLookAt(mPos.x,  mPos.y,  mPos.z,	
			  mView.x, mView.y, mView.z,	
			  mUp.x,   mUp.y,   mUp.z);
	
	glTranslatef(1.5f,0.0f,-7.0f);						// Move Right 1.5 Units And Into The Screen 7.0
	glBegin(GL_QUADS);									// Draw A Quad
/* Top Side of the Cube with different shades of colors */ 

glColor3f(0.0f,1.0f,0.0f);
glVertex3f( 1.0f, 1.0f,-1.0f); // Top Right
glColor3f(1.0f,0.0f,0.0f);
glVertex3f(-1.0f, 1.0f,-1.0f); // Top Left 
glColor3f(0.0f,0.0f,1.0f);
glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left 
glColor3f(0.0f,1.0f,1.0f);
glVertex3f( 1.0f, 1.0f, 1.0f); // Bottom Right

/* Bottom Side of the Cube with different shades of colors */ 

glColor3f(1.0f,0.5f,0.0f); 
glVertex3f( 1.0f,-1.0f, 1.0f); // Top Right 
glColor3f(1.0f,0.0f,0.0f); 
glVertex3f(-1.0f,-1.0f, 1.0f); // Top Left 
glColor3f(0.0f,1.0f,0.0f); 
glVertex3f(-1.0f,-1.0f,-1.0f); // Bottom Left
glColor3f(0.0f,0.0f,1.0f); 
glVertex3f( 1.0f,-1.0f,-1.0f); // Bottom Right 

/* Front Side of the Cube with different shades of colors */ 

glColor3f(1.0f,0.0f,0.0f);
glVertex3f( 1.0f, 1.0f, 1.0f); // Top Right 
glColor3f(0.0f,1.0f,0.0f);
glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left 
glColor3f(0.0f,0.0f,1.0f);
glVertex3f(-1.0f,-1.0f, 1.0f); // Bottom Left 
glColor3f(1.0f,0.0f,1.0f);
glVertex3f( 1.0f,-1.0f, 1.0f); // Bottom Right

/* BackSide of the Cube with different shades of colors */ 

glColor3f(1.0f,1.0f,0.0f);
glVertex3f( 1.0f,-1.0f,-1.0f); // Top Right
glColor3f(0.0f,1.0f,0.0f);
glVertex3f(-1.0f,-1.0f,-1.0f); // Top Left 
glColor3f(0.0f,0.0f,1.0f);
glVertex3f(-1.0f, 1.0f,-1.0f); // Bottom Left 
glColor3f(0.0f,1.0f,1.0f);
glVertex3f( 1.0f, 1.0f,-1.0f); // Bottom Right

/* LeftSide of the Cube with different shades of colors */ 

glColor3f(0.0f,0.0f,1.0f); 
glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right 
glColor3f(0.0f,0.5f,1.0f); 
glVertex3f(-1.0f, 1.0f,-1.0f); // Top Left
glColor3f(1.0f,0.5f,0.0f); 
glVertex3f(-1.0f,-1.0f,-1.0f); // Bottom Left
glColor3f(0.0f,1.0f,1.0f); 
glVertex3f(-1.0f,-1.0f, 1.0f); // Bottom Right 

/* Right Side of the Cube with different shades of colors */ 

glColor3f(1.0f,0.0f,1.0f); 
glVertex3f( 1.0f, 1.0f,-1.0f); // Top Right 
glColor3f(0.0f,0.0f,1.0f); 
glVertex3f( 1.0f, 1.0f, 1.0f); // Top Left 
glColor3f(0.0f,1.0f,1.0f); 
glVertex3f( 1.0f,-1.0f, 1.0f); // Bottom Left 
glColor3f(1.0f,0.5f,1.0f); 
glVertex3f( 1.0f,-1.0f,-1.0f); // Bottom Right 

glEnd();
return TRUE; 
}




GLvoid KillGLWindow(GLvoid)								
{
	if (fullscreen)										
	{
		ChangeDisplaySettings(NULL,0);					
		ShowCursor(TRUE);								
	}

	if (hRC)											
	{
		if (!wglMakeCurrent(NULL,NULL))					
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))	
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;					
	}

	if (hDC && !ReleaseDC(hWnd,hDC))
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;					
	}

	if (hWnd && !DestroyWindow(hWnd))	
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										
	}

	if (!UnregisterClass("OpenGL",hInstance))			
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									
	}
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				
	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		

	fullscreen=fullscreenflag;			

	hInstance			= GetModuleHandle(NULL);	
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc		= (WNDPROC) WndProc;					
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;									
	wc.lpszClassName	= "OpenGL";								

	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
	
	if (fullscreen)												
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;				
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
		
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;
			}
			else
			{
				
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;							
			}
		}
	}

	if (fullscreen)										
	{
		dwExStyle=WS_EX_APPWINDOW;						
		dwStyle=WS_POPUP;								
		ShowCursor(FALSE);								
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	
		dwStyle=WS_OVERLAPPEDWINDOW;					
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	

	
	if (!(hWnd=CreateWindowEx(	dwExStyle,						
								"OpenGL",						
								title,							
								dwStyle |						
								WS_CLIPSIBLINGS |				
								WS_CLIPCHILDREN,				
								0, 0,							
								WindowRect.right-WindowRect.left,
								WindowRect.bottom-WindowRect.top,	
								NULL,								
								NULL,								
								hInstance,							
								NULL)))								
	{
		KillGLWindow();			
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;			
	}

	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,							
		PFD_DRAW_TO_WINDOW |		
		PFD_SUPPORT_OPENGL |		
		PFD_DOUBLEBUFFER,			
		PFD_TYPE_RGBA,				
		bits,						
		0, 0, 0, 0, 0, 0,							
		0,											
		0,											
		0,											
		0, 0, 0, 0,									
		16,											
		0,											
		0,											
		PFD_MAIN_PLANE,								
		0,											
		0, 0, 0										
	};
	
	if (!(hDC=GetDC(hWnd)))							
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);						
	SetForegroundWindow(hWnd);						
	SetFocus(hWnd);									
	ReSizeGLScene(width, height);					

	if (!InitGL())									
	{
		KillGLWindow();								
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;									
}


LRESULT CALLBACK WndProc(	HWND	hWnd,			
							UINT	uMsg,			
							WPARAM	wParam,			
							LPARAM	lParam)			
{
	switch (uMsg)									
	{
		case WM_ACTIVATE:							
		{
			if (!HIWORD(wParam))					
			{
				active=TRUE;						
			}
			else
			{
				active=FALSE;						
			}

			return 0;								
		}

		case WM_SYSCOMMAND:							
		{
			switch (wParam)							
			{
				case SC_SCREENSAVE:					
				case SC_MONITORPOWER:				
				return 0;							
			}
			break;									
		}

		case WM_CLOSE:								
		{
			PostQuitMessage(0);						
			return 0;								
		}

		case WM_KEYDOWN:							
		{
			keys[wParam] = TRUE;					
			return 0;								
		}

		case WM_KEYUP:								
		{
			keys[wParam] = FALSE;					
			return 0;								
		}

		case WM_SIZE:								
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam)); 
			return 0;								
		}
	}

	
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int WINAPI WinMain(	HINSTANCE	hInstance,			
					HINSTANCE	hPrevInstance,		
					LPSTR		lpCmdLine,			
					int			nCmdShow)			
{
	MSG		msg;									
	BOOL	done=FALSE;								

	
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							
	}

	
	if (!CreateGLWindow("Tutorial - OpenGl",640,480,16,fullscreen))
	{
		return 0;									
	}

	while(!done)									
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	
		{
			if (msg.message==WM_QUIT)				
			{
				done=TRUE;							
			}
			else									
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		else										
		{

            	Keyboard_Input();    
				Mouse_Move(640,480);
	
			if (active)					
			{
					DrawGLScene();					
					SwapBuffers(hDC);				
			}
		
		}
	}

	
	KillGLWindow();									
	return (msg.wParam);							
}

