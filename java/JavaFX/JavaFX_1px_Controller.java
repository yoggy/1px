import java.net.URL;
import java.util.ResourceBundle;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;
import javafx.util.Duration;

public class JavaFX_1px_Controller implements Initializable {
	@FXML
	Canvas canvas;
	GraphicsContext gc;
	Timeline timer;
	
	@Override
	public void initialize(URL location, ResourceBundle resources) {
		gc = canvas.getGraphicsContext2D();
		draw();
		
		timer = new Timeline(new KeyFrame(Duration.millis(250), new EventHandler<ActionEvent>(){
            @Override
            public void handle(ActionEvent event) {
            	draw();
            }
        }));
        timer.setCycleCount(Timeline.INDEFINITE);
        timer.play();
	}

	void draw() {
		gc.setFill(Color.BLACK);
		gc.fillRect(0, 0, 200, 200);

		long n = (System.currentTimeMillis() / 250) % 4;
		
		switch((int)n) {
		case 0:
			gc.setFill(Color.RED);
			break;
		case 1:
			gc.setFill(Color.GREEN);
			break;
		case 2:
			gc.setFill(Color.BLUE);
			break;
		default:
			gc.setFill(Color.BLACK);
			break;
		}
		gc.fillRect(100, 100, 1, 1);
	}
}
