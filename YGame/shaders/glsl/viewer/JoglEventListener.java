/**
 * 
 */
package viewer;

import java.awt.event.*;

import net.java.games.jogl.GLCanvas;
import net.java.games.jogl.GLDrawable;
import net.java.games.jogl.GLEventListener;

/**
 * @author David Cornette
 *
 */
public class JoglEventListener implements GLEventListener, KeyListener, MouseListener, MouseMotionListener, MouseWheelListener {

	private GLCanvas canvas;
	private JoglMediator mediator;
	
	
	public JoglEventListener(GLCanvas canvas) {
		this.canvas = canvas;
		mediator = new JoglMediator();
	}

	public void init(GLDrawable drawable) {
		mediator.initialize(drawable);
		
	}

	public void display(GLDrawable drawable) {
		mediator.draw(drawable);
		
	}

	public void reshape(GLDrawable drawable, int x, int y, int width, int height) {
		mediator.reshape(drawable, x, y, width, height);
		
	}

	public void displayChanged(GLDrawable drawable, boolean modeChanged, boolean deviceChanged) {
		// Do nothing.  This isn't even implemented.
		
	}

	public void keyTyped(KeyEvent e) {
		ViewerWorld world = ViewerWorld.getWorld();
		world.incrementFrame();
		//System.out.println("Incrementing frame.  Frame: " + world.getFrame());
	}

	public void keyPressed(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void mouseClicked(MouseEvent e) {
		// TODO Auto-generated method stub
	}

	public void mousePressed(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void mouseReleased(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void mouseEntered(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void mouseExited(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void mouseDragged(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void mouseMoved(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void mouseWheelMoved(MouseWheelEvent e) {
		// TODO Auto-generated method stub
		
	}

}
