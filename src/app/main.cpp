#include <epoxy/gl.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/gtx/transform.hpp>

#include <glare/opengl/shader.hpp>
#include <glare/opengl/shader_program.hpp>
#include <glare/opengl/texture.hpp>

#include <glare/primitive/axis.hpp>
#include <glare/fps_camera.hpp>
#include <glare/ui/ui_context.hpp>
#include <glare/ui/light_config.hpp>
#include <glare/ui/camera_config.hpp>
#include <glare/debug/debug_window.hpp>
#include <glare/model/model_loader.hpp>

#include "app_context.hpp"

using namespace glare;

Texture load_texture(int unit, const std::string& path)
{
    int tex_w, tex_h, tex_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_data = stbi_load(path.c_str(), &tex_w, &tex_h,
                                        &tex_channels, 0);
    if (tex_data == nullptr) {
        throw 42;
    }

    Texture texture(unit, Texture::Type::Texture2D);
    texture.set_image(0, tex_w, tex_h, GL_RGB,
                      Texture::PixelFormat::RGB, GL_UNSIGNED_BYTE, tex_data);
    texture.generate_mipmap();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (epoxy_has_gl_extension("GL_ARB_texture_filter_anisotropic")) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY,
                        GL_MAX_TEXTURE_MAX_ANISOTROPY);
    }

    stbi_image_free(tex_data);

    return texture;
}

int main()
{
    AppContext gl_context;

    Texture tex_color = load_texture(0, "tex/stone_wall/stone_wall.color.jpg");
    //Texture tex_spec = load_texture(1, "tex/container/container.spec.jpg");
    Texture tex_normal = load_texture(2, "tex/stone_wall/stone_wall.normal.jpg");

    auto vert_shader = Shader::from_source_file(
        Shader::Type::Vertex,
        "shaders/shaded.vert.glsl"
    );
    auto frag_shader = Shader::from_source_file(
        Shader::Type::Fragment,
        "shaders/shaded.frag.glsl"
    );

    ShaderProgram program({&vert_shader, &frag_shader});
    program.link();
    program.use();

    Assimp::Importer assimp_importer;
    ModelLoader model_loader(assimp_importer);
    Model model = model_loader.load_from_file("models/suzanne.dae");

    glm::vec3 light_color(1.0f, 0.95f, 0.88f);

    FpsCamera camera(glm::vec3(2.0f, 0.8f, 1.2f), 60.0f, 1280.0f/1024.0f);
    gl_context.on_window_resized = [&camera](unsigned w, unsigned h) {
        glViewport(0, 0, w, h);
        camera.aspect_ratio = static_cast<float>(w) / static_cast<float>(h);
        camera.update_projection_matrix();
    };

    AxisPrimitive axis;
    UiContext ui_context(gl_context);
    DebugWindow debug;
    glare::LightConfig light_config(
        "Light 0 [point]",
        glm::vec3(1.2f, 2.0f, 1.4f),
        light_color * 0.2f,
        light_color * 0.6f,
        light_color
    );
    glare::CameraConfig camera_config(camera);

    program.set_uniform("g_material.texture", tex_color.unit());
    //program.set_uniform("g_material.specular_map", tex_spec.unit());
    //program.set_uniform("g_material.normal_map", tex_normal.unit());
    program.set_uniform("g_material.shininess", 128.0f);
    program.set_uniform("g_material.has_specular_map", false);
    program.set_uniform("g_material.has_normal_map", false);

    while (gl_context.is_running()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ui_context.frame_start();

        glm::mat4 model_mx = glm::rotate(static_cast<float>(glfwGetTime()) * 0.8f,
                                         glm::vec3(0.0f, 0.0f, 1.0f));
        //glm::mat4 model_mx(1.0f);
        program.use();
        program.set_uniform("g_model_mx", model_mx);
        program.set_uniform("g_view_mx", camera.view_matrix());
        program.set_uniform("g_proj_mx", camera.projection_matrix());
        program.set_uniform("g_light.position", light_config.position);
        program.set_uniform("g_light.ambient", light_config.ambient);
        program.set_uniform("g_light.diffuse", light_config.diffuse);
        program.set_uniform("g_light.specular", light_config.specular);

        model.draw(program);

        axis.draw({glm::mat4(1.0f), camera.view_matrix(), camera.projection_matrix()});

        debug.draw();
        light_config.draw();
        camera_config.draw();

        ui_context.frame_end();
        gl_context.swap_buffers();
        glfwPollEvents();
    }

    return 0;
}
