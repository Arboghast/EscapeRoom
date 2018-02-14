package mygame;

import java.io.File;

import com.jme3.app.Application;
import com.jme3.app.SimpleApplication;
import com.jme3.app.state.AbstractAppState;
import com.jme3.app.state.AppStateManager;
import com.jme3.asset.AssetManager;
import com.jme3.math.ColorRGBA;
import com.jme3.renderer.ViewPort;
import com.jme3.scene.Node;

import de.lessvoid.nifty.Nifty;
import de.lessvoid.nifty.controls.TextField;
import de.lessvoid.nifty.elements.render.TextRenderer;
import de.lessvoid.nifty.screen.Screen;
import de.lessvoid.nifty.screen.ScreenController;

public class EndScreen extends AbstractAppState implements ScreenController {

	private ViewPort viewPort;
  private Node rootNode;
  private Node guiNode;
  private AssetManager assetManager;
  private Node localRootNode = new Node("End Screen RootNode");
  private Node localGuiNode = new Node("End Screen GuiNode");
  private final ColorRGBA backgroundColor = ColorRGBA.Gray;
  Nifty nifty;
  private Screen screen;
  private XML app;
  private int seconds;
  private int minutes;
    
    
    public EndScreen(){
       
      }
    @Override
    public void initialize(AppStateManager stateManager, Application app) {
    	 super.initialize(stateManager, app);
         this.app = (XML) app;
        //TODO: initialize your AppState, e.g. attach spatials to rootNode
        //this is called on the OpenGL thread after the AppState has been attached
    }

    @Override
    public void update(float tpf) {
        //TODO: implement behavior during runtime
    }

    public void gotoScreen(String nextScreen)
    {
    	nifty.gotoScreen(nextScreen);
    }
    public void addScore(int minutes, int seconds)
    {
    	this.seconds = seconds;
    	this.minutes = minutes;
    }
    public void startGame(String nextScreen) {
    	
    }
    
    public void submitScore()
    {
String input = nifty.getCurrentScreen().findNiftyControl("input", TextField.class).getText();
    	
    	File CSV = new File("EscapeRoomScoreSheet.csv");
		CSVReader tracker =  new CSVReader(CSV);
    	tracker.writeToCSV(input,this.seconds,this.minutes);
    	
    	nifty.gotoScreen("scoreboard");
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