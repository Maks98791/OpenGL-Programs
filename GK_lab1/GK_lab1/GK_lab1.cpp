#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cstdlib>
#include <ctime>

byte RandColors()
{
	srand(time(NULL));

	return rand() % 127;
}

void DrawQuarter()
{
	srand(time(NULL));
	glColor3ub(rand() % 255, rand() % 255, rand() % 255);

	glBegin(GL_TRIANGLES);
		glVertex2f(-50.0f, 0.0f);
		glVertex2f(-50.0f, 50.0f);
		glVertex2f(0.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(-50.0f, 50.0f);
		glVertex2f(0.0f, 50.0f);
	glEnd();
}

void DrawTriangle()
{
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_TRIANGLES);
		glVertex2f(0.0f, -50.0f);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(50.0f, 0.0f);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLES);
		glVertex2f(-50.0f, 0.0f);
		glVertex2f(0.0f, -50.0f);
		glVertex2f(0.0f, 0.0f);
	glEnd();
}

void DrawTriangle3Colors()
{	
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.0f, -50.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(50.0f, 0.0f);
	glEnd();
}

void DistortedQuarter()
{
	srand(time(NULL));
	GLint add = rand() % 30;

	glColor3ub(rand() % 255, rand() % 255, rand() % 255);

	glBegin(GL_TRIANGLES);
	glVertex2i(-50 -add, 0);
	glVertex2i(-50 + add, 50);
	glVertex2i(0 + add, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2i(0 + add, 0);
	glVertex2i(-50 + add, 50);
	glVertex2i(0, 50 + add);
	glEnd();
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	//rysowanie kwadratu
	//DrawQuarter();

	//rysowanie trojkata (2 kolory)
	DrawTriangle();

	//rysowanie trojkata (3 kolory)
	//DrawTriangle3Colors();

	//rysowanie znieksztalconego kwadratu
	//DistortedQuarter();

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania
}

// Funkcja ustalaj�ca stan renderowania
void MyInit(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	// Kolor okna wn�trza okna - ustawiono na szary
}

// Funkcja s�u��ca do kontroli zachowania proporcji rysowanych obiekt�w
// niezale�nie od rozmiar�w okna graficznego
void ChangeSize(GLsizei horizontal, GLsizei vertical)
// Parametry horizontal i vertical (szeroko�� i wysoko�� okna) s�
// przekazywane do funkcji za ka�dym razem, gdy zmieni si� rozmiar okna
{
	GLfloat AspectRatio;

	// Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna
	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna urz�dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okre�lenie uk�adu wsp�rz�dnych obserwatora

	glLoadIdentity();
	// Okre�lenie przestrzeni ograniczaj�cej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp�czynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okre�lenie okna obserwatora.
	// Pozwala to zachowa� w�a�ciwe proporcje rysowanego obiektu
	// Do okre�lenia okna obserwatora s�u�y funkcja glOrtho(...)

	if (horizontal <= vertical)
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
	else
		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	// Okre�lenie uk�adu wsp�rz�dnych     

	glLoadIdentity();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wy�wietlania
	// GLUT_SINGLE - pojedynczy bufor wy�wietlania
	// GLUT_RGBA - model kolor�w RGB

	glutCreateWindow("Pierwszy program w OpenGL");
	// Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
	// Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
	// trzeba b�dzie przerysowa� okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst�pieniem do renderowania

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}