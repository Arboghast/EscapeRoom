package mygame;

import java.io.File;
import java.util.ArrayList;

import com.jme3.app.SimpleApplication;
import com.jme3.app.state.AppState;
import com.jme3.bullet.BulletAppState;
import com.jme3.bullet.collision.shapes.CapsuleCollisionShape;
import com.jme3.bullet.collision.shapes.CollisionShape;
import com.jme3.bullet.control.CharacterControl;
import com.jme3.bullet.control.RigidBodyControl;
import com.jme3.bullet.util.CollisionShapeFactory;
import com.jme3.collision.CollisionResults;
import com.jme3.font.BitmapText;
import com.jme3.input.KeyInput;
import com.jme3.input.MouseInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.input.controls.MouseButtonTrigger;
import com.jme3.input.controls.Trigger;
import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Ray;
import com.jme3.math.Vector3f;
import com.jme3.niftygui.NiftyJmeDisplay;
import com.jme3.scene.Geometry;
import com.jme3.scene.Node;
import com.jme3.scene.Spatial;
import com.jme3.scene.shape.Box;

import de.lessvoid.nifty.Nifty;
import de.lessvoid.nifty.elements.render.TextRenderer;

public class Main extends SimpleApplication implements ActionListener {
	// Creates constants that are used to create the room and the player
	private Spatial sceneModel;
	private BulletAppState bulletAppState;
	private RigidBodyControl landscape;
	private CharacterControl player;
	private Vector3f walkDirection = new Vector3f();
	private boolean left = false, right = false, up = false, down = false;
	Nifty nifty;
	ScreenManager screenControl;
	public boolean isRunning = false;

	private Geometry geom;
	private Geometry geom1;
	private Geometry geom2;
	private Geometry geom3;
	private int removed = 0;
	private final static Trigger mouseClick = new MouseButtonTrigger(MouseInput.BUTTON_LEFT);
	private final static String MAPPING_COLOR = "Toggle color";
	private boolean scoreboard = false;

	// Temporary vectors used on each frame.
	// They here to avoid instantiating new vectors on each frame
	private Vector3f camDir = new Vector3f();
	private Vector3f camLeft = new Vector3f();
	private long timer = 0;
	private int duration = -1; // 1 second delay for game to load
	private Spatial crowbar;
	private Spatial chest;

	public static void main(String[] args) {
		Main app = new Main();
		app.start();
	}

	public void loadGame() {

		inputManager.setCursorVisible(false);

		bulletAppState = new BulletAppState();
		stateManager.attach(bulletAppState);

		ScreenManager screenControl3 = (ScreenManager) nifty.getScreen("hud").getScreenController();
		stateManager.attach((AppState) screenControl3);

		// We re-use the fly-by camera for rotation, while positioning is handled by
		// physics
		viewPort.setBackgroundColor(new ColorRGBA(0.7f, 0.8f, 1f, 1f));
		flyCam.setEnabled(true);
		flyCam.setMoveSpeed(90);

		setUpKeys();
		initCrossHairs();

		// We load the scene from the zip file and adjust its size.
		sceneModel = assetManager.loadModel("Models/roomcoloured.j3o");
		sceneModel.setLocalScale(6f);

		// We set up collision detection for the scene by creating a
		// compound collision shape and a static RigidBodyControl with mass zero.
		CollisionShape sceneShape = CollisionShapeFactory.createMeshShape((Node) sceneModel);
		landscape = new RigidBodyControl(sceneShape, 0);
		sceneModel.addControl(landscape);

		// We set up collision detection for the player by creating
		// a capsule collision shape and a CharacterControl.
		// The CharacterControl offers extra settings for
		// size, stepheight, jumping, falling, and gravity.
		// We also put the player in its starting position.
		CapsuleCollisionShape capsuleShape = new CapsuleCollisionShape(1.5f, 6f, 1);
		player = new CharacterControl(capsuleShape, 0.05f);
		player.setJumpSpeed(10);
		player.setFallSpeed(30);

		player.setPhysicsLocation(new Vector3f(0, 4, 0));

		// We attach the scene and the player to the rootnode and the physics space,
		// to make them appear in the game world.
		
		crowbar = assetManager.loadModel("Models/crowbar.j3o");
		crowbar.setLocalScale(6f);
		
		
		
		chest = assetManager.loadModel("Models/bau.j3o");
		chest.setLocalScale(1f);
		
		
		rootNode.attachChild(chest);
		rootNode.attachChild(crowbar);

		Box mesh = new Box(1, 1, 1);
		geom = new Geometry("geom0", mesh);
		Material mat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
		mat.setColor("Color", ColorRGBA.Blue);
		geom.setMaterial(mat);
		rootNode.attachChild(geom);
		geom.setLocalTranslation(new Vector3f(15, 1, 30));

		geom1 = new Geometry("geom1", mesh);
		geom1.setMaterial(mat);
		rootNode.attachChild(geom1);
		geom1.setLocalTranslation(new Vector3f(15, 1, -30));

		geom2 = new Geometry("geom2", mesh);
		geom2.setMaterial(mat);
		rootNode.attachChild(geom2);
		geom2.setLocalTranslation(new Vector3f(15, 1, 15));

		geom3 = new Geometry("geom3", mesh);
		geom3.setMaterial(mat);
		rootNode.attachChild(geom3);
		geom3.setLocalTranslation(new Vector3f(15, 1, -15));
		// to make them appear in the game world.
		rootNode.attachChild(sceneModel);
		bulletAppState.getPhysicsSpace().add(landscape);
		bulletAppState.getPhysicsSpace().add(player);

		isRunning = true;  //toggle simple update lock
	}

