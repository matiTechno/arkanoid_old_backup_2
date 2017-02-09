#include <game.hh>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

Game::Game(unsigned width, unsigned height, const std::string& title):
    window(width, height, title),
    quitRequest(false),
    textures("res"),
    shaders("res"),
    pProcessor(int(width), int(height), int(window.getWindow().getSettings().antialiasingLevel))
{
    init();
}

void Game::enter()
{
    using floatMilli = std::chrono::duration<float, std::milli>;
    const float dt = 8.333f;
    auto currentTime = std::chrono::high_resolution_clock::now();
    float accumulator = 0.f;

    while(!quitRequest)
    {
        processInput();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration_cast<floatMilli>(newTime - currentTime).count();
        currentTime = newTime;
        if(frameTime > 30.f)
            frameTime = 30.f;

        accumulator += frameTime;

        while(accumulator >= dt)
        {
            updateLogic(dt/1000.f);
            accumulator -= dt;
        }
        update_one_per_frame(frameTime/1000.f);
        interpolate(accumulator/dt);

        render();
    }
}

void Game::processInput()
{
    // temp level
    paddle->setVelocity(glm::vec2());

    input1.update();
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            quitRequest = true;

        else if(event.type == sf::Event::KeyPressed)
            input1.keyDownEvent(event.key.code);

        else if(event.type == sf::Event::KeyReleased)
            input1.keyUpEvent(event.key.code);
    }
    if(input1.wasKeyPressed(sf::Keyboard::Escape))
        quitRequest = true;

    if(controller)
    {
        input2.update();
        try
        {
            controller->writeData('C');

            std::string events = controller->readData(1);
            for(auto event: events)
            {
                if(event < 91)
                    input2.keyDownEvent(event);
                else
                    input2.keyUpEvent(event - 32);
            }
        }
        catch(const std::exception&)
        {
            controller.reset();
            std::cout << "controller disconnected" << std::endl;
        }
        // temp level
        if(input2.isKeyPressed('A'))
            paddle->setVelocity(glm::vec2(paddle->getVelocity().x - paddle->getVelMod(), 0.f));
        if(input2.isKeyPressed('D'))
            paddle->setVelocity(glm::vec2(paddle->getVelocity().x + paddle->getVelMod(), 0.f));
        if(input2.wasKeyPressed('S'))
            ball->free();
    }
    // temp level
    else
    {
        if(input1.isKeyPressed(sf::Keyboard::A))
            paddle->setVelocity(glm::vec2(paddle->getVelocity().x - paddle->getVelMod(), 0.f));
        if(input1.isKeyPressed(sf::Keyboard::D))
            paddle->setVelocity(glm::vec2(paddle->getVelocity().x + paddle->getVelMod(), 0.f));
        if(input1.wasKeyPressed(sf::Keyboard::Space))
            ball->free();
    }
}

void Game::updateLogic(float dt)
{
    // temp level
    paddle->update(dt);
    ball->update(dt);
    for(auto& brick: bricks)
        brick.update(dt);
    for(auto& ex: explos)
        ex.update(dt);
    explos.erase(std::remove_if(explos.begin(), explos.end(),
                                [](const Explosion& e){return e.isEnd();}),
                 explos.end());
    doCollisions();

    (void)dt;
}

void Game::update_one_per_frame(float frameTime)
{
    emiter->update(frameTime);
    snow->update(frameTime);
    gFlame->update(frameTime);
}

void Game::interpolate(float alpha)
{
    // temp level
    paddle->updateSprite(alpha);
    ball->updateSprite(alpha);
    for(auto& brick: bricks)
        brick.updateSprite(alpha);
    for(auto& ex: explos)
        ex.updateSprite(alpha);

    (void)alpha;
}

void Game::render()
{
    window.clear();
    pProcessor.begRender();

    // temp level
    auto& shader = shaders.get("default_shader");

    renderer.render(shader, *background.get());
    renderer.render(shader, paddle->getSpriteC());
    renderer.render(shader, ball->getSpriteC());
    for(auto& brick: bricks)
        renderer.render(shader, brick.getSpriteC());
    for(auto& ex: explos)
        renderer.render(shader, ex.getSpriteC());
    emiter->render(shaders.get("particle_shader"));
    snow->render(shaders.get("particle_shader"));
    gFlame->render(shaders.get("particle_shader"));

    Text text{"res/AbyssinicaSIL-R.ttf", 24, glm::vec2(100.f, 100.f), glm::vec4(100.f, 100.f, 255.f, 1.f), 1.f,
                "game programmer\nmatimaci"};
    fEngine.render(shaders.get("font_shader"), text);

    pProcessor.endRender();
    pProcessor.render();
    window.display();
}

