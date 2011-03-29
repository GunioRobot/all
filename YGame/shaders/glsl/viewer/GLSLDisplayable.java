/**
 * 
 */
package viewer;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.HashMap;
import java.util.Iterator;

import net.java.games.jogl.GL;
import net.java.games.jogl.GLDrawable;
import net.java.games.jogl.util.BufferUtils;

/**
 * @author David Cornette
 *
 */
public class GLSLDisplayable extends Displayable {
	
	private int shaderprogram = 0;
	private int permtex = 0;
	private int simplextex = 0;
	private int gradtex = 0;
	
	private String vertsrc;
	private String fragsrc;
	private String noiselibsrc = "noiselib.glsl";
	private String utilsrc = "utilities.glsl";
	private Displayable disp = null;
	
	private HashMap<String, GLSLUniform> uniforms=new HashMap<String, GLSLUniform>();
	
	// these are used for the noise implementation
	// taken from the noise application by
	//  Author: Stefan Gustavson (stegu@itn.liu.se) 2004
	// I believe they are, however, originally proposed by Ken Perlin
	private static int perm[] = { 151, 160, 137, 91, 90, 15, 131, 13, 201, 95,
			96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37,
			240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62,
			94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56,
			87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139,
			48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
			230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
			63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200,
			196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3,
			64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255,
			82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
			223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153,
			101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79,
			113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242,
			193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249,
			14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204,
			176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222,
			114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180 };
	
	private static int grad3[][] = 	{{0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},
									{1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1},
									{1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0}, // 12 cube edges
									{1,0,-1},{-1,0,-1},{0,-1,1},{0,1,1}}; // 4 more to make 16
	
	// this one was created by Gustavson
	private static int grad4[][] = 	{{0,1,1,1}, {0,1,1,-1}, {0,1,-1,1}, {0,1,-1,-1}, // 32 tesseract edges
									{0,-1,1,1}, {0,-1,1,-1}, {0,-1,-1,1}, {0,-1,-1,-1},
									{1,0,1,1}, {1,0,1,-1}, {1,0,-1,1}, {1,0,-1,-1},
									{-1,0,1,1}, {-1,0,1,-1}, {-1,0,-1,1}, {-1,0,-1,-1},
									{1,1,0,1}, {1,1,0,-1}, {1,-1,0,1}, {1,-1,0,-1},
									{-1,1,0,1}, {-1,1,0,-1}, {-1,-1,0,1}, {-1,-1,0,-1},
									{1,1,1,0}, {1,1,-1,0}, {1,-1,1,0}, {1,-1,-1,0},
									{-1,1,1,0}, {-1,1,-1,0}, {-1,-1,1,0}, {-1,-1,-1,0}};
	
	// Gustavson gives this comment with this last one
	/* 
	 * This is a look-up table to speed up the decision on which simplex we
	 * are in inside a cube or hypercube "cell" for 3D and 4D simplex noise.
	 * It is used to avoid complicated nested conditionals in the GLSL code.
	 * The table is indexed in GLSL with the results of six pair-wise
	 * comparisons beween the components of the P=(x,y,z,w) coordinates
	 * within a hypercube cell.
	 * c1 = x>=y ? 32 : 0;
	 * c2 = x>=z ? 16 : 0;
	 * c3 = y>=z ? 8 : 0;
	 * c4 = x>=w ? 4 : 0;
	 * c5 = y>=w ? 2 : 0;
	 * c6 = z>=w ? 1 : 0;
	 * offsets = simplex[c1+c2+c3+c4+c5+c6];
	 * o1 = step(160,offsets);
	 * o2 = step(96,offsets);
	 * o3 = step(32,offsets);
	 * (For the 3D case, c4, c5, c6 and o3 are not needed.)
	 */
	private static short simplex4[][] = {{0,64,128,192},{0,64,192,128},{0,0,0,0},
		  {0,128,192,64},{0,0,0,0},{0,0,0,0},{0,0,0,0},{64,128,192,0},
		  {0,128,64,192},{0,0,0,0},{0,192,64,128},{0,192,128,64},
		  {0,0,0,0},{0,0,0,0},{0,0,0,0},{64,192,128,0},
		  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
		  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
		  {64,128,0,192},{0,0,0,0},{64,192,0,128},{0,0,0,0},
		  {0,0,0,0},{0,0,0,0},{128,192,0,64},{128,192,64,0},
		  {64,0,128,192},{64,0,192,128},{0,0,0,0},{0,0,0,0},
		  {0,0,0,0},{128,0,192,64},{0,0,0,0},{128,64,192,0},
		  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
		  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
		  {128,0,64,192},{0,0,0,0},{0,0,0,0},{0,0,0,0},
		  {192,0,64,128},{192,0,128,64},{0,0,0,0},{192,64,128,0},
		  {128,64,0,192},{0,0,0,0},{0,0,0,0},{0,0,0,0},
		  {192,64,0,128},{0,0,0,0},{192,128,0,64},{192,128,64,0}};

