/**
 * 
 */
package viewer;

/**
 * @author David Cornette
 *
 */
public class GLSLUniform {

	/**
	 * @author David Cornette
	 *
	 */

	private int xi;
	private int yi;
	private int zi;
	private int wi;
	
	private float xf;
	private float yf;
	private float zf;
	private float wf;
	
	private Class type;
	private byte dimension;
	
	/**
	 * @param xi
	 */
	public GLSLUniform(int xi) {
		super();
		this.type = Integer.class;
		dimension=1;
		this.xi = xi;
	}
	/**
	 * @param xi
	 * @param yi
	 */
	public GLSLUniform(int xi, int yi) {
		super();
		this.type = Integer.class;
		dimension=2;
		this.xi = xi;
		this.yi = yi;
	}
	/**
	 * @param xi
	 * @param yi
	 * @param zi
	 */
	public GLSLUniform(int xi, int yi, int zi) {
		super();
		this.type = Integer.class;
		dimension=3;
		this.xi = xi;
		this.yi = yi;
		this.zi = zi;
	}
	/**
	 * @param xi
	 * @param yi
	 * @param zi
	 * @param wi
	 */
	public GLSLUniform(int xi, int yi, int zi, int wi) {
		super();
		this.type = Integer.class;
		dimension=4;
		this.xi = xi;
		this.yi = yi;
		this.zi = zi;
		this.wi = wi;
	}
	/**
	 * @param xf
	 */
	public GLSLUniform(float xf) {
		super();
		this.type = Float.class;
		dimension=1;
		this.xf = xf;
	}
	/**
	 * @param xf
	 * @param yf
	 */
	public GLSLUniform(float xf, float yf) {
		super();
		this.type = Float.class;
		dimension=2;
		this.xf = xf;
		this.yf = yf;
	}
	/**
	 * @param xf
	 * @param yf
	 * @param zf
	 */
	public GLSLUniform(float xf, float yf, float zf) {
		super();
		this.type = Float.class;
		dimension=3;
		this.xf = xf;
		this.yf = yf;
		this.zf = zf;
	}
	/**
	 * @param xf
	 * @param yf
	 * @param zf
	 * @param wf
	 */
	public GLSLUniform(float xf, float yf, float zf, float wf) {
		super();
		this.type = Float.class;
		dimension=4;
		this.xf = xf;
		this.yf = yf;
		this.zf = zf;
		this.wf = wf;
	}
	/**
	 * @return Returns the wf.
	 */
	public float getWf() {
		return wf;
	}
	/**
	 * @param wf The wf to set.
	 */
	public void setWf(float wf) {
		this.wf = wf;
	}
	/**
	 * @return Returns the wi.
	 */
	public int getWi() {
		return wi;
	}
	/**
	 * @param wi The wi to set.
	 */
	public void setWi(int wi) {
		this.wi = wi;
	}
	/**
	 * @return Returns the xf.
	 */
	public float getXf() {
		return xf;
	}
	/**
	 * @param xf The xf to set.
	 */
	public void setXf(float xf) {
		this.xf = xf;
	}
	/**
	 * @return Returns the xi.
	 */
	public int getXi() {
		return xi;
	}
	/**
	 * @param xi The xi to set.
	 */
	public void setXi(int xi) {
		this.xi = xi;
	}
	/**
	 * @return Returns the yf.
	 */
	public float getYf() {
		return yf;
	}
	/**
	 * @param yf The yf to set.
	 */
	public void setYf(float yf) {
		this.yf = yf;
	}
	/**
	 * @return Returns the yi.
	 */
	public int getYi() {
		return yi;
	}
	/**
	 * @param yi The yi to set.
	 */
	public void setYi(int yi) {
		this.yi = yi;
	}
	/**
	 * @return Returns the zf.
	 */
	public float getZf() {
		return zf;
	}
	/**
	 * @param zf The zf to set.
	 */
	public void setZf(float zf) {
		this.zf = zf;
	}
	/**
	 * @return Returns the zi.
	 */
	public int getZi() {
		return zi;
	}
	/**
	 * @param zi The zi to set.
	 */
	public void setZi(int zi) {
		this.zi = zi;
	}
	/**
	 * @return Returns the dimension.
	 */
	public byte getDimension() {
		return dimension;
	}
	/**
	 * @param dimension The dimension to set.
	 */
	public void setDimension(byte dimension) {
		this.dimension = dimension;
	}
	/**
	 * @return Returns the type.
	 */
	public Class getType() {
		return type;
	}
	/**
	 * @param type The type to set.
	 */
	public void setType(Class type) {
		this.type = type;
	}
	
	
}
