package com.example.testffmpegapplication

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.testffmpegapplication.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = getConfiguration()
    }


    /**
     * 在 ffmpegNdkCustom.so 库中声明的原生c++的native方法，起连接作用
     */
    external fun stringFromFFTest(): String
    external fun getConfiguration(): String?

    companion object {
        init {
            //用于在程序启动时加载cmake编译好的目标库testffmpegapplication.so，注意大小写
            System.loadLibrary("testffmpegapplication")
        }
    }
}