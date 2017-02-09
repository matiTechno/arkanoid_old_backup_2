#ifndef APP_HH
#define APP_HH

#include <input.hh>
#include <mouse.hh>
#include <SFML/Window.hpp>
#include <serialport.hh>
#include <orthocamera.hh>
#include <deque>

struct InputPack
{
    Input<sf::Keyboard::Key, std::hash<std::underlying_type_t<sf::Keyboard::Key>>> keyboard;
    Input<char, std::hash<char>> avrInput;
    WinMouse<sf::Mouse::Button> winMouse;
};

class Layer
{
public:
    virtual void render() = 0;
    virtual void update(float frameTime) = 0;
    virtual void processInput(const InputPack& inputPack) = 0;
};

class Game: public Layer
{
public:
    OrthoCamera camera;
    bool is_mouse_visible;
};

class Menu: public Layer
{
public:
    bool isOpaque;
};

class GuiSystem
{
public:
    std::deque<Menu> menus;

    //OrthoCamera camera;
    bool is_mouse_visible;
};


class App
{
public:
    App();

private:
    InputPack inputPack;
    std::unique_ptr<SerialPort> avr;
    float aspect;
    // viewport is in the center of window
    // we can assume that xy represent top-left corner
    // isntead of lower-left as opengl says
    // zw is viewport size
    glm::vec4 viewport;
    sf::Window window;
    bool isRunning;

    void loop();
    void setOpengl();
    void processInput();
    void update(float frameTime);
    void render();

    GuiSystem gui;
    std::unique_ptr<Game> gameScene;
    //void setMouseCursorVisible(bool visible);
};

#endif // APP_HH
