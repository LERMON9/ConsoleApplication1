#include <iostream>  
#include <conio.h>  // для ввода с клавиатуры
#include <windows.h>  
using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN }; // Змейка не будет остонавливатся
eDirection dir;

// Скрытие курсора  
void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;  // Скрыть курсорчик, но тредуется нажать х  
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// Установка цвета  
void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Setup() {
    gameOver = false;  // Если gameOver будет True(Правда) то игра закончится
    dir = STOP;  
    x = width / 2;  // Начальные координатики змеи по оси X  
    y = height / 2; // Начальные координатики змеи по оси Y  
    fruitX = rand() % width;  // Установка рандомной позиции фруктика
    fruitY = rand() % height; // Установка рандомной позиции фруктика
    score = 0; // Счетчик, когда соберете 1 фруктик он даст 10 очков
    nTail = 0; 
    HideCursor(); // Скрываем курсор  
}

void Draw() {
    system("cls"); // Очисточка экранчика  
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << "\n";

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#";
            if (i == y && j == x) {
                SetColor(1); // Устанавливаем синенький цветик для головы  
                cout << "O"; // Головка  
                SetColor(7); // Возвращаем цветик по умолчанию  
            }
            else if (i == fruitY && j == fruitX) {
                SetColor(2); // Устанавливаем зеленинький цветик для фруктика
                cout << "+"; // Фруктик  
                SetColor(7); // Возвращаем цветик по умолчанию 
            }
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        SetColor(6); // Устанавливаем желтенький цветик для хвостика
                        cout << "o"; // Хвостик  
                        SetColor(7); // Возвращаем цветик по умолчанию  
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }
            if (j == width - 1)
                cout << "#";
        }
        cout << "\n";
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << "\n";
    cout << "Score: " << score << "\n";
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 72: // Стрелка вверх  
            dir = UP;
            break;
        case 80: // Стрелка вниз  
            dir = DOWN;
            break;
        case 75: // Стрелка влево  
            dir = LEFT;
            break;
        case 77: // Стрелка вправо  
            dir = RIGHT;
            break;
        case 'x':
            gameOver = true; // Завершение игры  
            break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x; // Сохраняем текущее положение головки 
    tailY[0] = y; // Сохраняем текущее положение головки

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX; // Обновляем хвостик  
        tailY[i] = prevY; // Обновляем хвостик  
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Изменяем положение в зависимости от направления  
    switch (dir) {
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    }

    // Вышла ли змея за границы?  
    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    // Ударилась ли змея об хвостик?  
    for (int i = 1; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

    // Съела ли змейка еду?  
    if (x == fruitX && y == fruitY) {
        score += 10;
        nTail++; // Увеличиваем длину хвостика  

        // Генерируем новые координаты для фруктика  
        do {
            fruitX = rand() % width;
            fruitY = rand() % height;
        } while ((fruitX == x && fruitY == y) ||
            count(tailX, tailX + nTail, fruitX) > 0 ||
            count(tailY, tailY + nTail, fruitY) > 0);
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(190); // Замедление игры  
    }
    return 0;
}