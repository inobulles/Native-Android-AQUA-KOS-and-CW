
package com.inobulles.obiwac.aqua;

import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import static com.inobulles.obiwac.aqua.MainActivity.TAG;

public class Lib {
	static {
		System.loadLibrary("native-lib");

	}

	private static final int MAX_FONTS = 1024;
	private static Font fonts[];

	public static void init_lib() {
		Log.v(MainActivity.TAG, "`init_lib` called\n");
		fonts = new Font[MAX_FONTS];

		int i;
		for (i = 0; i < MAX_FONTS; i++) {
			fonts[i] = null;

		}

	}

	private static String         buffer_path;
	private static File           buffer_file;
	private static BufferedReader buffer_buffered_reader;

	private static boolean put_file_in_buffer(String path) {
		if (buffer_buffered_reader != null) {
			try {
				buffer_buffered_reader.close();

			} catch (IOException exception) {
				exception.printStackTrace();

			}

		}

		buffer_path = Environment.getExternalStorageDirectory().getPath() + File.separator + path;
		buffer_file = new File(buffer_path);

		try {
			buffer_buffered_reader = new BufferedReader(new FileReader(buffer_file));
			return false;

		} catch (FileNotFoundException exception) {
			Log.w(TAG, String.format("WARNING File `%s` most probably does not exist\n", buffer_path));
			return true;

		}

	}

	public static long read_external_slash_internal_storage_path_bytes(String path) {
		if (put_file_in_buffer(path)) return -1;
		else                          return buffer_file.length();

	}

	public static String read_external_slash_internal_storage_path(String path) {
		if (!path.equals(buffer_path)) {
			if (!put_file_in_buffer(path)) {
				try {
					StringBuilder buffer = new StringBuilder();
					String        line;

					while ((line = buffer_buffered_reader.readLine()) != null) {
						buffer.append(line);
						buffer.append("\n");

					}

					return buffer.toString();

				} catch (IOException exception) {
					exception.printStackTrace();

				}

			}

		}

		Log.e(MainActivity.TAG, String.format("Failed to load file `%s`\n", path));
		return "(null)";

	}

	public static int new_font(int size, String path) {
		int index = -1;

		if (fonts == null) {
			Log.e(MainActivity.TAG, "WARNING You have not called `init_lib`\n");
			init_lib();

		}

		int i;
		for (i = 0; i < MAX_FONTS; i++) {
			if (fonts[i] == null) { /// TODO
				index = i;
				break;

			}

		}

		if (index < 0) {
			Log.w(TAG, String.format("WARNING You have too many fonts allocated (MAX_FONTS = %d) (use `font_remove` to remove them)\n", MAX_FONTS));
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
				Log.w(TAG, String.format("WARNING Cannot get font %s, as font %d does not exist\n", dimension, font));
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
			if (fonts[font] != null) return fonts[font].draw(text);
			else return null;

		} catch (IndexOutOfBoundsException exception) {
			// font >= MAX_FONTS
			return null;

		}

	}

	public static native void init(AssetManager asset_manager);
	public static native void dispose_all();
	public static native void resize(int width, int height);
	public static native void step();
	public static native void event(int pointer_index, int pointer_type, int x, int y, int quit, int release);
	public static native void give_internal_storage_path(String path);

}