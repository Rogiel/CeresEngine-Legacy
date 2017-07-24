//
// Created by Rogiel Sulzbach on 7/5/17.
//

#pragma once

#include "XYZ/Resource/Resource.hpp"
#include "XYZ/Graphics/Texture/TextureImage.hpp"

#include <glm/vec4.hpp>

namespace XYZ::Graphics::Texture {

	/**
	 * The texture wrap mode
	 */
	enum class TextureWrap {
		/**
		 * Causes ss coordinates to be clamped to the range [1/2N, 1 − 1/2N],
		 * where N is the size of the texture in the direction of clamping
		 */
				CLAMP_TO_EDGE = 1,

		/**
		 * evaluates ss coordinates in a similar manner to CLAMP_TO_EDGE. 
		 * However, in cases where clamping would have occurred in 
		 * CLAMP_TO_EDGE mode, the fetched texel data is substituted with 
		 * the values specified by TEXTURE_BORDER_COLOR
		 */
				CLAMP_TO_BORDER,

		/**
		 * Causes the integer part of the ss coordinate to be ignored; the 
		 * GL uses only the fractional part, thereby creating a repeating 
		 * pattern.
		 */
				REPEAT,

		/**
		 * Causes the ss coordinate to be set to the fractional part of 
		 * the texture coordinate if the integer part of ss is even; 
		 * if the integer part of ss is odd, then the ss texture coordinate 
		 * is set to 1−frac(s)1-frac⁡s, where frac(s)frac⁡s represents the 
		 * fractional part of ss.
		 */
				MIRRORED_REPEAT,

		/**
		 * Causes the the ss coordinate to be repeated as for 
		 * MIRRORED_REPEAT for one reptition of the texture, at which 
		 * point the coordinate to be clamped as in CLAMP_TO_EDGE.
		 */
				MIRROR_CLAMP_TO_EDGE
	};

	/**
	 * The texture magnification mode
	 */
	enum class TextureMagnification {
		/**
		 * Returns the value of the texture element that is nearest
		 * (in Manhattan distance) to the specified texture
		 * coordinates.
		 */
				NEAREST = 1,

		/**
		 * Returns the weighted average of the four texture elements
		 * that are closest to the specified texture coordinates.
		 * These can include items wrapped or repeated from other
		 * parts of a texture, depending on the values of S and T
		 * wrapping, and on the exact mapping.
		 */
				LINEAR
	};

	/**
	 * The texture minification mode
	 */
	enum class TextureMinification {
		/**
		 * Returns the value of the texture element that is nearest
		 * (in Manhattan distance) to the specified texture
		 * coordinates.
		 */
				NEAREST = 1,

		/**
		 * Returns the weighted average of the four texture elements
		 * that are closest to the specified texture coordinates.
		 * These can include items wrapped or repeated from other
		 * parts of a texture, depending on the values of
		 * TEXTURE_WRAP_S and TEXTURE_WRAP_T, and on the exact
		 * mapping.
		 */
				LINEAR,

		/**
		 * Chooses the mipmap that most closely matches the size of the
		 * pixel being textured and uses the NEAREST criterion (the
		 * texture element closest to the specified texture coordinates)
		 * to produce a texture value.
		 */
				NEAREST_MIPMAP_NEAREST,

		/**
		 * Chooses the mipmap that most closely matches the size of the
		 * pixel being textured and uses the LINEAR criterion (a weighted
		 * average of the four texture elements that are closest to the
		 * specified texture coordinates) to produce a texture value.
		 */
				LINEAR_MIPMAP_NEAREST,

		/**
		 * Chooses the two mipmaps that most closely match the size of the
		 * pixel being textured and uses the NEAREST criterion (the texture
		 * element closest to the specified texture coordinates) to produce
		 * a texture value from each mipmap. The final texture value is a
		 * weighted average of those two values.
		 */
				NEAREST_MIPMAP_LINEAR,

		/**
		 * Chooses the two mipmaps that most closely match the size of the
		 * pixel being textured and uses the LINEAR criterion (a weighted
		 * average of the texture elements that are closest to the specified
		 * texture coordinates) to produce a texture value from each mipmap.
		 * The final texture value is a weighted average of those two values.
		 */
				LINEAR_MIPMAP_LINEAR
	};

	class TextureImage;

	class Texture : public Resource::Resource<Texture> {
	public:
		/**
		 * Activate the texture
		 *
		 * @param slot the texture slot
		 */
		virtual void activate(unsigned int slot = 0) = 0;

		/**
		 * Deactivate the texture
		 *
		 * @param slot the texture slot
		 */
		virtual void deactivate(unsigned int slot) = 0;

	public:
		/**
		 * @return the number of channels in each texel
		 */
		virtual int getNumberOfChannels() const = 0;

		/**
		 * @return the number of bytes in each texel channel
		 */
		virtual int getBytesPerChannel() const = 0;

	public:
		/**
		 * Checks if the texture can be resized
		 *
		 * @return true if the texture can be resized, false otherwise
		 */
		virtual bool canResize() const = 0;

