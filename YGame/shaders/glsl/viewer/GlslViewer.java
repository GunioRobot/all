/**
 * 
 */
package viewer;

import java.awt.Color;
import java.awt.Frame;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.FileNotFoundException;
import java.io.IOException;

import net.java.games.jogl.*;
import obj.ObjMeshLoader;

/**
 * @author David Cornette
 *
 */
public class GlslViewer {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		String vs;
		String fs;
		
		if (args.length >= 2) {
			vs = args[0];
			fs = args[1];
		} else {
			vs = "lambert01vert.glsl";
			fs = "checkedlambfrag.glsl";
		}

		// TODO break this into different methods.
		GlslViewer viewer = new GlslViewer(vs, fs);

	}
	
	GlslViewer(String vertsrc, String fragsrc) {
		Frame frame = new Frame("GLSL Viewer");
		GLCapabilities capabilities = new GLCapabilities();
		capabilities.setDoubleBuffered(true);
		capabilities.setHardwareAccelerated(true);
		capabilities.setSampleBuffers(true);
		capabilities.setNumSamples(4);
		
		GLCanvas canvas = GLDrawableFactory.getFactory().createGLCanvas(capabilities);
		frame.add(canvas);
		frame.setSize(800, 600);
		
		Animator animator = new Animator(canvas);
		
		frame.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent event) {
				//TODO add stuff to do things like print out FPS or something
				System.exit(0);
			}
		});
		
//		addDragon(vertsrc, fragsrc);
		//addGrid(vertsrc, fragsrc);
		
		ViewerWorld world = ViewerWorld.getWorld();
		//world.setTimeMode(ViewerWorld.FRAME);
		//world.add(new AxisDisplayable(5.0, 5.0, 5.0));
		try {
			//Displayable d = new GLSLDisplayable(vertsrc, fragsrc, new MeshDisplayable(new ObjMeshLoader().loadFile("suzanne.obj")));
			//Displayable d = new GLSLDisplayable(vertsrc, fragsrc, new MeshDisplayable(new ObjMeshLoader().loadFile("Cube.obj")));
			//Displayable d = new GLSLDisplayable(vertsrc, fragsrc, new MeshDisplayable(new ObjMeshLoader().loadFile("spheref.obj")));
			//Displayable d = new GLSLDisplayable(vertsrc, fragsrc, new MeshDisplayable(new ObjMeshLoader().loadFile("spheres.obj")));
			//Displayable d = new GLSLDisplayable(vertsrc, fragsrc, new MeshDisplayable(new ObjMeshLoader().loadFile("suzanne2.obj")));
			Displayable d = new GLSLDisplayable(vertsrc, fragsrc, new MeshDisplayable(new ObjMeshLoader().loadFile("suzanne3.obj")));
			//Displayable d = new GLSLDisplayable(vertsrc, fragsrc, new MeshDisplayable(new ObjMeshLoader().loadFile("dragon.obj")));
			//Displayable d = new GLSLDisplayable(vertsrc, fragsrc, new MeshDisplayable(new ObjMeshLoader().loadFile("plane.obj")));
			//d.addTransformation(new RotateTransformation(180.0, -1.0, 0.0, 0.0));
			((GLSLDisplayable)d).addUniform("test4", 0.95f, 0.1f, 0.2f, 1.0f);
			d.addTransformation(new TranslateTransformation(0.0, 1.0, 0.0));
			d.addTransformation(new ScaleTransformation(2.0));
			d.addTransformation(new SpinnerTransformation(10, -1.0, 0.0, 0.0));
			d.addTransformation(new RotateTransformation(15.0, 0.0, 1.0, 0.0));
			world.add(d);
		} catch (FileNotFoundException e) {
			System.err.println("Oh dear, the mesh was not found.");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("Uh oh.  Can't read the mesh file.");
			e.printStackTrace();
		}
		// FIXME should not have such a class as SpinningTeapot Displayable
//		ViewerWorld world = ViewerWorld.getWorld();
//		Displayable d = new SpinningTeapotDisplayable(vertsrc, fragsrc);
//		d.addTransformation(new RotateTransformation(15.0, 1.0, 0.0, 0.0));
//		d.addTransformation(new TranslateTransformation(-0.1, -0.5, 0.0));
//		d.addTransformation(new ScaleTransformation(2.2));
//		d.addTransformation(new RotateTransformation(13.0, 0.0, 1.0, 0.0));
//		world.add(d);
		
		frame.setVisible(true);
		frame.setBackground(Color.WHITE);
		JoglEventListener jev = new JoglEventListener(canvas);
		canvas.addGLEventListener(jev);
		canvas.addKeyListener(jev);
		canvas.addMouseListener(jev);
		canvas.addMouseMotionListener(jev);
		canvas.addMouseWheelListener(jev);
		
		animator.start();
	}

	private void addGrid(String vertsrc, String fragsrc) {
		ViewerWorld world = ViewerWorld.getWorld();

		try {
			Displayable d = new GLSLDisplayable(vertsrc, fragsrc, new MeshDisplayable(new ObjMeshLoader().loadFile("grid12x12tris.obj")));
			d.addTransformation(new RotateTransformation(-90.0, 1.0, 0.0, 0.0));
			d.addTransformation(new ScaleTransformation(3.0));
//			((GLSLDisplayable)d).addUniform("scale", 0.6f);
//			//d.addTransformation(new SpinnerTransformation(10, -1.0, 0.0, 0.0));
//			d.addTransformation(new ScaleTransformation(0.3));
//			d.addTransformation(new TranslateTransformation(-16.0, 7.0, -13.0));
//			d.addTransformation(new RotateTransformation(57.0, 0.0, -1.0, 0.0));
//			d.addTransformation(new RotateTransformation(90.0, -1.0, 0.0, 0.0));
//			d.addTransformation(new RotateTransformation(15.0, 0.0, 1.0, 0.0));
			world.add(d);
		} catch (FileNotFoundException e) {
			System.err.println("Oh dear, the mesh was not found.");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("Uh oh.  Can't read the mesh file.");
			e.printStackTrace();
		}
	}

	private void addDragon(String vertsrc, String fragsrc) {
		ViewerWorld world = ViewerWorld.getWorld();

		try {
			Displayable d = new GLSLDisplayable(vertsrc, fragsrc, new MeshDisplayable(new ObjMeshLoader().loadFile("dragon.obj")));
			((GLSLDisplayable)d).addUniform("scale", 0.6f);
			//d.addTransformation(new SpinnerTransformation(10, -1.0, 0.0, 0.0));
			d.addTransformation(new ScaleTransformation(0.3));
			d.addTransformation(new TranslateTransformation(-16.0, 7.0, -13.0));
			d.addTransformation(new RotateTransformation(57.0, 0.0, -1.0, 0.0));
			d.addTransformation(new RotateTransformation(90.0, -1.0, 0.0, 0.0));
			d.addTransformation(new RotateTransformation(15.0, 0.0, 1.0, 0.0));
			world.add(d);
		} catch (FileNotFoundException e) {
			System.err.println("Oh dear, the mesh was not found.");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("Uh oh.  Can't read the mesh file.");
			e.printStackTrace();
		}
	}

}
