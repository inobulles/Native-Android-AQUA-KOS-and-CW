
#ifndef __AQUA__KOS_FUNCTIONS_SCREENSHOT_H
	#define __AQUA__KOS_FUNCTIONS_SCREENSHOT_H
	
	#define CREATE_TEXTURE_FROM_SCREENSHOT_FUNCTION 1
	
	texture_t create_texture_from_screenshot(void) {
		unsigned long long bpp   = 32;
		unsigned long long bytes = video_width() * video_height() * (bpp / 8);
		
		unsigned char* pixels = (unsigned char*) malloc(bytes);
		glReadBuffer(GL_FRONT);
		glReadPixels(0, 0, (GLsizei) video_width(), (GLsizei) video_height(), GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		
		unsigned long long* data  = (unsigned long long*) malloc(bytes);
		unsigned char*      data8 = (unsigned char*)      data;
		unsigned long long  pitch = video_width() * (bpp / 8);
		
		int y;
		for (y = 0; y < video_height(); y++) {
			memcpy(data8 + (video_height() - y - 1) * pitch, pixels + y * pitch, pitch);
			
		}
		
		texture_t texture = texture_create(data, bpp, video_width(), video_height());
		
		free(pixels);
		free(data);
		
		if (texture < 0) {
			printf("WARNING Could not create texture from screenshot\n");
			
		}
		
		return texture;
		
	}
	
#endif
