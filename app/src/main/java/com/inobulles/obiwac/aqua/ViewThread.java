package com.inobulles.obiwac.aqua;

public abstract class ViewThread implements Runnable {
	private volatile boolean stopped = false;
	abstract void view_run();

	public void run() {
		set_stopped(false);

		while (!stopped) {
			view_run();
			stop();

		}

	}

	private void set_stopped(boolean stopped) {
		this.stopped = stopped;

	}

	private void stop() {
		set_stopped(true);

	}

}
