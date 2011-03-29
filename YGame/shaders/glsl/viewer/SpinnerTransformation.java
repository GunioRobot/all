/**
 * 
 */
package viewer;

import net.java.games.jogl.GL;

/**
 * @author David Cornette
 *
 */
public class SpinnerTransformation implements Transformation {

	private double x;
	private double y;
	private double z;
	private double degreespersec;

	public SpinnerTransformation(double degreespersec, double x, double y, double z){
		this.degreespersec = degreespersec;
		this.x = x;
		this.y = y;
		this.z = z;
	}
	/* (non-Javadoc)
	 * @see viewer.Transformation#doTransformation(net.java.games.jogl.GL)
	 */
	public void doTransformation(GL gl) {
		double timeseconds = ViewerWorld.getWorld().getTime()/1000;
		double degrees = degreespersec * timeseconds;
		gl.glRotated(degrees, x, y, z);

	}

}
