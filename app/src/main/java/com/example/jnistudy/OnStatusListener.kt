package com.example.jnistudy

import android.content.Context
import java.io.File

interface OnStatusListener {
    fun a(int: Int, student: Student, file: File): Unit
    fun b(boolean: Boolean, context: Context): Unit
}