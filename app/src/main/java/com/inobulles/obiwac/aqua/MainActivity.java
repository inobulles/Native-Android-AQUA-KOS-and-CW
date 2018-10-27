
package com.inobulles.obiwac.aqua;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity {
	protected void create_thread() {
		new Thread(new Runnable() {
			@Override
			public void run() {
				Intent intent = new Intent(MainActivity.this, InstanceActivity.class);
				startActivity(intent);


			}

		}).start();

	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		create_thread();

	}

	@Override
	protected void onDestroy() {
		Log.e(InstanceActivity.TAG, "onDestroy");
		super.onDestroy();

	}

}
