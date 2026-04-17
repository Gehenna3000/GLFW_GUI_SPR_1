#include <GLFW/glfw3.h> 
#include<iostream> 
#include <cmath>
using namespace std;
enum
{
    FULL_WINDOW,
    ASPECT_1_1,
    ORTO,
    FRUST
};
GLint skala = FULL_WINDOW;
GLint rzut = ORTO;

GLfloat zakres = 10.0f;
GLfloat blisko = 1.0f;
GLfloat daleko = 50.0f;

float trans_x = 0;
float trans_y = 0;
float trans_z = 0;


float global_rotation_x = 0;
float global_rotation_y = 0;
float global_rotation_z = 0;


void display(float size, GLFWwindow* window) {

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    float cx = width / 2.0f;
    float cy = height / 2.0f;

    double mouse_pos_x, mouse_pos_y;
    glfwGetCursorPos(window, &mouse_pos_x, &mouse_pos_y);

    float time = glfwGetTime();

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);


    float s = 1.0;
    float angle = 0.5;

    // normalizacja (-1 ; 1)
    float nx = (((float)xpos / width) * 2.0f) - 1.0f;
    float ny = 1.0f - (((float)ypos / height) * 2.0f);

    nx *= width/64 ; 
    ny *= height/64 ; 


    float pos_z = 0;

    float shift_x =  nx - trans_x;
    float shift_y =  ny - trans_y;
    float shift_z = 1.0f;
    glColor3d(sin(time), cos(time), sin(time * 2));
    
    glLineWidth(2);

    glPushMatrix();

    glTranslatef(trans_x, trans_y,trans_z);
    glRotatef(global_rotation_x, 1,0,0);
    glRotatef(global_rotation_y, 0, 1, 0);
    glRotatef(global_rotation_z, 0, 0, 1);

    glBegin(GL_LINES);

    // ten teges
    glVertex3f(-s, -s, 0); glVertex3f(s, -s, 0);
    glVertex3f(-s, s, 0);  glVertex3f(-s - angle, 0, 0);
    glVertex3f(-s - angle, 0, 0);  glVertex3f(-s , -s, 0);
    glVertex3f(s, s, 0);   glVertex3f(-s, s, 0);
    glVertex3f(s, -s, 0);  glVertex3f(s + angle, 0, 0);
    glVertex3f(s + angle, 0, 0);  glVertex3f(s, s, 0);
    
    // shifted
    glVertex3f(-s + shift_x , -s + shift_y, shift_z); glVertex3f(s + shift_x, -s + shift_y, shift_z);
    glVertex3f(-s + shift_x, s + shift_y, shift_z);  glVertex3f(-s - angle + shift_x, 0 + shift_y, shift_z);
    glVertex3f(-s - angle + shift_x, 0 + shift_y, shift_z);  glVertex3f(-s + shift_x, -s + shift_y, shift_z);
    glVertex3f(s + shift_x, s + shift_y, shift_z);   glVertex3f(-s + shift_x, s + shift_y, shift_z);
    glVertex3f(s + shift_x, -s + shift_y, shift_z);  glVertex3f(s + angle + shift_x, 0 + shift_y, shift_z);
    glVertex3f(s + angle + shift_x, 0 + shift_y, shift_z);  glVertex3f(s + shift_x, s + shift_y, shift_z);

    // po��czenia
    glVertex3f(-s, -s, 0); glVertex3f(-s + shift_x, -s + shift_y, shift_z);
    glVertex3f(s, -s, 0); glVertex3f(s + shift_x, -s + shift_y, shift_z);
    glVertex3f(-s, s, 0); glVertex3f(-s + shift_x, s + shift_y, shift_z);
    glVertex3f(s, s, 0); glVertex3f(s + shift_x, s + shift_y, shift_z);
    glVertex3f(-s - angle, 0, 0); glVertex3f(-s - angle + shift_x, shift_y, shift_z);
    glVertex3f(s + angle, 0, 0); glVertex3f(s + angle + shift_x, shift_y, shift_z);

    glEnd();
    glPopMatrix();



}

const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        // RZUT I SKALA
        case GLFW_KEY_1:
            rzut = ORTO;
            break;
        case GLFW_KEY_2:
            rzut = FRUST;
            break;
        case GLFW_KEY_3:
            skala = ASPECT_1_1;
            break;
        case GLFW_KEY_4:
            skala = FULL_WINDOW;
            break;
        // POZYCJA
        case GLFW_KEY_W:
            trans_y += 1;
            break;
        case GLFW_KEY_S:
            trans_y -= 1;
            break;
        case GLFW_KEY_D:
            trans_x += 1;
            break;
        case GLFW_KEY_A:
            trans_x -= 1;
            break;
        case GLFW_KEY_E:
            trans_z += 1;
            break;
        case GLFW_KEY_Q:
            trans_z -= 1;
            break;
        //ROTACJA
        case GLFW_KEY_T:
            global_rotation_y += 15;
            break;
        case GLFW_KEY_G:
            global_rotation_y -= 15;
            break;
        case GLFW_KEY_H:
            global_rotation_x += 15;
            break;
        case GLFW_KEY_F:
            global_rotation_x -= 15;
            break;
        case GLFW_KEY_Y:
            global_rotation_z += 15;
            break;
        case GLFW_KEY_R:
            global_rotation_z -= 15;
            break;
        }
    }
}


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 600, "Przykladowe Okno GLFW", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSetKeyCallback(window, key_callback);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //glOrtho(-width / 2.0, width / 2.0, -height / 2.0, height / 2.0, -10.0, 10.0);

        if (rzut == ORTO)
        {
            if (skala == ASPECT_1_1)
            {
                if (width < height && width > 0)
                    glOrtho(-zakres, zakres, -zakres * height / width, zakres * height / width, -zakres, zakres);
                else
                    if (width >= height && height > 0)
                        glOrtho(-zakres * width / height, zakres * width / height, -zakres, zakres, -zakres, zakres);
            }
            else
                glOrtho(-zakres, zakres, -zakres, zakres, -zakres, zakres);
        }
        if (rzut == FRUST)
        {
            if (skala == ASPECT_1_1)
            {
                if (width < height && width > 0)
                    glFrustum(-zakres, zakres, -zakres * height / width, zakres * height / width, blisko, daleko);
                else
                    if (width >= height && height > 0)
                        glFrustum(-zakres * width / height, zakres * width / height, -zakres, zakres, blisko, daleko);
            }
            else
                glFrustum(-zakres, zakres, -zakres, zakres, blisko, daleko);
        }

        glMatrixMode(GL_MODELVIEW);

        float squareSize = static_cast<float>(std::min(width, height)) * 0.2f;

        display(squareSize, window);

        glEnable(GL_CULL_FACE);
        glClearColor(1, 1, 1, 1);
        glfwSwapBuffers(window);

        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);

        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
