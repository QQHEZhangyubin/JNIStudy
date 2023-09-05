package com.example.jnistudy

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.jnistudy.databinding.ActivityMainBinding
import com.example.nativelib.CuraEngineLib
import com.example.openmp.NativeLib

class MainActivity : AppCompatActivity() {

    private val TAG: String = "MainActivity"
    private lateinit var binding: ActivityMainBinding

    var mName: String? = null
    val mAnkerEngineLib = CuraEngineLib()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.buttonLoadFile.setOnClickListener {
            mAnkerEngineLib.extracted(this@MainActivity)
        }

        binding.buttonStartSlicer.setOnClickListener {
            mAnkerEngineLib.slicer(this@MainActivity)
        }

        binding.btnOpenmp.setOnClickListener {
            NativeLib().stringFromJNI()
        }

//        val a: Long  = 10000L
        binding.tvLd.text = String.format(getString(R.string.number), 123)
    }

    /**
     * A native method that is implemented by the 'jnistudy' native library,
     * which is packaged with this application.
     */
    external fun www(onStatusListener: OnStatusListener):ByteArray

    external fun writeStudent(student: Student): Unit

    external fun updateNameField(): Unit

    external fun stringFromJNI(): String

    external fun adf(a: Int, b: Boolean): Unit

    external fun cppJson(): Unit

    external fun ji():Unit

    companion object {
        // Used to load the 'jnistudy' library on application startup.
        init {
            System.loadLibrary("jnistudy")
        }

        external fun bbb(): Unit
    }
}