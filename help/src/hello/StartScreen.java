package hello;

import com.jme3.app.Application;
import com.jme3.app.SimpleApplication;
import com.jme3.app.state.AbstractAppState;
import com.jme3.app.state.AppStateManager;
import com.jme3.asset.AssetManager;
import com.jme3.math.ColorRGBA;
import com.jme3.renderer.ViewPort;
import com.jme3.scene.Node;

import de.lessvoid.nifty.Nifty;
import de.lessvoid.nifty.screen.Screen;
import de.lessvoid.nifty.screen.ScreenController;

public class StartScreen extends AbstractAppState implements ScreenController {

	private ViewPort viewPort;
  private Node rootNode;
  private Node guiNode;
  private AssetManager assetManager;
  private Node localRootNode = new Node("Start Screen RootNode");
  private Node localGuiNode = new Node("Start Screen GuiNode");
  private final ColorRGBA backgroundColor = ColorRGBA.Gray;
  Nifty nifty;
  private Screen screen;
  private ModelGame app;
    
    
    public StartScreen(){
       
      }
    @Override
    public void initialize(AppStateManager stateManager, Application app) {
    	 super.initialize(stateManager, app);
         this.app = (ModelGame) app;
        //TODO: initialize your AppState, e.g. attach spatials to rootNode
        //this is called on the OpenGL thread after the AppState has been attached
    }

    @Override
    public void update(float tpf) {
        //TODO: implement behavior during runtime
    }

    @Override
    public void cleanup() {
    	 rootNode.detachChild(localRootNode);
    	    guiNode.detachChild(localGuiNode);

        super.cleanup();
        //TODO: clean up what you initialized in the initialize method,
        //e.g. remove all spatials from rootNode
        //this is called on the OpenGL thread after the AppState has been detached
    }
    public void startGame(String nextScreen) {
    	nifty.gotoScreen(nextScreen);
      }

      public void quitGame() {
        app.stop();
      }

    
    
    public void bind(Nifty nifty, Screen screen) {
    	this.nifty = nifty;
        this.screen = screen;
    }

    public void onStartScreen() {
       
    }

    public void onEndScreen() {
       
    }
}