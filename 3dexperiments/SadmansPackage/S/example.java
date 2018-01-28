package S;

import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.paint.Color;
import javafx.scene.paint.PhongMaterial;
import javafx.scene.shape.Box;
import javafx.scene.shape.Cylinder;
import javafx.scene.shape.Sphere;
import javafx.stage.Stage;

public class example extends Application{
	 @Override
	   public void start(Stage primaryStage) {
	      
		 final PhongMaterial redMaterial = new PhongMaterial();
	       redMaterial.setSpecularColor(Color.ORANGE);
	       redMaterial.setDiffuseColor(Color.RED);
	 
	       final PhongMaterial blueMaterial = new PhongMaterial();
	       blueMaterial.setDiffuseColor(Color.BLUE);
	       blueMaterial.setSpecularColor(Color.LIGHTBLUE);

	       final PhongMaterial greyMaterial = new PhongMaterial();
	       greyMaterial.setDiffuseColor(Color.DARKGREY);
	       greyMaterial.setSpecularColor(Color.GREY);

	       final Box red = new Box(400, 400, 400);
	       red.setMaterial(redMaterial);
	 
	       final Sphere blue = new Sphere(200);
	       blue.setMaterial(blueMaterial);
	 
	       final Cylinder grey = new Cylinder(5, 100);
	       grey.setMaterial(greyMaterial);
	       
	      Group root=new Group(red,blue,grey);

	      Scene scene = new Scene(root, 700, 700);

	      primaryStage.setTitle("3D JavaFX");
	      primaryStage.setScene(scene);
	      primaryStage.show();
	   }

	   public static void main(String[] args) {
	      Application.launch(args);
	   }

}