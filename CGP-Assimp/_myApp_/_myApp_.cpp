#include <sb7.h>
#include <shader.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "Model.h"

class my_application : public sb7::application
{
public:
    void init() override
    {
        sb7::application::init();
        info.windowWidth = 1280;
        info.windowHeight = 720;
        info.majorVersion = 4;
        info.minorVersion = 3;
        strcpy(info.title, "CGP Assimp (sb7)");
    }

    void startup() override
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        const GLuint vs = sb7::shader::load("model_loading.vs", GL_VERTEX_SHADER);
        const GLuint fs = sb7::shader::load("model_loading.fs", GL_FRAGMENT_SHADER);
        const GLuint shaders[] = { vs, fs };
        program_ = sb7::program::link_from_shaders(shaders, 2, true);

        model_loc_ = glGetUniformLocation(program_, "model");
        view_loc_ = glGetUniformLocation(program_, "view");
        projection_loc_ = glGetUniformLocation(program_, "projection");

        projection_ = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(info.windowWidth) / static_cast<float>(info.windowHeight),
            0.1f,
            100.0f
        );

        const std::string path = resolveModelPath();
        if (path.empty())
        {
            std::cerr << "Model path not found." << std::endl;
            return;
        }

        model_ = std::make_unique<Model>(path);
        if (!model_->IsLoaded())
        {
            std::cerr << "Assimp failed to load model: " << path << std::endl;
        }
        else
        {
            std::cout << "Loaded model: " << path << std::endl;
        }
    }

    void render(double currentTime) override
    {
        const GLfloat black[] = { 0.08f, 0.08f, 0.10f, 1.0f };
        const GLfloat one[] = { 1.0f };
        glClearBufferfv(GL_COLOR, 0, black);
        glClearBufferfv(GL_DEPTH, 0, one);

        if (program_ == 0 || !model_ || !model_->IsLoaded())
        {
            return;
        }

        glUseProgram(program_);

        const glm::mat4 model = glm::rotate(
            glm::mat4(1.0f),
            static_cast<float>(currentTime) * 0.4f,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        const glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 1.2f, 4.0f),
            glm::vec3(0.0f, 0.8f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        glUniformMatrix4fv(model_loc_, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_loc_, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_loc_, 1, GL_FALSE, glm::value_ptr(projection_));

        model_->Draw(program_);
    }

    void onResize(int w, int h) override
    {
        sb7::application::onResize(w, h);
        glViewport(0, 0, w, h);

        if (h <= 0)
        {
            return;
        }

        projection_ = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(w) / static_cast<float>(h),
            0.1f,
            100.0f
        );
    }

    void shutdown() override
    {
        model_.reset();
        if (program_ != 0)
        {
            glDeleteProgram(program_);
            program_ = 0;
        }
    }

private:
    std::string resolveModelPath() const
    {
        namespace fs = std::filesystem;

        const fs::path candidates[] = {
            fs::path("resources/models/backpack.obj"),
            fs::path("_myApp_/resources/models/backpack.obj"),
            fs::path("../_myApp_/resources/models/backpack.obj")
        };

        for (const fs::path& p : candidates)
        {
            if (fs::exists(p))
            {
                return p.generic_string();
            }
        }

        return {};
    }

    GLuint program_ = 0;
    GLint model_loc_ = -1;
    GLint view_loc_ = -1;
    GLint projection_loc_ = -1;
    glm::mat4 projection_ = glm::mat4(1.0f);
    std::unique_ptr<Model> model_;
};

DECLARE_MAIN(my_application)