	/**
	 * @param vertsrc
	 * @param fragsrc
	 */
	public GLSLDisplayable(String vertsrc, String fragsrc, Displayable disp) {
		super();
		this.vertsrc = vertsrc;
		this.fragsrc = fragsrc;
		this.disp  = disp;
	}

	/* (non-Javadoc)
	 * @see viewer.Displayable#display(net.java.games.jogl.GLDrawable)
	 */
	@Override
	public void displayUntransformed(GLDrawable drawable) {
		GL gl = drawable.getGL();
		
		gl.glPushMatrix();
		
		// Thes things should probably *not* be here.  Not sure wherethe belong.  Likely in the
		// MeshDisplayable or equivalent.  Maybe in a class that sits in between.
		gl.glColor3d(1.0, 1.0, 1.0);
		gl.glMaterialfv(GL.GL_FRONT_AND_BACK, GL.GL_DIFFUSE, new float[] {0.2f, 0.7f, 0.9f, 1.0f});
		gl.glMaterialfv(GL.GL_FRONT_AND_BACK, GL.GL_SPECULAR, new float[] {0.75f, 0.75f, 0.75f, 1.0f});
		gl.glMaterialf(GL.GL_FRONT_AND_BACK, GL.GL_SHININESS, 100.0f);
		setupShaders(gl);
		
		//we need to set up the following 3 textures to use the noise functions
		setupNoiseTextures(gl);
		gl.glActiveTexture(GL.GL_TEXTURE0);
		gl.glBindTexture(GL.GL_TEXTURE_2D, permtex);
		gl.glActiveTexture(GL.GL_TEXTURE1);
		gl.glBindTexture(GL.GL_TEXTURE_1D, simplextex);
		gl.glActiveTexture(GL.GL_TEXTURE2);
		gl.glBindTexture(GL.GL_TEXTURE_2D, permtex);
		
		// uniform variables to set
		// these are for the noise implementation
		int permtexloc = gl.glGetUniformLocation(shaderprogram, bbifystring("permTexture"));
		gl.glUniform1i(permtexloc, 0); // which texture to use for the permutations
		int simplextexloc = gl.glGetUniformLocation(shaderprogram, bbifystring("simplexTexture"));
		gl.glUniform1i(simplextexloc, 1); // which texture to use for the simplex calculations
		int gradtexloc = gl.glGetUniformLocation(shaderprogram, bbifystring("gradTexture"));
		gl.glUniform1i(gradtexloc, 2); // which texture to use for the gradient
		
		// and this one is for the noise demo
		int timeloc = gl.glGetUniformLocation(shaderprogram, bbifystring("time"));
		double time = ViewerWorld.getWorld().getTime();
		gl.glUniform1f(timeloc, (float)time/1000); // send the time in seconds
		
		Iterator<String> iter = uniforms.keySet().iterator();
		while(iter.hasNext()) {
			String k = iter.next();
			GLSLUniform u = uniforms.get(k);
			int loc = gl.glGetUniformLocation(shaderprogram, bbifystring(k));
			if (u.getType() == Integer.class) {
				switch (u.getDimension()) {
					case 1:
						gl.glUniform1i(loc, u.getXi());
						break;
					case 2:
						gl.glUniform2i(loc, u.getXi(), u.getYi());
						break;
					case 3:
						gl.glUniform3i(loc, u.getXi(), u.getYi(), u.getZi());
						break;
					case 4:
						gl.glUniform4i(loc, u.getXi(), u.getYi(), u.getZi(), u.getWi());
						break;
					default:
						System.err.println("Improper uniform dimension: " + u.getDimension());
				}
			} else if (u.getType() == Float.class) {
				switch (u.getDimension()) {
					case 1:
						gl.glUniform1f(loc, u.getXf());
						break;
					case 2:
						gl.glUniform2f(loc, u.getXf(), u.getYf());
						break;
					case 3:
						gl.glUniform3f(loc, u.getXf(), u.getYf(), u.getZf());
						break;
					case 4:
						gl.glUniform4f(loc, u.getXf(), u.getYf(), u.getZf(), u.getWf());
						break;
					default:
						System.err.println("Improper uniform dimension: " + u.getDimension());
				}				
			} else {
				System.err.println("Unknown uniform type: " + u.getType());
			}
		}
		
		// Have the child do its thing
		disp.display(drawable);
		gl.glPopMatrix();
		cleanupShaders(gl);
	}
	
