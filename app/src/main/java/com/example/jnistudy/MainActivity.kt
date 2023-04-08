package com.example.jnistudy

import android.content.Context
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.TextView
import com.example.jnistudy.databinding.ActivityMainBinding
import com.example.nativelib.NativeLib
import org.json.JSONObject
import java.io.File

class MainActivity : AppCompatActivity() {

    private val TAG: String = "MainActivity"
    private lateinit var binding: ActivityMainBinding

    var mName: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

//        for(i in 0..1) {
//            stringFromJNI()
//        }
        // Example of a call to a native method
        binding.sampleText.text = "........"
        Log.d(TAG, "bekko: Thread.currentThread().id = "+ Thread.currentThread().name)

        Log.d(TAG, "onCreate: mName = "+ mName)
        updateNameField()
        Log.d(TAG, "onCreate: mName = "+ mName)
        val student = Student(name = "bekko", age = 25, score = 12.0f, isMale = false)
        Log.d(TAG, "student.toString() =  " + student.toString())
        writeStudent(student)
        Log.d(TAG, "student.NewtoString() =  " + student.toString())
        NativeLib().stringFromJNI()

        www(object : OnStatusListener{
            override fun a(int: Int, student: Student, file: File) {
                TODO("Not yet implemented")
            }

            override fun b(boolean: Boolean, context: Context) {
                TODO("Not yet implemented")
            }

        })
    }

    /**
     * A native method that is implemented by the 'jnistudy' native library,
     * which is packaged with this application.
     */
    external fun www(onStatusListener: OnStatusListener):JSONObject

    external fun writeStudent(student: Student): Unit

    external fun updateNameField(): Unit

    external fun stringFromJNI(): String

    external fun adf(a: Int, b: Boolean): Unit

    companion object {
        // Used to load the 'jnistudy' library on application startup.
        init {
            System.loadLibrary("jnistudy")
        }

        external fun bbb(): Unit
    }
}