#include <GL/glut.h>
#include <map>
#include <vector>
#include <iterator>
#include "input.h"
#include "circle.h"
#include "rect.h"
#include "misc.h"
#include "car.h"
#include <cmath>

using namespace std;

vector<Circle*> enemies;
Circle* arenaOut = NULL;
Circle* arenaIn = NULL;
Circle* player = NULL;
Car* playerCar = NULL;
Rect* startEnd = NULL;

GLfloat gx=0,gy=0;
GLfloat pgx=0,pgy=0;
GLfloat wheelAngle=0; // Car angle
int keyStatus[256];

// Deltas to adjust all the coordinates to the new coordinate system
float dx = 0;
float dy = 0;

// Window size
float windowHeight;
float windowWidth;

// Constants for player and bullets
float PLAYER_SIZE = 0;
float PLAYER_SPEED = 0;
float SHOT_SPEED = 0;
const float ANGLE_SPEED = 1;

// Global to sabe player's current orientation
float playerAngle = 0;

void printGlobals(){
	cout << "\nArenaOut" << endl;
	arenaOut->print();

	cout << "\nArenaIn" << endl;
	arenaIn->print();

	cout << "\nplayer" << endl;
	player->print();

	cout << "\nstartEnd" << endl;
	startEnd->print();

	vector<Circle*>::iterator it;

	cout << "\nEnemies" << endl;
	for(it = enemies.begin();it != enemies.end(); it++){
		(*it)->print();
	}
}

// This function calculates the deltas to adjust all coordinates so the center
// of the arena is the new origin
void setNewOrigin(){

	// Adjust y axis orientation to match OpenGL`s
	Point aoc = arenaOut->get_center();
	aoc.y = windowHeight - aoc.y;
	arenaOut->set_center(aoc);

	Point aic = arenaIn->get_center();
	aic.y = windowHeight - aic.y;
	arenaIn->set_center(aic);

	// Delta by which all x and y must be sutracted to set the center of the
	// arena as the new origin
	dx = arenaOut->get_center().x;
	dy = arenaOut->get_center().y;

	// Adjust arena
	Point origin = {0,0};
	arenaOut->set_center(origin);
	arenaIn->set_center(origin);

	// Adjust player
	Point pc = player->get_center();
	pc.x -= dx;
	pc.y = (windowHeight - pc.y) - dy;
	player->set_center(pc);
	playerCar = new Car(PLAYER_SIZE/1.3,RED,YELLOW);

	// Adjust startEnd rectangle
	Point se = startEnd->get_vertex();
	se.x -= dx;
	se.y = (windowHeight - se.y) - dy;
	startEnd->set_vertex(se);

	// Adjust enemies
	vector<Circle*>::iterator it;

	for(it = enemies.begin();it != enemies.end(); it++){
		Point center = (*it)->get_center();
		center.x -= dx;
		center.y = (windowHeight - center.y) - dy;
		(*it)->set_center(center);
	}

}

void keyUp(unsigned char key, int x, int y)
{
	keyStatus[key] = 0;
}
void keyPress(unsigned char key, int x, int y)
{
	keyStatus[key] = 1;
}

void idle (void)
{

	float deltaX, deltaY;

	// Save the previous values of gx and gy
	pgx = gx;
	pgy = gy;

	if((keyStatus['D'] == 1 || keyStatus['d'] == 1) && wheelAngle > -45+ANGLE_SPEED)
		wheelAngle -= ANGLE_SPEED;

	if((keyStatus['A'] == 1 || keyStatus['a'] == 1) && wheelAngle < 45-ANGLE_SPEED)
		wheelAngle += ANGLE_SPEED;

	if( keyStatus['W'] == 1 || keyStatus['w'] == 1 ){
		if(wheelAngle > 0){
			wheelAngle -= 1;
			playerAngle += 1;
		}else if(wheelAngle < 0){
			wheelAngle += 1;
			playerAngle -= 1;
		}

		gy += PLAYER_SPEED*cos(M_PI*playerAngle/180.0);
		gx += -PLAYER_SPEED*sin(M_PI*playerAngle/180.0);

	}

	if( keyStatus['S'] == 1 || keyStatus['s'] == 1 ){
		if(wheelAngle > 0){
			wheelAngle -= 1;
			playerAngle -= 1;
		}else if(wheelAngle < 0){
			wheelAngle += 1;
			playerAngle += 1;
		}

		gy -= PLAYER_SPEED*cos(M_PI*playerAngle/180.0);
		gx -= -PLAYER_SPEED*sin(M_PI*playerAngle/180.0);

	}

	glutPostRedisplay();
}

