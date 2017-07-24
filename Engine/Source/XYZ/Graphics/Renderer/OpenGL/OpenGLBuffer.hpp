//
// Created by Rogiel Sulzbach on 7/9/17.
//

#pragma once

#include <GL/glew.h>

namespace XYZ::Graphics::Renderer::OpenGL {

	template<typename T>
	class OpenGLUniformBuffer {
	private:
		GLuint ubo = 0;
		T object;

	public:
		OpenGLUniformBuffer() {}
		OpenGLUniformBuffer(GLuint ubo) : ubo(ubo) {
			if(ubo != 0) {
				glBufferData(GL_UNIFORM_BUFFER, sizeof(T), &object, GL_STREAM_DRAW);
			}
		}

		OpenGLUniformBuffer(const T& object) : object(object) {}

		OpenGLUniformBuffer(OpenGLUniformBuffer&& other) {
			ubo = other.ubo;
			object = std::move(other.object);

			// unset the other UBO
			other.ubo = 0;
		}

		OpenGLUniformBuffer& operator=(OpenGLUniformBuffer&& other) {
			ubo = other.ubo;
			object = std::move(other.object);

			// unset the other UBO
			other.ubo = 0;

			return *this;
		}

		~OpenGLUniformBuffer() {
			if(ubo != 0) {
				glDeleteBuffers(1, &ubo);
			}
		}

	public:
		void init() {
			if(ubo != 0) {
				return;
			}
			glGenBuffers(1, &ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(T), &object, GL_STREAM_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		operator bool() const {
			return ubo != 0;
		}

		operator GLuint() const {
			return ubo;
		}

		void update(size_t start = 0, size_t length = sizeof(T)) {
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, start, length, ((char*) &object + start));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

	public:
		T* operator->() {
			return &object;
		}

		T& operator*() {
			return object;
		}
	};

}

