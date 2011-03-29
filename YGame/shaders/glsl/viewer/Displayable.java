/**
 * Should this be an interface? probably. 
 */
package viewer;

import java.util.ArrayList;
import java.util.Iterator;

import net.java.games.jogl.GL;
import net.java.games.jogl.GLDrawable;

/**
 * @author David Cornette
 *
 */
public abstract class Displayable {
	
	ArrayList<Transformation> transformations = new ArrayList<Transformation>();
	
	public void display(GLDrawable drawable) {
		GL gl = drawable.getGL();
		
		gl.glPushMatrix();
		// apply transformations
		Iterator<Transformation> iter = transformations.iterator();
		while(iter.hasNext()) {
			Transformation t = iter.next();
			t.doTransformation(gl);
		}
		
		// Do the actual displaying
		displayUntransformed(drawable);
		
		gl.glPopMatrix();
	}
	
	protected abstract void displayUntransformed(GLDrawable drawable);

	public void addTransformation(int index, Transformation element) {
		transformations.add(index, element);
	}

	public boolean addTransformation(Transformation o) {
		return transformations.add(o);
	}

	public void clearTransformations() {
		transformations.clear();
	}
}
