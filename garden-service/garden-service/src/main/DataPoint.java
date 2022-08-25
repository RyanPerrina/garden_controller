package main;

class DataPoint {
        private String state;
	private boolean l1;
	private boolean l2;
	private int l3;
	private int l4;
	private float temp;
	private int light;
	
	public DataPoint(final String state, final boolean l1, final boolean l2, final int l3, final int l4, final float temp, final int light) {
		this.state = state;
	        this.l1 = l1;
		this.l2 = l2;
		this.l3 = l3;
		this.l4 = l4;
		this.temp = temp;
		this.light = light;
	}
	
    public String getState() {
        return this.state;
    }

    public boolean getL1() {
        return l1;
    }

    public boolean getL2() {
        return l2;
    }

    public int getL3() {
        return l3;
    }

    public int getL4() {
        return l4;
    }

    public float getTemp() {
        return temp;
    }

    public int getLight() {
        return light;
    }
}
