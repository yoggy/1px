import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class Swing extends JPanel {

	static JFrame frame;
	static Timer timer;
	int count;

	private static final long serialVersionUID = 3040345398325019473L;

	public void paintComponent(Graphics g) {
		g.setColor(Color.BLACK);
		g.fillRect(0, 0, getWidth(), getHeight());

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
		frame = new JFrame("1px - Swing version");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.addKeyListener(new KeyListener() {

			@Override
			public void keyPressed(KeyEvent e) {
			}

			@Override
			public void keyReleased(KeyEvent e) {
				if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
					frame.dispose();
				}
			}

			@Override
			public void keyTyped(KeyEvent e) {
			}
		});

		// add panel
		Swing panel = new Swing();
		panel.setPreferredSize(new Dimension(200, 200));
		frame.getContentPane().add(panel);

		// redraw timer task
		timer = new Timer();
		timer.schedule(new TimerTask() {
			@Override
			public void run() {
				frame.repaint();
			}
		}, 250, 250);

		frame.pack(); // resize JFrame
		frame.setVisible(true);
	}
}
