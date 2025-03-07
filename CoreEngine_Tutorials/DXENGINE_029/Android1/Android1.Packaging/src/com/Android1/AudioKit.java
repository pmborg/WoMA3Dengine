// --------------------------------------------------------------------------------------------
// Filename: AudioKit.java
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for comercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567222;

package com.Android1;

import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.content.Context;
import android.widget.Button;
import android.view.View;
import android.util.Log;
import android.os.Bundle;
import android.os.Handler;
import java.io.IOException;
import java.io.File;
import android.media.AudioTrack;
import android.media.AudioFormat;
import android.os.Environment;
import java.io.*;

//Classe para executar o audio:
public class AudioKit 
{
     private final int mSampleRateInHz=44100;
     private AudioTrack mPlayer;
     boolean mStopBool=false;
     InputStream is;

     public void playAudio(String fileName)
     {
        //Log.w("[WOMA]", "AudioKit::playAudio()");

        //This PC\A52 de Pedro\Armazenamento interno\Android\data\com.woma\files
        //fileName = "ff-16b-1c-44100hz.wav";
        String baseDir = Environment.getExternalStorageDirectory().getAbsolutePath();
        String pathDir = baseDir + "/Android/data/com.woma/files/";

		mPlayer = new AudioTrack(AudioManager.STREAM_MUSIC, 44100,
							     AudioFormat.CHANNEL_OUT_STEREO,           //CHANNEL_OUT_MONO | CHANNEL_OUT_STEREO
							     AudioFormat.ENCODING_PCM_16BIT, 88200,
							     AudioTrack.MODE_STREAM );

        File file = new File(pathDir + File.separator + fileName);
        if(file.exists()){
          //Log.w("[WOMA]", "The file " + file.getName() + " exists!");
         }else{
          //Log.w("[WOMA]", "The file no longer exists!");
         }

        byte[] music = new byte[(int) file.length()];
		
        mPlayer.play();

        try{
			is = new BufferedInputStream(new FileInputStream(file));
			int i = 0;
			while((i = is.read(music)) != -1) {
                if(mStopBool)
                    break;
                mPlayer.write(music, 0, i);
            }
            is.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

        //Log.w("[WOMA]", "AudioKit::stopAudioV2()");
        mPlayer.release();
     }

     public void stopAudio()
     {
        //Log.w("[WOMA]", "AudioKit::stopAudio()");
        mStopBool = true;
        mPlayer.stop();
     }
}
