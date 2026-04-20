#include <GLFW/glfw3.h>
#include<iostream> 
#include <fstream>
#include <stdlib.h>
using namespace std;

const int k = 640; 
const int w = 480; 
const int ps = 1; 

int Rs[w][k];
int Gs[w][k];
int Bs[w][k];

int R_copy[w][k];
int G_copy[w][k];
int B_copy[w][k];

bool green_pass = true; 
bool red_pass = true;
bool blue_pass = true;


float blur = 0.0; 

int Mdp[3][3] = { 1,1,1 
,1,1,1
,1,1,1 };

int Contrast[3][3] = { 0,-1,0 
,-1,5,-1
,0,-1,0 };

int Border[3][3] = { -1,-1,-1 
,-1,8,-1
,-1,-1,-1 };
int Emboss[3][3] = { -2, -1, 0
, -1, 1, 1
, 0, 1, 2 };

int sumMdp = 0; 

int lw, lk;

float global_brightnes = 0.0; 
float wobbly = 0.0; 
int Custom1[3][3] = { -8, -8, -8
, 0, 0, 0
, 8, 8, 8 };

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


    glPushMatrix();
    glTranslated(0, 0, -6);
    glBegin(GL_POINTS);
    for (int i = 0; i < lw; ++i)
        for (int j = 0; j < lk; ++j)
        {
            float red_ = 0;
            float green_ = 0;
            float blue_ =  0;
            if (green_pass)
                green_ = (Gs[i][j] + global_brightnes) / 255.0;
            if (red_pass )
                red_ = (Rs[i][j] + global_brightnes) / 255.0; 

            if (blue_pass)
                blue_ = (Bs[i][j] + global_brightnes) / 255.0;

            glColor3f(red_, green_ , blue_ );
            glVertex3f(j , i , 0);
        }

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

void MatrixTransform(int Matrix[3][3]){
    float sum = 0;
    for (int s = 0; s < 3; s++) {
        for (int d = 0; d < 3; d++) {
            sum += Matrix[s][d];
        }
    }
    if (sum == 0) sum = 1.0f;

    for (int i = 1; i < lw - 1; ++i) {
        for (int j = 1; j < lk - 1; ++j) {
            float R_ = 0, G_ = 0, B_ = 0;

            for (int s = 0; s < 3; s++) {
                for (int d = 0; d < 3; d++) {
                    R_ += Matrix[s][d] * R_copy[i - 1 + s][j - 1 + d];
                    G_ += Matrix[s][d] * G_copy[i - 1 + s][j - 1 + d];
                    B_ += Matrix[s][d] * B_copy[i - 1 + s][j - 1 + d];
                }
            }

            int finalR = static_cast<int>(R_ / sum);
            int finalG = static_cast<int>(G_ / sum);
            int finalB = static_cast<int>(B_ / sum);

            Rs[i][j] = (finalR > 255) ? 255 : (finalR < 0 ? 0 : finalR);
            Gs[i][j] = (finalG > 255) ? 255 : (finalG < 0 ? 0 : finalG);
            Bs[i][j] = (finalB > 255) ? 255 : (finalB < 0 ? 0 : finalB);
        }
    }
}

