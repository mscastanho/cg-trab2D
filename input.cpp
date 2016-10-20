#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include "input.h"
#include "misc.h"
#include "tinyxml2.h"
#include "circle.h"
#include "rect.h"
#include "car.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

using namespace tinyxml2;
using namespace std;

extern vector<Circle*> enemies;
extern Circle* arenaOut;
extern Circle* arenaIn;
extern Circle* player;
extern Car* playerCar;
extern Rect* startEnd;
extern float PLAYER_SPEED;
extern float BULLET_SPEED;
extern float PLAYER_SIZE;

string parseConfigFile (string filePath){

  XMLDocument configFile;

  configFile.LoadFile(filePath.c_str());

	if(configFile.ErrorID() != 0)
		cout << "Erro no arquivo de entrada config.xml" << endl;
	else{

		XMLElement* adaElement = configFile.FirstChildElement("aplicacao")->FirstChildElement("arquivoDaArena");

    string name = adaElement->Attribute("nome");

    string type = adaElement->Attribute("tipo");

    string path = adaElement->Attribute("caminho");

    XMLElement* carElement = configFile.FirstChildElement("aplicacao")->FirstChildElement("carro");

    carElement->QueryFloatAttribute("velCarro",&PLAYER_SPEED);
    carElement->QueryFloatAttribute("velTiro",&BULLET_SPEED);

    return path + name + "." + type;
  }

  return "";
}

Color parseColor(string color){

  if(color == "black")
    return BLACK;
  else if (color == "blue")
    return BLUE;
  else if (color == "green")
    return GREEN;
  else if (color == "cyan")
    return CYAN;
  else if (color == "red")
    return RED;
  else if (color == "magenta")
    return MAGENTA;
  else if (color == "yellow")
    return YELLOW;

    return WHITE;
}

void parseCircle(XMLElement* c){

  float cx,cy,r;
  c->QueryFloatAttribute("cx",&cx);
  c->QueryFloatAttribute("cy",&cy);
  c->QueryFloatAttribute("r",&r);

  string fill = c->Attribute("fill");
  string id = c->Attribute("id");

  Point center = {cx,cy};
  Color fillColor = parseColor(fill);

  Circle* circle = new Circle(center,r,fillColor);

  if(id == "Pista"){
    if(arenaOut == NULL)
      arenaOut = circle;
    else{
      if(arenaOut->get_radius() > circle->get_radius())
        arenaIn = circle;
      else{
        arenaIn = arenaOut;
        arenaOut = circle;
      }
    }
  }else if(id == "Inimigo")
    enemies.push_back(circle);
  else if(id == "Jogador"){
    player = circle;
    PLAYER_SIZE = circle->get_radius()*2;
  }
}

void parseRect(XMLElement* r){

  float x,y,width,height;
  r->QueryFloatAttribute("x",&x);
  r->QueryFloatAttribute("y",&y);
  r->QueryFloatAttribute("width",&width);
  r->QueryFloatAttribute("height",&height);

  string fill = r->Attribute("fill");
  string id = r->Attribute("id");

  Color fillColor = parseColor(fill);

  Rect* rect = new Rect(x,y,width,height,fillColor);

  startEnd = rect;
}


void parseSVGFile (string filePath){

  XMLDocument arenaFile;

  // Treat case when the filePath has '~'
  if(filePath[0] == '~'){
    filePath.erase(0,1);
    struct passwd *pw = getpwuid(getuid());
    string userHome = string(pw->pw_dir);
    filePath = userHome + filePath;

  }
  arenaFile.LoadFile(filePath.c_str());

  if(arenaFile.ErrorID() != 0)
		cout << "Erro no arquivo de entrada config.xml" << endl;
	else{

		XMLElement* svgElement = arenaFile.FirstChildElement("svg");

    XMLElement* next = svgElement->FirstChildElement();

    while(next != NULL){

      string tagType = next->Value();

      if(tagType == "rect"){
          parseRect(next);
      }else if (tagType == "circle"){
          parseCircle(next);
      }

      next = next->NextSiblingElement();

    }

  }
}
