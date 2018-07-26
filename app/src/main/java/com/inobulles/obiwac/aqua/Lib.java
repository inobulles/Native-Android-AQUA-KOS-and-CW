
package com.inobulles.obiwac.aqua;

import android.content.res.AssetManager;
import android.util.Log;

public class Lib {
	static {
		System.loadLibrary("native-lib");

	}

	private static final int MAX_FONTS = 1024;
	private static Font fonts[];

	public static void init_lib() {
		fonts = new Font[MAX_FONTS];

		int i;
		for (i = 0; i < MAX_FONTS; i++) {
			fonts = null;

		}

	}

	public static int new_font(String path, int size) {
		int index = -1;

		int i;
		for (i = 0; i < MAX_FONTS; i++) {
			if (fonts[i] == null) {
				index = i;
				break;

			}

		}

		if (index < 0) {
			Log.w(MainActivity.TAG, String.format("WARNING You have too many fonts allocated (MAX_FONTS = %d) (use `font_remove to remove them`)\n", MAX_FONTS));
			return -1;

		}

		fonts[index] = new Font(path, size);
		return index;

	}

	public static void font_remove(int font) {
		try {
			fonts[font].dispose();
			fonts[font] = null;

		} catch (IndexOutOfBoundsException exception) {
			// font >= MAX_FONTS

		}

	}

	private static int get_font_dimension(String dimension, int font, String text) {
		try {
			if (fonts[font] == null) {
				Log.w(MainActivity.TAG, String.format("WARNING Cannot get font %s, as font %d does not exist\n", dimension, font));
				return -1;

			} else {
				if (dimension.equals("width")) return fonts[font].get_width(text);
				else if (dimension.equals("height")) return fonts[font].get_height(text);

				return -1;

			}

		} catch (IndexOutOfBoundsException exception) {
			// font >= MAX_FONTS
			return -1;

		}

	}

	public static int get_font_width(int font, String text) { return get_font_dimension("width", font, text); }
	public static int get_font_height(int font, String text) { return get_font_dimension("height", font, text); }

	public static byte[] create_texture_from_font(int font, String text) { /// TODO
		try {
			if (fonts[font] == null) return fonts[font].draw(text);
			else return null;

		} catch (IndexOutOfBoundsException exception) {
			// font >= MAX_FONTS
			return null;

		}

	}

	public static native void init(AssetManager asset_manager);
	public static native void resize(int width, int height);
	public static native void step();
	public static native void event(int pointer_index, int pointer_type, int x, int y, int quit, int release);
	public static native void give_internal_storage_path(String path);

}