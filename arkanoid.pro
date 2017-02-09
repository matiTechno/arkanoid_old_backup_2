TEMPLATE = app
CONFIG += c++14 strict_c++
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    window.cpp \
    game.cpp \
    serialport.cpp \
    texture.cpp \
    shaderprogram.cpp \
    spriterenderer.cpp \
    vao.cpp \
    paddle.cpp \
    ball.cpp \
    brick.cpp \
    utility.cpp \
    level.cpp \
    animation.cpp \
    explosion.cpp \
    particle.cpp \
    postprocessor.cpp \
    font.cpp \
    app.cpp \
    orthocamera.cpp \
    viewport.cpp

LIBS += -lsfml-window -lsfml-system -lGLEW -lGL -lboost_system -lfreetype

QMAKE_CXXFLAGS += -pedantic

INCLUDEPATH += /home/mati/stb \
    /usr/include/freetype2

HEADERS += \
    window.hh \
    game.hh \
    input.hh \
    serialport.hh \
    texture.hh \
    shaderprogram.hh \
    sprite.hh \
    spriterenderer.hh \
    vao.hh \
    entity.hh \
    paddle.hh \
    ball.hh \
    brick.hh \
    utility.hh \
    level.hh \
    animation.hh \
    explosion.hh \
    particle.hh \
    postprocessor.hh \
    font.hh \
    app.hh \
    orthocamera.hh \
    mouse.hh \
    viewport.hh
