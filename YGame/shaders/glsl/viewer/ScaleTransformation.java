/**
 * 
 */
package viewer;

import net.java.games.jogl.GL;

/**
 * @author David Cornette
 *
 */
public class ScaleTransformation implements Transformation {

	
	private double x;
	private double y;
	private double z;

	/**
	 * @param x
	 */
	public ScaleTransformation(double x) {
		super();
		this.x = x;
		this.y = x;
		this.z = x;
	}

	/**
	 * @param x
	 * @param y
	 * @param z
	 */
	public ScaleTransformation(double x, double y, double z) {
		super();
		this.x = x;
		this.y = y;
		this.z = z;
	}

	/* (non-Javadoc)
	 * @see viewer.Transformation#doTransformation(net.java.games.jogl.GL)
	 */
	public void doTransformation(GL gl) {
		gl.glScaled(x, y, z);
	}

}