	// TODO break this up into seperate bits for each shader
	private void setupShaders(GL gl) {
		int v = gl.glCreateShader(GL.GL_VERTEX_SHADER);
		int f = gl.glCreateShader(GL.GL_FRAGMENT_SHADER);
		int noiselibf = gl.glCreateShader(GL.GL_FRAGMENT_SHADER);
		int noiselibv = gl.glCreateShader(GL.GL_VERTEX_SHADER);
		int util = gl.glCreateShader(GL.GL_FRAGMENT_SHADER);
		
		if (shaderprogram == 0) {
			System.out.println("initializing shaders");
		try {
			String vertexsource = stringifyfile(vertsrc);  
			String fragmentsource = stringifyfile(fragsrc);
			String noiselibsource = stringifyfile(noiselibsrc);
			String utilsource = stringifyfile(utilsrc);

			ByteBuffer bbv = bbifystring(vertexsource);
			gl.glShaderSource(v, 1, new ByteBuffer[]{bbv}, (int[])null);
			gl.glCompileShader(v);
			checkLogInfo(gl, v);

			ByteBuffer bbf = bbifystring(fragmentsource);
			gl.glShaderSource(f, 1, new ByteBuffer[]{bbf}, (int[])null);
			gl.glCompileShader(f);
			checkLogInfo(gl, f);
			
			ByteBuffer bbn = bbifystring(noiselibsource);
			gl.glShaderSource(noiselibf, 1, new ByteBuffer[]{bbn}, (int[])null);
			gl.glCompileShader(noiselibf);
			checkLogInfo(gl, noiselibf);
			gl.glShaderSource(noiselibv, 1, new ByteBuffer[]{bbn}, (int[])null);
			gl.glCompileShader(noiselibv);
			checkLogInfo(gl, noiselibv);

			
			ByteBuffer bbu = bbifystring(utilsource);
			gl.glShaderSource(util, 1, new ByteBuffer[]{bbu}, (int[])null);
			gl.glCompileShader(util);
			checkLogInfo(gl, util);
			
			shaderprogram = gl.glCreateProgram();
			gl.glAttachShader(shaderprogram, v);
			gl.glAttachShader(shaderprogram, f);
			gl.glAttachShader(shaderprogram, noiselibf);
			gl.glAttachShader(shaderprogram, noiselibv);
			gl.glAttachShader(shaderprogram, util);
			
			gl.glLinkProgram(shaderprogram);
			gl.glValidateProgram(shaderprogram);
			checkLogInfo(gl, shaderprogram);

		} catch (FileNotFoundException e) {
			System.err.println("Shader source file not found.");
		} catch (IOException e) {
			System.err.println("Could not read shader file.");
		}
		}
		gl.glUseProgram(shaderprogram);
		checkLogInfo(gl, shaderprogram);
		checkLogInfo(gl, v);
		checkLogInfo(gl, f);
		checkLogInfo(gl, noiselibf);
		checkLogInfo(gl, noiselibv);
		checkLogInfo(gl, util);
	}
	
	private String stringifyfile(String filename) throws IOException {
		BufferedReader br = new BufferedReader(new FileReader(filename));
		String res = "";
		String line;
		while ((line=br.readLine()) != null) {
			res += line + "\n";
		}
		return res;
	}
	
	/*
	 * Jogl seems to have versions of the GLSL api functions which require what
	 * should be a string as a string for the older versions ending in ARB. But
	 * for the newer versions found in OpenGL 2.0, they only have ones that take
	 * a byte buffer. Therefore, I can either use the older versions, or I can
	 * use the new versions and call this for all my strings.
	 */
	private ByteBuffer bbifystring(String s) {
		ByteBuffer bb = ByteBuffer.allocateDirect(s.length());
		try {
			bb.put(s.getBytes("US-ASCII"));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return null;
		}
		return bb;
	}
	
	// This is directly lifted from the jogl version of the NeHe demo
    private void checkLogInfo(GL gl, int obj) {
        IntBuffer iVal = BufferUtils.newIntBuffer(1);
        gl.glGetObjectParameterivARB(obj, GL.GL_OBJECT_INFO_LOG_LENGTH_ARB, iVal);

        int length = iVal.get();

        if (length <= 1) {
            return;
        }

        ByteBuffer infoLog = BufferUtils.newByteBuffer(length);

        iVal.flip();
        gl.glGetInfoLogARB(obj, length, iVal, infoLog);

        byte[] infoBytes = new byte[length];
        infoLog.get(infoBytes);
        System.out.println("GLSL Validation >> " + new String(infoBytes));
    }

	private void setupNoiseTextures(GL gl) {
		setupPermTexture(gl);
		setupSimplexTexture(gl);
		setupGradTexture(gl);
	}

