package S;

import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.paint.Color;
import javafx.scene.paint.PhongMaterial;
import javafx.scene.shape.Sphere;
import javafx.stage.Stage;

public class example extends Application{
	 @Override
	   public void start(Stage primaryStage) {
	      
	      Sphere sphere = new Sphere(200);
	      PhongMaterial material1 = new PhongMaterial();
	      material1.setDiffuseColor(Color.BLUE);
	      material1.setSpecularColor(Color.LIGHTBLUE);
	      material1.setSpecularPower(10.0);
	      sphere.setMaterial(material1);
	      Group root=new Group(sphere);

	      Scene scene = new Scene(root, 700, 700);

	      primaryStage.setTitle("3D JavaFX");
	      primaryStage.setScene(scene);
	      primaryStage.show();
	   }

	   public static void main(String[] args) {
	      Application.launch(args);
	   }

}