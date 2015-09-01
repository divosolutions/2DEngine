#pragma once

#include "SDL.h"

/**
* @brief Draw an SDL_Texture to an SDL_Renderer at the position and with the size described in the SDL_Rect dst
* @param renderer The renderer which should copy parts of a texture.
* @param texture The source texture
* @param dst The destination drawing area
**/
static inline void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *dst)
{
	SDL_RenderCopy(renderer, texture, NULL, dst);
}

/**
* @brief Draw an SDL_Texture to an SDL_Renderer at the position (dst_x, dst_y) and with the size (dst_width, dst_height)
* @param renderer The renderer which should copy parts of a texture.
* @param texture The source texture
* @param dst_x The destination x position
* @param dst_y The destination y position
* @param dst_width The destination width
* @param dst_height The destination height
**/
static inline void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, int dst_x, int dst_y, int dst_width, int dst_height)
{
	SDL_Rect dst = { dst_x, dst_y, dst_width, dst_height };
	render_texture(renderer, texture, &dst);
}

/**
* @brief Draw an SDL_Texture to an SDL_Renderer at the position (dst_x, dst_y)
* @param renderer The renderer which should copy parts of a texture.
* @param texture The source texture
* @param dst_x The destination x position
* @param dst_y The destination y position
**/
static inline void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, int dst_x, int dst_y)
{
	int width, height;

	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	render_texture(renderer, texture, dst_x, dst_y, width, height);
}

/**
* @brief Draw a src-part of the given SDL_Texture to an SDL_Renderer at the position and with the size described in the SDL_Rect dst
* @param renderer The renderer which should copy parts of a texture.
* @param texture The source texture
* @param dst The destination drawing area
**/
static inline void render_texture_clip(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dst)
{
	SDL_RenderCopy(renderer, texture, src, dst);
}

/**
* @brief Draw a part of the given SDL_Texture from position (src_x, src_y) with the width (src_width, src_height) to an SDL_Renderer at the position (dst_x, dst_y) and with the size (dst_width, dst_height)
* @param renderer The renderer which should copy parts of a texture.
* @param texture The source texture
* @param src_x The source x position
* @param src_y The source y position
* @param src_width The source width
* @param src_height The source height
* @param dst_x The destination x position
* @param dst_y The destination y position
* @param dst_width The destination width
* @param dst_height The destination height
**/
static inline void render_texture_clip(SDL_Renderer *renderer, SDL_Texture *texture, int src_x, int src_y, int src_width, int src_height, int dst_x, int dst_y, int dst_width, int dst_height)
{
	SDL_Rect src = { src_x, src_y, src_width, src_height };
	SDL_Rect dst = { dst_x, dst_y, dst_width, dst_height };
	render_texture_clip(renderer, texture, &src, &dst);
}

/**
* @brief Draw a part of the given SDL_Texture from position (src_x, src_y) to an SDL_Renderer at the position (dst_x, dst_y) and with the size (dst_width, dst_height)
* @param renderer The renderer which should copy parts of a texture.
* @param texture The source texture
* @param src_x The source x position
* @param src_y The source y position
* @param dst_x The destination x position
* @param dst_y The destination y position
* @param dst_width The destination width
* @param dst_height The destination height
**/
static inline void render_texture_clip(SDL_Renderer *renderer, SDL_Texture *texture, int src_x, int src_y, int dst_x, int dst_y, int dst_width, int dst_height)
{
	render_texture_clip(renderer, texture, src_x, src_y, dst_width, dst_height, dst_x, dst_y, dst_width, dst_height);
}

/**
* @brief Draw a part of the given SDL_Texture from position (src_x, src_y) with the width (src_width, src_height) to an SDL_Renderer at the position (dst_x, dst_y)
* @param renderer The renderer which should copy parts of a texture.
* @param texture The source texture
* @param src_x The source x position
* @param src_y The source y position
* @param dst_x The destination x position
* @param dst_y The destination y position
**/
static inline void render_texture_clip(SDL_Renderer *renderer, SDL_Texture *texture, int src_x, int src_y, int dst_x, int dst_y)
{
	int width, height;

	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	render_texture_clip(renderer, texture, src_x, src_y, dst_x, dst_y, width, height);
}
