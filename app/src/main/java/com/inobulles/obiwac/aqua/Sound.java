package com.inobulles.obiwac.aqua;

import android.content.Context;
import android.media.MediaPlayer;
import android.net.Uri;

import java.io.File;

public class Sound {
	private static final long DEFAULT_FREQUENCY = 48000;

	private MediaPlayer media_player;
	private static final long frequency = DEFAULT_FREQUENCY;

	Sound(Context context, String path) {
		media_player = MediaPlayer.create(context, Uri.fromFile(new File(path)));

	} public long get_ms() {
		return media_player.getDuration();

	} public void dispose() {
		media_player.release();

	} public void play_or_resume() {
		media_player.start();

	} public void stop() {
		media_player.stop();

	} public void pause() {
		media_player.pause();

	} public void volume(float fraction) {
		media_player.setVolume(fraction, fraction);

	} public void position(long ms) {
		media_player.seekTo((int) ms);

	} public long frequency() { /// TODO
		return frequency;

	}

}
