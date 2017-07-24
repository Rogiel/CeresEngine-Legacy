//
// Created by Rogiel Sulzbach on 7/5/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"

#include "XYZ/Graphics/Shader/FragmentShader.hpp"
#include "XYZ/Graphics/Shader/GeometryShader.hpp"
#include "XYZ/Graphics/Shader/VertexShader.hpp"

#include "XYZ/Graphics/Shader/ShaderBinary.hpp"

#include "XYZ/Graphics/Texture/Texture.hpp"

#include <memory>

#include <glm/glm.hpp>

namespace XYZ::Graphics::Shader {

    class ShaderProgram : public Resource::Resource<ShaderProgram> {
    public:
        /**
         * Activates the shader
         */
        virtual void activate() = 0;

        /**
         * Deactivates the shader
         */
        virtual void deactivate() = 0;

    public:
        /**
         * Sets a boolean uniform variable
         *
         * @param name the uniform name
         * @param b the uniform value
         */
        virtual void set(const std::string& name, bool b) = 0;

        /**
		 * Sets a float uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
        virtual void set(const std::string& name, float v) = 0;

        /**
		 * Sets a int uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
        virtual void set(const std::string& name, int v) = 0;

        /**
		 * Sets a unsigned int uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
        virtual void set(const std::string& name, unsigned int v) = 0;

        /**
		 * Sets a vec2 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
        virtual void set(const std::string& name, glm::vec2 v) = 0;

        /**
		 * Sets a vec3 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
        virtual void set(const std::string& name, glm::vec3 v) = 0;

        /**
		 * Sets a vec4 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
        virtual void set(const std::string& name, glm::vec4 v) = 0;

        /**
		 * Sets a mat2 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
        virtual void set(const std::string& name, glm::mat2 v) = 0;

        /**
		 * Sets a mat3 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
        virtual void set(const std::string& name, glm::mat3 v) = 0;

        /**
		 * Sets a mat4 uniform variable
		 *
		 * @param name the uniform name
		 * @param v the uniform value
		 */
        virtual void set(const std::string& name, glm::mat4 v) = 0;

    public:
        /**
		 * @return the shader binary
		 */
        virtual ShaderBinary getShaderBinary() const = 0;

    };

}

