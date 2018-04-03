//
// Created by Rogiel Sulzbach on 7/23/17.
//

#include "OpenGLDeferredRendering.hpp"
#include "OpenGLException.hpp"

#include <iostream>

namespace XYZ::Graphics::Renderer::OpenGL {

	void OpenGLDeferredRendering::renderBloomPass() {
		static unsigned int quadVAO = 0;
		static unsigned int quadVBO;
		if(quadVAO == 0) {
			float quadVertices[] = {
					// positions        // texture Coords
					-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
					1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};

			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) nullptr);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);

		bloomVerticalBlurFramebuffer.activate();
		bloomVerticalBlurFramebuffer.clear();

		bloomHorizontalBlurFramebuffer.activate();
		bloomHorizontalBlurFramebuffer.clear();

		if(!bloom) {
			return;
		}

		// extract the "over-exposed" fragments
		bloomExposureMappingShaderProgram.activate();
		bloomExposureMappingShaderProgram.set("scene", 0);
		lightingTexture.activate(0);

		bloomVerticalBlurFramebuffer.activate();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		int amount = 10;
		bloomBlurShaderProgram.activate();
		bloomBlurShaderProgram.set("scene", 0);

		for(unsigned int i = 0; i < amount; i++) {
			bloomHorizontalBlurFramebuffer.activate();
			bloomBlurShaderProgram.set("horizontal", true);
			bloomVerticalBlurTexture.activate(0);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			bloomVerticalBlurFramebuffer.activate();
			bloomBlurShaderProgram.set("horizontal", false);
			bloomHorizontalBlurTexture.activate(0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}

	void OpenGLDeferredRendering::renderHDRPass() {
		static unsigned int quadVAO = 0;
		static unsigned int quadVBO;
		if(quadVAO == 0) {
			float quadVertices[] = {
					// positions        // texture Coords
					-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
					1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};

			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) nullptr);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);

		renderer.getDefaultFramebuffer().activate();
		renderer.getDefaultFramebuffer().clear();

		hdrShaderProgram.activate();
		hdrShaderProgram.set("exposure", exposure);
		hdrShaderProgram.set("scene", 0);
		hdrShaderProgram.set("bloom", 1);
		hdrShaderProgram.set("hdr", hdr);

		lightingTexture.activate(0);
		bloomVerticalBlurTexture.activate(1);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

}