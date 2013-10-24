
#include "Globals.h"
#include "cGame.h"

//Delete console
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

cGame Game;
int inittime;

// Disable or enable the [X] button in top-right corne
BOOL EnableCloseButton(const HWND hwnd, const BOOL bState)
{
	HMENU	hMenu;
	UINT	dwExtra;

	if (hwnd == NULL) return FALSE;
	if ((hMenu = GetSystemMenu(hwnd, FALSE)) == NULL) return FALSE;
	dwExtra = bState ? MF_ENABLED : (MF_DISABLED | MF_GRAYED);
	return EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | dwExtra) != -1;
}

void AppRender()
{
	Game.Render();
}
void AppKeyboard(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key,true);
}
void AppKeyboardUp(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key,false);
}
void AppSpecialKeys(int key, int x, int y)
{
	Game.ReadSpecialKeyboard(key,true);
}
void AppSpecialKeysUp(int key, int x, int y)
{
	Game.ReadSpecialKeyboard(key,false);
}
void AppMouse(int button, int state, int x, int y)
{
	Game.ReadMouse(button,state,x,y);
}
void AppIdle()
{	
	int timeAct = glutGet(GLUT_ELAPSED_TIME);
	if(timeAct - inittime > (1000./FPS)) {
		inittime = timeAct;
		if(!Game.Loop()) Game.Finalize();
	}
}

void createWindow(bool fullScreen,int x,int y){
	if(fullScreen){
		char buffer[42];
		sprintf(buffer,"%dx%d:32",x,y);
		glutGameModeString(buffer);
		glutEnterGameMode();
	} else{
		glutInitWindowPosition(x,y);
		glutInitWindowSize(GAME_WIDTH,GAME_HEIGHT);
		glutCreateWindow("The Legend of Zelda");
		HWND windowHandle = FindWindow(NULL,"The Legend of Zelda");
		HICON hIcon = (HICON)LoadImage(NULL, "images\\/window_logo.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		SendMessage(windowHandle,WM_SETICON, ICON_BIG, (LPARAM)hIcon );
		EnableCloseButton(windowHandle, FALSE);
	}
}

void main(int argc, char** argv)
{
	bool fullScreen=false;
	int res_x,res_y,pos_x,pos_y;

	//GLUT initialization
	glutInit(&argc, argv);


	//RGBA with double buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);

	//Create centered window
	res_x = glutGet(GLUT_SCREEN_WIDTH);
	res_y = glutGet(GLUT_SCREEN_HEIGHT);
	pos_x = (res_x>>1)-(GAME_WIDTH>>1);
	pos_y = (res_y>>1)-(GAME_HEIGHT>>1);

	if(argc > 1){
		if(strcmp(argv[1],"fullScreen") == 0){
			fullScreen = true;
			createWindow(true,res_x,res_y);
		}
		else{
			createWindow(false,pos_x,pos_y);
		}
	}
	else{
		createWindow(false,pos_x,pos_y);
	}

	//Register callback functions
	glutDisplayFunc(AppRender);			
	glutKeyboardFunc(AppKeyboard);		
	glutKeyboardUpFunc(AppKeyboardUp);	
	glutSpecialFunc(AppSpecialKeys);	
	glutSpecialUpFunc(AppSpecialKeysUp);
	glutMouseFunc(AppMouse);
	glutIdleFunc(AppIdle);

	//Game initializations
	if (!Game.Init()){
		int msgboxID = MessageBox(
			NULL,
			"Error in the application",
			"Error",
			MB_OKCANCEL | MB_ICONERROR
		);
		switch (msgboxID)
		{
		case IDCANCEL:
			Game.Finalize();
			break;
		case IDCONTINUE:
			
			break;
		}
	}

	//Application loop
	inittime = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();
}
