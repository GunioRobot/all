/**
 * 
 */
package obj;

import java.io.FileNotFoundException;
import java.io.IOException;

import mesh.Mesh;

/**
 * @author David Cornette
 *
 */
public class ObjMeshLoadTest {

	/**
	 * @param args
	 * @throws IOException 
	 * @throws FileNotFoundException 
	 */
	public static void main(String[] args) throws FileNotFoundException, IOException {
		
		String obj;
		if (args.length >= 1) {
			obj = args[0];
		} else {
			obj = "Cube.obj";
		}
		ObjMeshLoader oml = new ObjMeshLoader();
		Mesh m = oml.loadFile(obj);
		System.out.println("The loaded mesh has " + m.numberOfFaces() + " faces.");
	}

}
