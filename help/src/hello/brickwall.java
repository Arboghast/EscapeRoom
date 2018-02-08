package hello;

import com.jme3.app.SimpleApplication;
import com.jme3.asset.TextureKey;
import com.jme3.bullet.BulletAppState;
import com.jme3.bullet.PhysicsSpace;
import com.jme3.bullet.collision.shapes.BoxCollisionShape;
import com.jme3.bullet.collision.shapes.SphereCollisionShape;
import com.jme3.bullet.control.RigidBodyControl;
import com.jme3.font.BitmapText;
import com.jme3.input.KeyInput;
import com.jme3.input.MouseInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.input.controls.MouseButtonTrigger;
import com.jme3.material.Material;
import com.jme3.math.Vector2f;
import com.jme3.math.Vector3f;
import com.jme3.renderer.queue.RenderQueue.ShadowMode;
import com.jme3.scene.Geometry;
import com.jme3.scene.shape.Box;
import com.jme3.scene.shape.Sphere;
import com.jme3.scene.shape.Sphere.TextureMode;
import com.jme3.texture.Texture;
import com.jme3.texture.Texture.WrapMode;

/**
 *
 * @author double1984
 */
public class brickwall extends SimpleApplication {

    static float bLength = 0.48f;
    static float bWidth = 0.24f;
    static float bHeight = 0.12f;
    Material mat;
    Material mat2;
    Material mat3;
    private static Sphere bullet;
    private static Box brick;
    private static SphereCollisionShape bulletCollisionShape;

    private BulletAppState bulletAppState;

    public static void main(String args[]) {
        brickwall f = new brickwall();
        f.start();
    }

    @Override
    public void simpleInitApp() {
        
        bulletAppState = new BulletAppState();
        bulletAppState.setThreadingType(BulletAppState.ThreadingType.PARALLEL);
        stateManager.attach(bulletAppState);

        bullet = new Sphere(32, 32, 0.4f, true, false);
        bullet.setTextureMode(TextureMode.Projected);
        bulletCollisionShape = new SphereCollisionShape(0.4f);
        brick = new Box(bLength, bHeight, bWidth);
        brick.scaleTextureCoordinates(new Vector2f(1f, .5f));

        initMaterial();
        initWall();
        initFloor();
        initCrossHairs();
        
        this.cam.setLocation(new Vector3f(0, 6f, 6f));
        cam.lookAt(Vector3f.ZERO, new Vector3f(0, 1, 0));
        cam.setFrustumFar(15);
        inputManager.addMapping("shoot", new MouseButtonTrigger(MouseInput.BUTTON_LEFT));
        inputManager.addListener(actionListener, "shoot");
        inputManager.addMapping("gc", new KeyTrigger(KeyInput.KEY_X));
        inputManager.addListener(actionListener, "gc");

        rootNode.setShadowMode(ShadowMode.Off);
    }

    private PhysicsSpace getPhysicsSpace() {
        return bulletAppState.getPhysicsSpace();
    }
    private ActionListener actionListener = new ActionListener() {

        public void onAction(String name, boolean keyPressed, float tpf) {
            if (name.equals("shoot") && !keyPressed) {
                Geometry bulletg = new Geometry("bullet", bullet);
                bulletg.setMaterial(mat2);
                bulletg.setShadowMode(ShadowMode.CastAndReceive);
                bulletg.setLocalTranslation(cam.getLocation());
                
                SphereCollisionShape bulletCollisionShape = new SphereCollisionShape(0.4f);
                RigidBodyControl bulletNode = new RigidBodyControl(bulletCollisionShape, 1);
                bulletNode.setLinearVelocity(cam.getDirection().mult(25));
                bulletg.addControl(bulletNode);
                rootNode.attachChild(bulletg);
                getPhysicsSpace().add(bulletNode);
            }
            if (name.equals("gc") && !keyPressed) {
                System.gc();
            }
        }
    };

    public void initWall() {
        float startpt = bLength / 4;
        float height = 0;
        for (int j = 0; j < 15; j++) {
            for (int i = 0; i < 4; i++) {
                Vector3f vt = new Vector3f(i * bLength * 2 + startpt, bHeight + height, 0);
                addBrick(vt);
            }
            startpt = -startpt;
            height += 2 * bHeight;
        }
    }

    public void initFloor() {
        Box floorBox = new Box(10f, 0.1f, 5f);
        floorBox.scaleTextureCoordinates(new Vector2f(3, 6));

        Geometry floor = new Geometry("floor", floorBox);
        floor.setMaterial(mat3);
        floor.setShadowMode(ShadowMode.Receive);
        floor.setLocalTranslation(0, -0.1f, 0);
        floor.addControl(new RigidBodyControl(new BoxCollisionShape(new Vector3f(10f, 0.1f, 5f)), 0));
        this.rootNode.attachChild(floor);
        this.getPhysicsSpace().add(floor);
    }

    public void initMaterial() {
        mat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        TextureKey key = new TextureKey("Textures/Terrain/BrickWall/BrickWall.jpg");
        key.setGenerateMips(true);
        Texture tex = assetManager.loadTexture(key);
        mat.setTexture("ColorMap", tex);

        mat2 = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        TextureKey key2 = new TextureKey("Textures/Terrain/Rock/Rock.PNG");
        key2.setGenerateMips(true);
        Texture tex2 = assetManager.loadTexture(key2);
        mat2.setTexture("ColorMap", tex2);

        mat3 = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        TextureKey key3 = new TextureKey("Textures/Terrain/Pond/Pond.jpg");
        key3.setGenerateMips(true);
        Texture tex3 = assetManager.loadTexture(key3);
        tex3.setWrap(WrapMode.Repeat);
        mat3.setTexture("ColorMap", tex3);
    }

    public void addBrick(Vector3f ori) {

        Geometry reBoxg = new Geometry("brick", brick);
        reBoxg.setMaterial(mat);
        reBoxg.setLocalTranslation(ori);
        //for geometry with sphere mesh the physics system automatically uses a sphere collision shape
        reBoxg.addControl(new RigidBodyControl(1.5f));
        reBoxg.setShadowMode(ShadowMode.CastAndReceive);
        reBoxg.getControl(RigidBodyControl.class).setFriction(0.6f);
        this.rootNode.attachChild(reBoxg);
        this.getPhysicsSpace().add(reBoxg);
    }

    protected void initCrossHairs() {
        guiFont = assetManager.loadFont("Interface/Fonts/Default.fnt");
        BitmapText ch = new BitmapText(guiFont, false);
        ch.setSize(guiFont.getCharSet().getRenderedSize() * 2);
        ch.setText("+"); // crosshairs
        ch.setLocalTranslation( // center
                settings.getWidth() / 2 - guiFont.getCharSet().getRenderedSize() / 3 * 2,
                settings.getHeight() / 2 + ch.getLineHeight() / 2, 0);
        guiNode.attachChild(ch);
    }
}