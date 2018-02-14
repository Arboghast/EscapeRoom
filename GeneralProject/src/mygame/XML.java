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
import com.jme3.font.BitmapText;
import com.jme3.input.KeyInput;
import com.jme3.input.MouseInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.input.controls.Trigger;
import com.jme3.light.AmbientLight;
import com.jme3.light.DirectionalLight;
import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Vector3f;
import com.jme3.niftygui.NiftyJmeDisplay;
import com.jme3.scene.Geometry;
import com.jme3.scene.Node;
import com.jme3.scene.Spatial;
import com.jme3.scene.shape.Box;

import de.lessvoid.nifty.Nifty;
import de.lessvoid.nifty.builder.LayerBuilder;
import de.lessvoid.nifty.builder.PanelBuilder;
import de.lessvoid.nifty.builder.ScreenBuilder;
import de.lessvoid.nifty.builder.TextBuilder;
import de.lessvoid.nifty.controls.button.builder.ButtonBuilder;
import de.lessvoid.nifty.controls.label.builder.LabelBuilder;
import de.lessvoid.nifty.elements.render.TextRenderer;
import de.lessvoid.nifty.screen.DefaultScreenController;
import com.jme3.input.controls.MouseButtonTrigger;

/**
   TODO:
   -- Ray Collision for the buttons--   //   /help/src/hello/OnclickExample.java  has ray collision setup
   -- Add furniture to the escape room--
   -- Connect Endgame CSV Reader and Scoreboard--
   
   TODO: OPTIONAL(If we have time)
   -- Style the Start, hud, and endgame Screens--
   -- Clean up this messy file--
 */
public class XML extends SimpleApplication
{
	Nifty nifty;
	EndScreen screenControl;
		
	public static void main(String[] args) 
	{
	    XML app = new XML();
	    app.start();
	}
	public void simpleInitApp()
	{
	    
		  flyCam.setEnabled(false);
		  NiftyJmeDisplay display = new NiftyJmeDisplay(assetManager, inputManager, audioRenderer, viewPort); //create jme-nifty-processor
	      nifty = display.getNifty();
	      nifty.addXml("Interface/End.xml");
	      nifty.gotoScreen("scoreboard");
	      screenControl = (EndScreen) nifty.getScreen("scoreboard").getScreenController();
	      screenControl.addScore(1, 1);
	      stateManager.attach((AppState) screenControl);
	      guiViewPort.addProcessor(display);
	}
}
