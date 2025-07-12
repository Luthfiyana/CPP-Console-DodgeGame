#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <algorithm>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 10;

class Obstacle
{
public:
    int x, y;
    char symbol;

    Obstacle(int startX, char sym) : x(startX), y(rand() % HEIGHT), symbol(sym) {}

    virtual void move() { x--; }
    virtual bool isHarmful() = 0;
};

// Berbahaya
class HarmfulObstacle : public Obstacle
{
public:
    HarmfulObstacle(int startX) : Obstacle(startX, 'X') {}
    bool isHarmful() override { return true; }
};

// Coin
class Coin : public Obstacle
{
public:
    Coin(int startX) : Obstacle(startX, 'O') {}
    bool isHarmful() override { return false; }
};

// class player
class Player
{
public:
    int x, y, score;

    Player() : x(2), y(HEIGHT / 2), score(0) {}

    void moveUp()
    {
        if (y > 0)
            y--;
    }

    void moveDown()
    {
        if (y < HEIGHT - 1)
            y++;
    }
};

// class game:
class Game
{
private:
    Player player;
    vector<Obstacle *> obstacles;
    bool isRunning;

public:
    Game() { isRunning = true; }

    ~Game()
    {
        for (auto obs : obstacles)
            delete obs;
        cout << "Game Over! Final Score: " << player.score << endl;
    }

    // fungsi generate obstacle
    void generateObstacle()
    {
        if (rand() % 10 < 3)
        {
            if (rand() % 2 == 0)
                obstacles.push_back(new HarmfulObstacle(WIDTH - 1));
            else
                obstacles.push_back(new Coin(WIDTH - 1));
        }
    }

    // fungsi memperbarui posisi obstacle
    void update()
    {
        for (auto &obs : obstacles)
            obs->move();

        obstacles.erase(remove_if(obstacles.begin(), obstacles.end(), [](Obstacle *obs)
                                  { return obs->x < 0; }),
                        obstacles.end());
        for (auto it = obstacles.begin(); it != obstacles.end();)
        {
            if ((*it)->x == player.x && (*it)->y == player.y)
            {
                if ((*it)->isHarmful())
                    isRunning = false; // kena rintangan
                else
                    player.score += 10; // ambil koin

                delete *it;
                it = obstacles.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // fungsi menggambar area permainan
    void draw()
    {
        system("cls");
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < HEIGHT; j++)
            {
                if (i == player.y && j == player.x)
                    cout << "P";
                else
                {
                    bool printed = false;
                    for (const auto &obs : obstacles)
                    {
                        if (obs->x == j && obs->y == i)
                        {
                            cout << obs->symbol;
                            printed = true;
                            break;
                        }
                    }
                    if (!printed)
                        cout << " ";
                }
            }
            cout << endl;
        }
        cout << "Score: " << player.score << endl;
    }

    // fungsi membaca input dari keyboard
    void input()
    {
        if (_kbhit())
        {
            char key = _getch();
            if (key == 'w')
                player.moveUp(); // player bergerak ke atas
            if (key == 's')
                player.moveDown(); // player bergerak ke bawah
            if (key == 'q')
                isRunning = false; // menghentikan game
        }
    }

    // fungsi run
    void run()
    {
        while (isRunning)
        {
            generateObstacle();
            update();
            draw();
            input();
            Sleep(100);
        }
    }
};

// fungsi Main
int main()
{
    srand(time(0));
    Game game;
    game.run();
    return 0;
}