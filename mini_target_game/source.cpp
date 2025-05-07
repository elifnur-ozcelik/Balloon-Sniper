/*
    Project Name : Balloon Sniper
    Created by : Elifnur Ozcelik
    Description :
    Balloon Sniper is a festival-themed shooting game where players aim to hit balloons and win a plush bunny as a prize. 
    The player controls a gun that moves left and right, attempting to hit the balloons. 
    The balloons are arranged in two rows: one row moves to the right, and the other moves to the left. 
    If the player successfully hits 20 or more balloons, a congratulatory message will appear on the screen, along with a plush bunny as a reward.
    
    Game Controls:
    - Use the LEFT and RIGHT arrow keys to move the shooter.
    - Press SPACE to fire a shot.
    - Press F1 to start or pause the game or to start a new game after it ends.

*/

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>

#define TIMER_PERIOD 20
#define MAX_BALLOONS 5
#define MIN_DISTANCE 100
#define FIRE_SPEED 10

int windowWidth = 900;
int windowHeight = 700;

typedef struct {
    float x, y;
    int r, g, b;
    int direction;
    int alive;
} Balloon;

typedef struct {
    float x, y;
    int active;
} Fire;

Balloon balloons[MAX_BALLOONS];
Fire fire = { 115, 100, 0 };
float shooterX = 100;
int score = 0;
int remainingTime = 30;
time_t startTime, currentTime;
int gameOver = 0;
int gamePaused = 0;

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

void circle(int x, int y, int r) {
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        angle = 2.0f * 3.1415926f * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void drawBalloon(Balloon b) {
    if (!b.alive) return;

    glColor3ub(b.r, b.g, b.b);
    circle(b.x, b.y, 40);

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(b.x, b.y - 40);
    glVertex2f(b.x, b.y - 80);
    glEnd();

    glColor3ub(b.r, b.g, b.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(b.x, b.y - 35);
    glVertex2f(b.x - 10, b.y - 50);
    glVertex2f(b.x + 10, b.y - 50);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    circle(b.x + 15, b.y + 15, 8);
}

int checkOverlap(int index, float newX, float newY) {
    for (int i = 0; i < MAX_BALLOONS; i++) {
        if (i != index && balloons[i].alive) {
            float dx = balloons[i].x - newX;
            float dy = balloons[i].y - newY;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < MIN_DISTANCE) {
                return 1;
            }
        }
    }
    return 0;
}

void resetBalloon(int i) {
    float newX, newY;
    int attempt = 0;
    do {
        if (i % 2 == 0) {
            balloons[i].direction = -1;
            newX = windowWidth + rand() % 200;
            newY = windowHeight - 180 + rand() % 20;
        }
        else {
            balloons[i].direction = 1;
            newX = -rand() % 200 - 40;
            newY = windowHeight - 260 + rand() % 20;
        }
        attempt++;
        if (attempt > 50) break;
    } while (checkOverlap(i, newX, newY));

    balloons[i].x = newX;
    balloons[i].y = newY;
    balloons[i].r = rand() % 256;
    balloons[i].g = rand() % 256;
    balloons[i].b = rand() % 256;
    balloons[i].alive = 1;
}

void drawFire() {
    if (fire.active) {
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(fire.x - 5, fire.y);
        glVertex2f(fire.x - 5, fire.y + 15);
        glVertex2f(fire.x + 5, fire.y + 15);
        glVertex2f(fire.x + 5, fire.y);
        glEnd();
    }
}

void drawEar(float baseX, float baseY, float leftX, float leftY, float rightX, float rightY) {
    glBegin(GL_TRIANGLES);
    glVertex2f(baseX, baseY);
    glVertex2f(leftX, leftY);
    glVertex2f(rightX, rightY);
    glEnd();
}

void drawBunny() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1.0f, 0.75f, 0.8f);
    circle(windowWidth / 2, windowHeight / 2 + -20, 100);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(windowWidth / 2, windowHeight / 2 + -20, 60);

    glColor3f(1.0f, 0.75f, 0.8f);
    circle(windowWidth / 2, windowHeight / 2 + 90.0f, 60);

    glColor3f(1.0f, 0.75f, 0.8f);
    circle(windowWidth / 2 + 80, windowHeight / 2, 20);
    glColor3f(1.0f, 1.0f, 1.0f);
    circle(windowWidth / 2 + 80, windowHeight / 2, 12);

    glColor3f(1.0f, 0.75f, 0.8f);
    circle(windowWidth / 2 - 80, windowHeight / 2, 20);
    glColor3f(1.0f, 1.0f, 1.0f);
    circle(windowWidth / 2 - 80, windowHeight / 2, 12);

    glColor3f(1.0f, 0.75f, 0.8f);
    circle(windowWidth / 2 + 40, windowHeight / 2 - 120, 24);
    glColor3f(1.0f, 1.0f, 1.0f);
    circle(windowWidth / 2 + 40, windowHeight / 2 - 120, 14);
    glColor3f(1.0f, 0.75f, 0.8f);
    circle(windowWidth / 2 - 40, windowHeight / 2 - 120, 24);
    glColor3f(1.0f, 1.0f, 1.0f);
    circle(windowWidth / 2 - 40, windowHeight / 2 - 120, 14);

    glColor3f(1.0f, 1.0f, 1.0f);
    circle(windowWidth / 2 + 20, windowHeight / 2 + 90.0f + 10, 10);
    circle(windowWidth / 2 - 20, windowHeight / 2 + 90.0f + 10, 10);

    glColor3f(0.0f, 0.0f, 0.0f);
    circle(windowWidth / 2 + 20, windowHeight / 2 + 90.0f + 10, 5);
    circle(windowWidth / 2 - 20, windowHeight / 2 + 90.0f + 10, 5);

    glColor3f(1.0f, 0.75f, 0.8f);
    drawEar(windowWidth / 2 + 20, windowHeight / 2 + 90.0f + 60 - 10,
        windowWidth / 2 + 30, windowHeight / 2 + 90.0f + 60 + 70,
        windowWidth / 2 + 50, windowHeight / 2 + 90.0f + 60 + 70);

    drawEar(windowWidth / 2 - 20, windowHeight / 2 + 90.0f + 60 - 10,
        windowWidth / 2 - 30, windowHeight / 2 + 90.0f + 60 + 70,
        windowWidth / 2 - 50, windowHeight / 2 + 90.0f + 60 + 70);

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(windowWidth / 2 - 20, windowHeight / 2 + 90.0f - 15);
    glVertex2f(windowWidth / 2 + 20, windowHeight / 2 + 90.0f - 15);
    glEnd();

    glutSwapBuffers();
}


