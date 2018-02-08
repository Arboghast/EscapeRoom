package hello;

import com.jme3.app.SimpleApplication;
import com.jme3.niftygui.NiftyJmeDisplay;
import de.lessvoid.nifty.Nifty;
import de.lessvoid.nifty.builder.ScreenBuilder;
import de.lessvoid.nifty.builder.LayerBuilder;
import de.lessvoid.nifty.builder.PanelBuilder;
import de.lessvoid.nifty.controls.button.builder.ButtonBuilder;
import de.lessvoid.nifty.screen.DefaultScreenController;

/**
 * @author iamcreasy
*/
public class Main extends SimpleApplication {

    public static void main(String[] args)
    {
        Main app = new Main();
        app.start();
    }

	@Override
	public void simpleInitApp()
	{
		NiftyJmeDisplay niftyDisplay = NiftyJmeDisplay.newNiftyJmeDisplay(assetManager, inputManager, audioRenderer, guiViewPort);
	    Nifty nifty = niftyDisplay.getNifty();
	    guiViewPort.addProcessor(niftyDisplay);
	    flyCam.setDragToRotate(true);
	    
	    nifty.gotoScreen(start);
	    
	    //makes start screen
		nifty.addScreen("start", new ScreenBuilder("start")
		{{
		    controller(new DefaultScreenController());
		}}.build(nifty));
		
		//add layer to start screen
		nifty.addScreen("start", new ScreenBuilder("start")
		{{
	        controller(new DefaultScreenController());

	         // layer added
	         layer(new LayerBuilder("background")
	         {{
		         childLayoutCenter();
		         backgroundColor("#000f");
	         }});

	         layer(new LayerBuilder("foreground")
	         {{
	             childLayoutVertical();
	             backgroundColor("#0000");
	         }});
	         // layer added

	      }}.build(nifty));
		
		//add panels
		nifty.addScreen("start", new ScreenBuilder("start") {{
	        controller(new DefaultScreenController());
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
	            }});

	            panel(new PanelBuilder("panel_mid") {{
	                childLayoutCenter();
	                alignCenter();
	                backgroundColor("#0f08");
	                height("50%");
	                width("75%");
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
	                }});

	                panel(new PanelBuilder("panel_bottom_right") {{
	                    childLayoutCenter();
	                    valignCenter();
	                    backgroundColor("#88f8");
	                    height("50%");
	                    width("50%");
	                }});
	            }}); // panel added
	        }});

	    }}.build(nifty));

	}
    
}