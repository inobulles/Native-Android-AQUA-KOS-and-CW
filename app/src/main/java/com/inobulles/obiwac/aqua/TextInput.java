package com.inobulles.obiwac.aqua;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.text.InputType;
import android.widget.EditText;

public class TextInput {
	private InstanceActivity activity;

	private AlertDialog.Builder builder;
	private final EditText      input;

	private boolean has_yet_responded;

	private void ok_text(String text) {
		has_yet_responded = true;
		Lib.text_input_response(text);

	}

	private void cancel_text() {
		has_yet_responded = true;
		Lib.text_input_response(null);

	}

	TextInput(Context context) {
		has_yet_responded = false;

		input = new EditText(context);
		input.setInputType(InputType.TYPE_CLASS_TEXT /* | InputType.TYPE_TEXT_VARIATION_NORMAL */);

		builder = new AlertDialog.Builder(context);

		builder.setTitle  ("Text input");
		builder.setMessage("Default Android text input for AQUA");
		builder.setView   (input);

		builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				ok_text(input.getText().toString());

			}
		});

		builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				dialog.cancel();
				cancel_text();

			}

		});

	}

	public void create() {
		builder.show();

	}

}
