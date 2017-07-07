#include <string>

#include <glm/glm.hpp>
#include <glbinding/gl/gl.h>

template <gl::GLenum T>
struct opengl_type;

template <>
struct opengl_type<gl::GL_R8I> {
    typedef std::int8_t type;
};

template <>
struct opengl_type<gl::GL_R8UI> {
    typedef std::uint8_t type;
};

template <>
struct opengl_type<gl::GL_R16I> {
    typedef std::int16_t type;
};

template <>
struct opengl_type<gl::GL_R16UI> {
    typedef std::uint16_t type;
};

template <>
struct opengl_type<gl::GL_R32I> {
    typedef std::int32_t type;
};

template <>
struct opengl_type<gl::GL_R32UI> {
    typedef std::uint32_t type;
};

template <>
struct opengl_type<gl::GL_R32F> {
    typedef float type;
};

template <>
struct opengl_type<gl::GL_RG32I> {
    typedef glm::ivec2 type;
};

template <>
struct opengl_type<gl::GL_RG32UI> {
    typedef glm::uvec2 type;
};

template <>
struct opengl_type<gl::GL_RG32F> {
    typedef glm::vec2 type;
};

template <>
struct opengl_type<gl::GL_RGB32I> {
    typedef glm::ivec3 type;
};

template <>
struct opengl_type<gl::GL_RGB32UI> {
    typedef glm::uvec3 type;
};

template <>
struct opengl_type<gl::GL_RGB32F> {
    typedef glm::vec3 type;
};

template <>
struct opengl_type<gl::GL_RGBA32I> {
    typedef glm::ivec4 type;
};

template <>
struct opengl_type<gl::GL_RGBA32UI> {
    typedef glm::uvec4 type;
};

template <>
struct opengl_type<gl::GL_RGBA32F> {
    typedef glm::vec4 type;
};

template <typename T>
struct GLTypeTraits;

template <>
struct GLTypeTraits<bool> {
    typedef bool element_type;
    static const size_t dimension = 1;
    static const gl::GLenum opengl_enum = gl::GL_BOOL;
    static std::string glsl_type() { return "bool"; }
};

template <>
struct GLTypeTraits<std::int8_t> {
    typedef std::int8_t element_type;
    static const size_t dimension = 1;
    typedef GLTypeTraits<std::int8_t> signed_type;
    typedef GLTypeTraits<std::uint8_t> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_R8I; }
    static std::string image_format() { return "r8i"; }
};

template <>
struct GLTypeTraits<std::uint8_t> {
    typedef std::uint8_t element_type;
    static const size_t dimension = 1;
    typedef GLTypeTraits<std::int8_t> signed_type;
    typedef GLTypeTraits<std::uint8_t> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_R8UI; }
    static std::string image_format() { return "r8ui"; }
};

template <>
struct GLTypeTraits<std::int16_t> {
    typedef std::int16_t element_type;
    static const size_t dimension = 1;
    typedef GLTypeTraits<std::int16_t> signed_type;
    typedef GLTypeTraits<std::uint16_t> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_R16I; }
    static std::string image_format() { return "r16i"; }
};

template <>
struct GLTypeTraits<std::uint16_t> {
    typedef std::uint16_t element_type;
    static const size_t dimension = 1;
    typedef GLTypeTraits<std::int16_t> signed_type;
    typedef GLTypeTraits<std::uint16_t> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_R16UI; }
    static std::string image_format() { return "r16ui"; }
};

template <>
struct GLTypeTraits<std::int32_t> {
    typedef std::int32_t element_type;
    static const size_t dimension = 1;
    static const gl::GLenum opengl_enum = gl::GL_INT;
    typedef GLTypeTraits<std::int32_t> signed_type;
    typedef GLTypeTraits<std::uint32_t> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_R32I; }
    static std::string glsl_type() { return "int"; }
    static std::string image_format() { return "r32i"; }
};

template <>
struct GLTypeTraits<std::uint32_t> {
    typedef std::uint32_t element_type;
    static const size_t dimension = 1;
    static const gl::GLenum opengl_enum = gl::GL_UNSIGNED_INT;
    typedef GLTypeTraits<std::int32_t> signed_type;
    typedef GLTypeTraits<std::uint32_t> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_R32UI; }
    static std::string glsl_type() { return "uint"; }
    static std::string image_format() { return "r32ui"; }
};

template <>
struct GLTypeTraits<float> {
    typedef float element_type;
    static const size_t dimension = 1;
    static const gl::GLenum opengl_enum = gl::GL_FLOAT;
    static gl::GLenum color_enum() { return gl::GL_R32F; }
    static std::string glsl_type() { return "float"; }
    static std::string image_format() { return "r32f"; }
};

