package mygame;

import com.jme3.app.SimpleApplication;
import com.jme3.niftygui.NiftyJmeDisplay;
import de.lessvoid.nifty.Nifty;
import de.lessvoid.nifty.builder.ScreenBuilder;
import de.lessvoid.nifty.builder.TextBuilder;
import de.lessvoid.nifty.builder.LayerBuilder;
import de.lessvoid.nifty.builder.PanelBuilder;
import de.lessvoid.nifty.controls.button.builder.ButtonBuilder;
import de.lessvoid.nifty.controls.label.builder.LabelBuilder;
import de.lessvoid.nifty.screen.DefaultScreenController;

/**
 * @author iamcreasy
*/
public class TestNiftyGui extends SimpleApplication {

    public static void main(String[] args) {
        TestNiftyGui app = new TestNiftyGui();
        app.start();
    }

    @Override
    public void simpleInitApp() {
    NiftyJmeDisplay niftyDisplay = NiftyJmeDisplay.newNiftyJmeDisplay(
            assetManager, inputManager, audioRenderer, guiViewPort);
    Nifty nifty = niftyDisplay.getNifty();
    guiViewPort.addProcessor(niftyDisplay);
    flyCam.setDragToRotate(true);

    nifty.loadStyleFile("nifty-default-styles.xml");
    nifty.loadControlFile("nifty-default-controls.xml");

    nifty.addScreen("start", new ScreenBuilder("start") {{
        controller(new mygame.ScreenManager());
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
    
    
    nifty.addScreen("hud", new ScreenBuilder("hud") {{
        controller(new DefaultScreenController());

        layer(new LayerBuilder("background") {{
            childLayoutCenter();
            backgroundColor("#000f");
            // <!-- ... -->
        }});

        layer(new LayerBuilder("foreground") {{
            childLayoutHorizontal();
            backgroundColor("#0000");

            // panel added
            panel(new PanelBuilder("panel_left") {{
                childLayoutVertical();
                backgroundColor("#0f08");
                height("100%");
                width("80%");
                // <!-- spacer -->
            }});

            panel(new PanelBuilder("panel_right") {{
                childLayoutVertical();
                backgroundColor("#00f8");
                height("100%");
                width("20%");

                panel(new PanelBuilder("panel_top_right1") {{  //timer
                    childLayoutCenter();
                    backgroundColor("#00f8");
                    height("15%");
                    width("100%");
                    
                    control(new LabelBuilder(){{
                        color("#000");
                        text("123");
                        width("100%");
                        height("100%");
                    }});
                }});

                panel(new PanelBuilder("panel_top_right2") {{ //squares left
                    childLayoutCenter();
                    backgroundColor("#44f8");
                    height("15%");
                    width("100%");
                    
                    control(new LabelBuilder(){{
                        color("#000");
                        text("123");
                        width("100%");
                        height("100%");
                    }});
                }});

                panel(new PanelBuilder("panel_bot_right") {{
                    childLayoutCenter();
                    valignCenter();
                    backgroundColor("#88f8");
                    height("70%");
                    width("100%");
                }});
            }}); // panel added
        }});
    }}.build(nifty));
    
    
    nifty.gotoScreen("start"); // start the screen
    }
}