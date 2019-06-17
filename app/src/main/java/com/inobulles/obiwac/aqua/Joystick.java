package com.inobulles.obiwac.aqua;

import android.view.InputDevice;

import java.util.ArrayList;

public class Joystick {
	public static ArrayList<Integer> get_game_controller_ids() {
		ArrayList<Integer> game_controller_ids = new ArrayList<>();
		int[] device_ids = InputDevice.getDeviceIds();

		for (int device_id : device_ids) {
			InputDevice device = InputDevice.getDevice(device_id);
			int sources = device.getSources();

			if (((sources & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD) || ((sources & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK)) {
				if (!game_controller_ids.contains(device_id)) {
					game_controller_ids.add(device_id);

				}

			}

		}

		return game_controller_ids;

	}

}
