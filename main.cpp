#include <vector>
#include <map>
#include <memory>
#include <iostream>

#include <glm/glm.hpp>
#include <glbinding/gl/gl.h>
#include <globjects/globjects.h>

#include <globjects/Texture.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Framebuffer.h>

#include <globjects/Shader.h>
#include <globjects/Program.h>

#include <globjects/State.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

#include <opencv2/opencv.hpp>

#include "GLTypeTraits.h"
#include "HeadlessGL.h"

class Geometry {
    struct Attribute {
        gl::GLsizei size;
        gl::GLint element_stride;
        gl::GLenum element_type;
        gl::GLint dimension;
        gl::GLint location;
        bool enabled;
        std::string glsl_type;
        globjects::ref_ptr<globjects::Buffer> buffer;
    };

public:
    Geometry() {
        m_attribute_updated = false;
    }

    template <typename T>
    void add_attribute(const std::vector<T> &data, gl::GLenum usage = gl::GL_STATIC_DRAW) {
        size_t location = m_attributes.size();
        m_attributes.emplace_back(std::make_unique<Attribute>());
        std::unique_ptr<Attribute> &att = m_attributes[location];
        att->buffer = globjects::make_ref<globjects::Buffer>();
        att->buffer->setData(data, usage);
        att->element_stride = sizeof(T);
        att->element_type = GLTypeTraits<typename GLTypeTraits<T>::element_type>::opengl_enum;
        att->dimension = gl::GLint(GLTypeTraits<T>::dimension);
        att->glsl_type = GLTypeTraits<T>::glsl_type();
        att->size = (gl::GLsizei)data.size();
        att->location = 0;
        att->enabled = false;
        m_attribute_updated = true;
    }

    void bind_attribute_input(int attribute, int input, bool enabled = true) {
        m_attributes[attribute]->location = input;
        m_attributes[attribute]->enabled = enabled;
        m_attribute_updated = true;
    }

    void draw() {
        if (m_attribute_updated) {
            m_attribute_updated = false;
            prepare();
        }
        m_vertexarray->drawArrays(gl::GL_TRIANGLES, 0, m_attributes[0]->size);
    }

private:
    void prepare() {
        if (!m_vertexarray) {
            m_vertexarray = globjects::make_ref<globjects::VertexArray>();
        }

        for (size_t i = 0; i < m_attributes.size(); ++i) {
            const std::unique_ptr<Attribute> &att = m_attributes[i];
            globjects::VertexAttributeBinding *binding = m_vertexarray->binding((gl::GLuint)i);
            binding->setAttribute(att->location);
            binding->setBuffer(att->buffer, 0, att->element_stride);
            if (att->element_type == gl::GL_FLOAT) {
                binding->setFormat(att->dimension, gl::GL_FLOAT);
            }
            else if (att->element_type == gl::GL_DOUBLE) {
                binding->setLFormat(att->dimension, gl::GL_DOUBLE);
            }
            else {
                binding->setIFormat(att->dimension, m_attributes[i]->element_type);
            }
            if (att->enabled) {
                m_vertexarray->enable((gl::GLint)i);
            }
            else {
                m_vertexarray->disable((gl::GLint)i);
            }
        }
    }

    bool m_attribute_updated;
    std::vector<std::unique_ptr<Attribute>> m_attributes;
    globjects::ref_ptr<globjects::VertexArray> m_vertexarray;
};

class Pass {
public:
    Pass() {
        m_viewport_w = m_viewport_h = 0;
        m_state = globjects::make_ref<globjects::State>(globjects::State::DeferredMode);
        m_shader_updated = false;
    }

    template <typename T>
    void add_color_attachment(const std::string name, bool use_rbo = false) {
        add_color_attachment(name, typename GLTypeTraits<T>::color_enum(), use_rbo);
    }

    void add_color_attachment(const std::string &name, gl::GLenum type, bool use_rbo = false) {
        m_colors.emplace_back(std::make_unique<Attachment>());
        m_colors.back()->name = name;
        m_colors.back()->is_texture = !use_rbo;
        m_colors.back()->type = type;
        m_colors.back()->create();

        GLSLVariable &var = m_fshader_outputs[m_colors.size() - 1];
        var.name = name;
        var.type = glsl_type(type);

        m_viewport_w = m_viewport_h = 0;
        m_shader_updated = true;
    }

    void add_depth_attachment(const gl::GLenum type = gl::GL_DEPTH_COMPONENT32F, bool use_rbo = true) {
        if (!m_depth) {
            m_depth = std::make_unique<Attachment>();
        }
        m_depth->is_texture = !use_rbo;
        m_depth->type = type;
        m_depth->create();
        m_viewport_w = m_viewport_h = 0;
    }

