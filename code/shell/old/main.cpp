/***********************************************
* Zeus CMD - GLUT Tutorial 02 : Keyboard Input *
* By Grant James (ZEUS)                        *
* http://www.zeuscmd.com                       *
***********************************************/
#include <iostream>
#include <GL/glut.h>

using namespace std;

bool ignoreRepeats = false;

bool init()
{
	return true;
}

void display()
{

}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	// Backspace
	case 8 :
		cout << "Pressed Backspace" << endl;
		break;

	// Enter
	case 13 :
		cout << "Pressed Enter" << endl;
		break;

	// Escape
	case 27 :
		cout << "Pressed Escape" << endl;
		break;

	// Delete
	case 127 :
		cout << "Pressed Delete" << endl;
		break;

	default : 
		cout << "Pressed key : " << (char)key 
			<< " at position : ("
			<< x << "," << y << ")" << endl;
		break;
	}

	int mod = glutGetModifiers();

	switch(mod)
	{
	case GLUT_ACTIVE_CTRL :
		cout << "Ctrl Held" << endl; break;
	case GLUT_ACTIVE_SHIFT :
		cout << "Shift Held" << endl; break;
	case GLUT_ACTIVE_ALT :
		cout << "Alt Held" << endl; break;
	}
}

void keyboardup(unsigned char key, int x, int y)
{
	if (key == 'a')
		cout << "a key lifted" << endl;
}

void special(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_F2 :
		cout << "Pressed F2" << endl;
		break;

	case GLUT_KEY_UP :
		cout << "Pressed Up" << endl;
		break;

	case GLUT_KEY_INSERT :
	{
		int modifiers = glutGetModifiers();

		if (
			modifiers == 
			(GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT))
		{
			cout << "Pressed Insert " <<
				"with only Ctrl and Alt" << endl;
		}
		else if (modifiers & GLUT_ACTIVE_CTRL &&
			modifiers & GLUT_ACTIVE_ALT)
		{
			cout << "Pressed Insert " <<
			"with Ctrl and Alt" << endl;
		}

		break;
	}

	case GLUT_KEY_F1 :
		ignoreRepeats = !ignoreRepeats;
		glutIgnoreKeyRepeat(ignoreRepeats);
		
		if (ignoreRepeats)
			cout << "Repeates disabled" << endl;
		else
			cout << "Repeats enabled" << endl;

		break;

	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitWindowPosition(200, 200);
	glutInitWindowSize(200, 200);

	glutCreateWindow("02 - Keyboard Input");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardup);
	glutSpecialFunc(special);

	if (!init())
		return 1;

	glutMainLoop();

	return 0;
}
