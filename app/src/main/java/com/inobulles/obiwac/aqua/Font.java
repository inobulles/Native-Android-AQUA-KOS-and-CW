package com.inobulles.obiwac.aqua;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.util.Log;

import java.nio.ByteBuffer;

public class Font {
	private Paint paint;
	private Paint.FontMetrics metrics;

	Font(String path, int size) {
		Typeface face = Typeface.createFromAsset(MainActivity.assets, path);
		paint = new Paint();

		paint.setAntiAlias(true);
		paint.setTextSize(size);
		paint.setColor(Color.WHITE);
		paint.setTypeface(face);

		metrics = paint.getFontMetrics();

	}

	public int get_width( String text) { return (int) paint.measureText(text); }
	public int get_height(String text) { return (int) Math.ceil(metrics.descent - metrics.ascent); }

	public byte[] draw(String text) {
		long texture_width  = (long) get_width( text);
		long texture_height = (long) get_height(text);

		if (texture_width <= 0 || texture_height <= 0) {
			return null;

		}

		Bitmap bitmap = Bitmap.createBitmap((int) texture_width, (int) texture_height, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		bitmap.eraseColor(0);

		canvas.drawText(text, 0, (int) texture_height - metrics.descent, paint);

		ByteBuffer buffer = ByteBuffer.allocate(bitmap.getRowBytes() * bitmap.getHeight());
		bitmap.copyPixelsToBuffer(buffer);

		byte[] array = buffer.array();
		bitmap.recycle();

		return array;

	}

	public void dispose() {

	}

}
