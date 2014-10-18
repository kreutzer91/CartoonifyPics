package com.example.opencv4androidexample;

public class ImageProc {
	public static native void CartoonProc(int[] pixels, int[] resultInt, int width, int height);
	
	public static native void CartoonProcNew(long addrSrc, long addrDst);
}
