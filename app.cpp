//#include <app.hh>
//#include <iostream>
//#include <utility.hh>
//#include <chrono>
//#define GLEW_NO_GLUE
//#include <GL/glew.h>

//App::App():
//    window(sf::VideoMode(800, 500), "game2", sf::Style::Default, sf::ContextSettings(0, 0, 4, 3, 3,
//                                                                                     sf::ContextSettings::Core))
//{
//    auto vMode = sf::VideoMode::getDesktopMode();
//    window.setPosition(sf::Vector2i(vMode.width/2 - window.getSize().x/2,
//                                    vMode.height/2 - window.getSize().y/2));
//    window.setVerticalSyncEnabled(true);
//    window.setKeyRepeatEnabled(false);

//    setOpengl();

//    try
//    {
//        avr = std::make_unique<SerialPort>("/dev/ttyUSB0", 9600, 20);
//    }
//    catch(const std::exception& e)
//    {
//        std::cout << e.what() << std::endl;
//    }

//    loop();
//}

//void App::loop()
//{
//    auto currentTime = std::chrono::high_resolution_clock::now();

//    while(isRunning)
//    {
//        processInput();

//        auto newTime = std::chrono::high_resolution_clock::now();
//        float frameTime = std::chrono::duration<float>(newTime- currentTime).count();
//        currentTime = newTime;

//        update(frameTime);

//        render();
//    }
//}

//void App::processInput()
//{
//    inputPack.keyboard.update();
//    inputPack.winMouse.update();

//    sf::Event event;
//    while(window.pollEvent(event))
//    {
//        if(event.type == sf::Event::Closed)
//        {
//            isRunning = false;
//        }
//        else if(event.type == sf::Event::Resized)
//        {
//            viewport = setViewport(aspect, window);
//        }
//        else if(event.type == sf::Event::LostFocus)
//        {

//        }
//        else if(event.type == sf::Event::KeyPressed)
//        {
//            inputPack.keyboard.keyDownEvent(event.key.code);
//        }
//        else if(event.type == sf::Event::KeyReleased)
//        {
//            inputPack.keyboard.keyUpEvent(event.key.code);
//        }
//        else if(event.type == sf::Event::MouseButtonPressed)
//        {
//            glm::vec2 pos(event.mouseButton.x, event.mouseButton.y);
//            if(is_in_viewport(pos, viewport))
//                inputPack.winMouse.buttonDownEvent(event.mouseButton.button);
//        }
//        else if(event.type == sf::Event::MouseButtonReleased)
//        {
//            inputPack.winMouse.buttonUpEvent(event.mouseButton.button);
//        }
//        else if(event.type == sf::Event::MouseWheelScrolled)
//        {
//            inputPack.winMouse.setWheelDelta(event.mouseWheelScroll.delta);
//        }
//        else if(event.type == sf::Event::MouseMoved)
//        {
//            glm::vec2 pos(event.mouseMove.x, event.mouseMove.y);
//            if(is_in_viewport(pos, viewport))
//                inputPack.winMouse.setPosition(pos);
//        }
//    }
//    if(avr)
//    {
//        inputPack.avrInput.update();
//        try
//        {
//            avr->writeData('C');

//            std::string events = avr->readData(1);
//            for(auto event: events)
//            {
//                if(event < 91)
//                    inputPack.avrInput.keyDownEvent(event);
//                else
//                    inputPack.avrInput.keyUpEvent(event - 32);
//            }
//        }
//        catch(const std::exception&)
//        {
//            avr.reset();
//            std::cout << "controller disconnected" << std::endl;
//        }
//    }

//    if(!gui.menus.empty())
//        gui.menus.front().processInput(inputPack);
//    else
//        gameScene->processInput(inputPack);
//}

//void App::setOpengl()
//{
//    glewExperimental = GL_TRUE;
//    GLenum err = glewInit();
//    if (GLEW_OK != err)
//        throw std::runtime_error("glewInit failed");

//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glClearColor(0.f, 0.f, 0.f, 0.f);
//}

//void App::update(float frameTime)
//{
//    if(!gui.menus.empty())
//        gui.menus.front().update(frameTime);
//    else
//        gameScene->update(frameTime);
//}

//void App::render()
//{
//    glClear(GL_COLOR_BUFFER_BIT);

//    for(auto& menu: gui.menus)
//    {
//        menu.render();
//        if(menu.isOpaque)
//            return;
//    }
//    gameScene->render();

//    window.display();
//}
