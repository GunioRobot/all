/**
 * 
 */
package viewer;

import java.util.Iterator;

import mesh.*;
import net.java.games.jogl.GL;
import net.java.games.jogl.GLDrawable;

/**
 * @author David Cornette
 *
 */
public class MeshDisplayable extends Displayable {

	Mesh mesh=null;
	int callList = 0;
	
	/**
	 * @param mesh
	 */
	public MeshDisplayable(Mesh mesh) {
		super();
		this.mesh = mesh;
	}

	/**
	 * 
	 */
	public MeshDisplayable() {
		super();
		// TODO Auto-generated constructor stub
	}

	/* (non-Javadoc)
	 * @see viewer.Displayable#display(net.java.games.jogl.GLDrawable)
	 */
	@Override
	public void displayUntransformed(GLDrawable drawable) {
		GL gl = drawable.getGL();
		
		if (callList == 0) {
			initCallList(gl);
		}

		gl.glPushMatrix();
		
		// For now, let's reposition the mesh the way that looks better
		//gl.glRotated(90, -1.0, 0.0, 0.0);
		//gl.glTranslated(1.0, 0.3, -0.1);
		
		//I could make the mesh carry these values.
		gl.glColor3d(1.0, 1.0, 1.0);
		gl.glMaterialfv(GL.GL_FRONT_AND_BACK, GL.GL_DIFFUSE, new float[] {0.2f, 0.7f, 0.9f, 1.0f});
		gl.glMaterialfv(GL.GL_FRONT_AND_BACK, GL.GL_SPECULAR, new float[] {0.75f, 0.75f, 0.75f, 1.0f});
		gl.glMaterialf(GL.GL_FRONT_AND_BACK, GL.GL_SHININESS, 100.0f);

		gl.glCallList(callList);
		gl.glPopMatrix();
	}

	private void initCallList(GL gl) {
		System.out.println("Initializing mesh " + mesh.getName());
		
		callList = gl.glGenLists(1);
		
		gl.glNewList(callList, GL.GL_COMPILE);
		
		Iterator<Face> iterf = mesh.getFaceList().iterator();
		while(iterf.hasNext()) {
			Face f = iterf.next();
			Vertex[] verts = f.getVerts();
			int drawtype = (verts.length == 4) ? GL.GL_QUADS : GL.GL_TRIANGLE_FAN;
			gl.glBegin(drawtype);
			for (int i = 0; i < verts.length; i++) {
				Vertex v = verts[i];
				Vec4 n = v.getNormal();
				// Lighting seems wrong if I don't negate the Y coord.
				// Is this related to having to negate the vector in the
				// lambertian shader?
				gl.glNormal3d(n.getX(), n.getY(), n.getZ());
				Vec4 tc = v.getTexcoords();
				if (tc != null) {
					gl.glTexCoord1d(tc.getX());
					gl.glTexCoord2d(tc.getX(), tc.getY());
					gl.glTexCoord3d(tc.getX(), tc.getY(), tc.getZ());
					gl.glTexCoord4d(tc.getX(), tc.getY(), tc.getZ(), tc.getW());
				}
				Vec4 loc = v.getLocation();
				gl.glVertex3d(loc.getX(), loc.getY(), loc.getZ());
			}
			gl.glEnd();
		}
		
		gl.glEndList();
	}

}
