package com.example.jnistudy

import org.junit.Test

import org.junit.Assert.*
import java.nio.ByteBuffer
import java.nio.ByteOrder

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
class ExampleUnitTest {
    @Test
    fun addition_isCorrect() {
        val errorContent = StringBuilder(
            """(${Integer.toHexString(200)}""".trimIndent()
        )
        errorContent.append(")")

        println(errorContent)
        val a = ByteArray(4)
        ByteBuffer.wrap(a).order(ByteOrder.BIG_ENDIAN);
    }
}