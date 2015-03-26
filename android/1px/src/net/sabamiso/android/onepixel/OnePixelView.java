package net.sabamiso.android.onepixel;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Rect;
import android.os.Handler;
import android.view.View;

public class OnePixelView extends View {

	boolean break_flag = false;
	Handler h = new Handler();

	Bitmap bitmap;

	public OnePixelView(Context context) {
		super(context);
		bitmap = Bitmap.createBitmap(200, 200, Bitmap.Config.ARGB_8888);
	}

	public void onResume() {
		break_flag = false;
		startTimer();
	}

	public void onPause() {
		stopTimer();
	}

	public void startTimer() {
		h.postDelayed(new Runnable() {
			@Override
			public void run() {
				invalidate();
				if (break_flag == false)
					startTimer();
			}
		}, 100);
	}

	public void stopTimer() {
		break_flag = true;
	}

	@SuppressLint("DrawAllocation")
	protected void onDraw(Canvas canvas) {
		bitmap.eraseColor(Color.BLACK);

		int n = (int) System.currentTimeMillis() / 250 % 4;
		switch (n) {
		case 0:
			bitmap.setPixel(100, 100, Color.RED);
			break;
		case 1:
			bitmap.setPixel(100, 100, Color.GREEN);
			break;
		case 2:
			bitmap.setPixel(100, 100, Color.BLUE);
			break;
		default:
			break;
		}

		Rect src = new Rect(0, 0, 200, 200);
		Rect dst = new Rect(0, 0, 200, 200);

		canvas.drawBitmap(bitmap, src, dst, null);
	}
}
