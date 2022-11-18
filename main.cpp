#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL\gl.h>
#include <GL\glut.h>
#include "SOIL/SOIL.h"

/////////   VARIÁVEIS GLOBAIS PARA OS ANGULOS////////////
float ang = 0;
float ang2 = 0;
float ang3 = 0;

float chaoZ = -500;
float chaoAng = 0;

////////////////////////// TEXTURA/////////////////////////////////

GLuint      texture[6], indiceTextura=0;
int LoadGLTextures()
{
    texture[0] = SOIL_load_OGL_texture ( "D:\\Programação\\CodeBlocks\\Porco\\textures\\front_head.png",
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    texture[1] = SOIL_load_OGL_texture ( "D:\\Programação\\CodeBlocks\\Porco\\textures\\back_head.png",
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    texture[2] = SOIL_load_OGL_texture ( "D:\\Programação\\CodeBlocks\\Porco\\textures\\top_head.png",
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    texture[3] = SOIL_load_OGL_texture ( "D:\\Programação\\CodeBlocks\\Porco\\textures\\left_head.png",
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    texture[4] = SOIL_load_OGL_texture ( "D:\\Programação\\CodeBlocks\\Porco\\textures\\right_head.png",
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    texture[5] = SOIL_load_OGL_texture ( "D:\\Programação\\CodeBlocks\\Porco\\textures\\grass.png",
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if(texture[0] == 0)
    {
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.54, 0.87, 0.95, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    return true;
}


void display();
///////////////////// O S S O /////////////////////////////////////////////
class Osso
{
public:
    Osso(float a, float l): largura(l), altura(a), conexcao(0) {}
    void desenha();

    void setConexcao(Osso *osso, float ang) {conexcao = osso, angulo = ang;}

    void setAngulo(float ang){angulo = ang;}
    float getAngulo(){return angulo;}

protected:
    float largura;
    float altura;

    float angulo;
    Osso *conexcao;
};

void Osso::desenha()
{
    glPushMatrix();  //salva o contexto(1)

      glTranslatef (0.0, altura/2.0, 0.0); //vai para o meio do osso

      glPushMatrix();   //salva o contexto(2)
        glScalef (largura, altura, largura);  //escala para o tamanho do osso

        glutSolidCube (largura);    //desenha o osso
      glPopMatrix();    //restaura o contexto(2)

      glTranslatef (0.0, altura/2.0, 0.0); // vai para a ponta do osso

      //glutSolidSphere(0.85*largura,8,8);        //desenha a bolinha

      if (conexcao)
      {
          glRotatef(angulo, 1.0, 0.0, 0.0); //rotaciona para o angulo da conexcao
          conexcao->desenha();              //desenha recursivamente
      }

    glPopMatrix();  //restaura o contexto(1)
};
////////////////////////P E R N A////////////////////////////////////
class Perna
{
public:
    Perna(float comprimento, float largura);
    void desenha() {a.desenha();}
    void setCurvatura(float curvatura);
    float getCurvatura() {return a.getAngulo()*100/90;}

protected:
    Osso a;
};

Perna::Perna(float comprimento, float largura)
  : a(comprimento*0.4,largura)//, b(comprimento*0.2,largura)
{
  // a.setConexcao(&b,0.0);
}

void Perna::setCurvatura(float curvatura)
{
   a.setAngulo(curvatura*0.9);
   //b.setAngulo(curvatura*-0.9);
}
///////////////////////C A B E Ç A/////////////////////////////////////
class Cabeca
{
public:
    Cabeca(float comprimento, float largura);
    void desenha() {a.desenha();}
    void setCurvatura(float curvatura);
    float getCurvatura() {return a.getAngulo()*100/90;}

protected:
    Osso a;
};

Cabeca::Cabeca(float comprimento, float largura)
  : a(comprimento*0.5,largura)
{
}

void Cabeca::setCurvatura(float curvatura)
{
}

//////////////////////////////////////////////////////////////////////////////

void Chao()
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture ( GL_TEXTURE_2D, texture[5] );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTranslatef(-500, -13, chaoZ);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(0, 0, 0);
        glTexCoord2f(0.0f, 800.0f);glVertex3f(0, 0, 10000);
        glTexCoord2f(800.0f, 800.0f);glVertex3f(10000, 0, 10000);
        glTexCoord2f(800.0f, 0.0f);glVertex3f(10000, 0, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//////////////////////////////////P O R C O///////////////////////////////////

class Corpo
{
public:
    Corpo(float grossura);
    void desenha();
    void setCurvatura(int perna,float curv);
    float getCurvatura(int perna) {return curvatura[perna];}
    void abrir(bool tudoJunto = false);
    void fechar(bool tudoJunto = false);
    void tchau();
    void home();
    void Andar();
    void Coice();
    void Comer();
protected:
    float grossura;

    Perna esqTras;
    Perna dirTras;
    Perna esqFren;
    Perna dirFren;
    Cabeca cabeca;
    float curvatura[5];
    float curvaturaZ[5];
};

Corpo::Corpo(float gros)
  : grossura(gros),
    esqTras(8, grossura),
    dirTras(8, grossura),
    esqFren(8, grossura),
    dirFren(8, grossura),
    cabeca(8, 2.30)
{
    for (int i=0;i<5;i++)
    {
        curvatura[i] = 0;
        curvaturaZ[i] = 0;
    }
}

void Corpo::desenha()
{
    glPushMatrix();
      glTranslatef(0.0,6.0*grossura,0.0);
      glPushMatrix();
      //PERNA ESQUERDA TRASEIRA
        glTranslatef(2.5, -7, -7);
        glScalef(3.5, 2.0, 3.5);
        glRotatef(curvatura[0]*0.9,1.0,0.0,0.0);
        glRotatef(curvaturaZ[0]*0.9,0.0,0.0,1.0);
        glRotatef(180, 1, 0, 0);
        esqTras.desenha();
      glPopMatrix();
      glPushMatrix();
      //PERNA DIREITA TRASEIRA
        glTranslatef(-4, -7, -7);
        glScalef(3.5, 2.0, 3.5);
        glRotatef(curvatura[1]*0.9,1.0,0.0,0.0);
        glRotatef(curvaturaZ[1]*0.9,0.0,0.0,1.0);
        glRotatef(180, 1, 0, 0);
        dirTras.desenha();
      glPopMatrix();
      glPushMatrix();
      //PERNA ESQUERDA FRONTAL
        glTranslatef(2.5, -7, 5);
        glScalef(3.5, 2.0, 3.5);
        glRotatef(curvatura[2]*0.9,1.0,0.0,0.0);
        glRotatef(curvaturaZ[2]*0.9,0.0,0.0,1.0);
        glRotatef(180, 1, 0, 0);
        esqFren.desenha();
      glPopMatrix();
      glPushMatrix();
      //PERNA DIREITA FRONTAL
        glTranslatef(-4, -7, 5);
        glRotatef(curvatura[3]*0.9,1.0,0.0,0.0);
        glRotatef(curvaturaZ[3]*0.9,0.0,0.0,1.0);
        glRotatef(180, 1, 0, 0);
        glScalef(3.5, 2.0, 3.5);
        dirFren.desenha();
      glPopMatrix();
      glPushMatrix();
        glTranslatef(-0.69, -1.5, 8);
        glRotatef(curvatura[4]*0.5,1.0,0.0,0.0);
        glRotatef(curvaturaZ[4]*0.9,0.0,0.0,1.0);
        glScalef(1.5,0.9,1.5);
        cabeca.desenha();

        //////TEXTURA DA FRENTE
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture ( GL_TEXTURE_2D, texture[0] );
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(-2.62, -2.58, 2.65);
            glTexCoord2f(1.0f, 0.0f);glVertex3f(2.63, -2.58, 2.65);
            glTexCoord2f(1.0f, 1.0f);glVertex3f(2.63, 6.6, 2.65);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(-2.62, 6.6, 2.65);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //////TEXTURA DE TRAS
        glPushMatrix();
        glTranslatef(0, 0, -5.3);
        glEnable(GL_TEXTURE_2D);
        glBindTexture ( GL_TEXTURE_2D, texture[1] );
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(-2.62, -2.58, 2.65);
            glTexCoord2f(1.0f, 0.0f);glVertex3f(2.63, -2.58, 2.65);
            glTexCoord2f(1.0f, 1.0f);glVertex3f(2.63, 6.6, 2.65);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(-2.62, 6.6, 2.65);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //////TEXTURA LADO ESQUERDO
        glPushMatrix();
        glRotatef(90, 0, 1, 0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture ( GL_TEXTURE_2D, texture[3] );
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(-2.62, -2.58, 2.65);
            glTexCoord2f(1.0f, 0.0f);glVertex3f(2.63, -2.58, 2.65);
            glTexCoord2f(1.0f, 1.0f);glVertex3f(2.63, 6.6, 2.65);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(-2.62, 6.6, 2.65);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        //////TEXTURA LADO DIREITO
        glPushMatrix();
        glRotatef(-90, 0, 1, 0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture ( GL_TEXTURE_2D, texture[4] );
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(-2.62, -2.58, 2.65);
            glTexCoord2f(1.0f, 0.0f);glVertex3f(2.63, -2.58, 2.65);
            glTexCoord2f(1.0f, 1.0f);glVertex3f(2.63, 6.6, 2.65);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(-2.62, 6.6, 2.65);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

      glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.75, 3.0, 0.0);
        glScalef(10.0, 8.0, 16);
        glutSolidCube(1.0);
    glPopMatrix();
}

void Corpo::setCurvatura(int perna,float curv)
{
    curvatura[perna] = curv;

    switch(perna)
    {
        case 0: esqTras.setCurvatura(curv); break;
        case 1: dirTras.setCurvatura(curv); break;
        case 2: esqFren.setCurvatura(curv); break;
        case 3: dirFren.setCurvatura(curv); break;
        case 4: cabeca.setCurvatura(curv); break;
    }
}

//////////////////// M O V I M E N T O S /////////////////////////////

void Corpo::abrir(bool tudoJunto)
{
    if (tudoJunto)
        for (int j=getCurvatura(1);j>=0;j-=5)
        {
            for (int i=4;i>=0;i--)
            {
                setCurvatura(i,j);

            }
            display();
        }
    else
        for (int i=4;i>=0;i--)
        {
            for (int j=getCurvatura(i);j>=0;j-=5)
            {
                setCurvatura(i,j);
                display();
            }
        }
}

void Corpo::fechar(bool tudoJunto)
{
    if (tudoJunto)
    for (int j=getCurvatura(1);j<=100;j+=5)
    {
        for (int i=0;i<5;i++)
        {
            setCurvatura(i,j);
        }
        display();
    }
    else
    for (int i=0;i<5;i++)
    {
        for (int j=getCurvatura(i);j<=100; j += 5)
        {
            setCurvatura(i,j);
            display();
        }
    }
}

void Corpo::tchau()
{
    for (int i = getCurvatura(0); i <= 15; i += 5)
    {
        ang2 -= 15;
        setCurvatura(3, i);
        setCurvatura(4, -(i * 5));
        setCurvatura(0, -i);
        setCurvatura(1, -i);
        display();
    }
    for (int i = getCurvatura(2); i >= -30; i -= 5)
    {
        setCurvatura(2, i);
        display();
    }
    for (int i = getCurvatura(4); i <= 55; i += 5)
    {
        setCurvatura(4, i);
        display();
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            curvaturaZ[2] -= 5;
            display();
        }
        for (int j = 0; j < 6; j++)
        {
            curvaturaZ[2] += 5;
            display();
        }
    }

    Sleep(1500);

    ang2 = 0;
    setCurvatura(3, 0);
    setCurvatura(4, 0);
    setCurvatura(0, 0);
    setCurvatura(1, 0);
    setCurvatura(2, 0);
}

void Corpo::Andar()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = getCurvatura(4); j >= -20; j -= 10)
        {
            setCurvatura(4, j);
            display();
        }
        for (double j = getCurvatura(2); j >= -30; j -= 2)
        {
            setCurvatura(2, j);
            setCurvatura(0, -j);
            setCurvatura(3, -j);
            setCurvatura(1, j);
            chaoZ -= 1;
            display();
        }
        for (double j = getCurvatura(2); j <= 0; j += 2)
        {
            setCurvatura(2, j);
            setCurvatura(0, -j);
            setCurvatura(3, -j);
            setCurvatura(1, j);
            display();
        }
        for (int j = getCurvatura(4); j <= 0; j += 10)
        {
            setCurvatura(4, j);
            display();
        }
        for (double j = getCurvatura(2); j <= 30; j += 2)
        {
            setCurvatura(2, j);
            setCurvatura(0, -j);
            setCurvatura(3, -j);
            setCurvatura(1, j);
            chaoZ -= 1;
            display();
        }
        for (double j = getCurvatura(2); j >= 0; j -= 2)
        {
            setCurvatura(2, j);
            setCurvatura(0, -j);
            setCurvatura(3, -j);
            setCurvatura(1, j);
            display();
        }
    }
}

void Corpo::Coice()
{
    for (int i = 0; i < 35; i+= 5)
    {
        ang2 += 5;
        display();
    }
    for (int i = getCurvatura(2); i >= -35; i -= 5)
    {
        setCurvatura(2, i);
        setCurvatura(3, i);
        setCurvatura(0, i);
        setCurvatura(1, i);
        display();
    }
    for (int i = getCurvatura(1); i <= 35; i += 5)
    {
        setCurvatura(1, i);
        display();
    }
    Sleep(1500);
    for (int i = 0; i < 35; i+= 5)
    {
        ang2 -= 5;
        display();
    }
    for (int i = getCurvatura(2); i <= 0; i += 5)
    {
        setCurvatura(2, i);
        setCurvatura(3, i);
        setCurvatura(0, i);
        setCurvatura(1, i);
        display();
    }
    for (int i = getCurvatura(1); i >= 0; i-= 5)
    {
        setCurvatura(1, i);
        display();
    }
}

void Corpo::Comer()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = getCurvatura(4); j <= 65; j += 5)
        {
            setCurvatura(4, j);
            display();
        }
        for (int j = getCurvatura(4); j >= 20; j -= 5)
        {
            setCurvatura(4, j);
            display();
        }
    }
}

void Corpo::home()
{
    abrir(true);

    if (ang < 0)
        ang += 360;
    if (ang2< 0)
        ang2+= 360;
    if (ang3< 0)
        ang3+= 360;
    if (chaoAng < 0)
        chaoAng += 360;


    while (ang > 0)
    {
        ang-= 10;
        display();
    }
    ang = 0;
    while (ang2 > 0)
    {
        ang2-= 10;
        display();
    }
    ang2 = 0;
    while (ang3 > 0)
    {
        ang3-= 10;
        display();
    }
    ang3 = 0;
    while (chaoAng > 0)
    {
        chaoAng -= 10;
        display();
    }
    chaoAng = 0;
    display();

}

/////////////////////////////////////////////////////////////
Corpo m(1.0);

///////////////////////////////////////////////////
void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LEQUAL);				// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //////////////ISSO AQUI EH PRA LUZ///////////E EU AINDA N SEI COMO FUNCA/////
    GLfloat			diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0};
    GLfloat			ambientLight[] = { 0.9f, 0.9f, 0.9f, 0.8};
    GLfloat			lightPos[] = { 0.0f, 500.0f, 100.0f, 1.0f };
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1.0f,1.0f,1.0f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, diffuseLight );
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 50);
    //////////////////////////////////////////////////////////////////////////
    glPushMatrix();
        glTranslatef (0.0, -5.0, -15.0);
        glRotatef (ang3, 0.0, 0.0, 1.0);
        glRotatef (ang, 0.0, 1.0, 0.0);
        glRotatef (ang2, 1.0, 0.0, 0.0);
        glColor3f(0.9, 0.49, 0.5);

        m.desenha();
    glPopMatrix();

    glPushMatrix();
        glRotatef(chaoAng, 0, 1, 0);
        glColor3f(0.0, 0.85, 0.0);
        Chao();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(110.0, (GLfloat) w/(GLfloat) h, 1.0, 110.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -10.0);
}