// This function returns true if the player has collided with any of the enemies
// or if it is outside of the arena. And return false otherwise.
bool player_collided(){

	bool insideArena = player->insideOf(arenaOut) && player->outsideOf(arenaIn);
	//cout << "insideArena: ";
	//cout << insideArena << endl;

	vector<Circle*>::iterator it;
	bool outsideEnemies = true;

	for(it = enemies.begin();it != enemies.end(); it++)
		outsideEnemies = outsideEnemies && player->outsideOf( (*it) );

	//cout << "outsideEnemies: ";
	//cout << outsideEnemies << endl;
	//cout << endl;

	return !(insideArena && outsideEnemies);

}

void display(void)
{
	player->increment_center(gx,0);

	bool collisionX = player_collided();

	if(collisionX)
	 	player->increment_center(-gx,0);

	player->increment_center(0,gy);

	bool collisionY = player_collided();

	if(collisionY)
	 	player->increment_center(0,-gy);

	glClear(GL_COLOR_BUFFER_BIT);

	arenaOut->draw();
	arenaIn->draw();
	Point transPoint = {(startEnd->get_vertex()).x,-(startEnd->get_vertex()).y};
	startEnd->draw(transPoint);

	//player->draw();
	Point carCenter = {player->get_center().x,player->get_center().y};
	playerCar->draw(carCenter,wheelAngle,playerAngle);

	vector<Circle*>::iterator it;
	for(it = enemies.begin();it != enemies.end(); it++)
		(*it)->draw();

	/* Trocar buffers */
	glutSwapBuffers();
	gx = gy = 0;
}


void init (Color bgColor, float xlim1, float xlim2, float ylim1, float ylim2)
{
	/*select background color */
	float r = bgColor.r;
	float g = bgColor.g;
	float b = bgColor.b;
	glClearColor((GLfloat) r,(GLfloat) g,(GLfloat) b,0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(xlim1,xlim2,ylim1,ylim2,-1.0,1.0);
}

int main (int argc, char** argv)
{
	string filePath;

	// Confere se o arquivo XML foi passado como parametro de entrada
	if (argc > 1 ){
		filePath = argv[1];
		filePath += "config.xml";
	}else{
		cout << "Por favor entre com o arquivo XML" << endl;
		cout << "Ex: trabalhocg <caminho para pasta de config.xml>" << endl;
		return 0;
	}

	if(filePath != ""){
		string arenaFile = parseConfigFile(filePath);

		if(arenaFile != "")
				parseSVGFile(arenaFile);

				glutInit(&argc,argv);

				// Utilizar Double Buffer e esquema de cores RGB
				glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);

				// Definindo o tamanho da janela a ser aberta
				windowWidth = arenaOut->get_radius()*2;
				windowHeight = windowWidth;
				glutInitWindowSize(windowWidth,windowHeight);

				// Escolher posicao da janela na tela
				glutInitWindowPosition(250,250);

				// Definindo o titulo da janela
				glutCreateWindow("Arena");

				//Fundo branco
				Color bgColor = {1.0,1.0,1.0};

				// Points to define the ortho
				float xlim1 = -arenaOut->get_radius();
				float xlim2 = arenaOut->get_radius();
				float ylim1 = xlim1;
				float ylim2 = xlim2;

				init(bgColor,xlim1,xlim2,ylim1,ylim2);
				setNewOrigin();
				glutDisplayFunc(display);
				glutKeyboardFunc(keyPress);
				glutKeyboardUpFunc(keyUp);
				glutIdleFunc(idle);
				glutMainLoop();
	}else{
		cout << "Nome do arquivo da arena vazio. Por favor verifique o seu config.xml\n";
		exit(1);
	}
}
