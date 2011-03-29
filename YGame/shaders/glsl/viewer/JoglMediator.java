/**
 * I took the basics from Kevin Duling's implementation in Jogl of the NeHe
 * tutorials.  I am not sure exactly how this is a mediator.  If so, how?  If not,
 * what is it? what should it be called?
 */
package viewer;

import java.util.Iterator;

import net.java.games.jogl.GL;
import net.java.games.jogl.GLDrawable;
import net.java.games.jogl.GLU;

/**
 * @author David Cornette
 *
 */
public class JoglMediator {
	
	Integer numberOfDraws = 0;
	private long firsttime;

	public void initialize(GLDrawable drawable) {
		GL gl = drawable.getGL();

		gl.glShadeModel(GL.GL_SMOOTH);
		gl.glClearColor(0.95f, 0.99f, 1.0f, 1.0f);
		gl.glClearDepth(1.0);
		gl.glEnable(GL.GL_DEPTH_TEST);
		gl.glDepthFunc(GL.GL_LEQUAL);
		gl.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST);
		gl.glEnable(GL.GL_LIGHTING);
		gl.glEnable(GL.GL_LIGHT0);
		gl.glFlush();
		
		firsttime = System.currentTimeMillis();
		
	}

	public void draw(GLDrawable drawable) {
		// TODO stop the hard-coding!
		GL gl = drawable.getGL();
		GLU glu = drawable.getGLU();
		
		numberOfDraws++;
		long thistime = System.currentTimeMillis();
		long timedifference = thistime - firsttime;

		ViewerWorld world = ViewerWorld.getWorld();
		
		world.setTime(timedifference);
		
		gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
		
		// TODO Light settings ought to come from the world
		//gl.glLightfv(GL.GL_LIGHT0, GL.GL_POSITION, new float[]{5.0f, 5.0f, 4.0f, 0.0f});
		// For some reason I don't understand, the light is not in the right position
		// at the beginning
		gl.glLightfv(GL.GL_LIGHT0, GL.GL_DIFFUSE, new float[]{1.0f, 1.0f, 1.0f, 1.0f});
		gl.glLightfv(GL.GL_LIGHT0, GL.GL_SPECULAR, new float[]{1.0f, 0.6f, 0.125f, 1.0f});
		gl.glFlush();

		// TODO Camera settings should also be part of the world
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glLoadIdentity();
		glu.gluLookAt(-1.0, 2.5, 7.0, 0.0, 0.0, -5.0, 0.0, 1.0, 0.0);
		
		gl.glLightfv(GL.GL_LIGHT0, GL.GL_POSITION, new float[]{4.0f, 5.0f, 7.0f, 1.0f});
		
		Iterator<Displayable> iter = world.getDisplayables().iterator();
		while (iter.hasNext()) {
			Displayable disp = iter.next();
			disp.display(drawable);
		}


		
		
		if (numberOfDraws % 50 == 0) {
			System.out.println("draw number " + numberOfDraws + " in "
					+ (timedifference / 1000) + " seconds. "
					+ (numberOfDraws / (timedifference / 1000.0))
					+ " FPS average.");
		}
		//System.out.println("Finished drawing.  Frame is: " + world.getFrame() + " Time is: " + world.getTime());
	}

	public void reshape(GLDrawable drawable, int x, int y, int width, int height) {
		// cribbed from Kevin Duling's version of the nehe sample code
		GL gl = drawable.getGL();
		GLU glu = drawable.getGLU();
		
		if (height <= 0)
			height = 1;
		float h = (float) width / (float) height;
		gl.glViewport(0, 0, width, height);
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glLoadIdentity();
		glu.gluPerspective(45.0f, h, 1.0, 20.0);
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glLoadIdentity();
	}

}
