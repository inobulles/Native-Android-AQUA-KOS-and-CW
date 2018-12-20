
#ifndef __AQUA__SDL2_SRC_FUNCTIONS_FONT_H
	#define __AQUA__SDL2_SRC_FUNCTIONS_FONT_H
	
	#include "../macros_and_inclusions.h"
	
	#if KOS_USES_SDL2
		#ifdef __USE_SDL_TTF
			#include <SDL2/SDL_ttf.h>
			#warning "WARNING Using the SDL_ttf library may cause problems on some platforms"
		#else
			#include <ft2build.h>
			#include FT_FREETYPE_H
			
			#include "SDL_ttf/SDL_ttf.c"
		#endif
	#endif
	
	#define __USE_SDL_TTF_PROVIDED 1
	
	#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
		typedef SDL_Surface* kos_font_surface_t;
	#else
		typedef struct {
			unsigned long long* pixels;
			
			unsigned long long  w;
			unsigned long long  h;
			
		} kos_font_surface_t;
	#endif
	
	typedef struct {
		unsigned char used;
		
		float size;
		char  path[MAX_PATH_LENGTH];
		char* text;
		
		kos_font_surface_t surface;
		
		#if KOS_USES_SDL2
			#ifdef __USE_SDL_TTF_PROVIDED
				TTF_Font* font;
			#else
				FT_Face font;
			#endif
		#endif
		
	} kos_font_t;
	
	#ifndef KOS_MAX_FONTS
		#define KOS_MAX_FONTS 4096
	#endif
	
	static kos_font_t kos_fonts[KOS_MAX_FONTS];
	
	#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
		static SDL_Color kos_font_colour;
	#elif KOS_USES_SDL2
		static FT_Library kos_freetype_library;
	#endif
	
	#ifndef KOS_CHECK_FONT
		#define KOS_CHECK_FONT(return_value) { \
			if (this < 0 && this >= KOS_MAX_FONTS && !kos_fonts[this].used) { \
				printf("WARNING Font %lld does not exist\n", this); \
				return (return_value); \
			} \
		}
	#endif
	
	static void kos_unuse_font(kos_font_t* this) {
		this->used    = 0;
		this->text    = NULL;
		
		#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
			this->font    = NULL;
			this->surface = NULL;
		#endif
		
	}
	
	void kos_init_fonts(void) { /// TO... IMPLEMENT?
		#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
			kos_font_colour.r = 0xFF;
			kos_font_colour.g = 0xFF;
			kos_font_colour.b = 0xFF;
			kos_font_colour.a = 0xFF;
		#else
		#endif
		
		unsigned long long i;
		for (i = 0; i < KOS_MAX_FONTS; i++) {
			kos_unuse_font(&kos_fonts[i]);
			
		}
		
		#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
			if (TTF_Init() == -1) {
				printf("WARNING SDL2 TTF could not initialize (%s)\n", TTF_GetError());
				return;
				
			}
		#elif KOS_USES_SDL2
			if (FT_Init_FreeType(&kos_freetype_library)) {
				printf("WARNING FreeType could not initialize\n");
				return;
				
			}
		#endif
		
	}
	
	void kos_destroy_fonts(void) {
		#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
			TTF_Quit();
		#elif KOS_USES_SDL2
			FT_Done_FreeType(kos_freetype_library);
		#endif
		
	}
	
	#if !(__USE_SDL_TTF_PROVIDED) && KOS_USES_SDL2
		unsigned long long kos_freetype_new_font(const char* path, unsigned long long size, FT_Face* font) {
			unsigned long long font_loading_error = FT_New_Face(kos_freetype_library, path, 0, font);
			
			if (font_loading_error == FT_Err_Unknown_File_Format) printf("WARNING Font could not be loaded (unknown file format)\n");
			if (FT_Set_Char_Size(*font, 0, size << 6, 300, 300))  printf("WARNING Failed to set font size\n");
			
			return font_loading_error;
			
		}
	#endif
	
	unsigned long long video_width(void);
	
	font_t new_font(const char* _path, unsigned long long size) {
		unsigned long long i;
		for (i = 0; i < KOS_MAX_FONTS; i++) {
			if (!kos_fonts[i].used) {
				kos_fonts[i].used = 1;
				
				GET_PATH((char*) _path);
				memcpy(kos_fonts[i].path, path, MAX_PATH_LENGTH * sizeof(char));
				
				kos_fonts[i].size = (float) size / _UI64_MAX;
				unsigned char font_loading_error = 0;
				unsigned long long size = kos_fonts[i].size * video_width();
				
				#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
					kos_fonts[i].font  = TTF_OpenFont(kos_fonts[i].path, size);
					font_loading_error = !kos_fonts[i].font;
				#elif KOS_USES_SDL2
					font_loading_error = kos_freetype_new_font(kos_fonts[i].path, size, &kos_fonts[i].font);
				#endif
				
				if (font_loading_error) {
					printf("WARNING Font could not be loaded (possibly an incorrect path? `%s`)\n", path);
					kos_fonts[i].used = 0;
					
					return -1;
					
				}
				
				return i;
				
			}
			
		}
		
		printf("WARNING You have surpassed the maximum font count (KOS_MAX_FONTS = %d)\n", KOS_MAX_FONTS);
		return -1;
		
	}
	
	void update_all_font_sizes(void) {
		unsigned long long i;
		for (i = 0; i < KOS_MAX_FONTS; i++) {
			if (kos_fonts[i].used) {
				unsigned char font_loading_error = 0;
				unsigned long long size = kos_fonts[i].size * video_width();
				
				#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
					TTF_CloseFont(kos_fonts[i].font);
					kos_fonts[i].font = TTF_OpenFont(kos_fonts[i].path, size);
				#elif KOS_USES_SDL2
					FT_Done_Face(kos_fonts[i].font);
					kos_freetype_new_font(kos_fonts[i].path, size, &kos_fonts[i].font);
				#endif
				
			}
			
		}
		
	}
	
	static void kos_font_create_text(kos_font_t* this, char* text) {
		if (
		#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
			!this->surface ||
		#endif
			(this->text == NULL || strcmp(text, this->text) != 0)) {
			#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
				if (this->surface) {
					SDL_FreeSurface(this->surface);
					this->surface = NULL;
					
				}
			#endif
			
			if      (this->text) {
				free(this->text);
				
			}
			
			this->text = (char*) malloc(strlen(text) + 1);
			strcpy(this->text,                 text);
			
			#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
				SDL_Surface* temp = TTF_RenderUTF8_Blended(this->font, text, kos_font_colour);
				this->surface = SDL_CreateRGBSurface(0, temp->w, temp->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
				
				SDL_BlitSurface(temp, NULL, this->surface, NULL);
				SDL_FreeSurface(temp);
				
				if (!this->surface) {
					printf("WARNING Could not create the font surface (SDL `%s`, TTF `%s`)\n", SDL_GetError(), TTF_GetError());
					this->used = 0;
					
					return;
					
				}
				
				SDL_LockSurface(this->surface);
				uint64_t* pixels = (uint64_t*) this->surface->pixels;
				
				unsigned long long i;
				for (i = 0; i < (this->surface->w * this->surface->h) / 2; i++) {
					pixels[i] &= 0xFF000000FF000000;
					pixels[i] += 0x00FFFFFF00FFFFFF;
					
				}
			#elif KOS_USES_SDL2
				FT_Matrix matrix;
				
				matrix.xx = (FT_Fixed) (1 * 0x10000L);
				matrix.xy = (FT_Fixed) (0 * 0x10000L);
				matrix.yx = (FT_Fixed) (0 * 0x10000L);
				matrix.yy = (FT_Fixed) (1 * 0x10000L);
				
				FT_Vector position;
				
				position.x = 0;
				position.y = 0;
				
				FT_GlyphSlot slot = this->font->glyph;
				
				this->surface.w = 1000;
				this->surface.h = 1000;
				this->surface.pixels = (unsigned long long*) malloc(this->surface.w * this->surface.h * 4);
				
				unsigned long long i;
				for (i = 0; i < strlen(text) - 1; i++) {
					FT_Set_Transform(this->font, &matrix, &position);
					
					if (FT_Load_Char(this->font, text[i], FT_LOAD_RENDER)) {
						printf("WARNING Failed to load \"%d\" (or %c) character\n", text[i], text[i]);
						continue;
						
					}
					
					FT_Bitmap bitmap = slot->bitmap;
					
					unsigned long long ox = slot->bitmap_left;
					unsigned long long oy = slot->bitmap_top;
					
					unsigned long long xmax = ox + bitmap.width;
					unsigned long long ymax = oy + bitmap.rows;
					
					unsigned long long x;
					unsigned long long y;
					
					unsigned long long p;
					unsigned long long q;
					
					printf("%lld %lld\n", xmax, ymax);
					
					for (x = ox; x < xmax; x++, p++) {
						for (y = oy; y < ymax; y++, q++) {
							if (x < 0 || y < 0) continue;
							((char*) this->surface.pixels)[q * bitmap.width + p] |= bitmap.buffer[q * bitmap.width + p];
							
						}
						
					}
					
					position.x += slot->advance.x;
					position.y += slot->advance.y;
					
				}
			#endif
			
		}
		
	}
	
	unsigned long long font_remove(font_t this) {
		KOS_CHECK_FONT(-1)
		
		if (kos_fonts[this].text != NULL) {
			free(kos_fonts[this].text);
			
		}
		
		#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
			if (kos_fonts[this].surface) {
				SDL_FreeSurface(kos_fonts[this].surface);
				
			}
			
			TTF_CloseFont(kos_fonts[this].font);
		#elif KOS_USES_SDL2
			FT_Done_Face(kos_fonts[this].font);
		#endif
		
		kos_unuse_font(&kos_fonts[this]);
		return 0;
		
	}
	
	texture_t create_texture_from_font(font_t this, char* text) {
		KOS_CHECK_FONT(0)
		
		kos_font_t* font = &kos_fonts[this];
		kos_font_create_text(font, text);
		
		#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
			return __texture_create(font->surface->pixels, 32, font->surface->w, font->surface->h, 0);
		#elif KOS_USES_SDL2
			return __texture_create(font->surface.pixels, 32, font->surface.w, font->surface.h, 0);
		#else
			return 0;
		#endif
		
	}
	
	unsigned long long get_font_width(font_t this, char* text) {
		KOS_CHECK_FONT(-1)
		
		kos_font_t* font = &kos_fonts[this];
		kos_font_create_text(font, text);
		
		#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
			return font->surface->w;
		#elif KOS_USES_SDL2
			return font->surface.w;
		#else
			return 100;
		#endif
		
	}
	
	unsigned long long get_font_height(font_t this, char* text) {
		KOS_CHECK_FONT(-1)
		
		kos_font_t* font = &kos_fonts[this];
		kos_font_create_text(font, text);
		
		#if __USE_SDL_TTF_PROVIDED && KOS_USES_SDL2
			return font->surface->h;
		#elif KOS_USES_SDL2
			return font->surface.h;
		#else
			return 100;
		#endif
		
	}
	
#endif
