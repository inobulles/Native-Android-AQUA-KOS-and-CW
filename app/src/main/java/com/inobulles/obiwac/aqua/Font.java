package com.inobulles.obiwac.aqua;

import android.graphics.*;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.util.Log;

import java.nio.IntBuffer;

public class Font {
	/*private Paint paint;
	private Paint.FontMetrics metrics;*/

	private TextPaint paint;
	private StaticLayout layout;
	private String current;

	Font(String path, int size) {
		Typeface face = Typeface.createFromAsset(InstanceActivity.assets, path);
		/*paint = new Paint();

		paint.setAntiAlias(true);
		paint.setTextSize(size);
		paint.setColor(Color.WHITE);
		paint.setStyle(Paint.Style.FILL);
		paint.setTypeface(face);
		paint.setHinting(Paint.HINTING_ON);

		metrics = paint.getFontMetrics();*/

		paint = new TextPaint();
		layout = null;

		paint.setAntiAlias(true);
		paint.setTextSize(size);
		paint.setColor(Color.WHITE);
		paint.setStyle(Paint.Style.FILL);
		paint.setTypeface(face);
		paint.setHinting(Paint.HINTING_ON);

	}

	private void create(String text) {
		if (text == null) {
			text = "FIXME: NO TEXT PASSED";

		} if (current == null || !current.equals(text) || layout == null) {
			current = text;
			layout = new StaticLayout(text, 0, text.length(), paint, Lib.main_activity.findViewById(android.R.id.content).getRootView().getWidth(), Layout.Alignment.ALIGN_NORMAL, 1.0f, 0.0f, false);//(current, paint, Lib.main_activity.findViewById(android.R.id.content).getRootView().getWidth(), Layout.Alignment.ALIGN_NORMAL, 1.0f, 0.0f, false);

		}

	}

	private int internal_get_width() {
		Rect bounds = new Rect();
		paint.getTextBounds(current, 0, current.length(), bounds);
		return (int) Math.ceil(bounds.width());


	}

	public int get_width (String text) { create(text); return internal_get_width(); }
	public int get_height(String text) { create(text); return layout.getHeight  (); }

	public int draw(String text, int TEXTURE_WRAP_TYPE, int SHARP_TEXTURES) {
		create(text);

		Bitmap bitmap = Bitmap.createBitmap(internal_get_width(), layout.getHeight(), Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);

		canvas.save();
		bitmap.eraseColor(Color.TRANSPARENT);

		canvas.translate(0, 0);
		layout.draw(canvas);
		canvas.restore();

		int texture = Utils.create_texture_from_bitmap(bitmap, TEXTURE_WRAP_TYPE, SHARP_TEXTURES);
		bitmap.recycle();
		return texture;

	}

	public void dispose() {

	}

}
