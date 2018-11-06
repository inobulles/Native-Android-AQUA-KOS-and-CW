
package com.inobulles.obiwac.aqua;

import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.List;

public class Lib {
	static {
		System.loadLibrary("native-lib");

	}

	private static final int MAX_FONTS = 4096;
	public  static Font fonts[];

	private static void init_lib() {
		Log.v(InstanceActivity.TAG, "`init_lib` called\n");
		fonts = new Font[MAX_FONTS];
		clear_fonts();

	}

	public static void clear_fonts() {
		int i;
		for (i = 0; i < MAX_FONTS; i++) {
			fonts[i] = null;

		}

	}

	private static String         buffer_path;
	private static File           buffer_file;
	private static BufferedReader buffer_buffered_reader;

	private static boolean put_file_in_buffer(String path) {
		Log.e(InstanceActivity.TAG, String.format("WARNING This function (%s) is deprecated\n", new Object(){}.getClass().getEnclosingMethod().getName()));

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
			Log.w(InstanceActivity.TAG, String.format("WARNING File `%s` most probably does not exist\n", buffer_path));
			return true;

		}

	}

	private static long read_external_slash_internal_storage_path_bytes(String path) {
		Log.e(InstanceActivity.TAG, String.format("WARNING This function (%s) is deprecated\n", new Object(){}.getClass().getEnclosingMethod().getName()));

		if (put_file_in_buffer(path)) return -1;
		else                          return buffer_file.length();

	}

	private static String read_external_slash_internal_storage_path(String path) {
		Log.e(InstanceActivity.TAG, String.format("WARNING This function (%s) is deprecated\n", new Object(){}.getClass().getEnclosingMethod().getName()));

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

		Log.e(InstanceActivity.TAG, String.format("Failed to load file `%s`\n", path));
		return "(null)";

	}

	private static int new_font(int size, String path) {
		int index = -1;

		if (fonts == null) {
			Log.e(InstanceActivity.TAG, "WARNING You have not called `init_lib`\n");
			init_lib();

		}

		int i;
		for (i = 0; i < MAX_FONTS; i++) {
			if (fonts[i] == null) {
				index = i;
				break;

			}

		}

		if (index < 0) {
			Log.w(InstanceActivity.TAG, String.format("WARNING You have too many fonts allocated (MAX_FONTS = %d) (use `font_remove` to remove them)\n", MAX_FONTS));
			return -1;

		}

		fonts[index] = new Font(path, size);
		return index;

	}

	private static void font_remove(int font) {
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
				Log.w(InstanceActivity.TAG, String.format("WARNING Cannot get font %s, as font %d does not exist\n", dimension, font));
				return -1;

			} else {
				if      (dimension.equals("width"))  return fonts[font].get_width(text);
				else if (dimension.equals("height")) return fonts[font].get_height(text);

				return -1;

			}

		} catch (IndexOutOfBoundsException exception) {
			// font >= MAX_FONTS
			return -1;

		}

	}

	private static int get_font_width (int font, String text) { return get_font_dimension("width",  font, text); }
	private static int get_font_height(int font, String text) { return get_font_dimension("height", font, text); }

	private static int create_texture_from_font(int font, String text, int TEXTURE_WRAP_TYPE, int SHARP_TEXTURES) {
		try {
			if (fonts[font] != null) return fonts[font].draw(text, TEXTURE_WRAP_TYPE, SHARP_TEXTURES);
			else                     return -1;

		} catch (IndexOutOfBoundsException exception) {
			// font >= MAX_FONTS
			return -1;

		}

	}

	private static InstanceActivity main_activity;

	public static void give_activity(InstanceActivity activity) {
		main_activity = activity;


	}

	private static Intent most_recent_package_intent;

	private static int package_exists(String package_name) {
		most_recent_package_intent = InstanceActivity.package_manager.getLaunchIntentForPackage(package_name);

		if (most_recent_package_intent == null) {
			return 0;

		}

		List<ResolveInfo> list = InstanceActivity.package_manager.queryIntentActivities(most_recent_package_intent, PackageManager.MATCH_DEFAULT_ONLY);
		return list.size() > 0 ? 1 : 0;

	}

	private static void package_open(String package_name) {
		if (package_exists(package_name) == 0) {
			Log.e(InstanceActivity.TAG, String.format("ERROR Package `%s` does not seem to exist\n", package_name));
			return;

		}

		main_activity.start_activity(most_recent_package_intent);

	}

	public static void text_input_response(String text) {
		give_text_input_response(text != null, text);

	}

	public static void open_text_input() {
		main_activity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				main_activity.text_input.create();
			}
		});

	}

	public static native void give_text_input_response(boolean has_response, String response);
	public static native void give_log_tag(String tag, boolean standalone);
	public static native void init(AssetManager asset_manager);
	public static native void dispose_all();
	public static native void start();
	public static native void resize(int width, int height);
	public static native void step();
	public static native void event(int pointer_index, int pointer_type, int x, int y, int quit, int release, int tray_offset);
	public static native void event_macro(int macro, int set);
	public static native void give_internal_storage_path(String path);

}