#ifndef POSTPROCESSOR_HH
#define POSTPROCESSOR_HH

#define GLEW_NO_GLU
#include <GL/glew.h>
#include <memory>
#include <vao.hh>
#include <texture.hh>
#include <shaderprogram.hh>

class FrameBuffer
{
public:
    FrameBuffer();
    void bind(GLenum target) const;

private:
    std::unique_ptr<GLuint, std::function<void(GLuint*)>> ID;
};

class RenderBuffer
{
public:
    RenderBuffer();
    void bind() const;
    GLuint getID() const;

private:
    std::unique_ptr<GLuint, std::function<void(GLuint*)>> ID;
};

class PostProcessor
{
public:
    PostProcessor(int width, int height, int samples);
    PostProcessor(const PostProcessor&) = delete;
    PostProcessor& operator=(const PostProcessor&) = delete;
    virtual ~PostProcessor() = default;

    void setShader(ShaderProgram& shader);
    void begRender() const;
    void endRender() const;
    void render() const;
    virtual void update(float dt);

private:
    VAO vao;
    BO vbo;
    Texture texture;
    RenderBuffer rb;
    FrameBuffer fb_rb;
    FrameBuffer fb_tex;
    glm::ivec2 size;
    ShaderProgram* shader;
};

#endif // POSTPROCESSOR_HH
