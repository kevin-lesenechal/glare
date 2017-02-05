#include "glare/shader/shader_preprocessor.hpp"
#include "glare/shader/fs_shader_source_loader.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

class LoaderMock : public ShaderSourceLoaderInterface
{
public:
    MOCK_METHOD1(load_source, std::string(const std::string&));
};

TEST(shader_preprocessor, defines)
{
    FsShaderSourceLoader loader;
    ShaderPreprocessor proc(loader);

    proc.define("FOO", "42");
    proc.define("BAR", "vec3(1.0f)");
    proc.define("REMOVE_ME", "true");
    proc.undefine("REMOVE_ME");

    std::string res = proc.preprocess(R"GLSL(#version 330 core

void main() {
    gl_Position = FOO * BAR;
}
)GLSL");

    EXPECT_EQ(R"GLSL(#version 330 core
#define BAR vec3(1.0f)
#define FOO 42
#line 2

void main() {
    gl_Position = FOO * BAR;
}
)GLSL", res);
}

TEST(shader_preprocessor, simple_include)
{
    FsShaderSourceLoader loader;
    ShaderPreprocessor proc(loader);

    std::string res = proc.preprocess(R"GLSL(#version 330 core

#include "hello.glsl"

void main() {
    gl_Position = FOO * BAR;
}
)GLSL");
}

}
