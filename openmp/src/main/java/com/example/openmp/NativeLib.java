package com.example.openmp;

public class NativeLib {

    // Used to load the 'openmp' library on application startup.
    static {
        System.loadLibrary("openmp");
    }

    /**
     * A native method that is implemented by the 'openmp' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}