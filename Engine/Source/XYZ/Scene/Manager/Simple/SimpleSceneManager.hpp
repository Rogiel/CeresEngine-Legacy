//
// Created by Rogiel Sulzbach on 7/4/17.
//

#pragma once

#include <iostream>
#include <glm/ext.hpp>

namespace XYZ::Scene::Manager::Simple {

    class SimpleSceneManager {
    private:
        glm::mat4 projection;
        glm::mat4 view;

    public:
        SimpleSceneManager(glm::mat4 projection, glm::mat4 view) :
                projection(projection), view(view) {}

    public:
        bool visible(glm::vec3 point) {
            auto p = projection * view * glm::vec4(point, 1.0);
            std::cout << glm::to_string(p) << std::endl;
            return (projection * view * glm::vec4(point, 1.0)).w > 0.0;
        }

    };

}


