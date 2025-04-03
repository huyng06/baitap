#include <SDL.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>  // Thêm thư viện này để dùng remove_if

// Cài đặt cơ bản
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_SIZE = 50;
const int OBSTACLE_SIZE = 50;
const int LANE_COUNT = 3;
const int LANE_WIDTH = SCREEN_WIDTH / LANE_COUNT;
const int FPS = 60;  // Thêm khai báo FPS

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

struct Entity {
    int x, y, size;
    void draw(Uint8 r, Uint8 g, Uint8 b) const {
        SDL_Rect rect = {x, y, size, size};
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
};

Entity player;
std::vector<Entity> obstacles;
int playerLane = 1;
bool running = true;
float speed = 5.0f;
int score = 0;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Subway Surfer Mini", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    player = {LANE_WIDTH * playerLane + (LANE_WIDTH - PLAYER_SIZE)/2,
             SCREEN_HEIGHT - 150, PLAYER_SIZE};
    srand(static_cast<unsigned>(time(0)));
}

void drawRoad() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    SDL_Rect road = {0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &road);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 1; i < LANE_COUNT; ++i) {
        SDL_RenderDrawLine(renderer, i*LANE_WIDTH, SCREEN_HEIGHT/2, i*LANE_WIDTH, SCREEN_HEIGHT);
    }
}

void handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT: if (playerLane > 0) playerLane--; break;
                case SDLK_RIGHT: if (playerLane < LANE_COUNT-1) playerLane++; break;
            }
            player.x = LANE_WIDTH * playerLane + (LANE_WIDTH - PLAYER_SIZE)/2;
        }
    }
}

void update() {
    for (auto& obs : obstacles) obs.y += static_cast<int>(speed);

    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
        [](const Entity& obs) { return obs.y > SCREEN_HEIGHT; }), obstacles.end());

    if (rand() % 100 < 3) {
        obstacles.push_back({
            (rand() % LANE_COUNT) * LANE_WIDTH + (LANE_WIDTH - OBSTACLE_SIZE)/2,
            -OBSTACLE_SIZE,
            OBSTACLE_SIZE
        });
    }

    for (const auto& obs : obstacles) {
        if (abs(player.x - obs.x) < PLAYER_SIZE && abs(player.y - obs.y) < PLAYER_SIZE) {
            running = false;
        }
    }

    speed += 0.005f;
    score++;
}

void render() {
    drawRoad();
    player.draw(255, 215, 0);
    for (const auto& obs : obstacles) obs.draw(255, 0, 0);
    SDL_RenderPresent(renderer);
}

void clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    init();

    Uint32 frameStart;
    int frameTime;

    while (running) {
        frameStart = SDL_GetTicks();

        handleInput();
        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000/FPS) {
            SDL_Delay(1000/FPS - frameTime);
        }
    }

    std::cout << "Game Over! Final Score: " << score << std::endl;
    clean();
    return 0;
}
