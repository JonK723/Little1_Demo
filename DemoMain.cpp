/**************************
 * Includes
 *
 **************************/

#include "StarScene.h"
#include "FountainScene.h"
#include "LogoScene.h"
#include "ScreenWriter.h"
#include "FontWriter.h"
#include "ObjModel.h"
#include "stdafx.h"



#define DIST 3.5


/**************************
 * Function Declarations
 *
 **************************/
LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool CreateGLWindow(const char *title, int width, int height, int bits);
void DestroyGLWindow();

HGLRC hRC = NULL;
HDC hDC = NULL;
HWND hWND = NULL;
HINSTANCE hInstance = NULL;

int fullW = 0; 
int fullH = 0;

bool fullscreen = true; // is the window fullscreen 
bool active = true;
bool started = false;

double tStart, tFinish, tFTime, fps;	// the start time for the timer, finish time, and frame rate
__int64 startclock;  // we want a start time
double rate_inv;


bool done = false;
unsigned int titlepic;
unsigned int presentpic;
unsigned int littlepic;
static float logoAlpha = 0.0;
static float logoAlpha2 = 0.0;

corona::Image *titleImg = NULL;
corona::Image *littleImg = NULL;
FSOUND_STREAM *soundStr = NULL;
FSOUND_DSPUNIT *myunit = NULL;
float *spectrum = NULL;
int soundChannel = 0;

ObjModel *logoModel = NULL;


string errorString = "Initialization Error";


/*
Lights for the logo scene
*/
float lpos1[] = {0.0, 0.0, -2.0};
float ambientLight[] = {0.50, 0.50, 1.0, 0.0};



BOOL initftime(void) {
  __int64 rate;

  // we need the accuracy
  if(!QueryPerformanceFrequency((LARGE_INTEGER*)&rate)) {
    return FALSE; // win errors
  }

  // usually the rate will be 1193180
  if(!rate) {
    return FALSE;
  }

  rate_inv=1.0/(double)rate;

  if(!QueryPerformanceCounter((LARGE_INTEGER*)&startclock)) {
    return FALSE; // win errors
  }

  return TRUE; // there is a clock
}

 
double getFTime(void) {
  // by dividing by its rate you get accurate seconds

  __int64 endclock;

  QueryPerformanceCounter((LARGE_INTEGER*)&endclock);

  return (double)(endclock-startclock)*rate_inv;
}


bool Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glPolygonMode(GL_BACK, GL_FILL);
	glPolygonMode(GL_FRONT, GL_FILL);
	glClearStencil(0);


	glLightfv(GL_LIGHT0, GL_POSITION, lpos1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	//Initialize the music
	FSOUND_Init(44100, 32, 0);
	
	if(!(soundStr = FSOUND_Stream_Open("data/little1.mp3", FSOUND_NONBLOCKING | FSOUND_LOOP_OFF, 0, 0)))
	{
		errorString = "Error loading sounds";
		return false;
	}
/*
	myunit = FSOUND_DSP_GetFFTUnit();
	if(myunit == NULL)
	{
		return false;
	}
	else
	{
		FSOUND_DSP_SetActive(myunit, true);
	}
*/
	titleImg = corona::OpenImage("data/acc1.jpg");
	if(!titleImg)
	{
		errorString = "Error opening image data/acc-cool2.jpg";
		return false;
	}

	glGenTextures(1, &titlepic);
	glBindTexture(GL_TEXTURE_2D, titlepic);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, titleImg->getWidth(), titleImg->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, titleImg->getPixels());

	delete titleImg;
	titleImg = NULL;

	littleImg = corona::OpenImage("data/littleone.jpg");
	if(!littleImg)
	{
		errorString = "Error opening image data/littleone.jpg";
		return false;
	}

	glGenTextures(1, &littlepic);
	glBindTexture(GL_TEXTURE_2D, littlepic);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, littleImg->getWidth(), littleImg->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, littleImg->getPixels());

	delete littleImg;
	littleImg = NULL;

	titleImg = corona::OpenImage("data/acc-proudpresent.jpg");
	if(!titleImg)
	{
		errorString = "Error opening image data/acc-proudpresent.jpg";
		return false;
	}
	glGenTextures(1, &presentpic);
	glBindTexture(GL_TEXTURE_2D, presentpic);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, titleImg->getWidth(), titleImg->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, titleImg->getPixels());
	delete titleImg;


	if(!SceneInit())
	{
		errorString = "Error initializing Scene 1";
		return false;
	}
	if(!FountainSceneInit())
	{
		errorString = "Error initalizing Scene 2";
		return false;
	}
	if(!InitLogoScene())
	{
		errorString = "Error initializing Scene 3";
		return false;
	}

