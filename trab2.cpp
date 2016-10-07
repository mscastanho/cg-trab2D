#include <GL/glut.h>
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

// Nome do arquivo de entrada
const string fileName = "config.xml";

void display(void)
{
	/* Limpar todos os pixels */
	glClear(GL_COLOR_BUFFER_BIT);
	
	/* Desenhar o Rectrado */
	if(existeRectrado){
		
		float xcNormalizado = xcRectrado/(float) larguraJanela;
		float ycNormalizado = ycRectrado/(float) alturaJanela;
		float tamQ_2_Normalizado = tamRectrado_2/(float) larguraJanela;
		
		glColor3f(rRectrado,gRectrado,bRectrado);
		glBegin(GL_POLYGON);
			glVertex3f(xcNormalizado+tamQ_2_Normalizado,ycNormalizado+tamQ_2_Normalizado,0.0);
			glVertex3f(xcNormalizado-tamQ_2_Normalizado,ycNormalizado+tamQ_2_Normalizado,0.0);
			glVertex3f(xcNormalizado-tamQ_2_Normalizado,ycNormalizado-tamQ_2_Normalizado,0.0);
			glVertex3f(xcNormalizado+tamQ_2_Normalizado,ycNormalizado-tamQ_2_Normalizado,0.0);
		glEnd();
	}

	/* Trocar buffers */
	glutSwapBuffers();
}

void init (int r, int g, int b)
{
	/*selecionar cor de fundo (preto) */
	glClearColor((GLfloat) r,(GLfloat) g,(GLfloat) b,0.0);

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);
}

int main (int argc, char** argv)
{
	
	string filePath;
	XMLDocument configFile;
	
	// Confere se o arquivo XML foi passado como parametro de entrada
	if (argc > 1 ){
		filePath = argv[1];
		filePath += fileName;
	}else{
		cout << "Por favor entre com o arquivo XML" << endl;
		cout << "Ex: trabalhocg <caminho para pasta de config.xml>" << endl;
		return 0;
	}
	
	if(filePath != ""){
		configFile.LoadFile(filePath.c_str());
	}
	
	if(configFile.ErrorID() != 0)
		cout << "Erro no arquivo de entrada config.xml" << endl;
	else{
		
		XMLElement* janelaElement = configFile.FirstChildElement("aplicacao")->FirstChildElement("janela");
		XMLElement* RectradoElement = configFile.FirstChildElement("aplicacao")->FirstChildElement("Rectrado");
		XMLElement* fundoElement = janelaElement->FirstChildElement("fundo");

		// Lendo o tamanho da janela
		
		janelaElement->FirstChildElement("largura")->QueryIntText(&larguraJanela);
		janelaElement->FirstChildElement("altura")->QueryIntText(&alturaJanela);
	
		// Lendo o titulo da janela
		const char* tituloJanela = janelaElement->FirstChildElement("titulo")->GetText();
		
		GLfloat rFundo,gFundo,bFundo;
		
		// Lendo cor do fundo
		fundoElement->QueryFloatAttribute("corR",&rFundo);
		fundoElement->QueryFloatAttribute("corG",&gFundo);
		fundoElement->QueryFloatAttribute("corB",&bFundo);
		
		// Lendo atributos do Rectrado
		int lado;
		RectradoElement->QueryIntAttribute("tamanho",&lado);
		tamRectrado_2 = lado/2;
		RectradoElement->QueryFloatAttribute("corR",&rRectrado);
		RectradoElement->QueryFloatAttribute("corG",&gRectrado);
		RectradoElement->QueryFloatAttribute("corB",&bRectrado);
		
		// zerar o vetor de status das teclas
		memset(keyStatus,256,sizeof(int));
		
		glutInit(&argc,argv);
		
		// Utilizar Double Buffer e esquema de cores RGB
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
		
		// Definindo o tamanho da janela a ser aberta
		glutInitWindowSize(larguraJanela,alturaJanela);
		
		// Escolher posicao da janela na tela
		glutInitWindowPosition(250,250);
		
		// Definindo o titulo da janela
		glutCreateWindow(tituloJanela);
		

		init(rFundo,gFundo,bFundo);
		
		glutDisplayFunc(display);
		glutMouseFunc(mouse);
		glutMotionFunc(motion);
		glutMainLoop();
		
	}

	return 0;
}

