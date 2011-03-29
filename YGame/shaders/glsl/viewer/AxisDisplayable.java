/**
 * 
 */
package viewer;

import net.java.games.jogl.GL;
import net.java.games.jogl.GLDrawable;

/**
 * @author David Cornette
 *
 */
public class AxisDisplayable extends Displayable {

	private double xSize = 5.0;
	private double ySize = 5.0;
	private double zSize = 5.0;
	private boolean shaded = false;
	private float width = 3.0f;

	public AxisDisplayable(double xSize, double ySize, double zSize) {
		this.xSize=xSize;
		this.ySize=ySize;
		this.zSize=zSize;
	}

	/* (non-Javadoc)
	 * @see viewer.Displayable#display(net.java.games.jogl.GLDrawable)
	 */
	@Override
	public void displayUntransformed(GLDrawable drawable) {
		GL gl = drawable.getGL();

		//draw some axes so we have a sense of where we are.
		gl.glLineWidth(width);
		if (shaded == false) {
			gl.glDisable(GL.GL_LIGHTING);;
		}
		gl.glBegin(GL.GL_LINES);
			// X-Axis, in red
			gl.glColor3d(1.0, 0.0, 0.0);
			gl.glVertex3d(0.0, 0.0, 0.0);
			gl.glVertex3d(xSize, 0.0, 0.0);
			// Y-Axis, in green
			gl.glColor3d(0.0, 1.0, 0.0);
			gl.glVertex3d(0.0, 0.0, 0.0);
			gl.glVertex3d(0.0, ySize, 0.0);
			// Z-Axis, in blue
			gl.glColor3d(0.0, 0.0, 1.0);
			gl.glVertex3d(0.0, 0.0, 0.0);
			gl.glVertex3d(0.0, 0.0, zSize);
		gl.glEnd();
		if (shaded == false) {
			gl.glEnable(GL.GL_LIGHTING);
		}
	}

}