	private void setupGradTexture(GL gl) {
		if (gradtex == 0) {
			gl.glActiveTexture(GL.GL_TEXTURE2);
			int tmp[] = new int[1];
			gl.glGenTextures(1, tmp);
			gradtex = tmp[0];
			gl.glBindTexture(GL.GL_TEXTURE_2D, gradtex);
			ByteBuffer pixels = ByteBuffer.allocateDirect(256 * 256 * 4);
			for (int i = 0; i < 256; i++) {
				for (int j = 0; j < 256; j++) {
					int value = perm[(j + perm[i]) & 0xFF];
					pixels.put((byte)(grad4[value & 0x1F][0] * 64 + 64));
					pixels.put((byte)(grad4[value & 0x1F][1] * 64 + 64));
					pixels.put((byte)(grad4[value & 0x1F][2] * 64 + 64));
					pixels.put((byte)(grad4[value & 0x1F][3] * 64 + 64));
				}
			}
			
			gl.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGBA, 256, 256, 0, GL.GL_RGBA, GL.GL_UNSIGNED_BYTE, pixels);
			gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_NEAREST);
			gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_NEAREST);
		}
	}

	private void setupSimplexTexture(GL gl) {
		if (simplextex == 0) {
			gl.glActiveTexture(GL.GL_TEXTURE1);
			int tmp[] = new int[1];
			gl.glGenTextures(1, tmp);
			simplextex =tmp[0];
			gl.glBindTexture(GL.GL_TEXTURE_1D, simplextex);
			ByteBuffer pixels = ByteBuffer.allocateDirect(64 * 4);
			for (int i = 0; i < 64; i++) {
				for (int j = 0; j < 4; j++) {
					pixels.put((byte)(simplex4[i][j]));
				}
			}
			gl.glTexImage1D(GL.GL_TEXTURE_1D, 0, GL.GL_RGBA, 64, 0, GL.GL_RGBA, GL.GL_UNSIGNED_BYTE, pixels);
			gl.glTexParameteri(GL.GL_TEXTURE_1D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_NEAREST);
			gl.glTexParameteri(GL.GL_TEXTURE_1D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_NEAREST);
		}
	}

	// Based on initPermTexture from GLSLnoise4.c
	private void setupPermTexture(GL gl) {
		if (permtex == 0) {
			gl.glActiveTexture(GL.GL_TEXTURE0);
			int tmp[] = new int[1];
			gl.glGenTextures(1, tmp);
			permtex = tmp[0];
			gl.glBindTexture(GL.GL_TEXTURE_2D, permtex);
			ByteBuffer pixels = ByteBuffer.allocateDirect(256 * 256 * 4);
			for (int i = 0; i < 256; i++) {
				for (int j = 0; j < 256; j++) {
					int value = perm[(j + perm[i]) & 0xFF];
					pixels.put((byte)(grad3[value & 0x0F][0] * 64 + 64));
					pixels.put((byte)(grad3[value & 0x0F][1] * 64 + 64));
					pixels.put((byte)(grad3[value & 0x0F][2] * 64 + 64));
					pixels.put((byte)(value));
				}
			}
			
			gl.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGBA, 256, 256, 0, GL.GL_RGBA, GL.GL_UNSIGNED_BYTE, pixels);
			gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_NEAREST);
			gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_NEAREST);
		}
	}
	
	// Stop using this shader
	private void cleanupShaders(GL gl) {
		gl.glUseProgram(0);
	}
	
	public void addUniform(String key, int x) {
		GLSLUniform u = new GLSLUniform(x);
		uniforms.put(key, u);
	}
	public void addUniform(String key, int x, int y) {
		GLSLUniform u = new GLSLUniform(x, y);
		uniforms.put(key, u);
	}
	public void addUniform(String key, int x, int y, int z) {
		GLSLUniform u = new GLSLUniform(x, y, z);
		uniforms.put(key, u);
	}
	public void addUniform(String key, int x, int y, int z, int w) {
		GLSLUniform u = new GLSLUniform(x, y, z, w);
		uniforms.put(key, u);
	}
	
	public void addUniform(String key, float x) {
		GLSLUniform u = new GLSLUniform(x);
		uniforms.put(key, u);
	}
	public void addUniform(String key, float x, float y) {
		GLSLUniform u = new GLSLUniform(x, y);
		uniforms.put(key, u);
	}
	public void addUniform(String key, float x, float y, float z) {
		GLSLUniform u = new GLSLUniform(x, y, z);
		uniforms.put(key, u);
	}
	public void addUniform(String key, float x, float y, float z, float w) {
		GLSLUniform u = new GLSLUniform(x, y, z, w);
		uniforms.put(key, u);
	}
	
}
