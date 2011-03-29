/**
 * 
 */
package obj;

import java.io.*;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import mesh.Face;
import mesh.Mesh;
import mesh.Vertex;

/**
 * @author David Cornette
 *
 */
public class ObjMeshLoader {
	private ArrayList<V3> vertices = new ArrayList<V3>();
	private ArrayList<V3> normals = new ArrayList<V3>();
	private ArrayList<V3> texcoords = new ArrayList<V3>();

	/**
	 * @author David Cornette
	 *
	 */
	private class V3 {

		private double x = 0;
		private double y = 0;
		private double z = 0;
		
		public V3() {
			super();
		}

		/**
		 * @param x
		 */
		public V3(double x) {
			super();
			// TODO Auto-generated constructor stub
			this.x = x;
		}

		/**
		 * @param x
		 * @param y
		 */
		public V3(double x, double y) {
			super();
			this.x = x;
			this.y = y;
		}

		/**
		 * @param x
		 * @param y
		 * @param z
		 */
		public V3(double x, double y, double z) {
			super();
			this.x = x;
			this.y = y;
			this.z = z;
		}

		public double getX() {
			return x;
		}

		public void setX(double x) {
			this.x = x;
		}

		public double getY() {
			return y;
		}

		public void setY(double y) {
			this.y = y;
		}

		public double getZ() {
			return z;
		}

		public void setZ(double z) {
			this.z = z;
		}

	}

