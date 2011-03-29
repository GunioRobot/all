/**
 * 
 */
package viewer;

import net.java.games.jogl.GL;

/**
 * @author David Cornette
 *
 */
public class RotateTransformation implements Transformation {

	private double angle;
	private double x;
	private double y;
	private double z;

	public RotateTransformation(double angle, double x, double y, double z){
		this.angle = angle;
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	/* (non-Javadoc)
	 * @see viewer.Transformation#doTransformation()
	 */
	public void doTransformation(GL gl) {
		gl.glRotated(angle, x, y, z);
	}

}
