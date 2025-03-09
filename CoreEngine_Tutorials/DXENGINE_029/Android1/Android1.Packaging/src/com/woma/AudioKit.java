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

package com.woma;

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
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import android.app.NativeActivity;
import android.os.Bundle;
import android.util.Log;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

//This PC\A52 de Pedro\Armazenamento interno\Android\data\com.woma\files
//Classe para executar o audio:
public class AudioKit 
{
     private final int mSampleRateInHz=44100;
     private AudioTrack mPlayer;
     boolean mStopBool=false;
     InputStream is;

     public void playAudio(String fileName)
     {
	 InputStream is;

        Log.w("[WOMA]", "AudioKit::playAudio()");

		//String baseDir = "/storage/emulated/0";
        String baseDir = Environment.getExternalStorageDirectory().getAbsolutePath();
		Log.w("[WOMA]", "baseDir: "+baseDir);
        String filePath = baseDir + "/Android/data/com.woma/files/"+ fileName;
		Log.w("[WOMA]", "filePath: "+filePath);

		int sampleRate;
		short numChannels;
		{
		File file = new File(filePath);

		//File file = new File(fileName);
		Log.w("[WOMA]", "fileName: " + filePath);

		byte[] header = new byte[44]; // WAV header is 44 bytes long
		try {
			FileInputStream fis = new FileInputStream(file);
			int bytesRead = fis.read(header);
			if (bytesRead != 44) {
				throw new IOException("Invalid WAV file: Expected 44-byte header.");
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

        // Parse the WAV header
        ByteBuffer buffer = ByteBuffer.wrap(header);
        buffer.order(ByteOrder.LITTLE_ENDIAN); // WAV files are little-endian

        // RIFF header
        String chunkId = new String(header, 0, 4); // "RIFF"
        String format = new String(header, 8, 4); // "WAVE"
        Log.w("[WOMA]", "Chunk ID: " + chunkId);
        Log.w("[WOMA]", "Format: " + format);

        // "fmt " chunk
        String subChunk1Id = new String(header, 12, 4); // "fmt "
        int subChunk1Size = buffer.getInt(16); 			// 16 bits of format data
        short audioFormat = buffer.getShort(20); 		// Audio format (1 = PCM)
        numChannels = buffer.getShort(22); 				// Number of channels (1 = mono, 2 = stereo)
        sampleRate = buffer.getInt(24); 				// Sample rate (e.g., 44100)
        int byteRate = buffer.getInt(28); 				// Byte rate (sampleRate * numChannels * bitsPerSample/8)
        short blockAlign = buffer.getShort(32); 		// Block align (numChannels * bitsPerSample/8)
        short bitsPerSample = buffer.getShort(34); 		// Bits per sample (e.g., 16)
            
        Log.w("[WOMA]", "Subchunk1 ID: " + subChunk1Id);
        Log.w("[WOMA]", "Subchunk1 Size: " + subChunk1Size);
        Log.w("[WOMA]", "Audio Format: " + audioFormat);
        Log.w("[WOMA]", "Number of Channels: " + numChannels);
        Log.w("[WOMA]", "Sample Rate: " + sampleRate);
        Log.w("[WOMA]", "Byte Rate: " + byteRate);
        Log.w("[WOMA]", "Block Align: " + blockAlign);
        Log.w("[WOMA]", "Bits Per Sample: " + bitsPerSample);
		}
		
		int channels;
		if (numChannels == 1)
			channels=AudioFormat.CHANNEL_OUT_MONO;
		else
			channels=AudioFormat.CHANNEL_OUT_STEREO;

		int mPlayBufferSize = AudioTrack.getMinBufferSize(sampleRate,
                AudioFormat.CHANNEL_OUT_STEREO, AudioFormat.ENCODING_PCM_16BIT);
		Log.w("[WOMA]", "mPlayBufferSize: "+mPlayBufferSize);

		mPlayer = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate,
								 channels,
							     AudioFormat.ENCODING_PCM_16BIT, mPlayBufferSize,
							     AudioTrack.MODE_STREAM );

		File file = new File(filePath);

        if(file.exists()){
          Log.w("[WOMA]", "The file " + file.getName() + " exists!");
         }else{
          Log.w("[WOMA]", "The file no longer exists!");
         }

        byte[] music = new byte[(int) file.length()];
		
		Log.w("[WOMA]", "mPlayer.play();");
        mPlayer.play();

        try{
			
			is = new BufferedInputStream(new FileInputStream(file));
			int i = 0;
			while((i = is.read(music)) != -1) {
                if(mStopBool)
                    break;
                mPlayer.write(music, 0, i);
            }
		} catch (IOException e) {
			e.printStackTrace();
		}
        
		//Log.w("[WOMA]", "stopAudio();");
        //stopAudio();
		//is.close();
     }

     public void stopAudio()
     {
        Log.w("[WOMA]", "AudioKit::stopAudio()");
        mStopBool = true;
        mPlayer.stop();
     }
}
