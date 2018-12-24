package com.inobulles.obiwac.aqua;

import android.graphics.Bitmap;
import android.opengl.GLES20;
import android.opengl.GLUtils;

import java.nio.IntBuffer;

public class Utils {
	public static int create_texture_from_bitmap(Bitmap bitmap, int TEXTURE_WRAP_TYPE, int SHARP_TEXTURES) {
		GLES20.glPixelStorei(GLES20.GL_UNPACK_ALIGNMENT, 1);

		IntBuffer texture = IntBuffer.allocate(1);
		GLES20.glGenTextures(1, texture);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texture.get(0));

		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, TEXTURE_WRAP_TYPE); // x axis
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, TEXTURE_WRAP_TYPE); // y axis

		if (SHARP_TEXTURES == 1) {
			GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);
			GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);

		} else {
			GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
			GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);

		}

		GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
		return texture.get();

	}

}
