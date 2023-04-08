package com.example.jnistudy

class OuterClass {
    private val outerField = "Outer Field"

     internal class InnerClass {
        fun accessOuterField() {
            println("Accessing outer field: ")
        }
    }
}