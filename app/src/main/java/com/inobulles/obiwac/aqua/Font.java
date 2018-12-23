package com.inobulles.obiwac.aqua;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.opengl.GLES20;
import android.opengl.GLUtils;

import java.nio.IntBuffer;

public class Font {
	private Paint paint;
	private Paint.FontMetrics metrics;

	Font(String path, int size) {
		Typeface face = Typeface.createFromAsset(InstanceActivity.assets, path);
		paint = new Paint();

		paint.setAntiAlias(true);
		paint.setTextSize(size);
		paint.setColor(Color.WHITE);
		paint.setStyle(Paint.Style.FILL);
		paint.setTypeface(face);

		metrics = paint.getFontMetrics();

	}

	public int get_width (String text) { return (int) paint.measureText(text); }
	public int get_height(String text) { return (int) Math.ceil(metrics.descent - metrics.ascent); }

	public int draw(String text, int TEXTURE_WRAP_TYPE, int SHARP_TEXTURES) {
		if (text == null) {
			text = "Hello world!";

		}



		long texture_width  = (long) get_width( text);
		long texture_height = (long) get_height(text);

		Bitmap bitmap = Bitmap.createBitmap((int) texture_width, (int) texture_height, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		bitmap.eraseColor(Color.TRANSPARENT);

		canvas.drawText(text, 0, (int) texture_height - metrics.descent, paint);

		/*int[] pixels = new int[bitmap.getWidth() * bitmap.getHeight()];
		bitmap.getPixels(pixels, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());

		long i;
		for (i = 0; i < pixels.length; i++) {
			pixels[(int) i] = (pixels[(int) i] & 0xFF000000);

		}

		bitmap.setPixels(pixels, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());*/
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
		bitmap.recycle();

		return texture.get();

	}

	public void dispose() {

	}

}
