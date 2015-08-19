import java.awt.Color;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.Timer;
import java.util.TimerTask;

public class Awt extends Frame implements KeyListener {
	static Awt frame;
	static Timer timer;
	int count;

	public Awt() {
		setSize(200, 200);
		setBackground(Color.BLACK);
		addKeyListener(this);
	}

	@Override
	public void keyPressed(KeyEvent arg0) {
		if (arg0.getKeyCode() == KeyEvent.VK_ESCAPE) {
			System.exit(0);
		}
	}

	@Override
	public void keyReleased(KeyEvent arg0) {
	}

	@Override
	public void keyTyped(KeyEvent arg0) {
	}

	public void paint(Graphics g) {
		switch (count % 4) {
		case 0:
			g.setColor(Color.RED);
			break;
		case 1:
			g.setColor(Color.GREEN);
			break;
		case 2:
			g.setColor(Color.BLUE);
			break;
		case 3:
			g.setColor(Color.BLACK);
			break;
		}
		g.drawRect(100, 100, 1, 1);
		count++;
	}

	public static void main(String[] args) {
		frame = new Awt();
		timer = new Timer();
		timer.schedule(new TimerTask() {
			@Override
			public void run() {
				frame.repaint();
			}
		}, 250, 250);
		frame.setVisible(true);
	}
}
