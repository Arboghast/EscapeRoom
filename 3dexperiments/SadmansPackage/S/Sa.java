package S;

import javafx.application.Application;
import javafx.geometry.Point3D;
import javafx.scene.AmbientLight;
import javafx.scene.Group;
import javafx.scene.PerspectiveCamera;
import javafx.scene.PointLight;
import javafx.scene.Scene;
import javafx.scene.SceneAntialiasing;
import javafx.scene.paint.Color;
import javafx.scene.paint.PhongMaterial;
import javafx.scene.shape.Box;
import javafx.scene.shape.Sphere;
import javafx.scene.transform.Rotate;
import javafx.scene.transform.Translate;
import javafx.stage.Stage;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.PickResult;

public class Sa  extends Application{
	


	    private final Group root = new Group();
	    private PerspectiveCamera camera;
	    private final double sceneWidth = 800;
	    private final double sceneHeight = 600;

	    private double mousePosX;
	    private double mousePosY;
	    private double mouseOldX;
	    private double mouseOldY;
	    private final Rotate rotateX = new Rotate(-20, Rotate.X_AXIS);
	    private final Rotate rotateY = new Rotate(-20, Rotate.Y_AXIS);

	    private volatile boolean isPicking=false;
	    private Point3D vecIni, vecPos;
	    private double distance;
	    private Sphere s;

	    @Override
	    public void start(Stage stage) {
	        Box floor = new Box(1500, 10, 1500);
	        floor.setMaterial(new PhongMaterial(Color.GRAY));
	        floor.setTranslateY(150);
	        root.getChildren().add(floor);

	        Sphere sphere = new Sphere(150);
	        sphere.setMaterial(new PhongMaterial(Color.RED));
	        sphere.setTranslateY(-5);
	        root.getChildren().add(sphere);

	        Scene scene = new Scene(root, sceneWidth, sceneHeight, true, SceneAntialiasing.BALANCED);
	        scene.setFill(Color.web("3d3d3d"));

	        camera = new PerspectiveCamera(true);
	        camera.setVerticalFieldOfView(false);

	        camera.setNearClip(0.1);
	        camera.setFarClip(100000.0);
	        camera.getTransforms().addAll (rotateX, rotateY, new Translate(0, 0, -3000));

	        PointLight light = new PointLight(Color.GAINSBORO);
	        root.getChildren().add(light);
	        root.getChildren().add(new AmbientLight(Color.WHITE));
	        scene.setCamera(camera);

	        scene.setOnMousePressed((MouseEvent me) -> {
	            mousePosX = me.getSceneX();
	            mousePosY = me.getSceneY();
	            PickResult pr = me.getPickResult();
	            if(pr!=null && pr.getIntersectedNode() != null && pr.getIntersectedNode() instanceof Sphere){
	                distance=pr.getIntersectedDistance();
	                s = (Sphere) pr.getIntersectedNode();
	                isPicking=true;
	                vecIni = unProjectDirection(mousePosX, mousePosY, scene.getWidth(),scene.getHeight());
	            }
	        });
	        scene.setOnMouseDragged((MouseEvent me) -> {
	            mousePosX = me.getSceneX();
	            mousePosY = me.getSceneY();
	            if(isPicking){
	                vecPos = unProjectDirection(mousePosX, mousePosY, scene.getWidth(),scene.getHeight());
	                Point3D p=vecPos.subtract(vecIni).multiply(distance);
	                s.getTransforms().add(new Translate(p.getX(),p.getY(),p.getZ()));
	                vecIni=vecPos;
	                PickResult pr = me.getPickResult();
	                if(pr!=null && pr.getIntersectedNode() != null && pr.getIntersectedNode()==s){
	                    distance=pr.getIntersectedDistance();
	                } else {
	                    isPicking=false;
	                }
	            } else {
	                rotateX.setAngle(rotateX.getAngle()-(mousePosY - mouseOldY));
	                rotateY.setAngle(rotateY.getAngle()+(mousePosX - mouseOldX));
	                mouseOldX = mousePosX;
	                mouseOldY = mousePosY;
	            }
	        });
	        scene.setOnMouseReleased((MouseEvent me)->{
	            if(isPicking){
	                isPicking=false;
	            }
	        });

	        stage.setTitle("3D Dragging");
	        stage.setScene(scene);
	        stage.show();
	    }

	    /*
	     From fx83dfeatures.Camera3D
	     http://hg.openjdk.java.net/openjfx/8u-dev/rt/file/5d371a34ddf1/apps/toys/FX8-3DFeatures/src/fx83dfeatures/Camera3D.java
	    */
	    public Point3D unProjectDirection(double sceneX, double sceneY, double sWidth, double sHeight) {
	        double tanHFov = Math.tan(Math.toRadians(camera.getFieldOfView()) * 0.5f);
	        Point3D vMouse = new Point3D(tanHFov*(2*sceneX/sWidth-1), tanHFov*(2*sceneY/sWidth-sHeight/sWidth), 1);

	        Point3D result = localToSceneDirection(vMouse);
	        return result.normalize();
	    }

	    public Point3D localToScene(Point3D pt) {
	        Point3D res = camera.localToParentTransformProperty().get().transform(pt);
	        if (camera.getParent() != null) {
	            res = camera.getParent().localToSceneTransformProperty().get().transform(res);
	        }
	        return res;
	    }

	    public Point3D localToSceneDirection(Point3D dir) {
	        Point3D res = localToScene(dir);
	        return res.subtract(localToScene(new Point3D(0, 0, 0)));
	    }

	    public static void main(String[] args) {
	        launch(args);
	    }
}