template <>
struct GLTypeTraits<double> {
    typedef double element_type;
    static const size_t dimension = 1;
    static const gl::GLenum opengl_enum = gl::GL_DOUBLE;
    static std::string glsl_type() { return "double"; }
};

template <>
struct GLTypeTraits<glm::bvec2> {
    typedef bool element_type;
    static const size_t dimension = 2;
    static std::string glsl_type() { return "bvec2"; }
};

template <>
struct GLTypeTraits<glm::ivec2> {
    typedef std::int32_t element_type;
    static const size_t dimension = 2;
    typedef GLTypeTraits<glm::ivec2> signed_type;
    typedef GLTypeTraits<glm::uvec2> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_RG32I; }
    static std::string glsl_type() { return "ivec2"; }
    static std::string image_format() { return "rg32i"; }
};

template <>
struct GLTypeTraits<glm::uvec2> {
    typedef std::uint32_t element_type;
    static const size_t dimension = 2;
    typedef GLTypeTraits<glm::ivec2> signed_type;
    typedef GLTypeTraits<glm::uvec2> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_RG32UI; }
    static std::string glsl_type() { return "uvec2"; }
    static std::string image_format() { return "rg32ui"; }
};

template <>
struct GLTypeTraits<glm::vec2> {
    typedef float element_type;
    static const size_t dimension = 2;
    static gl::GLenum color_enum() { return gl::GL_RG32F; }
    static std::string glsl_type() { return "vec2"; }
    static std::string image_format() { return "rg32f"; }
};

template <>
struct GLTypeTraits<glm::dvec2> {
    typedef double element_type;
    static const size_t dimension = 2;
    static std::string glsl_type() { return "dvec2"; }
};

template <>
struct GLTypeTraits<glm::bvec3> {
    typedef bool element_type;
    static const size_t dimension = 3;
    static std::string glsl_type() { return "bvec3"; }
};

template <>
struct GLTypeTraits<glm::ivec3> {
    typedef std::int32_t element_type;
    static const size_t dimension = 3;
    typedef GLTypeTraits<glm::ivec3> signed_type;
    typedef GLTypeTraits<glm::uvec3> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_RGB32I; }
    static std::string glsl_type() { return "ivec3"; }
};

template <>
struct GLTypeTraits<glm::uvec3> {
    typedef std::uint32_t element_type;
    static const size_t dimension = 3;
    typedef GLTypeTraits<glm::ivec3> signed_type;
    typedef GLTypeTraits<glm::uvec3> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_RGB32UI; }
    static std::string glsl_type() { return "uvec3"; }
};

template <>
struct GLTypeTraits<glm::vec3> {
    typedef float element_type;
    static const size_t dimension = 3;
    static gl::GLenum color_enum() { return gl::GL_RGB32F; }
    static std::string glsl_type() { return "vec3"; }
};

template <>
struct GLTypeTraits<glm::dvec3> {
    typedef double element_type;
    static const size_t dimension = 3;
    static std::string glsl_type() { return "dvec3"; }
};

template <>
struct GLTypeTraits<glm::bvec4> {
    typedef bool element_type;
    static const size_t dimension = 4;
    static std::string glsl_type() { return "bvec4"; }
};

template <>
struct GLTypeTraits<glm::ivec4> {
    typedef std::int32_t element_type;
    static const size_t dimension = 4;
    typedef GLTypeTraits<glm::ivec4> signed_type;
    typedef GLTypeTraits<glm::uvec4> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_RGBA32I; }
    static std::string glsl_type() { return "ivec4"; }
    static std::string image_format() { return "rgba32i"; }
};

template <>
struct GLTypeTraits<glm::uvec4> {
    typedef std::uint32_t element_type;
    static const size_t dimension = 4;
    typedef GLTypeTraits<glm::ivec4> signed_type;
    typedef GLTypeTraits<glm::uvec4> unsigned_type;
    static gl::GLenum color_enum() { return gl::GL_RGBA32UI; }
    static std::string glsl_type() { return "uvec4"; }
    static std::string image_format() { return "rgba32ui"; }
};

template <>
struct GLTypeTraits<glm::vec4> {
    typedef float element_type;
    static const size_t dimension = 4;
    static gl::GLenum color_enum() { return gl::GL_RGBA32F; }
    static std::string glsl_type() { return "vec4"; }
    static std::string image_format() { return "rgba32f"; }
};

template <>
struct GLTypeTraits<glm::dvec4> {
    typedef double element_type;
    static const size_t dimension = 4;
    static std::string glsl_type() { return "dvec4"; }
};

template <>
struct GLTypeTraits<glm::mat2> {
    typedef float element_type;
    static const size_t dimension = 4;
    static std::string glsl_type() { return "mat2"; }
};

template <>
struct GLTypeTraits<glm::mat2x3> {
    typedef float element_type;
    static const size_t dimension = 6;
    static std::string glsl_type() { return "mat2x3"; }
};