void Game::init()
{
    try
    {
        controller = std::make_unique<SerialPort>("/dev/ttyUSB0", 9600, 20);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    glm::mat4 proj = glm::ortho(0.f, float(window.getSize().x), float(window.getSize().y), 0.f);
    shaders.get("default_shader").bind();
    glUniformMatrix4fv(shaders.get("default_shader").getUniLoc("projection"), 1, GL_FALSE, &proj[0][0]);
    shaders.get("particle_shader").bind();
    glUniformMatrix4fv(shaders.get("particle_shader").getUniLoc("projection"), 1, GL_FALSE, &proj[0][0]);
    pProcessor.setShader(shaders.get("postp_shader"));
    shaders.get("font_shader").bind();
    glUniformMatrix4fv(shaders.get("font_shader").getUniLoc("projection"), 1, GL_FALSE, &proj[0][0]);

    fEngine.loadFont("res/AbyssinicaSIL-R.ttf", 48);

    // temp level
    background = std::make_unique<Sprite>();
    background->texture = &textures.get("background1");
    background->size = window.getSize();
    background->texCoords = glm::uvec4(0, 72, 448, 448);
    background->color = glm::vec4(166.f, 67.f, 33.f, 0.3f);
    Sprite ballSp;
    ballSp.texture = &textures.get("pieces2");
    ballSp.texCoords = glm::uvec4(400, 128, 16, 16);
    ball = std::make_unique<Ball>(std::move(ballSp));
    ball->setSize(glm::vec2(22.f, 22.f));
    Sprite paddleSp;
    paddleSp.texture = &textures.get("pieces");
    paddleSp.texCoords = glm::uvec4(116, 72, 64, 16);
    paddle = std::make_unique<Paddle>(std::move(paddleSp), *ball.get());
    paddle->setSize(glm::vec2(96.f, 24.f));
    paddle->setPosition(glm::vec2(200.f, 400.f));
    paddle->setVelMod(180.f);
    paddle->setMinReflectAngle(glm::pi<float>()/6.f);
    ball->setVelocity(glm::vec2(150.f, -150.f));
    ball->setPaddle(paddle.get());
    ball->reset();
    Sprite sp1;
    sp1.texture = &textures.get("pieces");
    sp1.texCoords = glm::uvec4(8, 8, 32, 16);
    bricks.push_back(sp1);
    bricks.push_back(sp1);
    bricks.push_back(sp1);
    bricks[0].setPosition(glm::vec2(10.f, 10.f));
    bricks[0].setSize(glm::vec2(30.f, 470.f));
    bricks[1].setPosition(glm::vec2(460.f, 0.f));
    bricks[1].setSize(glm::vec2(30.f, 470.f));
    bricks[2].setPosition(glm::vec2(10.f, 10.f));
    bricks[2].setSize(glm::vec2(480.f, 30.f));
    Sprite sp8;
    sp8.texture = &textures.get("terrain_atlas");
    sp8.texCoords = glm::vec4(544.f, 394.f, 32.f, 116.f);
    bricks.push_back(sp8);
    bricks[3].setPosition(glm::vec2(60.f, window.getSize().y - 116.f));
    bricks[3].setSize(glm::vec2(32.f, 116.f));
    {
        Animation anim(2.f, loadTexCoords("res/heart.sprites", std::vector<float>{0.4f, 0.1f, 0.1f}),
                       true, Origin::middle);
        Sprite sprite;
        sprite.texture = &textures.get("heart");
        Explosion e(sprite, anim);
        e.setPosition(glm::vec2(400.f, 460.f));
        explos.push_back(e);
    }
    {
        Animation anim(1.f, loadTexCoords("res/book.sprites", 0.1f), true, Origin::bottom);
        Sprite sprite;
        sprite.texture = &textures.get("book");
        Explosion e(sprite, anim);
        e.setPosition(glm::vec2(330.f, 480.f));
        explos.push_back(e);
    }
    {
        Animation anim(2.f, loadTexCoords("res/coin.sprites", 0.1f), true, Origin::middle);
        Sprite sprite;
        sprite.texture = &textures.get("coin");
        Explosion e(sprite, anim);
        e.setPosition(glm::vec2(280.f, 460.f));
        explos.push_back(e);
    }
    PData pData{glm::vec2(1.f, 5.f), glm::vec4(200.f, 10.f, 10.f, 0.1f), glm::vec4(255.f, 80.f, 30.f, 0.3f),
                glm::vec2(4.f, 8.f), &textures.get("particle"), glm::vec4(-5.f, -10.f, 5.f, -30.f),
                glm::vec4(-1.f, -0.5f, 1.f, -5.f),
                [](float dt, Particle& p)
        {
            if(p.maxLife > 6.f && p.life < 3.f)
            {
                p.color.b += dt * 40.f;
                p.acceleration.x = -5.f;
            }
        }};

    emiter = std::make_unique<ParticleGenerator>(0.0008f, bricks[3].getPosition(),
            -1000.f, glm::vec4(8.f, 0.f, 16.f, 16.f), true, GL_SRC_ALPHA, GL_ONE, pData);

    PData snowData{glm::vec2(1.f, 4.f), glm::vec4(200.f, 200.f, 200.f, 1.f), glm::vec4(200.f, 200.f, 250.f, 1.f),
                glm::vec2(30.f, 30.f), &textures.get("particle2"), glm::vec4(0.f, 20.f, 0.f, 40.f),
                glm::vec4(), [this](float, Particle& p)
        {
            if(p.position.y > this->window.getSize().y)
                p.life = 0.f;
        }};

    snow = std::make_unique<ParticleGenerator>(0.2f, glm::vec2(0.f, 0.f),
                                               -1000.f, glm::vec4(0.f, 0.f, window.getSize().x, 3.f),
                                               false, GL_SRC_ALPHA, GL_ONE, snowData);

    PData gFlameData{glm::vec2(1.f, 5.f), glm::vec4(10.f, 200.f, 10.f, 0.1f), glm::vec4(80.f, 255.f, 30.f, 0.3f),
                glm::vec2(0.5f, 1.f), &textures.get("particle"), glm::vec4(40.f, 40.f, -40.f, -40.f), glm::vec4(), nullptr};

    gFlame = std::make_unique<ParticleGenerator>(0.002f, glm::vec2(),
                                                 -1000.f, glm::vec4(0.f, 0.f, ball->getSize()),
                                                 true, GL_SRC_ALPHA, GL_ONE, gFlameData,
                                                 [this](float)
    {
        gFlame->setPosition(ball->getPosition());
    });

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    enter();
}

void Game::doCollisions()
{
    if(ball->getPosition().y > window.getSize().y)
    {
        // temp animation
        Animation anim(1.f, loadTexCoords("res/effect2.sprites", 0.03f), false, Origin::bottom);
        Sprite sprite;
        sprite.texture = &textures.get("effect2");
        Explosion e(sprite, anim);
        e.setPosition(glm::vec2(ball->getPosition().x, window.getSize().y));
        e.set();
        explos.push_back(e);

        ball->reset();
        return;
    }

    auto coll1 = ballRectCollision(*ball.get(), *paddle.get());
    if(coll1.isCollision)
    {
        if(!ball->isImmune())
        {
            paddle->reflect(coll1.penetration);
            ball->setPosition(ball->getPrevPosition());
            ball->setImmune(0.5);

            //temp animation
            Animation anim(0.75f, loadTexCoords("res/effect3.sprites", 0.06f), false, Origin::middle);
            Sprite sprite;
            sprite.texture = &textures.get("effect3");
            Explosion e(sprite, anim);
            e.setPosition(coll1.point);
            e.set();
            explos.push_back(e);
        }
    }
    for(auto& brick: bricks)
    {
        auto coll2 = ballRectCollision(*ball.get(), brick);
        if(coll2.isCollision)
        {
            reflectVel(*ball.get(), coll2.penetration);
            ball->setPosition(ball->getPrevPosition());

            // temp animation
            Animation anim(1.f, loadTexCoords("res/effect1.sprites", 0.035f), false, Origin::middle);
            Sprite sprite;
            sprite.texture = &textures.get("effect1");
            Explosion e(sprite, anim);
            e.setPosition(coll2.point);
            e.set();
            explos.push_back(e);
        }
    }

    for(auto& brick: bricks)
    {
        auto coll3 = rectRectCollision(*paddle.get(), brick);
        if(coll3.isCollision)
        {
            paddle->setVelocity(glm::vec2());

            if(ball->isStuck())
            {
                glm::vec2 dPos = paddle->getPrevPosition() - paddle->getPosition();
                ball->setPosition(ball->getPosition() + dPos);
            }
            paddle->setPosition(paddle->getPrevPosition());
        }
    }
}
