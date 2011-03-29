/**
 * 
 */
package viewer;

import net.java.games.jogl.GL;

/**
 * @author David Cornette
 *
 */
public class TranslateTransformation implements Transformation {

	private double x;
	private double y;
	private double z;
	
	public TranslateTransformation(double x, double y, double z){
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	/* (non-Javadoc)
	 * @see viewer.Transformation#doTransformation(net.java.games.jogl.GL)
	 */
	public void doTransformation(GL gl) {
		gl.glTranslated(x, y, z);
	}

}
