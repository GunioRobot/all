package mesh;

public class Face {
	
	private Vertex verts[];
	
	public Face() {
		super();
	}
	
	public Face(Vertex[] faceverts) {
		verts = faceverts;
	}

	/**
	 * @return Returns the verts.
	 */
	public Vertex[] getVerts() {
		return verts;
	}

	/**
	 * @param verts The verts to set.
	 */
	public void setVerts(Vertex[] verts) {
		this.verts = verts;
	}

}
