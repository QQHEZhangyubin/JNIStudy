package com.example.nativelib

import android.content.Context
import android.os.Environment
import android.util.Log
import java.io.ByteArrayInputStream
import java.io.File
import java.io.FileOutputStream

class CuraEngineLib {

    /**
     * A native method that is implemented by the 'nativelib' native library,
     * which is packaged with this application.
     */
//    external fun stringFromJNI(): String
//
//    external fun test(file: File, inputStream: ByteArrayInputStream)

    external fun nativeSlice(stlName: String,
                             stlAbsolutePath: String,
                             configPath:String): Int

    //
    fun slicer(context: Context): Unit {
        val cacheDirAbsolutePath = context.applicationContext.cacheDir.absolutePath

        nativeSlice("cam_low.stl",
            "$cacheDirAbsolutePath/model",
            "$cacheDirAbsolutePath/config")
    }

    fun extracted(context: Context) {
        // 将assets目录中stl文件 写入到cache目录中

        Log.d("BEKKO", "将res/raw中stl文件 写入到cache目录中")
        val cacheDir = context.applicationContext.cacheDir

        //在cache目录下创建一个文件夹model
        val modelDir = File(cacheDir, "model")
        modelDir.mkdir()

        val downloadFolderPath =
            Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).absolutePath

        Log.d("BEKKO", "outputDir2 = $downloadFolderPath")

        // 往downloadFolderPath中写入测试文本文件
        val testFile = File(downloadFolderPath, "ankerOut")
        if (!testFile.exists()) {
            testFile.mkdir()
        }

        val definitionsDir = File(cacheDir, "definitions")
        definitionsDir.mkdir()



        //        if (cacheDir.listFiles().isEmpty()) {
        val fileNames = arrayOf(
            "cam_low.stl",
            "fan_stator_low.stl",
            "head_with_mid.stl",
            "trex_h_mid.stl",
            "vlamp_high.stl",
            "voronol_high.stl"
        )
        for (file_name in fileNames) {

            val inputStream = context.assets.open(file_name)
            val file = File(modelDir, file_name)
            val outputStream = FileOutputStream(file)
            inputStream.copyTo(outputStream)
            inputStream.close()
            outputStream.close()
        }

        val jsonfileNames = arrayOf(
            "anker_fdmextruder.def.json",
            "anker_fdmprinter.def.json",

            )
        for (jfile_name in jsonfileNames) {

            val inputStream = context.assets.open(jfile_name)
            val file = File(definitionsDir, jfile_name)
            val outputStream = FileOutputStream(file)
            inputStream.copyTo(outputStream)
            inputStream.close()
            outputStream.close()
        }

        val file = File(cacheDir, "config")
        file.mkdir()
        // 将asssets目录中的config文件夹及其子目录 写入到cache目录中

//        val open = context.assets.open("config/")
////        val file = File(config, "config")
//        val outputStream = FileOutputStream(file)
//        open.copyTo(outputStream)
//        open.close()
//        outputStream.close()


    }


    companion object {
        // Used to load the 'nativelib' library on application startup.
        init {
            System.loadLibrary("AnkerEngine")
        }

    }
}