	// Brings up the start screen and loads in the game files using a separated class
	public void simpleInitApp() {
		/** Set up Physics */

		flyCam.setEnabled(false);
		NiftyJmeDisplay display = new NiftyJmeDisplay(assetManager, inputManager, audioRenderer, viewPort); // create jme-nifty-processor
		nifty = display.getNifty();
		nifty.addXml("Interface/Start.xml");
		nifty.gotoScreen("start");
		screenControl = (ScreenManager) nifty.getScreen("start").getScreenController();
		stateManager.attach((AppState) screenControl);
		guiViewPort.addProcessor(display);

	}

	// Creates crosshair in the center of the screen
	protected void initCrossHairs() {
		guiNode.detachAllChildren();
		guiFont = assetManager.loadFont("Interface/Fonts/Default.fnt");
		BitmapText ch = new BitmapText(guiFont, false);
		ch.setSize(guiFont.getCharSet().getRenderedSize() * 2);
		ch.setText("+");
		ch.setLocalTranslation(settings.getWidth() / 2 - guiFont.getCharSet().getRenderedSize() / 3 * 2,
				settings.getHeight() / 2 + ch.getLineHeight() / 2, 0);
		guiNode.attachChild(ch);
	}

	/**
	 * We over-write some navigational key mappings here, so we can add
	 * physics-controlled walking and jumping:
	 */
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
		inputManager.addMapping(MAPPING_COLOR, mouseClick);
		inputManager.addListener(actionsListener, new String[] { MAPPING_COLOR });
	}

	/*
	 * Action listener which listens to mouseclicks and where the player is clicking
	 * and performs different tasks depending on where the player clicked in the
	 * game
	 */
	private ActionListener actionsListener = new ActionListener() {
		public void onAction(String name, boolean isPressed, float tpf) {
			CollisionResults results = new CollisionResults();
			Ray ray = new Ray(cam.getLocation(), cam.getDirection());
			rootNode.collideWith(ray, results);

			// For each hit, we know distance, impact point, name of geometry.
			// float dist = results.getCollision(i).getDistance();
			Vector3f pt = results.getCollision(0).getContactPoint(); 		// point where object is 'clicked on
			String hit = results.getCollision(0).getGeometry().getName(); 		// name of the object
			System.out.println("  You shot " + hit + " at " + pt + ". ");
			System.out.println("one done" + removed);
			if (!isPressed) {
				if (hit.equals("geom0") || hit.equals("geom1") || hit.equals("geom2") || hit.equals("geom3")) {
					rootNode.detachChild(results.getCollision(0).getGeometry());
					removed++;

				}

			}
		}
	};

	/**
	 * These are our custom actions triggered by key presses. We do not walk yet, we
	 * just keep track of the direction the user pressed.
	 */
	/*
	 * Binds the WSAD and Space keys to allow the player to move
	 * forward,backwards,left,right, and jump
	 */
	public void onAction(String binding, boolean value, float tpf) {
		if (binding.equals("Left")) {
			if (value) {
				left = true;
			} else {
				left = false;
			}
		} else if (binding.equals("Right")) {
			if (value) {
				right = true;
			} else {
				right = false;
			}
		} else if (binding.equals("Up")) {
			if (value) {
				up = true;
			} else {
				up = false;
			}
		} else if (binding.equals("Down")) {
			if (value) {
				down = true;
			} else {
				down = false;
			}
		} else if (binding.equals("Jump")) {
			player.jump();
		}
	}

	/**
	 * This is the main event loop--walking happens here. We check in which
	 * direction the player is walking by interpreting the camera direction forward
	 * (camDir) and to the side (camLeft). The setWalkDirection() command is what
	 * lets a physics-controlled player walk. We also make sure here that the camera
	 * moves with player.
	 */
	@Override
	public void simpleUpdate(float tpf) {

		if (isRunning) {
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

			// checks time
			if (System.currentTimeMillis() - timer >= 1000) {
				// Every second
				duration++;
				timer = System.currentTimeMillis();
			}

			// update HUD Display
			int minutes = duration / 60;
			String formattedH = String.format("%02d", minutes);

			int seconds = duration - (60 * minutes);
			String formattedM = String.format("%02d", seconds);

			nifty.getCurrentScreen().findElementByName("time").getRenderer(TextRenderer.class)
					.setText("Time Spent: " + formattedH + ":" + formattedM);

			// Code To trigger Endgame Screen
			System.out.println(removed);
			if (removed == 4) {
				guiNode.detachAllChildren();
				flyCam.setEnabled(false);
				isRunning = false;
				nifty.gotoScreen("endgame");
				ScreenManager screenControl3 = (ScreenManager) nifty.getScreen("endgame").getScreenController();
				screenControl.addScore(minutes, seconds); //Use this to pass in game time to csv
				stateManager.attach((AppState) screenControl3);
			}
		}
		if (scoreboard) {
			// Sends the player to the scoreboard which carries data from a CSV file
			scoreboard = false;
			nifty.gotoScreen("scoreboard");
			ScreenManager screenControl3 = (ScreenManager) nifty.getScreen("scoreboard").getScreenController();
			stateManager.attach((AppState) screenControl3);

			File CSV = new File("EscapeRoomScoreSheet.csv");
			CSVReader tracker = new CSVReader(CSV);

			ArrayList<String> data = tracker.getRowData();
			for (int i = 0; i < data.size(); i++) {
				nifty.getCurrentScreen().findElementByName("help").getRenderer(TextRenderer.class).setText(data.get(i));
			}
		}
	}
}
