/**
 * 
 */
package mesh;

/**
 * @author David Cornette
 *
 */
public class Vertex {

	private Vec4 location = null;
	private Vec4 texcoords = null;
	private Vec4 normal = null;
	
	public Vertex(double x, double y, double z) {
		location = new Vec4(x, y, z, 1.0);
	}

	public void setTexCoords(double u, double v, double w) {
		texcoords = new Vec4(u, v, w, 0.0);
	}

	public void setNormal(double x, double y, double z) {
		normal = new Vec4(x, y, z, 0.0);
	}

	/**
	 * @return Returns the location.
	 */
	public Vec4 getLocation() {
		return location;
	}

	/**
	 * @return Returns the normal.
	 */
	public Vec4 getNormal() {
		return normal;
	}

	/**
	 * @return Returns the texcoords.
	 */
	public Vec4 getTexcoords() {
		return texcoords;
	}

}
