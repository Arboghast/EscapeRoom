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
import com.jme3.renderer.queue.RenderQueue.Bucket;
import com.jme3.scene.Geometry;
import com.jme3.scene.shape.Box;
import com.jme3.scene.shape.Sphere;

public class buttonPuzzles extends SimpleApplication {
	
	private Geometry geom;
	private Geometry geom1;
	private Geometry geom2;
	private Geometry geom3;
	private int removed = 0;
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
		inputManager.addListener(actionsListener1, new String[]{MAPPING_COLOR});
		inputManager.addListener(actionsListener2, new String[]{MAPPING_COLOR});
		inputManager.addListener(actionsListener3, new String[]{MAPPING_COLOR});
			Box mesh = new Box(3, 3, 3);
	        geom = new Geometry("Box", mesh);
	        Material mat = new Material(assetManager,
	                "Common/MatDefs/Misc/Unshaded.j3md");
	        mat.setColor("Color", ColorRGBA.Blue);
	        geom.setMaterial(mat);
	        rootNode.attachChild(geom);
	        
	        geom1 = new Geometry("Box", mesh);
	        geom1.setMaterial(mat);
	        rootNode.attachChild(geom1);
	        geom1.setLocalTranslation(new Vector3f(15,0,0));
	        

	        geom2 = new Geometry("Box", mesh);
	        geom2.setMaterial(mat);
	        rootNode.attachChild(geom2);
	        geom2.setLocalTranslation(new Vector3f(30,0,0));
	        
	        geom3 = new Geometry("Box", mesh);
	        geom3.setMaterial(mat);
	        rootNode.attachChild(geom3);
	        geom3.setLocalTranslation(new Vector3f(45,0,0));
	        //boxes.add(geom);
		
	}
	private ActionListener actionsListener = new ActionListener() {
   	 public void onAction(String name, boolean isPressed, float tpf) {
        	System.out.println("one done");
        	if(!isPressed)
        	{
        		rootNode.detachChild(geom);// removes the object 
        		removed = 1;
        	}
        		
   	 }
   };
   private ActionListener actionsListener1 = new ActionListener() {
	   	 public void onAction(String name, boolean isPressed, float tpf) {
	        	System.out.println("2 done");
	        	if(removed == 1)
	        	{
	        		rootNode.detachChild(geom1); // removes the object
	        		removed = 2;
	        	}
	        		
	   	 }
	   };
	private ActionListener actionsListener2 = new ActionListener() {
		public void onAction(String name, boolean isPressed, float tpf) {
		        	System.out.println("3 done");
		        	if(removed == 2)
		        	{
		        		rootNode.detachChild(geom2); // removes the object 
		        		removed = 3;
		        	}
		        		
		   	 }
		   };
    private ActionListener actionsListener3 = new ActionListener() {
		public void onAction(String name, boolean isPressed, float tpf) {
			        System.out.println("4 done");
			        	if(removed == 3)
			        	{
			        		rootNode.detachChild(geom3); // removes the object
			        		removed = 4;
			        	}
			        		
			   	 }
			   };	   
		   
	public static void main(String[] args){
    	buttonPuzzles app = new buttonPuzzles();
        app.start(); // start the game

    }

}
