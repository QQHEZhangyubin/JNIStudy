package com.example.nativelib

import java.io.ByteArrayInputStream
import java.io.File

class NativeLib {

    /**
     * A native method that is implemented by the 'nativelib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun test(file: File, inputStream: ByteArrayInputStream)

    companion object {
        // Used to load the 'nativelib' library on application startup.
        init {
            System.loadLibrary("nativelib")
        }
    }
}