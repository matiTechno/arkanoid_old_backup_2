#ifndef FONT_HH
#define FONT_HH

#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <string>
#include <texture.hh>
#include <vector>
#include <glm/glm.hpp>
#include <vao.hh>
class ShaderProgram;

struct Text
{
    std::string filename;
    unsigned size;
    glm::vec2 position;
    glm::vec4 color;
    float scale;
    std::string data;
};

struct Character
{
    // texCoords.zw = size
    glm::vec4 texCoords;
    glm::uvec2 bearing;
    unsigned advanceX;
};

struct FontData
{
    Texture texture;
    unsigned newLineSpace;
    std::unordered_map<char, Character> characters;
};

class FontEngine
{
public:
    FontEngine();
    ~FontEngine();
    FontEngine(const FontEngine&) = delete;
    FontEngine& operator=(const FontEngine&) = delete;

    FontData& loadFont(const std::string& filename, unsigned size);
    void render(const ShaderProgram& shader, const Text& text);

private:
    FT_Library ftLib;
    FT_Error error;
    std::unordered_map<std::string, std::unordered_map<unsigned, FontData>> fonts;
    VAO vao;
    BO vbo;
};

#endif // FONT_HH
