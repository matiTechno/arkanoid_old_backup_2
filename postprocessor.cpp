#include <postprocessor.hh>

FrameBuffer::FrameBuffer()
{
    auto deleter = [](GLuint* ID)
    {
        glDeleteFramebuffers(1, ID);
        delete ID;
    };

    ID = std::unique_ptr<GLuint, decltype(deleter)>(new GLuint, deleter);

    glGenFramebuffers(1, ID.get());
}

void FrameBuffer::bind(GLenum target) const
{
    glBindFramebuffer(target, *ID);
}

RenderBuffer::RenderBuffer()
{
    auto deleter = [](GLuint* ID)
    {
        glDeleteRenderbuffers(1, ID);
        delete ID;
    };

    ID = std::unique_ptr<GLuint, decltype(deleter)>(new GLuint, deleter);

    glGenRenderbuffers(1, ID.get());
}

void RenderBuffer::bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, *ID);
}

GLuint RenderBuffer::getID() const
{
    return *ID;
}

PostProcessor::PostProcessor(int width, int height, int samples):
    texture(GL_RGB8, width, height, GL_RGB, GL_UNSIGNED_BYTE),
    size(width, height)
{
    GLfloat vertices[] =
    {
        -1.f, -1.f, 0.f, 0.f,
        -1.f, 1.f, 0.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, -1.f, 1.f, 0.f,
        -1.f, -1.f, 0.f, 0.f
    };

    vbo.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vao.bind();
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    rb.bind();
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGB8, width, height);

    fb_rb.bind(GL_FRAMEBUFFER);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rb.getID());
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    fb_tex.bind(GL_FRAMEBUFFER);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getID(), 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void PostProcessor::setShader(ShaderProgram& shader)
{
    this->shader = &shader;
}

void PostProcessor::begRender() const
{
    fb_rb.bind(GL_FRAMEBUFFER);
    glClear(GL_COLOR_BUFFER_BIT);
}
void PostProcessor::endRender() const
{
    fb_rb.bind(GL_READ_FRAMEBUFFER);
    fb_tex.bind(GL_DRAW_FRAMEBUFFER);
    glBlitFramebuffer(0, 0, size.x, size.y,
                      0, 0, size.x, size.y,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::render() const
{
    shader->bind();
    texture.bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessor::update(float dt)
{
    (void)dt;
}