	public Mesh loadFile(String filename) throws FileNotFoundException, IOException {
		BufferedReader br = new BufferedReader(new FileReader(filename));
		
		// Some ugly regular expressions
		Pattern commentpat = Pattern.compile("^#");
		Pattern mtllibpat = Pattern.compile("^mtllib\\s+(\\S*)$"); // This will fail if there is whitespace at the end, I guess
		Pattern usemtlpat = Pattern.compile("^usemtl\\s+(\\S*)$");
		Pattern opat   = Pattern.compile("^o\\s+(\\S*)$");
		Pattern vpat   = Pattern.compile("^v\\s+(-?\\d*.?\\d+(?:e-?\\d+)?)\\s+(-?\\d*.?\\d+(?:e-?\\d+)?)\\s+(-?\\d*.?\\d+(?:e-?\\d+)?)$");
		Pattern vnpat  = Pattern.compile("^vn\\s+(-?\\d*.?\\d+(?:e-?\\d+)?)\\s+(-?\\d*.?\\d+(?:e-?\\d+)?)\\s+(-?\\d*.?\\d+(?:e-?\\d+)?)$");
		Pattern vtpat = Pattern.compile("^vt\\s+(-?\\d*.?\\d+(?:e-?\\d+)?)(?:\\s+(-?\\d*.?\\d+(?:e-?\\d+)?)(?:\\s+(-?\\d*.?\\d+(?:e-?\\d+)?))?)?$");
		Pattern fpat  = Pattern.compile("^f\\s+(\\d+)(?:/(\\d*))?(?:/(\\d+))?\\s+(\\d+)(?:/(\\d*))?(?:/(\\d+))?\\s+(\\d+)(?:/(\\d*))?(?:/(\\d+))?(?:\\s+(\\d+)(?:/(\\d*))?(?:/(\\d+))?)?$");
		Pattern spat   = Pattern.compile("^s\\s+(\\S*)$");
		Pattern blankpat = Pattern.compile("^\\s*$");
		
		vertices.add(new V3()); // obj uses an array index base of 1, so just add a dummy
		normals.add(new V3()); // obj uses an array index base of 1, so just add a dummy
		texcoords.add(new V3()); // obj uses an array index base of 1, so just add a dummy
		
		Mesh mesh = new Mesh();
		
		String line;
		while ((line=br.readLine()) != null) {
			//System.out.println("line: " + line);
			Matcher commentm = commentpat.matcher(line);
			Matcher mtllibm = mtllibpat.matcher(line);
			Matcher usemtlm = usemtlpat.matcher(line);
			Matcher om = opat.matcher(line);
			Matcher vm = vpat.matcher(line);
			Matcher vnm = vnpat.matcher(line);
			Matcher vtm = vtpat.matcher(line);
			Matcher fm = fpat.matcher(line);
			Matcher sm = spat.matcher(line);
			Matcher blankm = blankpat.matcher(line);
			
			
			if (commentm.find()) {
				//System.out.println("Comment line.");
			} else if (mtllibm.find()) {
				//System.out.println("mtllib:" + mtllibm.group(1));
			} else if (usemtlm.find()) {
				//System.out.println("usemtl:" + usemtlm.group(1));
			} else if (om.find()) {
				//System.out.println("Object line:" + om.group(1));
				// Might as well set the name
				mesh.setName(om.group(1));
			} else if (vm.find()) {
				vertices.add(new V3(Double.parseDouble(vm.group(1)), Double.parseDouble(vm.group(2)), Double.parseDouble(vm.group(3))));
				//System.out.println("vertex line:" + vm.group(1) + " " + vm.group(2) + " " + vm.group(3));
			} else if (vnm.find()) {
				// vertex normal
				double x = Double.parseDouble(vnm.group(1));
				double y = Double.parseDouble(vnm.group(2));
				double z = Double.parseDouble(vnm.group(3));
				normals.add(new V3(x, y, z));
				//System.out.println("vertex normal:" + vnm.group(1) + " " + vnm.group(2) + " " + vnm.group(3));
			} else if (vtm.find()) {
				// Texture Coordinates
				double u;
				double v;
				double w;
				u = Double.parseDouble(vtm.group(1));
				if (vtm.groupCount() == 1) {
					v = 0;
					w = 0;
				} else if (vtm.groupCount() == 2) {
					v = Double.parseDouble(vtm.group(2));
					w = 0;
				} else {
					// must be 3, then
					v = Double.parseDouble(vtm.group(2));
					w = Double.parseDouble(vtm.group(3));
				}
				texcoords.add(new V3(u, v, w));
				//System.out.println("3D texture coord:" + vtm.group(1) + " " + vtm.group(2) + vtm.group(3));
			} else if (fm.find()) {
				String tc1s = fm.group(2);
				String tc2s = fm.group(5);
				String tc3s = fm.group(8);
				String tc4s = fm.group(11);
							
				boolean hastexcoords = ((!tc1s.equals("")) && (!tc2s.equals("")) && (!tc3s.equals("")) && ((tc4s == null) || (!tc4s.equals(""))));
				

				String n1s = fm.group(3);
				String n2s = fm.group(6);
				String n3s = fm.group(9);
				String n4s = fm.group(12);
				boolean hasnormals = ((!n1s.equals("")) && (!n2s.equals("")) && (!n3s.equals("")) && ((n4s == null) || (!n4s.equals(""))));
				
				int vert1 = Integer.parseInt(fm.group(1));
				int vert2 = Integer.parseInt(fm.group(4));
				int vert3 = Integer.parseInt(fm.group(7));
				int vert4=0;
				if (fm.group(11) != null) {
					vert4 = Integer.parseInt(fm.group(10));
				}
				
				int tc1=0, tc2=0, tc3=0, tc4=0;
				if (hastexcoords) {
					tc1 = Integer.parseInt(tc1s);
					tc2 = Integer.parseInt(tc2s);
					tc3 = Integer.parseInt(tc3s);
					if (tc4s != null) {
						tc4 = Integer.parseInt(tc4s);
					}
				}
				
				int n1=0, n2=0, n3=0, n4=0;
				if (hasnormals) {
					n1 = Integer.parseInt(n1s);
					n2 = Integer.parseInt(n2s);
					n3 = Integer.parseInt(n3s);
					if (n4s != null) {
						n4 = Integer.parseInt(n4s);
					}
				}
				
				Vertex v1 = createVertex(vert1, tc1, n1);
				Vertex v2 = createVertex(vert2, tc2, n2);
				Vertex v3 = createVertex(vert3, tc3, n3);
				Vertex v4 = null;
				Vertex faceverts[];
				if (vert4 == 0) {
					faceverts = new Vertex[] {v1, v2, v3};
				} else {
					v4 = createVertex(vert4, tc4, n4);
					faceverts = new Vertex[] {v1, v2, v3, v4};
				} 
				
				Face f = new Face(faceverts);
				
				mesh.add(f);
				//System.out.println("face number " + mesh.numberOfFaces() + " faces");
				
			} else if (sm.find()) {
				//System.out.println("smoothing group:" + sm.group(1));
			} else if (blankm.find()) {
				//System.out.println("blank line...");
			} else {
				//System.out.println("Other type of line");
			}
		}
		return mesh;
	}

	private Vertex createVertex(int vertnum, int tcnum, int nnum) {
		if (vertnum == 0)
			return null;
		V3 vert = vertices.get(vertnum);
		Vertex v = new Vertex(vert.getX(), vert.getY(), vert.getZ());
		if (tcnum != 0){
			V3 tc = texcoords.get(tcnum);
			v.setTexCoords(tc.getX(), tc.getY(), tc.getZ());
		}
		if (nnum !=0) {
			V3 n = normals.get(nnum);
			v.setNormal(n.getX(), n.getY(), n.getZ());
		}
		return v;
	}

}
