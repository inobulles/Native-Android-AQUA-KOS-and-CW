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
	private TextPaint paint;
	private StaticLayout layout;
	private String current;

	Font(String path, int size) {
		Typeface face = Typeface.createFromAsset(InstanceActivity.assets, path);

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
			text = "FIXME No text passed";

		} if (current == null || !current.equals(text) || layout == null) {
			current = text;
			layout = new StaticLayout(text, 0, text.length(), paint, Lib.main_activity.findViewById(android.R.id.content).getRootView().getWidth(), Layout.Alignment.ALIGN_NORMAL, 1.0f, 0.0f, false);//(current, paint, Lib.main_activity.findViewById(android.R.id.content).getRootView().getWidth(), Layout.Alignment.ALIGN_NORMAL, 1.0f, 0.0f, false);

		}

	}

	private int internal_get_width() {
		double width = 0;
		for (int i = 0; i < layout.getLineCount(); i++) {
			width = Math.max(width, layout.getLineWidth(i));

		}

		return (int) Math.ceil(width);

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
