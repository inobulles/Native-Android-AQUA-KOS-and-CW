
package com.inobulles.obiwac.aqua;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;

public class MainActivity extends Activity {
	protected void create_thread() {
		Handler handler = new Handler(Looper.getMainLooper());

		handler.post(new Runnable() {
			@Override
			public void run() {
				Intent intent = new Intent(MainActivity.this, InstanceActivity.class);
				startActivity(intent);

			}

		});

	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		create_thread();

	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

	}

}
