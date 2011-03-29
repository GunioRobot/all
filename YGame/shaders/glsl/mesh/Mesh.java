/**
 * 
 */
package mesh;

import java.util.ArrayList;

/**
 * @author David Cornette
 *
 */
public class Mesh {
	
	private String name=null;
	private ArrayList<Face> faces = new ArrayList<Face>();
	
	public void setName(String name) {
		this.name = name;
		
	}

	/**
	 * @return Returns the name.
	 */
	public String getName() {
		return name;
	}

	public void add(Face f) {
		faces.add(f);
	}

	public int numberOfFaces() {
		return faces.size();
	}
	
	public ArrayList<Face> getFaceList() {
		return faces;
	}

}
