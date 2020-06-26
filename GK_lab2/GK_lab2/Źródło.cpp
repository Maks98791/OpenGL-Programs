#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

typedef float point3[3];
constexpr auto M_PI = 3.14159265358979323846;;

// Funkcja rysuj�ca osie uk�adu wsp�rz�dnych
void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz�tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz�tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz�tek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}

// Transformacja o 60 stopni wy�wietlanej grafiki
void Transformation()
{
	// Mo�liwe parametry: GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE
	glMatrixMode(GL_MODELVIEW);

	// Wykonanie translacji
	//glTranslated(TYP x, TYPE y, TYPE z);

	// Wykonanie skalowania
	//glScaled(TYPE x, TYPE y, TYPE z);

	// Wykonuje obr�t o k�t angle wok� osi oznaczonej przez punkt (0,0,0) i punkt (1, 1, 1) - OBR�T O 60 STOPNI
	glRotated(60.0, 1.0, 1.0, 1.0);
}

// Funkcja rysuj�ca jajko
void Egg()
{
	int N = 100;
	float tab[100][100][3];
	float u, v;
	float x, y, z;
	float a = 0.0;

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			u = (float) i / 99.0;
			v = (float) j / 99.0;
			// x
			tab[i][j][0] = ((-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(v * M_PI));
			// y
			tab[i][j][1] = (160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2)) - 5;
			// z
			tab[i][j][2] = ((-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(v * M_PI));
		}
	}

	// Czerwony kolor
	glColor3f(1.0f, 0.0f, 0.0f);

	// Rysowanie jajka kropkami
	/*glBegin(GL_POINTS);
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			glVertex3f(tab[i][j][0], tab[i][j][1], tab[i][j][2]);
		}
	}
	glEnd();
	*/
	// Rysowanie jajka paskami
	glBegin(GL_LINES);
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if (j != 99)
			{
				glVertex3f(tab[i][j][0], tab[i][j][1], tab[i][j][2]);
				glVertex3f(tab[i][j + 1][0], tab[i][j + 1][1], tab[i][j + 1][2]);
				//glVertex3f(tab[i][j][0], tab[i][j][1], tab[i][j][2]);
				//glVertex3f(tab[j][i][0], tab[j][i][1], tab[j][i][2]);
			}
		}
	}
	glEnd();
}

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba przerysowa� scen�)
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	// Czyszczenie macierzy bie��cej
	glLoadIdentity();

	// Narysowanie osi przy pomocy funkcji zdefiniowanej wy�ej
	//Axes();

	// Ustawienie koloru rysowania na bia�y
	glColor3f(1.0f, 1.0f, 1.0f);

	// Transformacja wy�wietlanego obiektu
	//Transformation();

	// Narysowanie obrazu czajnika do herbaty
	//glutWireTeapot(3.0);

	// Narysowanie jajka
	Egg();

	// Przekazanie polece� rysuj�cych do wykonania
	glFlush();

	glutSwapBuffers();
}

// Funkcja ustalaj�ca stan renderowania
void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny
}

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s�
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okre�laj�cej proporcj�
	// wymiar�w okna 
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  
	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji 
	glLoadIdentity();
	// Czyszcznie macierzy bie��cej            
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp�czynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okre�lenie tak zwanej
	// przestrzeni ograniczaj�cej pozwalaj�cej zachowa� w�a�ciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczj�cej s�u�y funkcja
	// glOrtho(...)            
	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else

		glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu                                   

	glLoadIdentity();
	// Czyszcenie macierzy bie��cej
}

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Uk�ad wsp�rz�dnych 3-D");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  Bedzie ona wywo�ywana za ka�dym razem
	// gdy zajdzie potrzba przeryswania okna 
	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// zazmiany rozmiaru okna      
	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}