    template <typename T>
    void add_vshader_uniform(const std::string &name) {
        GLSLVariable var;
        var.name = name;
        var.type = GLTypeTraits<T>::glsl_type();
        m_vshader_uniforms.push_back(var);
        m_shader_updated = true;
    }

    template <typename T>
    void add_fshader_uniform(const std::string &name) {
        GLSLVariable var;
        var.name = name;
        var.type = GLTypeTraits<T>::glsl_type();
        m_fshader_uniforms.push_back(var);
        m_shader_updated = true;
    }

    template <typename T>
    void add_vshader_input(size_t location, const std::string &name) {
        GLSLVariable &var = m_vshader_inputs[location];
        var.name = name;
        var.type = GLTypeTraits<T>::glsl_type();
        m_shader_updated = true;
    }

    template <typename T>
    void add_vfshader_interface(const std::string &name) {
        GLSLVariable var;
        var.name = name;
        var.type = GLTypeTraits<T>::glsl_type();
        m_vfshader_interfaces.push_back(var);
        m_shader_updated = true;
    }

    void set_shader(const std::string &vs, const std::string &fs) {
        m_vshader_source = vs;
        m_fshader_source = fs;
        m_shader_updated = true;
    }

    globjects::State* state() {
        return m_state.get();
    }

    template <typename T>
    void set_uniform(const std::string &name, const T &value) {
        if (m_program) {
            m_program->setUniform(name, value);
        }
    }

    void begin(int w, int h) {
        if (w != m_viewport_w || h != m_viewport_h) {
            m_viewport_w = w;
            m_viewport_h = h;
            prepare_framebuffer();
        }

        if (m_shader_updated) {
            m_shader_updated = false;
            prepare_shader();
        }

        m_framebuffer->bind();
        if (m_program) {
            m_program->use();
        }
        m_state->apply();

        gl::glViewport(0, 0, w, h);
        m_framebuffer->clear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    }

    void end() {
        m_framebuffer->unbind();
    }

    void show_color_attachment(size_t id) {
        m_colors[id]->show(m_viewport_w, m_viewport_h);
    }

private:
    void prepare_framebuffer() {
        if (!m_framebuffer) {
            m_framebuffer = globjects::make_ref<globjects::Framebuffer>();
        }

        for (size_t i = 0; i < m_colors.size(); ++i) {
            m_colors[i]->storage(m_viewport_w, m_viewport_h);
            m_colors[i]->attach(m_framebuffer.get(), gl::GL_COLOR_ATTACHMENT0 + (int)i);
        }

        if (m_depth) {
            m_depth->storage(m_viewport_w, m_viewport_h);
            m_depth->attach(m_framebuffer.get(), gl::GL_DEPTH_ATTACHMENT);
        }

        if (m_colors.size() > 0) {
            std::vector<gl::GLenum> draw_buffers;
            for (size_t i = 0; i < m_colors.size(); ++i) {
                draw_buffers.push_back(gl::GL_COLOR_ATTACHMENT0 + (int)i);
            }
            m_framebuffer->setDrawBuffers(draw_buffers);
        }
        else {
            m_framebuffer->setDrawBuffer(gl::GL_NONE);
        }

        m_framebuffer->printStatus(true);
    }

    void prepare_shader() {
        if (!m_program) {
            m_program = globjects::make_ref<globjects::Program>();
        }
        std::string vshader_code = "#version 430\n";
        std::string fshader_code = "#version 430\n";

        for (auto &v : m_vshader_uniforms) {
            vshader_code += v.declaration_line("uniform");
        }
        for (auto &f : m_fshader_uniforms) {
            fshader_code += f.declaration_line("uniform");
        }

        for (auto &v : m_vshader_inputs) {
            std::string layout = "location = " + std::to_string(v.first);
            vshader_code += v.second.declaration_line("in", layout);
        }

        for (auto &v : m_vfshader_interfaces) {
            char c = v.type[0];
            if(c=='b' ||c =='u'||c=='i') {
                vshader_code += "flat ";
                fshader_code += "flat ";
            }
            vshader_code += v.declaration_line("out");
            fshader_code += v.declaration_line("in");
        }

        for (auto &f : m_fshader_outputs) {
            std::string layout = "location = " + std::to_string(f.first);
            fshader_code += f.second.declaration_line("out", layout);
        }

        vshader_code += "void main() {\n\t" + m_vshader_source + "\n}";
        fshader_code += "void main() {\n\t" + m_fshader_source + "\n}";

        m_vshader = globjects::Shader::fromString(gl::GL_VERTEX_SHADER, vshader_code);
        m_fshader = globjects::Shader::fromString(gl::GL_FRAGMENT_SHADER, fshader_code);
        m_program->attach(m_vshader.get(), m_fshader.get());
        m_program->link();
        if (m_program->infoLog().size()>0) {
            std::cout << m_program->infoLog() << std::endl;
        }
    }

