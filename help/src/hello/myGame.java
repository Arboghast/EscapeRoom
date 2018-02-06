
package hello;

import com.jme3.app.SimpleApplication;
import com.jme3.bullet.BulletAppState;
import com.jme3.material.Material;
import com.jme3.scene.Geometry;
import com.jme3.scene.Spatial;
import com.jme3.scene.shape.Box;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Vector3f;
import com.jme3.bullet.BulletAppState;
import com.jme3.bullet.collision.shapes.CapsuleCollisionShape;
import com.jme3.bullet.collision.shapes.CollisionShape;
import com.jme3.bullet.control.CharacterControl;
import com.jme3.bullet.control.RigidBodyControl;
import com.jme3.bullet.util.CollisionShapeFactory;
import com.jme3.input.KeyInput;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.light.AmbientLight;
import com.jme3.light.DirectionalLight;
import com.jme3.app.SimpleApplication;
import com.jme3.asset.plugins.ZipLocator;
import com.jme3.bullet.BulletAppState;
import com.jme3.bullet.collision.shapes.CapsuleCollisionShape;
import com.jme3.bullet.collision.shapes.CollisionShape;
import com.jme3.bullet.control.CharacterControl;
import com.jme3.bullet.control.RigidBodyControl;
import com.jme3.bullet.util.CollisionShapeFactory;
import com.jme3.input.KeyInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.light.AmbientLight;
import com.jme3.light.DirectionalLight;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Vector3f;
import com.jme3.scene.Node;
import com.jme3.scene.Spatial;

/** Sample 1 - how to get started with the most simple JME 3 application.
 * Display a blue 3D cube and view from all sides by
 * moving the mouse and pressing the WASD keys. */
public class myGame extends SimpleApplication implements ActionListener{
	
	  private Spatial sceneModel;
	  private BulletAppState bulletAppState;
	  private RigidBodyControl landscape;
	  private CharacterControl player;
	  private Vector3f walkDirection = new Vector3f();
	  private boolean left = false, right = false, up = false, down = false;
	  private Vector3f camDir = new Vector3f();
	  private Vector3f camLeft = new Vector3f();



    public static void main(String[] args){
        myGame app = new myGame();
        app.start(); // start the game
        
    }

    @Override
    public void simpleInitApp() {
    	
    	bulletAppState = new BulletAppState();
        stateManager.attach(bulletAppState);
        
        
    	//flyCam.setEnabled(true);
        viewPort.setBackgroundColor(new ColorRGBA(0.7f, 0.8f, 1f, 1f));
    	flyCam.setMoveSpeed(25);
    	setUpKeys();
        setUpLight();
        
        Box a = new Box(4, 1, 1); // create cube shape
        
        
        
        
        Geometry geom1 = new Geometry("Box", a);  // create cube geometry from the shape
        geom1.setLocalTranslation(new Vector3f(0,0,15));
        Material mat1 = new Material(assetManager,
          "Common/MatDefs/Misc/Unshaded.j3md");  // create a simple material
        mat1.setColor("Color", ColorRGBA.Red);   // set color of material to blue
        geom1.setMaterial(mat1);   
        
        assetManager.registerLocator("roomcolour.zip", ZipLocator.class);
        sceneModel = assetManager.loadModel("roomcoloured.scene");
        sceneModel.setLocalScale(2f);
        
        /*
         * Thank you Nehon for your response. Really appreciate your advise on following problem as well.



I just followed the Hello Animation tutorial in the JME3 tutorial set. It was working fine for me. After that I added an OgreXml mesh to the Models folder and wanted to animate that instead of default Oto.mesh.xml. But I am getting a NullPointerException when it is starting. Please find the code block used to load this model and the exception thrown pasted at the end.



This cityboy 3D model I downloaded from the Google Sketchup Warehouse and I converted that into cityboy.mesh.xml using a Google Sketchup export plug-in.

Is that the cause to this error? Do I have missed something?


         */
        CollisionShape sceneShape = CollisionShapeFactory.createMeshShape((Node) sceneModel);
        landscape = new RigidBodyControl(sceneShape, 0);
        sceneModel.addControl(landscape);
       
        CapsuleCollisionShape capsuleShape = new CapsuleCollisionShape(1.5f, 6f, 1);
        player = new CharacterControl(capsuleShape, 0.05f);
        player.setJumpSpeed(20);
        player.setFallSpeed(30);
        player.setGravity(30);
        player.setPhysicsLocation(new Vector3f(0, 50, 0));

        // We attach the scene and the player to the rootnode and the physics space,
        // to make them appear in the game world.
        rootNode.attachChild(sceneModel);
        bulletAppState.getPhysicsSpace().add(landscape);
        bulletAppState.getPhysicsSpace().add(player);
    }
    
    private void setUpLight() {
        // We add light so we see the scene
        AmbientLight al = new AmbientLight();
        al.setColor(ColorRGBA.White.mult(1.3f));
        rootNode.addLight(al);

        DirectionalLight dl = new DirectionalLight();
        dl.setColor(ColorRGBA.White);
        dl.setDirection(new Vector3f(2.8f, -2.8f, -2.8f).normalizeLocal());
        rootNode.addLight(dl);
      }

      /** We over-write some navigational key mappings here, so we can
       * add physics-controlled walking and jumping: */
      private void setUpKeys() {
        inputManager.addMapping("Left", new KeyTrigger(KeyInput.KEY_A));
        inputManager.addMapping("Right", new KeyTrigger(KeyInput.KEY_D));
        inputManager.addMapping("Up", new KeyTrigger(KeyInput.KEY_W));
        inputManager.addMapping("Down", new KeyTrigger(KeyInput.KEY_S));
        inputManager.addMapping("Jump", new KeyTrigger(KeyInput.KEY_SPACE));
        inputManager.addListener(this, "Left");
        inputManager.addListener(this, "Right");
        inputManager.addListener(this, "Up");
        inputManager.addListener(this, "Down");
        inputManager.addListener(this, "Jump");
      }
      public void onAction(String binding, boolean value, float tpf) {
    	    if (binding.equals("Left")) {
    	      if (value) { left = true; } else { left = false; }
    	    } else if (binding.equals("Right")) {
    	      if (value) { right = true; } else { right = false; }
    	    } else if (binding.equals("Up")) {
    	      if (value) { up = true; } else { up = false; }
    	    } else if (binding.equals("Down")) {
    	      if (value) { down = true; } else { down = false; }
    	    } else if (binding.equals("Jump")) {
    	      player.jump();
    	    }
    	  }

    	  /**
    	   * This is the main event loop--walking happens here.
    	   * We check in which direction the player is walking by interpreting
    	   * the camera direction forward (camDir) and to the side (camLeft).
    	   * The setWalkDirection() command is what lets a physics-controlled player walk.
    	   * We also make sure here that the camera moves with player.
    	   */
    	  @Override
    	    public void simpleUpdate(float tpf) {
    	        camDir.set(cam.getDirection()).multLocal(0.6f);
    	        camLeft.set(cam.getLeft()).multLocal(0.4f);
    	        walkDirection.set(0, 0, 0);
    	        if (left) {
    	            walkDirection.addLocal(camLeft);
    	        }
    	        if (right) {
    	            walkDirection.addLocal(camLeft.negate());
    	        }
    	        if (up) {
    	            walkDirection.addLocal(camDir);
    	        }
    	        if (down) {
    	            walkDirection.addLocal(camDir.negate());
    	        }
    	        player.setWalkDirection(walkDirection);
    	        cam.setLocation(player.getPhysicsLocation());
    	    }
}
