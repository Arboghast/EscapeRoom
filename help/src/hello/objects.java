package hello;

import com.jme3.app.SimpleApplication;
import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Vector3f;
import com.jme3.scene.Geometry;
import com.jme3.scene.Node;
import com.jme3.scene.shape.Box;

public class objects extends SimpleApplication
{
	public static void main(String[] args)
	{
        myGame app = new myGame();
        app.start(); // start the game
    }
	@Override
	public void simpleInitApp()
	{
		//Creates a blue box at coordinates (1, -1, 1)
		Box box1 = new Box(1, 1, 1);
		Geometry blue = new Geometry("Box", box1);
		blue.setLocalTranslation(new Vector3f(1, -1, 1));
		Material mat1 = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
		mat1.setColor("Color", ColorRGBA.Blue);
		blue.setMaterial(mat1);
		
		//Create a pivot node at (0, 0, 0) and attach it to the root node
		Node pivot = new Node("pivot"); // creates node
		rootNode.attachChild(pivot); //puts node in scene
		
		pivot.attachChild(blue);
		
		pivot.rotate(.4f, .4f, 0f);
	}
}
