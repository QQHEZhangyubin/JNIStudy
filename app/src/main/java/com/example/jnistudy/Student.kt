package com.example.jnistudy

data class Student(
    val name:String,
    val age:Int,
    val score:Float,
    val isMale: Boolean
) {
    override fun toString(): String {
        return "Student(name='$name', mAge=$age, score=$score, isMale=$isMale)"
    }
}
