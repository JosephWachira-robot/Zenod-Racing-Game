#include <iostream>
#include <unistd.h>     // for usleep()
#include <termios.h>    // for terminal input
#include <fcntl.h>      // for non-blocking input
#include <cstdlib>
#include <ctime>

using namespace std;

const int width = 20;
const int height = 20;
int carPos = width / 2;
bool gameOver = false;
int score = 0;
char road[height][width];

void setup() {
    srand(time(0));
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            road[i][j] = ' ';
}

// Cross-platform input functions for Linux
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
    system("clear");  // for Linux
    cout << "====== ZENOD ======\n";
    cout << "Score: " << score << "\n";
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << "\n";

    for (int i = 0; i < height; i++) {
        cout << "#";
        for (int j = 0; j < width; j++) {
            if (i == height - 1 && j == carPos)
                cout << "A"; // Car
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
        switch (key) {
            case 'a':
                if (carPos > 0) carPos--;
                break;
            case 'd':
                if (carPos < width - 1) carPos++;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

void logic() {
    // Shift road down
    for (int i = height - 2; i >= 0; i--)
        for (int j = 0; j < width; j++)
            road[i + 1][j] = road[i][j];

    // Generate new row
    for (int j = 0; j < width; j++)
        road[0][j] = (rand() % 30 == 0) ? '#' : ' ';

    // Check collision
    if (road[height - 1][carPos] == '#')
        gameOver = true;
    else
        score++;
}

int main() {
    setup();
    initTermios(true);  // Start raw input mode

    while (!gameOver) {
        draw();
        input();
        logic();
        usleep(100000); // 100ms
    }

    initTermios(false); // Restore terminal
    cout << "\nGame Over! Final Score: " << score << "\n";
    return 0;
}
