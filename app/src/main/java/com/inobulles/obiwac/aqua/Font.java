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

	Font(String path, int size) {
		Typeface face = Typeface.createFromAsset(MainActivity.assets, path);
		paint = new Paint();

		paint.setAntiAlias(true);
		paint.setTextSize(size);
		paint.setColor(Color.WHITE);
		paint.setTypeface(face);

	}

	public int get_width(String text) {
		return (int) paint.measureText(text);

	}

	public int get_height(String text) {
		return (int) paint.getTextSize();

	}

	public byte[] draw(String text) {
		long texture_width = get_width(text);
		long texture_height = get_height(text);

		Bitmap bitmap = Bitmap.createBitmap((int) texture_width, (int) texture_height, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		bitmap.eraseColor(0);

		Paint.FontMetrics metric = paint.getFontMetrics();
		canvas.drawText(text, 0, ((int) Math.ceil(metric.descent - metric.ascent)) - (metric.descent * 2), paint); /// TODO (probably not metric.descent * 2)

		ByteBuffer buffer = ByteBuffer.allocate(bitmap.getRowBytes() * bitmap.getHeight());
		bitmap.copyPixelsToBuffer(buffer);

		byte[] array = buffer.array();
		bitmap.recycle();

		return array;

	}

	public void dispose() {

	}

}
