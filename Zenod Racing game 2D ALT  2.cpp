#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cstdlib>
#include <ctime>
#include <thread>

using namespace std;

const int width = 30;
const int height = 20;
int carPos = width / 2;
bool gameOver = false;
int score = 0;
char road[height][width];

// ASCII Car (3 characters wide)
string car[1] = { "<►>" };

// 🎵 Background music thread
void playMusic() {
    system("mpg123 -q music.mp3 &");  // You can replace this with `aplay sound.wav &`
}

void setup() {
    srand(time(0));
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            road[i][j] = ' ';
    playMusic();
}

void initTermios(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

bool kbhit() {
    struct termios oldt, newt;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }
    return false;
}

char getch() {
    char buf = 0;
    read(0, &buf, 1);
    return buf;
}

void draw() {
    system("clear");
    cout << "====== ZENOD: Tarmac Fury ======\n";
    cout << "Score: " << score << "\n";
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << "\n";

    for (int i = 0; i < height; i++) {
        cout << "#";
        for (int j = 0; j < width; j++) {
            // Car rendering (3 chars wide)
            if (i == height - 1 && j == carPos - 1 && carPos > 0 && carPos < width - 1)
                cout << car[0];
            else
                cout << road[i][j];
        }
        cout << "#\n";
    }

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << "\n";
}

void input() {
    if (kbhit()) {
        char key = getch();
        // Arrow keys send 3 chars: '\033' '[' 'A'..'D'
        if (key == '\033') {
            getch(); // skip [
            switch (getch()) {
                case 'C': // right
                    if (carPos < width - 2) carPos++;
                    break;
                case 'D': // left
                    if (carPos > 1) carPos--;
                    break;
            }
        } else if (key == 'x') {
            gameOver = true;
        }
    }
}

void logic() {
    for (int i = height - 2; i >= 0; i--)
        for (int j = 0; j < width; j++)
            road[i + 1][j] = road[i][j];

    for (int j = 0; j < width; j++) {
        // Sidewalk
        if (j == 0 || j == width - 1)
            road[0][j] = '=';
        // Lane markings
        else if (j == width / 2 && rand() % 2 == 0)
            road[0][j] = '|';
        // Obstacles
        else
            road[0][j] = (rand() % 40 == 0) ? '#' : ' ';
    }

    // Check collision
    if (road[height - 1][carPos] == '#' ||
        road[height - 1][carPos - 1] == '#' ||
        road[height - 1][carPos + 1] == '#') {
        gameOver = true;
    } else {
        score++;
    }
}

int main() {
    setup();
    initTermios(true);

    while (!gameOver) {
        draw();
        input();
        logic();
        usleep(120000); // 120ms
    }

    initTermios(false);
    cout << "\n💥 Game Over! Final Score: " << score << "\n";
    cout << "🎮 Thanks for playing Zenod!\n"; //You Can add your name here instead of Zenod
    system("pkill mpg123"); // Stop the music
    return 0;
}