/////////////// A T A L H O S //////////////////////

void keyboard (unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'q':
            if (m.getCurvatura(0) < 100)
                m.setCurvatura(0,m.getCurvatura(0)+5);
            break;
        case 'a':
            if (m.getCurvatura(0) > 0)
                m.setCurvatura(0,m.getCurvatura(0)-5);
            break;
        case 'w':
            if (m.getCurvatura(1) < 100)
                m.setCurvatura(1,m.getCurvatura(1)+5);
            break;
        case 's':
            if (m.getCurvatura(1) > 0)
                m.setCurvatura(1,m.getCurvatura(1)-5);
            break;
         case 'e':
            if (m.getCurvatura(2) < 100)
                m.setCurvatura(2,m.getCurvatura(2)+5);
            break;
        case 'd':
            if (m.getCurvatura(2) > 0)
                m.setCurvatura(2,m.getCurvatura(2)-5);
            break;
        case 'r':
            if (m.getCurvatura(3) < 100)
                m.setCurvatura(3,m.getCurvatura(3)+5);
            break;
        case 'f':
            if (m.getCurvatura(3) > 0)
                m.setCurvatura(3,m.getCurvatura(3)-5);
            break;
        case 't':
            if (m.getCurvatura(4) < 100)
                m.setCurvatura(4,m.getCurvatura(4)+5);
            break;
        case 'g':
            if (m.getCurvatura(4) > 0)
                m.setCurvatura(4,m.getCurvatura(4)-5);
            break;
        case '.': //>
            ang += 5;
            chaoAng += 5;
            if (ang>360)
                ang -= 360;
            break;
        case ',': //<
            ang -= 5;
            chaoAng -= 5;
            if (ang<0)
                ang += 360;
            break;
        case ']':
            ang2 += 5;
            if (ang2>360)
                ang2 -= 360;
            break;
        case '[':
            ang2 -= 5;
            if (ang2<0)
                ang2 += 360;
            break;
        case '*' :
            m.abrir(true);
            break;
        case '/' :
            m.fechar(true);
            break;
        case 27: //ESC
            exit(0);
            break;
        case '9':
            ang3 += 5;
            if (ang3>360)
                ang3 -= 360;
            break;
        case '0':
            ang3 -= 5;
            if (ang3<0)
                ang3 += 360;
            break;
        case '1' :
            m.tchau();
            break;
        case '2' :
            m.Andar();
            break;
        case '3' :
            m.Coice();
            break;
        case '4' :
            m.Comer();
            break;
        case '5':
            m.home();
            break;

        default:
            return;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (300, 300);
    glutInitWindowPosition (200, 200);
    glutCreateWindow (argv[0]);
    init ();

    printf("FullScreen?(y/n) ");
    if (getchar() == 'y')
        glutFullScreen();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    if (!LoadGLTextures())
    {
		return 1; //SE A TEXTURA NAO CARREGAR RETORNA FALSO
	}

    glutMainLoop();
    return 0;
}