void MatrixTransform(float Matrix[3][3]){
    float sum = 0;
    for (int s = 0; s < 3; s++) {
        for (int d = 0; d < 3; d++) {
            sum += Matrix[s][d];
        }
    }
    if (sum == 0) sum = 1.0f;

    for (int i = 1; i < lw - 1; ++i) {
        for (int j = 1; j < lk - 1; ++j) {
            float R_ = 0, G_ = 0, B_ = 0;

            for (int s = 0; s < 3; s++) {
                for (int d = 0; d < 3; d++) {
                    R_ += Matrix[s][d] * R_copy[i - 1 + s][j - 1 + d];
                    G_ += Matrix[s][d] * G_copy[i - 1 + s][j - 1 + d];
                    B_ += Matrix[s][d] * B_copy[i - 1 + s][j - 1 + d];
                }
            }

            int finalR = static_cast<int>(R_ / sum);
            int finalG = static_cast<int>(G_ / sum);
            int finalB = static_cast<int>(B_ / sum);

            Rs[i][j] = (finalR > 255) ? 255 : (finalR < 0 ? 0 : finalR);
            Gs[i][j] = (finalG > 255) ? 255 : (finalG < 0 ? 0 : finalG);
            Bs[i][j] = (finalB > 255) ? 255 : (finalB < 0 ? 0 : finalB);
        }
    }
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_UP:
            global_brightnes += 10.0;
            break;
        case GLFW_KEY_DOWN:
            global_brightnes -= 10.0;
            break;
        case GLFW_KEY_RIGHT:
            wobbly += 1;
            break;
        case GLFW_KEY_LEFT:
            wobbly -= 1;
            break;
        case GLFW_KEY_P:
            for (int i = 0; i < lw; ++i)
                for (int j = 0; j < lk; ++j)
                {
                    Rs[i][j] = R_copy[i][j];
                    Gs[i][j] = G_copy[i][j];
                    Bs[i][j] = B_copy[i][j];
                }
            break;
        case GLFW_KEY_B:
            blue_pass = !blue_pass; 
            break;
        case GLFW_KEY_G:
            green_pass = !green_pass;
            break;
        case GLFW_KEY_R:
            red_pass = !red_pass;
            break;

        case GLFW_KEY_U:
            MatrixTransform(Mdp);
            break;
        case GLFW_KEY_1:
            for (int i = 1; i < lw - 1; i++) {
                for (int j = 1; j < lk - 1; j++) {
                    int r_v[9], g_v[9], b_v[9];
                    int count = 0;
                    for (int s = 0; s < 3; s++) {
                        for (int d = 0; d < 3; d++) {
                            r_v[count] = R_copy[i-1+s][j-1+d];
                            g_v[count] = G_copy[i-1+s][j-1+d];
                            b_v[count] = B_copy[i-1+s][j-1+d];
                            count++;
                        }
                    }
                    for (int x = 0; x < 9; x++) {
                        for (int y = 0; y < 8; y++) {
                            if (r_v[y] > r_v[y+1]) { int t = r_v[y]; r_v[y] = r_v[y+1]; r_v[y+1] = t; }
                            if (g_v[y] > g_v[y+1]) { int t = g_v[y]; g_v[y] = g_v[y+1]; g_v[y+1] = t; }
                            if (b_v[y] > b_v[y+1]) { int t = b_v[y]; b_v[y] = b_v[y+1]; b_v[y+1] = t; }
                        }
                    }
                    Rs[i][j] = r_v[4]; Gs[i][j] = g_v[4]; Bs[i][j] = b_v[4];
                }
            }
            break;
        case GLFW_KEY_I:
            for (int i = 1; i < lw-1; i++) {
                for (int j = 1; j < lk-1; j++)
                {
                    int max_R = 0; 
                    int max_G = 0;
                    int max_B = 0;

                    for (int s = 0; s < 3; s++) {
                        for (int d = 0; d < 3; d++) {
                            if (R_copy[i - 1 + s][j - 1 + d] > max_R) max_R = R_copy[i - 1 + s][j - 1 + d];
                            if (G_copy[i - 1 + s][j - 1 + d] > max_G) max_G = G_copy[i - 1 + s][j - 1 + d];
                            if (B_copy[i - 1 + s][j - 1 + d] > max_B) max_B = B_copy[i - 1 + s][j - 1 + d];
                        }
                    }
                    Rs[i][j] = max_R;
                    Gs[i][j] = max_G;
                    Bs[i][j] = max_B;
                }
            }
            break;
        case GLFW_KEY_2:
            for (int i = 1; i < lw-1; i++) {
                for (int j = 1; j < lk-1; j++)
                {
                    int max_R = 1000000; 
                    int max_G = 1000000;
                    int max_B = 10000000;

                    for (int s = 0; s < 3; s++) {
                        for (int d = 0; d < 3; d++) {
                            if (R_copy[i - 1 + s][j - 1 + d] < max_R) max_R = R_copy[i - 1 + s][j - 1 + d];
                            if (G_copy[i - 1 + s][j - 1 + d] < max_G) max_G = G_copy[i - 1 + s][j - 1 + d];
                            if (B_copy[i - 1 + s][j - 1 + d] < max_B) max_B = B_copy[i - 1 + s][j - 1 + d];
                        }
                    }
                    Rs[i][j] = max_R;
                    Gs[i][j] = max_G;
                    Bs[i][j] = max_B;
                }
            }
            break;
        case GLFW_KEY_N:
            for (int i = 1; i < lw-1; i++) {
                for (int j = 1; j < lk-1; j++)
                {

                    Rs[i][j] = 255 - Rs[i][j];
                    Gs[i][j] = 255 -Gs[i][j];
                    Bs[i][j] = 255 -Bs[i][j];
                }
            }
            break;

        case GLFW_KEY_S:
            for (int i = 1; i < lw-1; i++) {
                for (int j = 1; j < lk-1; j++)
                {
                    float avrage = 0.299 * Rs[i][j] + 0.587*Gs[i][j] + 0.114* Bs[i][j] ;
                    Rs[i][j] = avrage;
                    Gs[i][j] = avrage;
                    Bs[i][j] = avrage;
                }
            }
            break;
        case GLFW_KEY_D:
            for (int i = 1; i < lw-1; i++) {
                for (int j = 1; j < lk-1; j++)
                {
                    float avrage = 0.299 * Rs[i][j] + 0.587*Gs[i][j] + 0.114* Bs[i][j] ;

                    float tresh = 100 ; 

                    if(avrage > tresh)
                        avrage = 255 ; 
                    else
                        avrage = 0 ; 
                    Rs[i][j] = avrage;
                    Gs[i][j] = avrage;
                    Bs[i][j] = avrage;
                }
            }
            break;
        case GLFW_KEY_F:
           MatrixTransform(Border);
            break;
        case GLFW_KEY_C:
           MatrixTransform(Contrast);
            break;
        case GLFW_KEY_E:
           MatrixTransform(Emboss);
            break;
        case GLFW_KEY_Q:
           MatrixTransform(Emboss);
            break;
        }
    }
}
int main(void)
{
    ifstream plik("D:/Scripts/cpp/OpenGL/GLFW_GUI_SPR_1/obraz3.txt");
    plik >> lw >> lk;
    cout << "wiersze=" << lw << " kolumny=" << lk << endl;
    for (int i = 0; i < lw; ++i)
        for (int j = 0; j < lk; ++j)
        {
            plik >> Rs[i][j];
            plik >> Gs[i][j];
            plik >> Bs[i][j];
        }
    plik.close();
    for (int i = 0; i < lw; ++i)
        for (int j = 0; j < lk; ++j)
        {
            R_copy[i][j] = Rs[i][j];
            G_copy[i][j] = Gs[i][j];
            B_copy[i][j] = Bs[i][j];
        }
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Przykladowe Okno GLFW", NULL, NULL);

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
        glOrtho(0,k,0,w, 2.0, 100.0);

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