    struct Attachment {
        std::string name;
        bool is_texture;
        gl::GLenum type;
        globjects::ref_ptr<globjects::Texture> texture;
        globjects::ref_ptr<globjects::Renderbuffer> renderbuffer;
        void create() {
            if (is_texture) {
                texture = globjects::make_ref<globjects::Texture>(gl::GL_TEXTURE_2D);
                renderbuffer = nullptr;
            }
            else {
                renderbuffer = globjects::make_ref<globjects::Renderbuffer>();
                texture = nullptr;
            }
        }
        void storage(int w, int h) {
            if (is_texture) {
                texture->storage2D(1, type, w, h);
            }
            else {
                renderbuffer->storage(type, w, h);
            }
        }
        void attach(globjects::Framebuffer *fbo, gl::GLenum attachment) {
            if (is_texture) {
                fbo->attachTexture(attachment, texture);
            }
            else {
                fbo->attachRenderBuffer(attachment, renderbuffer);
            }
        }
        void show(int w, int h) {
            if (is_texture) {
                cv::Mat img(h, w, CV_8UC4);
                texture->getImage(0, gl::GL_BGRA, gl::GL_UNSIGNED_BYTE, img.data);
                cv::imshow("Texture", img);
                cv::waitKey(0);
            }
        }
    };

    struct GLSLVariable {
        std::string type;
        std::string name;

        std::string declaration_line(const std::string &qualifier, const std::string &layout = "") {
            std::string result;
            if (layout.size() > 0) {
                result = "layout(" + layout + ") ";
            }
            result += qualifier + " " + type + " " + name + ";\n";
            return result;
        }
    };

    int m_viewport_w;
    int m_viewport_h;

    std::vector<std::unique_ptr<Attachment>> m_colors;
    std::unique_ptr<Attachment> m_depth;
    globjects::ref_ptr<globjects::Framebuffer> m_framebuffer;

    globjects::ref_ptr<globjects::State> m_state;

    bool m_shader_updated;
    std::vector<GLSLVariable> m_vshader_uniforms;
    std::vector<GLSLVariable> m_fshader_uniforms;
    std::map<size_t, GLSLVariable> m_vshader_inputs;
    std::vector<GLSLVariable> m_vfshader_interfaces;
    std::map<size_t, GLSLVariable> m_fshader_outputs;

    std::string m_vshader_source;
    std::string m_fshader_source;
    globjects::ref_ptr<globjects::Shader> m_vshader;
    globjects::ref_ptr<globjects::Shader> m_fshader;
    globjects::ref_ptr<globjects::Program> m_program;
};

class Renderer {
public:
    void set_n_passes(size_t n_passes) {
        m_passes.resize(n_passes);
        for (size_t i = 0; i < m_passes.size(); ++i) {
            if (!m_passes[i]) {
                m_passes[i] = std::make_unique<Pass>();
            }
        }
    }

    size_t n_passes() const {
        return m_passes.size();
    }

    Pass *pass(size_t i) {
        return m_passes[i].get();
    }

private:
    std::vector<std::unique_ptr<Pass>> m_passes;
};



#define glsl_main(source) "" # source

int main(int argc, char *argv[]) {
    HeadlessGL gl;
    gl.make_current();
    globjects::init();

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
    std::unique_ptr<Geometry> geometry = std::make_unique<Geometry>();

    std::vector<glm::vec3> points;
    points.emplace_back(0.0f, 0.0f, 0.5f);
    points.emplace_back(0.0f, 1.0f, 0.5f);
    points.emplace_back(1.0f, 0.0f, 0.5f);
    geometry->add_attribute(points);

    renderer->set_n_passes(1);
    renderer->pass(0)->add_vshader_input<glm::vec3>(0, "vertex_coord");
    renderer->pass(0)->add_color_attachment<glm::vec4>("frag_color");
    renderer->pass(0)->set_shader(
        glsl_main(
            gl_Position = vec4(vertex_coord, 1.0f);
        ),
        glsl_main(
            frag_color = 1.0f.xxxx;
        )
    );
    renderer->pass(0)->state()->clearColor({0.0f, 0.0f, 0.0f, 0.0f});
    renderer->pass(0)->state()->clearDepth(1.0f);
    renderer->pass(0)->begin(640, 480);

    geometry->bind_attribute_input(0, 0);
    geometry->draw();

    renderer->pass(0)->end();
    renderer->pass(0)->show_color_attachment(0);
    return 0;
}
