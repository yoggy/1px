import java.util.Timer;
import java.util.TimerTask;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.KeyEvent;
import org.eclipse.swt.events.KeyListener;
import org.eclipse.swt.events.PaintEvent;
import org.eclipse.swt.events.PaintListener;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.widgets.Canvas;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;

public class SWT1px extends Canvas {
	static Display display;
	static SWT1px canvas;

	static Timer timer;
	static int count = 0;

	static Color color_black;
	static Color color_red;
	static Color color_green;
	static Color color_blue;

	public SWT1px(Composite arg0, int arg1) {
		super(arg0, arg1);

		addPaintListener(new PaintListener() {
			@Override
			public void paintControl(PaintEvent evt) {
				GC g = evt.gc;
				
				switch(count % 4) {
				case 0:
					g.setForeground(color_red);
					break;
				case 1:
					g.setForeground(color_green);
					break;
				case 2:
					g.setForeground(color_blue);
					break;
				case 3:
					g.setForeground(color_black);
					break;
				}
				g.drawPoint(100,  100);
				
				count ++;
			}
		});
	}

	public static void main(String args[]) {
		display = new Display();

		Shell shell = new Shell(display, SWT.SHELL_TRIM & (~SWT.RESIZE));
		shell.setText("1px - swt version");
		shell.addKeyListener(new KeyListener() {

			@Override
			public void keyPressed(KeyEvent evt) {
			}

			@Override
			public void keyReleased(KeyEvent evt) {
				if (evt.keyCode == 27) {
					timer.cancel();
					display.dispose();
				}
			}
		});

		color_black = new Color(display, 0, 0, 0);
		color_red = new Color(display, 255, 0, 0);
		color_green = new Color(display, 0, 255, 0);
		color_blue = new Color(display, 0, 0, 255);

		canvas = new SWT1px(shell, SWT.NONE);
		canvas.setBounds(0, 0, 200, 200);
		canvas.setBackground(color_black);

		// resize window
		shell.pack();
		
		// show window
		shell.open();

		// redraw timer
		timer = new Timer();
		timer.schedule(new TimerTask() {
			@Override
			public void run() {
				display.asyncExec(new Runnable() {
					@Override
					public void run() {
						canvas.redraw();
					}});
			}
		}, 250, 250);

		// event loop
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch())
				display.sleep();
		}
		
		color_black.dispose();
		color_red.dispose();
		color_green.dispose();
		color_blue.dispose();
		
		canvas.dispose();
	}
}
