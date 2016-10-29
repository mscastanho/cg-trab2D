#include <GL/glut.h>
#include <map>
#include <vector>
#include <list>
#include <iterator>
#include "input.h"
#include "circle.h"
#include "rect.h"
#include "misc.h"
#include "car.h"
#include "bullet.h"
#include <cmath>

using namespace std;

vector<Car*> enemies;
Circle* arenaOut = NULL;
Circle* arenaIn = NULL;
Car* player = NULL;
Rect* startEnd = NULL;

GLfloat gx=0,gy=0;
GLfloat pgx=0,pgy=0;
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
float PLAYER_BULLET_SPEED = 0;
float ANGLE_SPEED = 1;
float ENEMY_SPEED = 0;
float ENEMY_SHOT_FREQ = 0;
float ENEMY_BULLET_SPEED = 0;

// Global to save last mouse position
float lastMouseX;
float lastMouseY;

// Global to save the mouse status
bool mousePressed;

// List of player bullets that have been shot
list<Bullet*> player_bullets;
list<Bullet*> enemy_bullets;

// Text variable
static char str[2000];
void * font = GLUT_BITMAP_9_BY_15;

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
	Point pc = player->get_position();
	pc.x -= dx;
	pc.y = (windowHeight - pc.y) - dy;
	player->set_position(pc);

	// Adjust startEnd rectangle
	Point se = startEnd->get_vertex();
	se.x -= dx;
	se.y = (windowHeight - se.y) - dy;
	startEnd->set_vertex(se);

	// Adjust enemies
	vector<Car*>::iterator it;

	for(it = enemies.begin();it != enemies.end(); it++){
		Point pos = (*it)->get_position();
		pos.x -= dx;
		pos.y = (windowHeight - pos.y) - dy;
		(*it)->set_position(pos);
	}

}

bool isOutsideWindow(Bullet* b){

	Point pos = b->get_position();

	//Check if bullet is out of the window
	bool cond1 = pos.x > windowWidth/2;
	bool cond2 = pos.x < -windowWidth/2;
	bool cond3 = pos.y > windowHeight/2;
	bool cond4 = pos.y < -windowHeight/2;

	return cond1 || cond2 || cond3 || cond4;
}

void controlEnemyShots(){
	static GLdouble previousTime = 0;
	GLdouble currentTime;
	GLdouble timeDifference;
	GLdouble shotPeriod = 1/ENEMY_SHOT_FREQ;
	vector<Car*>::iterator it;

	// Get time from the beginning of the game
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeDifference = currentTime - previousTime;

	if (timeDifference > shotPeriod){
		previousTime = currentTime;

		for(it = enemies.begin() ; it != enemies.end() ; it++){

			Point canonPosition = (*it)->getBulletInitPos();
			float playerAngle = (*it)->get_cAngle();
			float canonAngle = (*it)->get_cnAngle();
			Bullet* b = new Bullet(canonPosition,RED,playerAngle+canonAngle);
			enemy_bullets.push_back(b);
		}
	}
}

void updateBullets(GLdouble timeDiff){

	list<Bullet*>::iterator it;
	list<int> toDelete;
	int i;
	float playerBulletSpeed = PLAYER_BULLET_SPEED*timeDiff;
	float enemyBulletSpeed = ENEMY_BULLET_SPEED*timeDiff;

	//Update bullets
	for(it = player_bullets.begin(), i = 0;it != player_bullets.end(); it++, i++)
		(*it)->update(playerBulletSpeed);

	for(it = enemy_bullets.begin(), i = 0;it != enemy_bullets.end(); it++, i++)
		(*it)->update(enemyBulletSpeed);

	player_bullets.remove_if(isOutsideWindow);
	enemy_bullets.remove_if(isOutsideWindow);
}

void displayBullets(){
	list<Bullet*>::iterator it;

	for(it = player_bullets.begin();it != player_bullets.end(); it++)
		(*it)->draw();

	for(it = enemy_bullets.begin();it != enemy_bullets.end(); it++)
		(*it)->draw();
}

void updateCar(GLdouble timeDiff) {

	float wheelAngle = player->get_wAngle();
	bool wPressed, sPressed, aPressed, dPressed;

	dPressed = (keyStatus['D'] == 1 || keyStatus['d'] == 1) && wheelAngle > -45+ANGLE_SPEED;

	aPressed = (keyStatus['A'] == 1 || keyStatus['a'] == 1) && wheelAngle < 45-ANGLE_SPEED;

	wPressed = keyStatus['W'] == 1 || keyStatus['w'] == 1;

	sPressed = keyStatus['S'] == 1 || keyStatus['s'] == 1;

	Point delta = player->update(wPressed,sPressed,aPressed,dPressed,timeDiff);

	gy += delta.y;
	gx += delta.x;

}

