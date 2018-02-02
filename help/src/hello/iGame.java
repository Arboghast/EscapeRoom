package hello;

import com.jme3.app.SimpleApplication;
import com.jme3.bullet.BulletAppState;
import com.jme3.bullet.collision.shapes.CapsuleCollisionShape;
import com.jme3.bullet.collision.shapes.CollisionShape;
import com.jme3.bullet.collision.shapes.CompoundCollisionShape;
import com.jme3.bullet.collision.shapes.HullCollisionShape;
import com.jme3.bullet.control.CharacterControl;
import com.jme3.bullet.control.RigidBodyControl;
import com.jme3.bullet.util.CollisionShapeFactory;
import com.jme3.material.Material;
import com.jme3.scene.Geometry;
import com.jme3.scene.Node;
import com.jme3.scene.Spatial;
import com.jme3.scene.shape.Box;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Vector3f;

/** Sample 1 - how to get started with the most simple JME 3 application.
 * Display a blue 3D cube and view from all sides by
 * moving the mouse and pressing the WASD keys. */
public class iGame extends SimpleApplication {

    public static void main(String[] args){
        myGame app = new myGame();
        app.start(); // start the game
        
    }

    @Override
    public void simpleInitApp() {
    	BulletAppState bulletAppState = new BulletAppState();
    	
    	//flyCam.setEnabled(true);
    	flyCam.setMoveSpeed(25);
        Box a = new Box(4, 1, 1); // create cube shape
        Box b = new Box(4, 1, 1); // create cube shape
        Box c = new Box(1, 1, 4); // create cube shape
        Box d = new Box(1, 1, 4); // create cube shape
        Box e = new Box(4, 1, 4);
        Box f = new Box(4, 1, 4);
        

        Geometry geom1 = new Geometry("Box", a);  // create cube geometry from the shape
        
        geom1.setLocalTranslation(new Vector3f(0,0,15));

        Material mat1 = new Material(assetManager,
          "Common/MatDefs/Misc/Unshaded.j3md");  // create a simple material
        mat1.setColor("Color", ColorRGBA.Red);   // set color of material to blue
        geom1.setMaterial(mat1);                   // set the cube's material
        rootNode.attachChild(geom1);              // make the cube appear in the scene
        Geometry geom2 = new Geometry("Box", b);  // create cube geometry from the shape
        
        geom2.setLocalTranslation(new Vector3f(0,0,5));

        Material mat2 = new Material(assetManager,
          "Common/MatDefs/Misc/Unshaded.j3md");  // create a simple material
        mat2.setColor("Color", ColorRGBA.Blue);   // set color of material to blue
        geom2.setMaterial(mat2);                   // set the cube's material
        rootNode.attachChild(geom2);              // make the cube appear in the scene
        Geometry geom3 = new Geometry("Box", c);  // create cube geometry from the shape
        
        geom3.setLocalTranslation(new Vector3f(-5,0,10));
        
        Material mat3 = new Material(assetManager,
          "Common/MatDefs/Misc/Unshaded.j3md");  // create a simple material
        mat3.setColor("Color", ColorRGBA.Green);   // set color of material to blue
        geom3.setMaterial(mat3);                   // set the cube's material
        rootNode.attachChild(geom3);              // make the cube appear in the scene
        Geometry geom4 = new Geometry("Box", d);  // create cube geometry from the shape
        
        geom4.setLocalTranslation(new Vector3f(5,0,10));

        Material mat4 = new Material(assetManager,
          "Common/MatDefs/Misc/Unshaded.j3md");  // create a simple material
        mat4.setColor("Color", ColorRGBA.Yellow);   // set color of material to blue
        geom4.setMaterial(mat4);                   // set the cube's material
        rootNode.attachChild(geom4);              // make the cube appear in the scene
        Geometry geom5 = new Geometry("Box", e);  // create cube geometry from the shape
        
        geom5.setLocalTranslation(new Vector3f(0,2,10));

        Material mat5 = new Material(assetManager,
          "Common/MatDefs/Misc/Unshaded.j3md");  // create a simple material
        mat5.setColor("Color", ColorRGBA.Cyan);   // set color of material to blue
        geom5.setMaterial(mat5);                   // set the cube's material
        rootNode.attachChild(geom5);              // make the cube appear in the scene
        Geometry geom6 = new Geometry("Box", f);  // create cube geometry from the shape
        
        geom6.setLocalTranslation(new Vector3f(0,-2,10));

        Material mat6 = new Material(assetManager,
          "Common/MatDefs/Misc/Unshaded.j3md");  // create a simple material
        mat6.setColor("Color", ColorRGBA.Magenta);   // set color of material to blue
        geom6.setMaterial(mat6);                   // set the cube's material
        rootNode.attachChild(geom6);              // make the cube appear in the scene
        getCollisionShape(geom1);
        getCollisionShape(geom2);
        getCollisionShape(geom3);
        getCollisionShape(geom4);
        getCollisionShape(geom5);
        getCollisionShape(geom6);
    	}
     /*   CapsuleCollisionShape capsuleShape = new CapsuleCollisionShape(1.5f, 6f, 1);
        CharacterControl player = new CharacterControl(capsuleShape, 0.05f);
        player.setJumpSpeed(20);
        player.setFallSpeed(30);
        //player.setGravity(new Vector3f(0,-30f,0));
        player.setPhysicsLocation(new Vector3f(0, 10, 0));
        
        bulletAppState.getPhysicsSpace().add(player); */
        
        private CompoundCollisionShape getCollisionShape(Spatial s){
            CompoundCollisionShape res = new CompoundCollisionShape();
            if(s instanceof Node)
        	for(Spatial child : ((Node)s).getChildren())
        		if(child instanceof Geometry)
        			res.addChildShape(new HullCollisionShape(((Geometry)child).getMesh()), Vector3f.ZERO);
            return res;
        }
        

    
}
