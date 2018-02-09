package mygame;


import com.jme3.app.SimpleApplication;
import com.jme3.app.state.AppState;
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
import com.jme3.niftygui.NiftyJmeDisplay;
import com.jme3.scene.Node;
import com.jme3.scene.Spatial;

import de.lessvoid.nifty.Nifty;
import de.lessvoid.nifty.builder.LayerBuilder;
import de.lessvoid.nifty.builder.PanelBuilder;
import de.lessvoid.nifty.builder.ScreenBuilder;
import de.lessvoid.nifty.builder.TextBuilder;
import de.lessvoid.nifty.controls.button.builder.ButtonBuilder;
import de.lessvoid.nifty.controls.label.builder.LabelBuilder;
import de.lessvoid.nifty.screen.DefaultScreenController;

/**
 * Example 9 - How to make walls and floors solid.
 * This collision code uses Physics and a custom Action Listener.
 * @author normen, with edits by Zathras
 */
public class Main extends SimpleApplication
        implements ActionListener {

  private Spatial sceneModel;
  private BulletAppState bulletAppState;
  private RigidBodyControl landscape;
  private CharacterControl player;
  private Vector3f walkDirection = new Vector3f();
  private boolean left = false, right = false, up = false, down = false;
  Nifty nifty;
  StartScreen screenControl;
  private boolean isRunning = false;
  
  //Temporary vectors used on each frame.
  //They here to avoid instanciating new vectors on each frame
  private Vector3f camDir = new Vector3f();
  private Vector3f camLeft = new Vector3f();

  public static void main(String[] args) {
    Main app = new Main();
    app.start();
  }

  public void loadGame()
  {
      
     
	  bulletAppState = new BulletAppState();
	    stateManager.attach(bulletAppState);
	    //bulletAppState.getPhysicsSpace().enableDebug(assetManager);
	    
	//    StartScreen screenControl3 = (StartScreen) nifty.getScreen("hud").getScreenController();
	//    stateManager.attach((AppState) screenControl3);

	    // We re-use the flyby camera for rotation, while positioning is handled by physics
	    viewPort.setBackgroundColor(new ColorRGBA(0.7f, 0.8f, 1f, 1f));
	    flyCam.setEnabled(true);
	    flyCam.setMoveSpeed(90);
	    setUpKeys();
	    

	    // We load the scene from the zip file and adjust its size.
	    assetManager.registerLocator("town.zip", ZipLocator.class);
	    sceneModel = assetManager.loadModel("main.scene");
	  //  sceneModel = assetManager.loadModel("Models/roomcoloured.j3o");
	    sceneModel.setLocalScale(6f);

	    // We set up collision detection for the scene by creating a
	    // compound collision shape and a static RigidBodyControl with mass zero.
	    CollisionShape sceneShape =
	            CollisionShapeFactory.createMeshShape((Node) sceneModel);
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
	    rootNode.attachChild(sceneModel);
	    bulletAppState.getPhysicsSpace().add(landscape);
	    bulletAppState.getPhysicsSpace().add(player);
	    
	    
	    
	    isRunning = true;
  }
  public void simpleInitApp() {
    /** Set up Physics */
	  
	  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  // https://github.com/kimlercorey/ninjahunter/blob/master/src/cmsc325/finalProject/NinjaHunter.java
	//  flyCam.setDragToRotate(true);
	  flyCam.setEnabled(false);
	  NiftyJmeDisplay display = new NiftyJmeDisplay(assetManager, inputManager, audioRenderer, viewPort); //create jme-nifty-processor
      nifty = display.getNifty();
      nifty.addXml("Interface/Start.xml");
      nifty.gotoScreen("start");
      screenControl = (StartScreen) nifty.getScreen("start").getScreenController();
      stateManager.attach((AppState) screenControl);
      guiViewPort.addProcessor(display); 
	    
	/*  flyCam.setDragToRotate(true);
	  NiftyJmeDisplay display = new NiftyJmeDisplay(assetManager, inputManager, audioRenderer, viewPort); //create jme-nifty-processor
      nifty = display.getNifty();
      
      nifty.loadStyleFile("nifty-default-styles.xml");
      nifty.loadControlFile("nifty-default-controls.xml");

      nifty.addScreen("start", new ScreenBuilder("start") {{
          controller(new mygame.StartScreen());
          layer(new LayerBuilder("background") {{
              childLayoutCenter();
              backgroundColor("#000f");
              // <!-- ... -->
          }});

          layer(new LayerBuilder("foreground") {{
                  childLayoutVertical();
                  backgroundColor("#0000");

              // panel added
                  panel(new PanelBuilder("panel_top") {{
                      childLayoutCenter();
                      alignCenter();
                      backgroundColor("#f008");
                      height("25%");
                      width("75%");

                      // add text
                      text(new TextBuilder() {{
                          text("The Escape Room");
                          font("Interface/Fonts/Default.fnt");
                          height("100%");
                          width("100%");
                      }});

                  }});
              

                  panel(new PanelBuilder("panel_mid") {{
                      childLayoutCenter();
                      alignCenter();
                      backgroundColor("#0f08");
                      height("50%");
                      width("75%");
                      // add text
                      text(new TextBuilder() {{
                          text("Here goes some text describing the game and the rules and stuff. "+
                               "Incidentally, the text is quite long and needs to wrap at the end of lines. ");
                          font("Interface/Fonts/Default.fnt");
                          wrap(true);
                          height("100%");
                          width("100%");
                      }});

                  }});

              panel(new PanelBuilder("panel_bottom") {{
                  childLayoutHorizontal();
                  alignCenter();
                  backgroundColor("#00f8");
                  height("25%");
                  width("75%");

                  panel(new PanelBuilder("panel_bottom_left") {{
                      childLayoutCenter();
                      valignCenter();
                      backgroundColor("#44f8");
                      height("50%");
                      width("50%");

                      // add control
                      control(new ButtonBuilder("StartButton", "Start") {{
                        alignCenter();
                        valignCenter();
                        height("50%");
                        width("50%");
                        visibleToMouse(true);
                        interactOnClick("startGame(hud)");
                      }});

                  }});

                  panel(new PanelBuilder("panel_bottom_right") {{
                      childLayoutCenter();
                      valignCenter();
                      backgroundColor("#88f8");
                      height("50%");
                      width("50%");

                      // add control
                      control(new ButtonBuilder("QuitButton", "Quit") {{
                        alignCenter();
                        valignCenter();
                        height("50%");
                        width("50%");
                        visibleToMouse(true);
                        interactOnClick("quitGame()");
                      }});

                  }});
              }}); // panel added
          }});

      }}.build(nifty));
      
      nifty.gotoScreen("start");
      screenControl = (StartScreen) nifty.getScreen("start").getScreenController();
      stateManager.attach((AppState) screenControl);
      guiViewPort.addProcessor(display); */
	    
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

  /** These are our custom actions triggered by key presses.
   * We do not walk yet, we just keep track of the direction the user pressed. */
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
		}
    }
}
