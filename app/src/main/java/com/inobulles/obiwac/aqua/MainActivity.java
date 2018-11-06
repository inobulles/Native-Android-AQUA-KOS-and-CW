
package com.inobulles.obiwac.aqua;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity {
	protected Runnable current_runnable;

	protected void create_thread() {
		current_runnable = new Runnable() {
			@Override
			public void run() {
				Intent intent = new Intent(MainActivity.this, InstanceActivity.class);
				startActivity(intent);


			}

		};

		new Thread(current_runnable).start();

	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		create_thread();

	}

	protected boolean first_pause = true;

	@Override
	public void onResume() {
		super.onResume();

		if (!first_pause) create_thread();
		else first_pause = false;

	}

	@Override
	protected void onDestroy() {
		Log.e(InstanceActivity.TAG, "onDestroy");
		super.onDestroy();

	}

}