/*
	if(!WriterInit())
		return false;
*/

	if(!initftime())
	{
		errorString = "Error initializing timer";
		return false;
	}

	soundChannel = FSOUND_Stream_Play(FSOUND_FREE, soundStr);

	return true;
}

void ReshapeWindow(int w, int h)
{
	if(h==0)
		h = 1;

	float ratio = 1.0f*w/h;
	
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, ratio, 0.5, 1000.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void DrawLetterBox()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);

	beginOrtho(800, 600);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 0, 0);
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);	


	glColor4f(0.0,0.0,0.0,1.0);
	glTranslatef(0.0,0.0,1.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2i(0,140);
	glVertex2i(0,0);
	glVertex2i(805,0);
	glVertex2i(805,140);
	glEnd();
	
	glBegin(GL_TRIANGLE_FAN);
	glVertex2i(0,605);
	glVertex2i(0,460);
	glVertex2i(805,460);
	glVertex2i(805,605);
	glEnd();
	
	glTranslatef(0.0,0.0,1.0);
	
	glDisable(GL_STENCIL_TEST);

	endOrtho();

	glDisable(GL_DEPTH_TEST);
}


void DrawLogo(float t)
{
	static float tmpAngle = 0.0;
	float seconds = fabs((t-tStart))*1000.0f;

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE);
	glEnable(GL_TEXTURE_2D);

	beginOrtho(800, 600);

	if((seconds > 0.0001) && (seconds <= 21000.0001))
	{
		
		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glBindTexture(GL_TEXTURE_2D, titlepic);
		glPushMatrix();
		glTranslatef(1.0,1.0,1.0);

		glBegin(GL_TRIANGLE_FAN);
		glColor4f(1.0,1.0,1.0,logoAlpha);	
		glTexCoord2f(0.0, 1.0);	glVertex3f(150.0,220.0,0.0);
		glTexCoord2f(0.0, 0.0);	glVertex3f(150.0,370.0,0.0);
		glTexCoord2f(1.0, 0.0);	glVertex3f(650.0,370.0,0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(650.0,220.0,0.0);
		glEnd();
		glPopMatrix();

		if((seconds >= 8000.0001) && (seconds <= 11000.0001))
		{
			logoAlpha += (t*1000.0)/6000.00;
		}
		else if(seconds >= 19000.0001)
		{
			logoAlpha -=  (t*1000.0)/2000.00;
		}
	}
	else if((seconds >= 28000.0001) && (seconds <= 41000.0001))
	{
		glBindTexture(GL_TEXTURE_2D, presentpic);
	
		glPushMatrix();
		glTranslatef(1.0,-1.0,1.0);

		glBegin(GL_TRIANGLE_FAN);
		glColor4f(1.0,1.0,1.0,logoAlpha2);	
		glTexCoord2f(0.0, 1.0);	glVertex3f(150.0,220.0,0.0);
		glTexCoord2f(0.0, 0.0);	glVertex3f(150.0,370.0,0.0);
		glTexCoord2f(1.0, 0.0);	glVertex3f(650.0,370.0,0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(650.0,220.0,0.0);
		glEnd();
		glPopMatrix();

		if((seconds >= 29000.0001) && (seconds <= 34000.0001))
		{
			logoAlpha2 += (t*1000.0)/6000.00;
		}
		else if(seconds >= 37000.0001)
		{
			logoAlpha2 -=  (t*1000.0)/5000.00;
		}
	}
	else if((seconds >= 109005.001) && (seconds <= 115000.001))
	{	
		glBindTexture(GL_TEXTURE_2D, littlepic);
		glPushMatrix();
		glTranslatef(1.0,1.0,1.0);

		glBegin(GL_TRIANGLE_FAN);
		glColor3f(1.0,1.0,1.0);	
		glTexCoord2f(0.0, 1.0);	glVertex3f(150.0,220.0,0.0);
		glTexCoord2f(0.0, 0.0);	glVertex3f(150.0,370.0,0.0);
		glTexCoord2f(1.0, 0.0);	glVertex3f(650.0,370.0,0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(650.0,220.0,0.0);
		glEnd();
		glPopMatrix();
	}
		
	endOrtho();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE);
	glDisable(GL_TEXTURE_2D);
}


void DrawEQ(float t)
{
	float seconds = fabs((t-tStart))*1000.0;

	if(!spectrum)
	{
		return;
	}

	if(seconds >= 24000.0001)
	{
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glLineWidth(1.5f);

		glPushMatrix();
		glTranslatef(-10.5, 4.0, -10.0);
		for(int n = 0; n < 32; n++)
		{
			glPushMatrix();
			glTranslatef(0.0, -(float)n/4.0, 0.0);
			for(float x = 0.00; x < spectrum[n]; x++)
			{
				glPushMatrix();
				glTranslatef(x/4.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glColor3f(1.0, 0.0, 0.0);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(spectrum[n]+0.05, 0.0, 0.0);
				glEnd();
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
}

// simple draw routine
void Draw()
{
	if(!started)
	{
		tStart = getFTime();
		started = true;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	// All rendering functions go here. Functions are responsible for determining if they should execute their internals
	{
	DrawLogo(tFTime);

	DrawStarField(tFTime);	

	DrawFountain(tFTime);

	DrawLogoScene(tFTime);

	DrawLetterBox();
	}
	glFlush();
}


/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{    
    MSG msg;
    BOOL bQuit = FALSE;
  
    if(!CreateGLWindow("Little One - Accession", 1024, 768, 32))
    {
        return 0;                      
    } 
    
    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
			if(active)
			{
				do{
					tFinish = getFTime();
				}while(tFinish == tStart);	         

				tFTime = tFinish-tStart;
				tStart = tFinish; 

				fps = 1.0/tFTime;
			
				Draw();

				if(!FSOUND_IsPlaying(soundChannel))
				{
					bQuit = TRUE;
				}

			    SwapBuffers(hDC);
				FSOUND_Update();
			}
        } 
   }
   
    /* destroy the window explicitly */
    DestroyGLWindow ();

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/
LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ACTIVATE:
         if(!HIWORD(wParam))
         {
           active = true;                   
         }
         else
         {
           active = false;    
         }
         return 0;
    case WM_SYSCOMMAND:
         switch(wParam)
         {
         case SC_SCREENSAVE:
         case SC_MONITORPOWER:
              return 0;
         }
         break;
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;
    case WM_DESTROY:
        return 0;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
		case VK_SPACE:
			angle[2] -= 360.0*(tFTime*1000)/(1000);
			return 0;
        }
        return 0;
    case WM_SIZE:
         ReshapeWindow(LOWORD(lParam),HIWORD(lParam));
         return 0;
	default:
		break;    
    }

	return DefWindowProc (hWnd, message, wParam, lParam);
}


bool CreateGLWindow(const char *title, int width, int height, int bits)
{
    int iFormat;
    WNDCLASS wc;
    DWORD dwExStyle;
    DWORD dwStyle;
    RECT WindowRect;
    
    WindowRect.left = (long)0;
    WindowRect.bottom = (long)height;
    WindowRect.right = (long)width;
    WindowRect.top = (long)0;
    
    hInstance = GetModuleHandle(NULL);
    
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0; 
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "OpenGL";
    
    if(!RegisterClass(&wc))
    {
       MessageBoxA(NULL, "Failed to register class", "Initialization Error", MB_OK | MB_ICONEXCLAMATION);
       return false;          
    }

   DEVMODE dmSettings;
   
   memset(&dmSettings, 0, sizeof(dmSettings));
   
   dmSettings.dmSize = sizeof(dmSettings);
   dmSettings.dmPelsWidth = width;
   dmSettings.dmPelsHeight = height;
   dmSettings.dmBitsPerPel = bits;
   dmSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

   fullW = width;
   fullH = height;
   
   if(ChangeDisplaySettings(&dmSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
   {
      MessageBoxA(NULL, "The requested video mode is not supported, closing the demo", "Exiting Demo", MB_OK | MB_ICONSTOP);
      return false;    
   }              
    
   dwExStyle = WS_EX_APPWINDOW;
   dwStyle = WS_POPUP;
   ShowCursor(false);
    
    AdjustWindowRectEx(&WindowRect, dwStyle,false,dwExStyle);
   

    if(!(hWND=CreateWindowEx(dwExStyle,(LPCTSTR)"OpenGL","Accession",WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle, 0, 0, WindowRect.right-WindowRect.left,
         WindowRect.bottom-WindowRect.top, NULL,NULL,hInstance,NULL)))
    {
         DestroyGLWindow();
         MessageBoxA(NULL, "Could not create window", "Window Creation Error", MB_OK | MB_ICONEXCLAMATION);
         return false;
    }
    
    /* set the pixel format for the DC */
    static PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
    1, // Version Number
    PFD_DRAW_TO_WINDOW | // Format Must Support Window
    PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
    PFD_DOUBLEBUFFER, // Must Support Double Buffering
    PFD_TYPE_RGBA, // Request An RGBA Format
    bits, // Select Our Color Depth
    0, 0, 0, 0, 0, 0, // Color Bits Ignored
    0, // No Alpha Buffer
    0, // Shift Bit Ignored
    0, // No Accumulation Buffer
    0, 0, 0, 0, // Accumulation Bits Ignored
    16, // 16Bit Z-Buffer (Depth Buffer)
    8, // No Stencil Buffer
    0, // No Auxiliary Buffer
    PFD_MAIN_PLANE, // Main Drawing Layer
    0, // Reserved
    0, 0, 0 // Layer Masks Ignored
    };

    /* get the device context (DC) */
    hDC = GetDC (hWND);
    if(hDC == NULL)
    {
       DestroyGLWindow();
       MessageBoxA(NULL, "Cannot create GL Device Context", "Error", MB_OK | MB_ICONEXCLAMATION);
       return false;       
    }
    
    if(!(iFormat=ChoosePixelFormat(hDC, &pfd)))
    {
       DestroyGLWindow();
       MessageBoxA(NULL, "Could not find the requested pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
       return false;                                    
    }
    
    if(!SetPixelFormat(hDC, iFormat, &pfd))
    {
       DestroyGLWindow();
       MessageBoxA(NULL, "Could not set up the requested pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
       return false;                           
    }
    
    /* create and enable the render context (RC) */
    if(!(hRC=wglCreateContext(hDC)))
    {
       DestroyGLWindow();
       MessageBoxA(NULL, "Unable to create device context", "Error", MB_OK|MB_ICONEXCLAMATION);
       return false;                          
    }
    
    if(!wglMakeCurrent(hDC, hRC ))
    {
      DestroyGLWindow();
      MessageBoxA(NULL, "Unable to make device context current", "Error", MB_OK|MB_ICONEXCLAMATION);
      return false;                    
    }
    
    ShowWindow(hWND, SW_SHOW);
    SetForegroundWindow(hWND);
    SetFocus(hWND);
    ReshapeWindow(width, height);

    if(!Initialize())
    {
       DestroyGLWindow();
	   MessageBoxA(NULL, errorString.c_str(), "Error", MB_OK | MB_ICONEXCLAMATION);
       return false;                 
    }
    
    return true;
}


/*
Destroy the opengl window
*/
void DestroyGLWindow()
{
    ChangeDisplaySettings(NULL, 0);
    ShowCursor(true);
     
     if(hRC)
     {
        if(!wglMakeCurrent(NULL, NULL))
        {
           MessageBoxA(NULL, "Could not release DC and RC", "Shutdown Error", MB_OK | MB_ICONINFORMATION);
        }
        
        if(!wglDeleteContext(hRC))
        {
           MessageBoxA(NULL, "Could not delete RC", "Shutdown Error", MB_OK | MB_ICONINFORMATION);                          
        }
        hRC = NULL;
     }
     
     if(hDC && !ReleaseDC(hWND, hDC))
     {
        MessageBoxA(NULL, "Unable to release DC", "Shutdown error", MB_OK | MB_ICONINFORMATION);
        hDC = NULL;
     }
     
     if(hWND && !DestroyWindow(hWND))
     {
        MessageBoxA(NULL, "Unable to destroy HWND", "Shutdown error", MB_OK | MB_ICONINFORMATION);
        hWND = NULL;
     }
     
     if(!UnregisterClass("OpenGL", hInstance))
     {
        MessageBoxA(NULL, "Unable to unregister OpenGL class", "Shutdown Error", MB_OK | MB_ICONINFORMATION);
        hInstance = NULL;
     }
} 

