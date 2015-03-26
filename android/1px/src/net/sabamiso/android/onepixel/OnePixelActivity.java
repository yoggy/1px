package net.sabamiso.android.onepixel;
import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;


public class OnePixelActivity extends Activity {

	OnePixelView view;

	protected void onCreate(Bundle savedInstanceState) {
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		super.onCreate(savedInstanceState);
		
		view = new OnePixelView(this);
		setContentView(view);
	}
	
	public void onResume() {
		view.onResume();
		super.onResume();
	}
	
	public void onPause() {
		view.onPause();
		super.onPause();
	}
}