void display() {
    glClearColor(0.7, 0.85, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    glColor3f(0.5f, 0.0f, 0.13f);
    glBegin(GL_QUADS);
    glVertex2f(120, height);
    glVertex2f(120, height - 120);
    glVertex2f(width, height - 120);
    glVertex2f(width, height);
    glEnd();

    glColor3f(0.56f, 0.93f, 0.56f);
    drawText(windowWidth / 2 -100, height -60, "Welcome to Spring Fest");

    glColor3f(0.5f, 0.0f, 0.13f);
    glBegin(GL_QUADS);
    glVertex2f(70, height / 2 - 60 - 40);
    glVertex2f(70, height / 2 + 60 - 40);
    glVertex2f(width - 90, height / 2 + 60 - 40);
    glVertex2f(width - 90, height / 2 - 60 - 40);
    glEnd();
    
    glColor3f(1.0f, 0.08f, 0.58f);
    drawText(width / 2 - 150, height / 2 -50, "Shoot 20 Balloons to Win a Plush Bunny!");

    glColor3f(0.5f, 0.0f, 0.13f);
    glBegin(GL_QUADS);
    glVertex2f(windowWidth / 2 + 300, 350);
    glVertex2f(windowWidth / 2 + 300, windowHeight);
    glVertex2f(windowWidth / 2 + 300 + 10, windowHeight);
    glVertex2f(windowWidth / 2 + 300 + 10, 350);
    glEnd();

    glColor3f(0.5f, 0.0f, 0.13f);
    glBegin(GL_QUADS);
    glVertex2f(windowWidth / 2 - 300, 350);
    glVertex2f(windowWidth / 2 - 300, windowHeight);
    glVertex2f(windowWidth / 2 - 300 + 10, windowHeight);
    glVertex2f(windowWidth / 2 - 300 + 10, 350);
    glEnd();

    glColor3f(0.5f, 0.0f, 0.13f);
    glBegin(GL_TRIANGLES);
    glVertex2f(120, height);
    glVertex2f(120, height - 120);
    glVertex2f(0, height - 120);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, 30);
    glVertex2f(0, 0);
    glVertex2f(width, 0);
    glVertex2f(width, 30);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    drawText(windowWidth * 0.25, windowHeight * 0.05 -30, "Press SPACE to fire | Press F1 to start-pause game");

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(shooterX, 100);
    glVertex2f(shooterX, 30);
    glVertex2f(shooterX + 30, 30);
    glVertex2f(shooterX + 30, 100);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(shooterX + 10, 100);
    glVertex2f(shooterX + 10, 115);
    glVertex2f(shooterX + 20, 115);
    glVertex2f(shooterX + 20, 100);
    glEnd();

    drawFire();

    for (int i = 0; i < MAX_BALLOONS; i++) {
        drawBalloon(balloons[i]);
    }
    
    glColor3f(0.05f, 0.05f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(width, 0);
    glVertex2f(width - 120, 0);
    glVertex2f(width - 120, height);
    glVertex2f(width, height);
    glEnd();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(width - 105, height - 50, "Remaining");
    drawText(width - 90,height - 70, "Time:");
    char timeText[50];
    sprintf_s(timeText, sizeof(timeText), "%d", remainingTime);
    drawText(width - 75, height - 90, timeText);

    drawText((width + (width - 120)) / 2 - 30, height / 2 + 10, "Score:");
    char scoreText[50];
    sprintf_s(scoreText, sizeof(scoreText), "%d", score);
    drawText((width + (width - 120)) / 2 - 10, height / 2 - 10, scoreText);

    if (remainingTime == 0 && score >= 20) {
        drawBunny();
        drawText(windowWidth - 580, windowHeight - 50, "Congratulations, You Won!");
    }
    glutSwapBuffers();
}

void onResize(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

int checkCollision(Balloon b) {
    if (!b.alive) return 0;

    float dx = fabs(fire.x - b.x);
    float dy = fabs(fire.y - b.y);
    if (dx < 40 && dy < 40) {
        return 1;
    }
    return 0;
}

void onTimer(int v) {
    if (gamePaused || gameOver) return;

    time(&currentTime);
    int elapsedTime = (int)difftime(currentTime, startTime);
    remainingTime = 30 - elapsedTime;

    if (remainingTime <= 0) {
        remainingTime = 0;
        gameOver = 1;
        printf("Game Over! Final Score: %d\n", score);
    }

    for (int i = 0; i < MAX_BALLOONS; i++) {
        if (balloons[i].alive) {
            if (balloons[i].direction == -1) {
                balloons[i].x -= 3;
                if (balloons[i].x < -40) {
                    resetBalloon(i);
                }
            }
            else {
                balloons[i].x += 3;
                if (balloons[i].x > windowWidth + 40) {
                    resetBalloon(i);
                }
            }
        }
    }

    if (fire.active) {
        fire.y += FIRE_SPEED;
        if (fire.y > windowHeight) {
            fire.active = 0;
        }
        else {
            for (int i = 0; i < MAX_BALLOONS; i++) {
                if (checkCollision(balloons[i])) {
                    balloons[i].alive = 0;
                    resetBalloon(i);
                    fire.active = 0;
                    score++;
                    break;
                }
            }
        }
    }
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    glutPostRedisplay();
}

void onKeyboard(unsigned char key, int x, int y) {
    if (key == 32 && !fire.active && !gameOver) {
        fire.x = shooterX + 15;
        fire.y = 100;
        fire.active = 1;
    }
}

void onSpecial(int key, int x, int y) {
    if (key == GLUT_KEY_F1) {
        if (gameOver) {
            score = 0;
            remainingTime = 30;
            gameOver = 0;
            gamePaused = 0;
            time(&startTime);
            for (int i = 0; i < MAX_BALLOONS; i++) {
                resetBalloon(i);
            }
            glutTimerFunc(TIMER_PERIOD, onTimer, 0);
        }
        else {
            gamePaused = !gamePaused;
            if (!gamePaused) {
                time_t resumeTime;
                time(&resumeTime);
                startTime += (resumeTime - currentTime);
                glutTimerFunc(TIMER_PERIOD, onTimer, 0);
            }
        }
    }
    if (key == GLUT_KEY_LEFT) {
        if (!gamePaused && !gameOver) {
            shooterX -= 10;
            if (shooterX < 0) shooterX = 0;
        }
    }
    if (key == GLUT_KEY_RIGHT) {
        if (!gamePaused && !gameOver) {
            shooterX += 10;
            if (shooterX > windowWidth - 30) shooterX = windowWidth - 30;
        }
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));
    for (int i = 0; i < MAX_BALLOONS; i++) {
        resetBalloon(i);
    }
    time(&startTime);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Balloon Shooting Game | created by Elifnur Ozcelik");
    glutDisplayFunc(display);
    glutReshapeFunc(onResize);
    glutKeyboardFunc(onKeyboard);
    glutSpecialFunc(onSpecial);
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    glutMainLoop();
    return 0;
}