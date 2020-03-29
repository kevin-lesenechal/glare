#include "glare/shader/shader_preprocessor.hpp"
#include "glare/shader/file_shader_source_loader.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <glm/vec3.hpp>

using namespace glare;

namespace {

class LoaderMock : public ShaderSourceLoaderInterface
{
public:
    MOCK_METHOD1(load_source, std::string(const std::string&));
};

TEST(shader_preprocessor, defines)
{
    FileShaderSourceLoader loader;
    ShaderPreprocessor proc(loader);

    std::map<std::string, std::string> defines;
    defines["FOO"] = "42";
    defines["BAR"] = "vec3(1.0f, 2.0f, 3.0f)";
    defines["BAZ"] = "false";

    std::string res = proc.preprocess(R"GLSL(#version 330 core

void main() {
    gl_Position = FOO * BAR;
}
)GLSL", defines);

    EXPECT_EQ(R"GLSL(#version 330 core
#define BAR vec3(1.0f, 2.0f, 3.0f)
#define BAZ false
#define FOO 42
#line 2

void main() {
    gl_Position = FOO * BAR;
}
)GLSL", res);
}

TEST(shader_preprocessor, simple_include)
{
    std::string base_glsl = R"GLSL(#version 330 core

#include "hello.glsl"

void main() {
    gl_Position = FOO * BAR;
}
)GLSL";
    std::string include_glsl = R"GLSL(#version 330 core

float my_function(float n) {
    return n * 2.0f;
}
)GLSL";

    using testing::Return;

    LoaderMock loader;
    EXPECT_CALL(loader, load_source("hello.glsl"))
        .WillOnce(Return(include_glsl));

    ShaderPreprocessor proc(loader);

    std::string res = proc.preprocess(base_glsl, {});

    EXPECT_EQ(R"GLSL(#version 330 core

#line 1
//#version 330 core

float my_function(float n) {
    return n * 2.0f;
}
#line 4

void main() {
    gl_Position = FOO * BAR;
}
)GLSL", res);
}

}