void mouse(int botao, int estado, int x, int y){
	// Changing y axis orientation
	y = windowHeight - y;

	if(botao == GLUT_LEFT_BUTTON){

		if(estado == GLUT_DOWN)
			mousePressed = true;
		else if(mousePressed){
			// In this case the mouse was pressed.
			// trying to detect a press+release task
			mousePressed = false;
			Point canonPosition = player->getBulletInitPos();
			float playerAngle = player->get_cAngle();
			float canonAngle = player->get_cnAngle();
			Bullet* b = new Bullet(canonPosition,GREEN,playerAngle+canonAngle);
			player_bullets.push_back(b);
		}
	}

	glutPostRedisplay();
}

void passiveMotion(int x, int y){

	float canonAngle = player->get_cnAngle();
	if(x > lastMouseX && canonAngle > -45+ANGLE_SPEED )
		player->inc_cnAngle(-ANGLE_SPEED);
	else if (x < lastMouseX && canonAngle < 45-ANGLE_SPEED)
		player->inc_cnAngle(ANGLE_SPEED);

	lastMouseX = x;
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
	static GLdouble previousTime = 0;
	GLdouble currentTime;
	GLdouble timeDifference;
	float deltaX, deltaY;

	// Get time from the beginning of the game
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeDifference = currentTime - previousTime;
	previousTime = currentTime;

	// Save the previous values of gx and gy
	pgx = gx;
	pgy = gy;


	updateCar(timeDifference);
	controlEnemyShots();
	updateBullets(timeDifference);
	glutPostRedisplay();

}

// This function returns true if the player has collided with any of the enemies
// or if it is outside of the arena. And return false otherwise.
bool player_collided(){

	bool insideArena = player->insideOf(arenaOut) && player->outsideOf(arenaIn);

	vector<Car*>::iterator it;
	bool outsideEnemies = true;

	for(it = enemies.begin();it != enemies.end(); it++)
		outsideEnemies = outsideEnemies && player->outsideOf( (*it) );

	return !(insideArena && outsideEnemies);

}

void printScore(){

	static GLdouble initTime = 0;
	static int minutes;
	GLdouble seconds;
	GLdouble currentTime;
	GLdouble elapsed;

	// Get time from the beginning of the game
	currentTime = glutGet(GLUT_ELAPSED_TIME);

	elapsed = currentTime - initTime;
	minutes = (int) elapsed/60000;
	seconds = elapsed/1000 - minutes*60;

	//Create a string to be printed
	char *tmpStr;
	sprintf(str, "Time: %2d:%2.2f", minutes, seconds );
	//Define the position to start printing
	glColor3f(0.0,0.0,0.0);
	glRasterPos2f(windowWidth/2-15*9,windowHeight/2-20);
	//Print  the first Char with a certain font
	//glutBitmapLength(font,(unsigned char*)str);
	tmpStr = str;
	//Print each of the other Char at time

	while( *tmpStr ){
			glutBitmapCharacter(font, *tmpStr);
			tmpStr++;
	}
}

void display(void)
{
	player->inc_position(gx,0);
	player->inc_position(0,gy);

	bool collisionX = player_collided();
	bool collisionY = player_collided();

	if(collisionX || collisionY){
	 	player->inc_position(-gx,0);
	 	player->inc_position(0,-gy);
	}

	glClear(GL_COLOR_BUFFER_BIT);

	arenaOut->draw();
	arenaIn->draw();
	Point transPoint = {(startEnd->get_vertex()).x,-(startEnd->get_vertex()).y};
	startEnd->draw(transPoint);

	player->draw();

	vector<Car*>::iterator it;
	for(it = enemies.begin();it != enemies.end(); it++)
		(*it)->draw();

	displayBullets();

	printScore();

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
				glutMouseFunc(mouse);
				glutKeyboardUpFunc(keyUp);
				glutIdleFunc(idle);
				glutPassiveMotionFunc(passiveMotion);
				glutMainLoop();
	}else{
		cout << "Nome do arquivo da arena vazio. Por favor verifique o seu config.xml\n";
		exit(1);
	}
}
