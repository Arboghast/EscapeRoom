package hello;
import java.util.ArrayList;

import com.jme3.app.SimpleApplication;
import com.jme3.input.KeyInput;
import com.jme3.input.MouseInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.AnalogListener;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.input.controls.MouseButtonTrigger;
import com.jme3.input.controls.Trigger;
import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Vector3f;
import com.jme3.scene.Geometry;
import com.jme3.scene.shape.Box;
import com.jme3.scene.shape.Sphere;

public class buttonPuzzles extends SimpleApplication {
	
	private Geometry geom;
	private final static Trigger mouseClick = new MouseButtonTrigger(MouseInput.BUTTON_LEFT);
	private ArrayList<Geometry> boxes = new ArrayList<Geometry>(); 
	private final static String  MAPPING_COLOR = "Toggle color";
	private boolean toggled =  false;
	private boolean isCreated = false;
	int trigger = 0;
	@Override
	public void simpleInitApp() {
		// TODO Auto-generated method stub
		inputManager.addMapping(MAPPING_COLOR, mouseClick);
		inputManager.addListener(actionsListener, new String[]{MAPPING_COLOR});
		for(int i = 0; i <= 7; i++)
		{	
			Box mesh = new Box(3, 3, 3);
	        geom = new Geometry("Box", mesh);
	        Material mat = new Material(assetManager,
	                "Common/MatDefs/Misc/Unshaded.j3md");
	        mat.setColor("Color", ColorRGBA.Blue);
	        geom.setMaterial(mat);
	        rootNode.attachChild(geom);
	        int x = 15;
	        geom.setLocalTranslation(new Vector3f(x * i,0,0));
	        boxes.add(geom);
		}
		
	}
	private ActionListener actionsListener = new ActionListener() {
   	 public void onAction(String name, boolean isPressed, float tpf) {
        	System.out.println("Mapping detected (discrete): "+name );
        	if(!isPressed)
        	{
        		for(int i = 0; i <= boxes.size(); i++)
        		{
        			Geometry abox = boxes.get(i);
        			Material mat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
					mat.setColor("Color", ColorRGBA.Red);
					abox.setMaterial(mat);
					rootNode.attachChild(abox);
        		}
        	}
        		
   	 }
   };
	public static void main(String[] args){
    	buttonPuzzles app = new buttonPuzzles();
        app.start(); // start the game

    }

}
