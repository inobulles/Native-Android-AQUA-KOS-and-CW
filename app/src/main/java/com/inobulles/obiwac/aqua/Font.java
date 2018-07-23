package com.inobulles.obiwac.aqua;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Typeface;

import java.io.ByteArrayOutputStream;

public class Font {
	private Paint paint;

	public Font(String path, int size) {
		Typeface face = Typeface.createFromFile(path);
		paint = new Paint();

		paint.setAntiAlias(true);
		paint.setTextSize(size);
		paint.setColor(0xFFFFFFFF);
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
		bitmap.eraseColor(0x00000000);

		canvas.drawText(text, 0, 1, 0, 0, paint);

		ByteArrayOutputStream stream = new ByteArrayOutputStream();
		bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream);

		return stream.toByteArray();

	}

	public void dispose() {

	}

}
