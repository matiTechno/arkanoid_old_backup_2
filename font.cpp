#include <font.hh>
#include <shaderprogram.hh>

FontEngine::FontEngine()
{
    error = FT_Init_FreeType(&ftLib);
    if(error)
        throw std::runtime_error("FreeType init failed");

    vbo.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);

    vao.bind();
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    glEnableVertexAttribArray(0);
}

FontEngine::~FontEngine()
{
    FT_Done_FreeType(ftLib);
}

FontData& FontEngine::loadFont(const std::string& filename, unsigned size)
{
    auto it_o = fonts.find(filename);
    if(it_o !=fonts.end())
    {
        auto it_i = it_o->second.find(size);
        if(it_i != it_o->second.end())
        {
            it_i->second.texture.bind();
            return it_i->second;
        }
    }

    FT_Face face;
    error = FT_New_Face(ftLib, filename.c_str(), 0, &face);

    if(error == FT_Err_Unknown_File_Format)
        throw std::runtime_error("unsupported font format: " + filename);
    else if(error)
        throw std::runtime_error("font file could not be opened: " + filename);

    FT_Set_Pixel_Sizes(face, 0, size);

    unsigned newLineSpace = unsigned(face->size->metrics.height) >> 6;

    FT_GlyphSlot slot = face->glyph;

    unsigned maxSizeY = 0, summX = 0;

    for(std::size_t i = 32; i < 127; ++i)
    {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER))
            continue;

        summX += slot->bitmap.width;
        maxSizeY = std::max(maxSizeY, slot->bitmap.rows);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    Texture texture(GL_R8, GLsizei(summX), GLsizei(maxSizeY), GL_RED, GL_UNSIGNED_BYTE);

    std::unordered_map<char, Character> characters;

    unsigned x = 0;

    for(std::size_t i = 32; i < 127; ++i)
    {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER))
            continue;

        glTexSubImage2D(GL_TEXTURE_2D, 0, GLint(x), 0, GLsizei(slot->bitmap.width), GLsizei(slot->bitmap.rows),
                        GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);

        characters.emplace(char(i), Character{glm::vec4(x, 0.f, slot->bitmap.width, slot->bitmap.rows),
                                              glm::uvec2(slot->bitmap_left, slot->bitmap_top), unsigned(slot->advance.x) >> 6});

        x += slot->bitmap.width;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    FT_Done_Face(face);

    return fonts[filename].emplace(size, FontData{std::move(texture), newLineSpace, std::move(characters)}).first->second;
}

void FontEngine::render(const ShaderProgram& shader, const Text& text)
{
    shader.bind();
    auto& font = loadFont(text.filename, text.size);

    glUniform4f(shader.getUniLoc("spriteColor"),
                text.color.x/255.f, text.color.y/255.f, text.color.z/255.f, text.color.w);

    vao.bind();
    vbo.bind(GL_ARRAY_BUFFER);

    glm::vec2 pos = text.position;

    for(auto i: text.data)
    {
        if(i == '\n')
        {
            pos.x = text.position.x;
            pos.y += font.newLineSpace * text.scale;
            continue;
        }

        auto character = font.characters[i];

        GLfloat posX = pos.x + character.bearing.x * text.scale;
        GLfloat posY = pos.y + (character.texCoords.w - character.bearing.y) * text.scale;
        GLfloat w = character.texCoords.z * text.scale;
        GLfloat h = character.texCoords.w * text.scale;

        GLfloat vertices[] =
        {
            posX, posY, 0.f, 1.f,
            posX + w, posY, 1.f, 1.f,
            posX + w, posY - h, 1.f, 0.f,
            posX + w, posY - h, 1.f, 0.f,
            posX, posY -h, 0.f, 0.f,
            posX, posY, 0.f, 1.f
        };

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        pos.x += character.advanceX * text.scale;

        auto coords = character.texCoords;
        auto& tex = font.texture;

        glm::vec2 texSize(GLfloat(coords.z)/GLfloat(tex.getSize().x), GLfloat(coords.w)/GLfloat(tex.getSize().y));
        glm::vec2 texShift(GLfloat(coords.x)/GLfloat(tex.getSize().x), GLfloat(coords.y)/GLfloat(tex.getSize().y));
        glUniform2f(shader.getUniLoc("texSize"), texSize.x, texSize.y);
        glUniform2f(shader.getUniLoc("texShift"), texShift.x, texShift.y);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
