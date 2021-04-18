package com.example.media;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        MediaSt codec = new MediaSt();

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(codec.get_version());
    }
}