		/**
		 * Resizes the texture.
		 *
		 * @param width the new texture width
		 * @param height the new texture height
		 */
		virtual void resize(size_t width, size_t height) = 0;

		/**
		 * Checks if the texture image can be update
		 *
		 * @return true if the texture can be updated, false otherwise
		 */
		virtual bool canUpdate() = 0;

		/**
		 * Updates the texture image data.
		 *
		 * @param textureImage the new texture to update with
		 */
		virtual void update(const TextureImage& textureImage) = 0;

	public:
		/**
		 * Sets the wrap mode for the texture
		 *
		 * @param x the mode to be used on the S axis
		 * @param y the mode to be used on the T axis
		 */
		virtual void setWrapMode(TextureWrap s, TextureWrap t);

		/**
		 * Gets the wrap mode for the texture
		 *
		 * @return a tuple returning the wrap mode for the S
		 * and T axes
		 */
		virtual std::tuple<TextureWrap, TextureWrap> getWrapMode() const;

		/**
		 * Sets the wrap mode for texture coordinate s.
		 *
		 * @param mode the wrap mode for texture coordinate s.
		 */
		virtual void setWrapModeS(TextureWrap mode) = 0;

		/**
		 * Gets the wrap mode for texture coordinate s.
		 *
		 * @return the wrap mode for texture coordinate s.
		 */
		virtual TextureWrap getWrapModeS() const = 0;

		/**
		 * Sets the wrap mode for texture coordinate t.
		 *
		 * @param mode the wrap mode for texture coordinate t.
		 */
		virtual void setWrapModeT(TextureWrap mode) = 0;

		/**
		 * Gets the wrap mode for texture coordinate t.
		 *
		 * @return the wrap mode for texture coordinate t.
		 */
		virtual TextureWrap getWrapModeT() const = 0;

		/**
		 * Sets the texture border color. Used when the wrapping mode
		 * is set to BORDER_COLOR.
		 *
		 * @param borderColor the color to be used when wrapping
		 */
		virtual void setBorderColor(glm::vec4 borderColor) = 0;

		/**
		 * Gets the texture border color. Used when the wrapping mode
		 * is set to BORDER_COLOR.
		 *
		 * @return the color to be used when wrapping
		 */
		virtual glm::vec4 getBorderColor() const = 0;

	public:
		/**
		 * Sets the texture magnification and minification function is used
		 * whenever the level-of-detail function used when sampling from
		 * the texture determines that the texture should be magified or
		 * minified.
		 *
		 * @param magnificationFilter the filter to be used when magnifying
		 * @param minificationFilter  the filter to be used when minifying
		 */
		virtual void setMagnificationMinificationFilter(TextureMagnification magnificationFilter,
														TextureMinification minificationFilter);

		/**
		 * Gets the texture magnification and minification function is used
		 * whenever the level-of-detail function used when sampling from
		 * the texture determines that the texture should be magified or
		 * minified.
		 *
		 * @return a tuple with both the magnification and minification filters
		 */
		virtual std::tuple<TextureMagnification, TextureMinification> getMagnificationMinificationFilter() const;

		/**
		 * Sets the texture magnification function is used whenever the
		 * level-of-detail function used when sampling from the texture
		 * determines that the texture should be magified.
		 *
		 * @param filter the filter to be used when magnifying
		 */
		virtual void setMagnificationFilter(TextureMagnification filter) = 0;

		/**
		 * Gets the texture magnification function used by the level-of-detail
		 * function when sampling from the texture determines that the
		 * texture should be magified.
		 *
		 * @return the filter to be used when magifying
		 */
		virtual TextureMagnification getMagnificationFilter() const = 0;

		/**
		 * Sets the texture minifying function is used by the level-of-detail
		 * function when sampling from the texture determines that the
		 * texture should be minified.
		 *
		 * @param filter the filter to be used when minifying
		 */
		virtual void setMinificationFilter(TextureMinification filter) = 0;

		/**
		 * Gets the texture minifying function used by the level-of-detail
		 * function when sampling from the texture determines that the
		 * texture should be minified.
		 *
		 * @return the filter to be used when minifying
		 */
		virtual TextureMinification getMinificationFilter() const = 0;

		/**
		 * Checks if the texture object supports mipmap generation
		 *
		 * @return true if mipmap generation is supported
		 */
		virtual bool canGenerateMipmaps() const = 0;

		/**
		 * Generate mipmaps for the texture.
		 */
		virtual void generateMipmaps() = 0;

	public:
		/**
		 * Creates a new texture image from the contents of the texture
		 *
		 * @return a newly created texture image with the contents of this texture
		 */
		virtual TextureImage::Ptr toImage() const = 0;

	public:
		/**
		 * A event called whenever the engine is running low on memory.
		 *
		 * When calling this method, resources are able to release
		 * cached data.
		 *
		 * @return true if the entire resource can be unloaded,
		 * false otherwise.
		 */
		bool didReceiveMemoryWarning() override;

	};

}

