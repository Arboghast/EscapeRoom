package mygame;

import java.io.File;

import com.jme3.app.Application;
import com.jme3.app.state.AbstractAppState;
import com.jme3.app.state.AppStateManager;

import de.lessvoid.nifty.Nifty;
import de.lessvoid.nifty.controls.TextField;
import de.lessvoid.nifty.screen.Screen;
import de.lessvoid.nifty.screen.ScreenController;

public class ScreenManager extends AbstractAppState implements ScreenController {

	Nifty nifty;
	private Screen screen;
	private Main app;
	private static int seconds;
	private static int minutes;

	@Override
	public void initialize(AppStateManager stateManager, Application app) {
		super.initialize(stateManager, app);
		this.app = (Main) app;
		// this is called on the OpenGL thread after the AppState has been attached
	}

	@Override
	public void update(float tpf) {
		// TODO: implement behavior during runtime
	}

	public void gotoScreen(String nextScreen) {   //Switches screens
		nifty.gotoScreen(nextScreen);
	}

	public void addScore(int minutes, int seconds) {  //Sends minutes and seconds from java to gui
		this.seconds = seconds;
		this.minutes = minutes;
	}

	public void startGame(String nextScreen) {  //loads the starting screen and loads game
		nifty.gotoScreen(nextScreen);
		app.loadGame();
	}

	public void quitGame() { 	//Ends the game when function is called
		app.stop();
	}

	public void submitScore() {    
		String input = nifty.getCurrentScreen().findNiftyControl("input", TextField.class).getText();

		File CSV = new File("EscapeRoomScoreSheet.csv");     //Receives User input and adds it along with minutes and seconds to the CSV file
		CSVReader tracker = new CSVReader(CSV);
		tracker.writeToCSV(input, this.minutes, this.seconds);
		
		
		app.scoreboard = true;
	}

	public void bind(Nifty nifty, Screen screen) {
		this.nifty = nifty;
		this.screen = screen;
	}

	@Override
	public void onEndScreen() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onStartScreen() {
		// TODO Auto-generated method stub
		
	}

}