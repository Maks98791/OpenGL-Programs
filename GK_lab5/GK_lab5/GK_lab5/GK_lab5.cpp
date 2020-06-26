#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>

 // Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie
 // FileName, alokuje pamiêæ i zwraca wskaŸnik (pBits) do bufora w którym
 // umieszczone s¹ dane.
 // Ponadto udostêpnia szerokoœæ (ImWidth), wysokoœæ (ImHeight) obrazu
 // tekstury oraz dane opisuj¹ce format obrazu wed³ug specyfikacji OpenGL
 // (ImComponents) i (ImFormat).
 // Jest to bardzo uproszczona wersja funkcji wczytuj¹cej dane z pliku TGA.
 // Dzia³a tylko dla obrazów wykorzystuj¹cych 8, 24, or 32 bitowy kolor.
 // Nie obs³uguje plików w formacie TGA kodowanych z kompresj¹ RLE.
GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat)
{
	// Struktura dla nag³ówka pliku  TGA     
	#pragma pack(1)
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;

	#pragma pack(8)
	FILE* pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte* pbitsperpixel = NULL;

	// Wartoœci domyœlne zwracane w przypadku b³êdu
	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	fopen_s(&pFile, FileName, "rb");
	if (pFile == NULL)
		return NULL;

	// Przeczytanie nag³ówka pliku 
	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

	// Odczytanie szerokoœci, wysokoœci i g³êbi obrazu
	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;

	// Sprawdzenie, czy g³êbia spe³nia za³o¿one warunki (8, 24, lub 32 bity)
	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	// Obliczenie rozmiaru bufora w pamiêci
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	// Alokacja pamiêci dla danych obrazu
	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}

	// Ustawienie formatu OpenGL
	switch (sDepth)
	{
	case 3:

		*ImFormat = GL_BGR_EXT;

		*ImComponents = GL_RGB8;

		break;

	case 4:

		*ImFormat = GL_BGRA_EXT;

		*ImComponents = GL_RGBA8;

		break;

	case 1:

		*ImFormat = GL_LUMINANCE;

		*ImComponents = GL_LUMINANCE8;

		break;
	};

	fclose(pFile);

	return pbitsperpixel;
}

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };    // inicjalizacja po³o¿enia obserwatora

static GLfloat theta = 0.0;    // k¹t obrotu obiektu
static GLfloat theta2 = 0.0;
static GLfloat pix2angle;      // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy 
							   // 0 - nie naciœniêto ¿adnego klawisza
							   // 1 - wciœniêty zosta³ lewy klawisz
							   // 2 - wciœniêty zosta³ prawy klawisz

static int x_pos_old = 0;      // poprzednia pozycja kursora myszy
static int y_pos_old = 0;

static int delta_x = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹ i poprzedni¹ kursora myszy
static int delta_y = 0;

void DrawTriangle()
{
	glBegin(GL_TRIANGLES);
		glNormal3f(0.0f, 0.0f, 3.0f);	//Wektory normalne z poprzednich zajeæ
		glTexCoord2f(0.0f, 0.0f);		//-->Funkcja okreœlaj¹ca, które wierzcho³ki 
		glVertex3f(0.0f, 4.0f, 3.0f);	//trójk¹ta naniesionego na wzorzec tekstury
		glNormal3f(0.0f, 0.0f, 3.0f);	//odpowiadaj¹ którym wierzcho³kom
		glTexCoord2f(3.0f, 0.0f);
		glVertex3f(-4.0f, 0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 3.0f);
		glTexCoord2f(1.5f, 2.0f);
		glVertex3f(4.0f, 0.0f, 0.0f);
	glEnd();
}

// Funkcja "bada" stan myszy i ustawia wartoœci odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;       // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		y_pos_old = y;

		status = 1;          // wciêniêty zosta³ lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		status = 2;
	}
	else
		status = 0;          // nie zosta³ wciêniêty ¿aden klawisz 
}

// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich zmiennych globalnych
void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy
	delta_y = y - y_pos_old;

	x_pos_old = x;               // podstawienie bie¿¹cego po³o¿enia jako poprzednie
	y_pos_old = y;

	glutPostRedisplay();         // przerysowanie obrazu sceny
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy biezacej

	// Zdefiniowanie po³o¿enia obserwatora
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if (status == 1)                     // jeœli lewy klawisz myszy wciêniêty
	{
		theta += delta_x * pix2angle;    // modyfikacja k¹ta obrotu o kat proporcjonalny
		theta2 += delta_y * pix2angle;	 // do ró¿nicy po³o¿eñ kursora myszy
		glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k¹t
		glRotatef(theta2, 1.0, 0.0, 0.0);
		//glScalef(theta * 0.05, theta2 * 0.05, theta * 0.05);
	}
	else if (status == 2)
	{
		theta += delta_x * pix2angle;    // modyfikacja k¹ta obrotu o kat proporcjonalny
		theta2 += delta_y * pix2angle;	 // do ró¿nicy po³o¿eñ kursora myszy
		glScalef(theta * 0.05, theta2 * 0.05, theta * 0.05);
	}

	// Narysowanie trójk¹ta
	DrawTriangle();

	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania
	glutSwapBuffers();
}

// Funkcja ustalaj¹ca stan renderowania
void MyInit(void)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// Zmienne dla obrazu tekstury

	GLbyte* pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;


	/*************************************************************************************/

	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga
	pBytes = LoadTGAImage("E://Code//C++//Grafika komputerowa OpenGL//GK_lab5//tekstury//P6_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

   // Zdefiniowanie tekstury 2-D
	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	// Zwolnienie pamieci
	free(pBytes);

	// Wlaczenie mechanizmu teksturowania
	glEnable(GL_TEXTURE_2D);

	// Ustalenie trybu teksturowania
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Okreslenie sposobu nakladania tekstur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//  Definicja materialu
	//  i definicja zrodla swiatla
	GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	// wspoczynniki ka =[kar,kag,kab] dla swiatla otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsplczynniki kd =[kdr,kdg,kdb] swiata rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspoczynniki ks =[ksr,ksg,ksb] dla swiatla odbitego               

	GLfloat mat_shininess = { 30.0 };
	// wspoczynnik n opisujacy polysk powierzchni

	// Ustawienie patrametrow materialu
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };

	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };

	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat att_constant = { 1.0 };

	GLfloat att_linear = { 0.05 };

	GLfloat att_quadratic = { 0.001 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);   
	glEnable(GL_LIGHT0);     
	glEnable(GL_DEPTH_TEST); 
}

// Funkcja s³u¿¹ca do kontroli zachowania proporcji rysowanych obiektów niezale¿nie od rozmiarów okna graficznego
void ChangeSize(GLsizei horizontal, GLsizei vertical)
// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej

	gluPerspective(70, 1.0, 1.0, 300.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
	// relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu     

	glLoadIdentity();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);


	glutCreateWindow("Teksturowanie");
	// Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
	// Biblioteka GLUT bêdzie wywo³ywa³a t¹ funkcjê za ka¿dym razem, gdy
	// trzeba bêdzie przerysowaæ okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy¿ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst¹pieniem do renderowania

	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania niewidocznych elementów sceny

	glutMouseFunc(Mouse);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie ruchu myszy

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}