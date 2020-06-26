#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

typedef float point3[3];
constexpr auto M_PI = 3.14159265358979323846;;

// Funkcja rysuj¹ca osie uk³adu wspó³rzêdnych
void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz¹tek i koniec obrazu osi y
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

// Transformacja o 60 stopni wyœwietlanej grafiki
void Transformation()
{
	// Mo¿liwe parametry: GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE
	glMatrixMode(GL_MODELVIEW);

	// Wykonanie translacji
	//glTranslated(TYP x, TYPE y, TYPE z);

	// Wykonanie skalowania
	//glScaled(TYPE x, TYPE y, TYPE z);

	// Wykonuje obrót o k¹t angle wokó³ osi oznaczonej przez punkt (0,0,0) i punkt (1, 1, 1) - OBRÓT O 60 STOPNI
	glRotated(60.0, 1.0, 1.0, 1.0);
}

// Funkcja rysuj¹ca jajko
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

// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana gdy trzeba przerysowaæ scenê)
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	// Czyszczenie macierzy bie¿¹cej
	glLoadIdentity();

	// Narysowanie osi przy pomocy funkcji zdefiniowanej wy¿ej
	//Axes();

	// Ustawienie koloru rysowania na bia³y
	glColor3f(1.0f, 1.0f, 1.0f);

	// Transformacja wyœwietlanego obiektu
	//Transformation();

	// Narysowanie obrazu czajnika do herbaty
	//glutWireTeapot(3.0);

	// Narysowanie jajka
	Egg();

	// Przekazanie poleceñ rysuj¹cych do wykonania
	glFlush();

	glutSwapBuffers();
}

// Funkcja ustalaj¹ca stan renderowania
void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc¹cy (wype³nienia okna) ustawiono na czarny
}

// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okreœlaj¹cej proporcjê
	// wymiarów okna 
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  
	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji 
	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej            
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspó³czynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okreœlenie tak zwanej
	// przestrzeni ograniczaj¹cej pozwalaj¹cej zachowaæ w³aœciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczj¹cej s³u¿y funkcja
	// glOrtho(...)            
	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else

		glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu                                   

	glLoadIdentity();
	// Czyszcenie macierzy bie¿¹cej
}

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Uk³ad wspó³rzêdnych 3-D");

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bedzie ona wywo³ywana za ka¿dym razem
	// gdy zajdzie potrzba przeryswania okna 
	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// zazmiany rozmiaru okna      
	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}