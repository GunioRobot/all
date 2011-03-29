/**
 * 
 */
package viewer;

import java.util.ArrayList;
import java.util.List;

/**
 * @author David Cornette
 *
 */
public class ViewerWorld {

	public static final int REAL_TIME = 0;
	public static final int FRAME = 1;
	
	// There should only be one world, I think.
	// So I am using a singleton.
	private static ViewerWorld theWorld;
	
	private ViewerWorld() {
		displayables = new ArrayList<Displayable>();
	}

	public static synchronized ViewerWorld getWorld() {
		if (theWorld == null) {
			theWorld = new ViewerWorld();
		}
		return theWorld;
	}
	
	private List<Displayable> displayables;
	private double time;
	private int framenum = 0;
	private int timeMode = REAL_TIME;
	private double framerate = 30.0;
	
	public void add(Displayable d) {
		displayables.add(d);
	}
	
	public List<Displayable> getDisplayables() {
		return displayables;
	}

	public void setTime(double time) {
		this.time = time;
	}

	/**
	 * @return Returns the time.
	 */
	public double getTime() {
		if (timeMode == REAL_TIME) {
			return time;
		} else if (timeMode == FRAME) {
			return framenum * 1000.0 / framerate;
		} else {
			return 0.0;
		}
	}

	public void setTimeMode(int timeMode) {
		this.timeMode  = timeMode; 
	}

	public int getTimeMode() {
		return timeMode;
	}

	public double getFramerate() {
		return framerate;
	}

	public void setFramerate(double framerate) {
		this.framerate = framerate;
	}

	public void incrementFrame() {
		this.framenum++;
	}
	
	public int getFrame() {
		return framenum;
	}

}
