package hello;

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
import com.jme3.scene.Geometry;
import com.jme3.scene.shape.Box;

public class toggletesting extends SimpleApplication 
{
	private Geometry geom;
	private final static Trigger spaceBar = new KeyTrigger(KeyInput.KEY_SPACE);
	private final static Trigger mouseClick = new MouseButtonTrigger(MouseInput.BUTTON_LEFT);
	private final static String  MAPPING_COLOR = "Toggle Size";

	@Override
	public void simpleInitApp() {
		 inputManager.addMapping(MAPPING_COLOR, spaceBar);
		 inputManager.addMapping(MAPPING_COLOR, mouseClick);
		 inputManager.addListener(actionListener, new String[]{MAPPING_COLOR});
		 inputManager.addListener(analogListener, new String[]{MAPPING_COLOR});
		Box mesh = new Box(1, 1, 1);
        geom = new Geometry("Box", mesh);
        Material mat = new Material(assetManager,
                "Common/MatDefs/Misc/Unshaded.j3md");
        mat.setColor("Color", ColorRGBA.Blue);
        geom.setMaterial(mat);
        rootNode.attachChild(geom);
		
	}
	private ActionListener actionListener = new ActionListener() {
        public void onAction(String name, boolean isPressed, float tpf) {
        	System.out.println("Mapping detected (discrete): "+name );
        	if(name.equals(MAPPING_COLOR)&& !isPressed)
        	{
        		Box next = new Box(1,3,3);
        		geom = new Geometry("Box", next);
        		Material mat = new Material(assetManager,
                        "Common/MatDefs/Misc/Unshaded.j3md");
                mat.setColor("Color", ColorRGBA.Red);
                geom.setMaterial(mat);
                rootNode.attachChild(geom);
        	}
           
        }
    };
    private AnalogListener analogListener = new AnalogListener() {
    	public void onAnalog(String name, float intensity, float tpf) {
            System.out.println("Mapping detected (discrete): "+name );
            if (name.equals(MAPPING_COLOR)) 
        {
            	Box next = new Box(1,1,1);
        		geom = new Geometry("Box", next);
        		Material mat = new Material(assetManager,
                        "Common/MatDefs/Misc/Unshaded.j3md");
                mat.setColor("Color", ColorRGBA.Red);
                geom.setMaterial(mat);
                rootNode.attachChild(geom);
        }
    	}
    };
	 public static void main(String[] args) {
	        toggletesting app = new toggletesting();
	        app.start();

	    }

    
}
