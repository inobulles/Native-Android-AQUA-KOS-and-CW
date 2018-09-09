
package com.inobulles.obiwac.aqua;

import android.Manifest;
import android.app.Activity;

import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.util.Log;
import android.view.MotionEvent;

import java.io.File;

public class MainActivity extends /* AppCompatActivity */ Activity {
	public static final String TAG = "AQUA";

	private static View view;
	public  static AssetManager assets;

	private static boolean first_resume;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		assets = getAssets();

		first_resume = false;

		view = new View(getApplication());
		setContentView(view);

		File directory = Environment.getExternalStorageDirectory();

		if (directory == null) Log.w(TAG, "WARNING Failed to get external path\n");
		else Lib.give_internal_storage_path(directory.toString());

		if (Build.VERSION.SDK_INT >= 23) {
			int read_permission  = PackageManager.PERMISSION_DENIED;
			int write_permission = PackageManager.PERMISSION_DENIED;

			while (read_permission != PackageManager.PERMISSION_GRANTED && write_permission != PackageManager.PERMISSION_GRANTED) {
				read_permission  = ActivityCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE);
				write_permission = ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE);

				if (read_permission != PackageManager.PERMISSION_GRANTED || write_permission != PackageManager.PERMISSION_GRANTED) {
					this.requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE}, 100);

				}

			}

		}

	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		Lib.dispose_all();

	}

	@Override
	protected void onPause() {
		super.onPause();
		view.onPause();
		Lib.dispose_all();
		
	}

	@Override
	protected void onResume() {
		super.onResume();
		view.onResume();

		/*if (first_resume) {
			Lib.start();

		} else {
			first_resume = true;

		}*/

	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int x = (int) event.getX();
		int y = (int) event.getY();

		int pointer_index = event.getActionIndex();
		int pointer_id = event.getPointerId(pointer_index);
		int masked_action = event.getActionMasked();

		switch (masked_action) {
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
			case MotionEvent.ACTION_MOVE: {
				Lib.event(pointer_index, 1, x, y, 0, 0);
				break;

			}

			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_CANCEL: {
				Lib.event(pointer_index, 1, x, y, 0, 1);
				break;
				
			} default: {
				Log.e("AQUA", String.format("WOOWOWOWOWO NEW TOUCGH EVENT DETECTED %d %d %d %d %d", pointer_index, pointer_id, masked_action, x, y));
				break;

			}

		}
		
//		invalidate();
		return true;

	}

}
