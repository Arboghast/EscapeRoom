package hello;

import com.jme3.app.SimpleApplication;
import com.jme3.collision.CollisionResults;
import com.jme3.input.KeyInput;
import com.jme3.input.MouseInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.AnalogListener;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.input.controls.MouseAxisTrigger;
import com.jme3.input.controls.MouseButtonTrigger;
import com.jme3.math.Ray;
import com.jme3.math.Vector3f;
import com.jme3.scene.Geometry;

public class TestControls extends SimpleApplication {
	private ActionListener actionListener = new ActionListener(){
        public void onAction(String name, boolean pressed, float tpf){
            System.out.println(name + " = " + pressed);
        }
    };
    public AnalogListener analogListener = new AnalogListener() {
        public void onAnalog(String name, float value, float tpf) {
            System.out.println(name + " = " + value);
        }
    };
    private ActionListener actionListener1 = new ActionListener() {
        public void onAction(String name, boolean pressed, float tpf) {
            System.out.println("Hello");
        }
      };

    public static void main(String[] args){
        TestControls app = new TestControls();
        app.start();
    }
 
    @Override
    public void simpleInitApp() {
        // Test multiple inputs per mapping
        inputManager.addMapping("My Action",
                new KeyTrigger(KeyInput.KEY_SPACE),
                new MouseAxisTrigger(MouseInput.AXIS_WHEEL, false),
        		new KeyTrigger(KeyInput.KEY_1));

        // Test multiple listeners per mapping
        inputManager.addListener(actionListener, "My Action");
        inputManager.addListener(analogListener, "My Action");
        inputManager.addListener(actionListener1,"My Action");
    }
    

}
