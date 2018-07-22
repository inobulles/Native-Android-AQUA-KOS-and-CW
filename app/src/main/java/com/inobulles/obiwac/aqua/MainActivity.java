
package com.inobulles.obiwac.aqua;

//import android.support.v7.app.AppCompatActivity;
import android.app.Activity;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;

public class MainActivity extends /* AppCompatActivity */ Activity {
	private static View view;
	public static AssetManager assets;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		assets = getAssets();

		view = new View(getApplication());
		setContentView(view);

		Lib.give_internal_storage_path(this.getFilesDir().getAbsolutePath());

	}

	@Override
	protected void onPause() {
		super.onPause();
		view.onPause();
		
	}

	@Override
	protected void onResume() {
		super.onResume();
		view.onResume();

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