template <>
struct GLTypeTraits<glm::mat2x4> {
    typedef float element_type;
    static const size_t dimension = 8;
    static std::string glsl_type() { return "mat2x4"; }
};

template <>
struct GLTypeTraits<glm::mat3x2> {
    typedef float element_type;
    static const size_t dimension = 6;
    static std::string glsl_type() { return "mat3x2"; }
};

template <>
struct GLTypeTraits<glm::mat3> {
    typedef float element_type;
    static const size_t dimension = 9;
    static std::string glsl_type() { return "mat3"; }
};

template <>
struct GLTypeTraits<glm::mat3x4> {
    typedef float element_type;
    static const size_t dimension = 12;
    static std::string glsl_type() { return "mat3x4"; }
};

template <>
struct GLTypeTraits<glm::mat4x2> {
    typedef float element_type;
    static const size_t dimension = 8;
    static std::string glsl_type() { return "mat4x2"; }
};

template <>
struct GLTypeTraits<glm::mat4x3> {
    typedef float element_type;
    static const size_t dimension = 12;
    static std::string glsl_type() { return "mat4x3"; }
};

template <>
struct GLTypeTraits<glm::mat4> {
    typedef float element_type;
    static const size_t dimension = 16;
    static std::string glsl_type() { return "mat4"; }
};

template <>
struct GLTypeTraits<glm::dmat2> {
    typedef double element_type;
    static const size_t dimension = 4;
    static std::string glsl_type() { return "dmat2"; }
};

template <>
struct GLTypeTraits<glm::dmat2x3> {
    typedef double element_type;
    static const size_t dimension = 6;
    static std::string glsl_type() { return "dmat2x3"; }
};

template <>
struct GLTypeTraits<glm::dmat2x4> {
    typedef double element_type;
    static const size_t dimension = 8;
    static std::string glsl_type() { return "dmat2x4"; }
};

template <>
struct GLTypeTraits<glm::dmat3x2> {
    typedef double element_type;
    static const size_t dimension = 6;
    static std::string glsl_type() { return "dmat3x2"; }
};

template <>
struct GLTypeTraits<glm::dmat3> {
    typedef double element_type;
    static const size_t dimension = 9;
    static std::string glsl_type() { return "dmat3"; }
};

template <>
struct GLTypeTraits<glm::dmat3x4> {
    typedef double element_type;
    static const size_t dimension = 12;
    static std::string glsl_type() { return "dmat3x4"; }
};

template <>
struct GLTypeTraits<glm::dmat4x2> {
    typedef double element_type;
    static const size_t dimension = 8;
    static std::string glsl_type() { return "dmat4x2"; }
};

template <>
struct GLTypeTraits<glm::dmat4x3> {
    typedef double element_type;
    static const size_t dimension = 12;
    static std::string glsl_type() { return "dmat4x3"; }
};

template <>
struct GLTypeTraits<glm::dmat4> {
    typedef double element_type;
    static const size_t dimension = 16;
    static std::string glsl_type() { return "dmat4"; }
};

inline std::string glsl_type(gl::GLenum type) {
    switch (type) {
    case gl::GL_R32I:
        return GLTypeTraits<typename opengl_type<gl::GL_R32I>::type>::glsl_type();
    case gl::GL_R32UI:
        return GLTypeTraits<typename opengl_type<gl::GL_R32UI>::type>::glsl_type();
    case gl::GL_R32F:
        return GLTypeTraits<typename opengl_type<gl::GL_R32F>::type>::glsl_type();
    case gl::GL_RG32I:
        return GLTypeTraits<typename opengl_type<gl::GL_RG32I>::type>::glsl_type();
    case gl::GL_RG32UI:
        return GLTypeTraits<typename opengl_type<gl::GL_RG32UI>::type>::glsl_type();
    case gl::GL_RG32F:
        return GLTypeTraits<typename opengl_type<gl::GL_RG32F>::type>::glsl_type();
    case gl::GL_RGB32I:
        return GLTypeTraits<typename opengl_type<gl::GL_RGB32I>::type>::glsl_type();
    case gl::GL_RGB32UI:
        return GLTypeTraits<typename opengl_type<gl::GL_RGB32UI>::type>::glsl_type();
    case gl::GL_RGB32F:
        return GLTypeTraits<typename opengl_type<gl::GL_RGB32F>::type>::glsl_type();
    case gl::GL_RGBA32I:
        return GLTypeTraits<typename opengl_type<gl::GL_RGBA32I>::type>::glsl_type();
    case gl::GL_RGBA32UI:
        return GLTypeTraits<typename opengl_type<gl::GL_RGBA32UI>::type>::glsl_type();
    case gl::GL_RGBA32F:
        return GLTypeTraits<typename opengl_type<gl::GL_RGBA32F>::type>::glsl_type();
    default:
        return "";
    }
}

