package com.inobulles.obiwac.aqua;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

public class InstanceActivity extends Activity {
	public static String TAG = "AQUA";

	public static AssetManager   assets;
	public static PackageManager package_manager;

	public TextInput text_input;

	public boolean standalone = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Lib.give_activity(this);

		super.onCreate(savedInstanceState);
		onWindowFocusChanged(true);

		BufferedReader meta_reader = null;
		String         orientation = "landscape";
		List<String>   permissions = new ArrayList<>();

		standalone = false;

		try {
			meta_reader = new BufferedReader(new InputStreamReader(getAssets().open("root/meta.meta")));
			String line;

			while ((line = meta_reader.readLine()) != null) {
				String[] components = line.split(":");

				switch (components[0]) {
					case "orientation": orientation =   components[1];                                        break;
					case "permission":  permissions.add(components[1]);                                       break;
					case "tag":         TAG         =   components[1];                                        break;
					case "standalone":  standalone  =   components[1].compareTo("true") == 0;                 break;
					default: Log.e(TAG, String.format("WARNING Unknown meta element (%s)\n", components[0])); break;

				}

			}

		} catch (IOException exception) {
			Log.e(TAG, "WARNING Could not load meta file (meta.meta)\n");

			permissions.add("read external storage");
			permissions.add("write external storage");
			permissions.add("internet");

		} finally {
			if (meta_reader != null) {
				try                           { meta_reader.close();         }
				catch (IOException exception) { exception.printStackTrace(); }

			}

		}

		Lib.give_log_tag(TAG, standalone);

		switch (orientation) {
			case "landscape": setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);    break;
			case "portrait":  setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);     break;
			case "full":      setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);         break;
			default:          Log.e(TAG, String.format("WARNING Unknown orientation `%s`\n", orientation)); break;

		}

		package_manager = getApplicationContext().getPackageManager();
		assets = getAssets();

		create_view_thread();
		File directory = Environment.getExternalStorageDirectory();

		if (directory == null) Log.w(TAG, "WARNING Failed to get external path\n");
		else Lib.give_internal_storage_path(directory.toString());

		if (Build.VERSION.SDK_INT >= 23) {
			String   manifest_permission;
			String[] manifest_permissions = new String[permissions.size()];
			int current = 0;

			for (String permission : permissions) {
				switch (permission) {
					case "read external storage":  manifest_permission = Manifest.permission.READ_EXTERNAL_STORAGE;  break;
					case "write external storage": manifest_permission = Manifest.permission.WRITE_EXTERNAL_STORAGE; break;
					case "internet":               manifest_permission = Manifest.permission.INTERNET;               break;
					default:                       manifest_permission = permission;                                 break;

				}

				manifest_permissions[current++] = manifest_permission;

			}

			boolean asking_permissions = true;
			while  (asking_permissions) {
				asking_permissions = false;

				for (String permission : manifest_permissions) {
					if (ActivityCompat.checkSelfPermission(this, permission) == PackageManager.PERMISSION_DENIED) {
						asking_permissions = true;

					}

				}

				this.requestPermissions(manifest_permissions, 100);

			}

		} else {
			Log.e(TAG, String.format("WARNING Can not set permissions as Build.VERSION.SDK_INT (%d) < 23. Put permissions in `AndroidManifest.xml`\n", Build.VERSION.SDK_INT));

		}

		text_input = new TextInput(InstanceActivity.this);
		directory  = new File(directory + File.separator + "root" + File.separator + "root");

		if (!directory.exists() && !directory.isDirectory() && !directory.mkdirs()) {
			Log.w(TAG, "WARNING Could not create `root/root` directory\n");

		}

	}

	protected ViewThread view_thread = null;

	protected void create_view_thread() {
		if (view_thread == null) {
			view_thread = new ViewThread() {
				@Override
				public void view_run() {
					setContentView(new View(getApplication()));

				}

			};
			runOnUiThread(view_thread);

		}

	}

	public void start_activity(Intent intent) {
		getApplicationContext().startActivity(intent);

	}

	private void dispose_all() {
		if (Lib.fonts != null) {
			Lib.dispose_all();
			Lib.clear_fonts();

		}

	}

	protected void resize() { create_view_thread(); }
	protected void kill()   { finish(); }

	@Override
	protected void onDestroy() {
		dispose_all();
		super.onDestroy();

	}

	protected boolean first_pause = false;

	@Override
	protected void onPause() {
		super.onPause();

		if  (first_pause) resize();
		else first_pause = true;

	}

	@Override protected void onResume()                                      { super.onResume(); }
	@Override public    void onConfigurationChanged(Configuration newConfig) { super.onConfigurationChanged(newConfig); }

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int tray_offset = 0;

		int x = (int) event.getX();
		int y = (int) event.getY();

		int pointer_index = event.getActionIndex();
		int pointer_id    = event.getPointerId(pointer_index);
		int masked_action = event.getActionMasked();

		switch (masked_action) {
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
			case MotionEvent.ACTION_MOVE: {
				Lib.event(pointer_index, 1, x, y, 0, 0, tray_offset);
				break;

			}

			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_CANCEL: {
				Lib.event(pointer_index, 1, x, y, 0, 1, tray_offset);
				break;

			} default: {
				Log.e("AQUA", String.format("WOOWOWOWOWO NEW TOUCGH EVENT DETECTED %d %d %d %d %d", pointer_index, pointer_id, masked_action, x, y));
				return super.onTouchEvent(event);

			}

		}

		return true;

	}

	private boolean on_key(int key_code, KeyEvent event, int release) {
		switch (key_code) {
			case KeyEvent.KEYCODE_HEADSETHOOK: {
				Lib.event_macro(0, release == 0 ? 1 : 0);
				return true;

			} default: {
				return false;

			}

		}

	}

	@Override public boolean onKeyDown(int keyCode, KeyEvent event) { return on_key(keyCode, event, 0) || super.onKeyDown(keyCode, event); }
	@Override public boolean onKeyUp  (int keyCode, KeyEvent event) { return on_key(keyCode, event, 1) || super.onKeyUp(keyCode, event); }

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);

		if (hasFocus) {
			getWindow().getDecorView().setSystemUiVisibility(
				(Build.VERSION.SDK_INT > 16 ? View.SYSTEM_UI_FLAG_LAYOUT_STABLE          : 0) |
				(Build.VERSION.SDK_INT > 16 ? View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION : 0) |
				(Build.VERSION.SDK_INT > 16 ? View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN      : 0) |
				(Build.VERSION.SDK_INT > 14 ? View.SYSTEM_UI_FLAG_HIDE_NAVIGATION        : 0) |
				(Build.VERSION.SDK_INT > 16 ? View.SYSTEM_UI_FLAG_FULLSCREEN             : 0) |
				(Build.VERSION.SDK_INT > 19 ? View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY       : 0)
			);

		}

	}

	@Override
	public void onBackPressed() { super.onBackPressed(); }

}
