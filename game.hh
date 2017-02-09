#ifndef GAME_HH
#define GAME_HH

#include <SFML/Window/Keyboard.hpp>
#include <input.hh>
#include <window.hh>
#include <serialport.hh>
#include <memory>
#include <texture.hh>
#include <shaderprogram.hh>
#include <spriterenderer.hh>
#include <postprocessor.hh>
#include <font.hh>

// temp level
#include <paddle.hh>
#include <ball.hh>
#include <brick.hh>
#include <vector>
#include <utility.hh>
#include <explosion.hh>
#include <particle.hh>

class Game
{
public:
    Game(unsigned width, unsigned height, const std::string& title);

private:
    void enter();
    void processInput();
    void updateLogic(float dt);
    void update_one_per_frame(float frameTime);
    void doCollisions();
    void interpolate(float alpha);
    void render();
    void init();

    // must be initialized before opengl resources
    Window window;
    // ...
    bool quitRequest;
    std::unique_ptr<SerialPort> controller;
    Input<sf::Keyboard::Key, std::hash<std::underlying_type_t<sf::Keyboard::Key>>> input1;
    Input<char, std::hash<char>> input2;
    TexMan textures;
    ShaderMan shaders;
    SpriteRenderer renderer;
    PostProcessor pProcessor;
    FontEngine fEngine;

    // temp level
    std::unique_ptr<Paddle> paddle;
    std::unique_ptr<Ball> ball;
    std::vector<Brick> bricks;
    std::vector<Explosion> explos;
    std::unique_ptr<Sprite> background;
    std::unique_ptr<ParticleGenerator> emiter;
    std::unique_ptr<ParticleGenerator> snow;
    std::unique_ptr<ParticleGenerator> gFlame;
};

#endif // GAME_HH
