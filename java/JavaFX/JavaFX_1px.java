import java.io.IOException;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

public class JavaFX_1px extends Application {
	Scene scene;
	
	public static void main(String[] args) {
		launch(args);
	}

	@Override
	public void start(Stage stage) {
		try {
			BorderPane root = (BorderPane) FXMLLoader.load(getClass().getResource(
					"JavaFX_1px.fxml"));
			
			scene = new Scene(root);
			scene.addEventHandler(KeyEvent.KEY_PRESSED,
					new EventHandler<KeyEvent>() {
						@Override
						public void handle(KeyEvent t) {
							if (t.getCode() == KeyCode.ESCAPE) {
								Stage sb = (Stage) scene.getWindow();
								sb.close();
							}
						}
					});

			stage.setScene(scene);
			stage.setResizable(false);
			stage.show();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}