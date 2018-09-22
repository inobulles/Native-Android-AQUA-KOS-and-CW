
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

public class MainActivity extends /* AppCompatActivity */ Activity {
	public static String TAG = "AQUA";

	public static AssetManager   assets;
	public static PackageManager package_manager;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.e(TAG,"onCreate");
		Lib.give_activity(this);

		super.onCreate(savedInstanceState);
		onWindowFocusChanged(true);

		BufferedReader meta_reader = null;
		String       orientation = "landscape";
		List<String> permissions = new ArrayList<>();

		try {
			meta_reader = new BufferedReader(new InputStreamReader(getAssets().open("root/meta.meta")));
			String line;

			while ((line = meta_reader.readLine()) != null) {
				String[] components = line.split(":");

				if      (components[0].equals("orientation")) orientation =   components[1];
				else if (components[0].equals("permission"))  permissions.add(components[1]);
				else if (components[0].equals("tag"))         TAG =           components[1];

			}

		} catch (IOException exception) {
			Log.e(TAG, "WARNING Could not load meta file (meta.meta)\n");

			permissions.add("read external storage");
			permissions.add("write external storage");

		} finally {
			if (meta_reader != null) {
				try                           { meta_reader.close();         }
				catch (IOException exception) { exception.printStackTrace(); }

			}

		}

		switch (orientation) {
			case "landscape": setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);    break;
			case "portrait":  setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);     break;
			case "full":      setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);         break;
			default:          Log.e(TAG, String.format("WARNING Unknown orientation `%s`\n", orientation)); break;

		}

		package_manager = getApplicationContext().getPackageManager();
		assets = getAssets();

		setContentView(new View(getApplication()));
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

	}

	public void start_activity(Intent intent) {
		getApplicationContext().startActivity(intent);

	}

	private void dispose_all() {
		Lib.dispose_all();

	}

	@Override
	protected void onDestroy() {
		Log.e(TAG,"onDestroy");

		super.onDestroy();
		dispose_all();

	}

	@Override
	protected void onPause() {
		Log.e(TAG,"onPause");
		//dispose_all();
		super.onPause();
		
	}

	@Override
	protected void onResume() {
		Log.e(TAG,"onResume");
		//Lib.start();
		super.onResume();

	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		Log.e(TAG, "onConfigurationChanged");
		super.onConfigurationChanged(newConfig);

	}

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
		
//		invalidate();
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

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		return on_key(keyCode, event, 0) || super.onKeyDown(keyCode, event);

	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		return on_key(keyCode, event, 1) || super.onKeyUp(keyCode, event);

	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);

		if (hasFocus) {
			getWindow().getDecorView().setSystemUiVisibility(
				View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
				View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
				View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
				View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
				View.SYSTEM_UI_FLAG_FULLSCREEN |
				View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
			);

		}

	}

	@Override
	public void onBackPressed() {
		super.onBackPressed();

	}

}
