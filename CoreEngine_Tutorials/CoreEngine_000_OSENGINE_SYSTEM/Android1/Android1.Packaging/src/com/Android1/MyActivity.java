// https://www.tutorialspoint.com/android/android_hello_world_example.htm

// https://github.com/codepath/android_hello_world
// C:\Users\pedro\Downloads\android_hello_world-master\android_hello_world-master

// Android 8.1 	            27 	    Oreo_MR1
// Android 8.0 	 NDK        26 	    Oreo 	
// Android 7.1 	 PACKAGING  25 	    Nougat_MR1
// Android 7.0 	            24 	    Nougat 	
// Android 6.0 	            23 	    MARSHMALLOW 	
// Android 5.1 	            22 	    LOLLIPOP_MR1 	
// Android 5.0 	            21 	    LOLLIPOP 	
// Android 4.4W 	        20 	    KITKAT_WATCH 	KitKat for Wearables Only
// Android 4.4 	            19 	    KITKAT 	
// Android 4.3 	            18 	    JELLY_BEAN_MR2 	
// Android 4.2, 4.2.2 	    17 	    JELLY_BEAN_MR1 	
// Android 4.1, 4.1.1 	    16 	    JELLY_BEAN 	
// Android 4.0.3, 4.0.4 	15 	    ICE_CREAM_SANDWICH_MR1 	
// Android 4.0,4.0.1,4.0.2  14 	    ICE_CREAM_SANDWICH 	
// Android 3.2 	            13 	    HONEYCOMB_MR2 	
// Android 3.1.x 	        12 	    ONEYCOMB_MR1 	
// Android 3.0.x 	        11 	    NEYCOMB 	
// Android 2.3.4,2.3.3	    10 	    GERBREAD_MR1 	
// Android 2.3.2,2.3.1,2.3   9 	    GINGERBREAD 	
// Android 2.2.x 	         8 	    FROYO
// Android 2.1.x 	         7 	    ECLAIR_MR1 	
// Android 2.0.1 	         6 	    ECLAIR_0_1 	
// Android 2.0 	             5 	    ECLAIR 	
// Android 1.6 	             4 	    DONUT 	
// Android 1.5 	             3 	    CUPCAKE 	
// Android 1.1 	             2 	    BASE_1_1 	
// Android 1.0 	             1 	    BASE

//-------------------------------
// Android Libraries:
//
//      android.app - Provides access to the application model and is the cornerstone of all Android applications.
//
//      android.content - Facilitates content access, publishing and messaging between applications and application components.
//
//      android.database - Used to access data published by content providers and includes SQLite database management classes.
//
//      android.opengl - A Java interface to the OpenGL ES 3D graphics rendering API.
//
//      android.os - Provides applications with access to standard operating system services including messages, system services and inter-process communication.
//
//      android.text - Used to render and manipulate text on a device display.
//
//      android.view - The fundamental building blocks of application user interfaces.
//
//      android.widget - A rich collection of pre-built user interface components such as buttons, labels, list views, layout managers, radio buttons etc.
//
//      android.webkit - A set of classes intended to allow web-browsing capabilities to be built into applications.

//-------------------------------
// Application Framework:
//
//      Activity Manager - Controls all aspects of the application lifecycle and activity stack.
//      
//      Content Providers - Allows applications to publish and share data with other applications.
//      
//      Resource Manager - Provides access to non-code embedded resources such as strings, color settings and user interface layouts.
//      
//      Notifications Manager - Allows applications to display alerts and notifications to the user.
//      
//      View System - An extensible set of views used to create application user interfaces.

//-------------------------------
//Components:
//
//      1 Activities
//       They dictate the UI and handle the user interaction to the smart phone screen.
//      
//      2 Services
//       They handle background processing associated with an application.
//      
//      3 Broadcast Receivers
//       They handle communication between Android OS and applications.
//      
//      4 Content Providers
//       They handle data and database management issues.

//-------------------------------
//      1 	Java
//      This contains the .java source files for your project. By default, it includes an MainActivity.java source file having an activity class that runs when your app is launched using the app icon.
//      
//      2 	res/drawable-hdpi
//      This is a directory for drawable objects that are designed for high-density screens.
//      
//      3 	res/layout
//      This is a directory for files that define your app's user interface.
//      
//      4 	res/values
//      This is a directory for other various XML files that contain a collection of resources, such as strings and colours definitions.
//      
//      5 	AndroidManifest.xml
//      This is the manifest file which describes the fundamental characteristics of the app and defines each of its components.
//      
//      6 	Build.gradle
//      This is an auto generated file which contains compileSdkVersion, buildToolsVersion, applicationId, minSdkVersion, targetSdkVersion, versionCode and versionName

package com.Android1;

//LIBS: C:\WoMAengine2023\Android-WomaEngine\Android2\Android2.Packaging\jars
import android.util.Log;
import android.app.NativeActivity;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;

//Toast:
import android.view.Gravity;
import android.widget.Toast;

// DownloadFiles
import java.io.DataInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import android.os.Environment;
import java.net.URI;
import java.io.BufferedReader;
import java.net.URLConnection;
import java.io.InputStreamReader;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.OutputStreamWriter;
//import com.woma.R;

//https://www.cnblogs.com/MMLoveMeMM/articles/3610386.html  
public class MyActivity extends NativeActivity
{
Toast toast;

    protected void onCreate(Bundle savedInstanceState) {                                                                                                                                  
        //Log.w("[WOMA]Java", "JAVA:onCreate()");
        super.onCreate(savedInstanceState);    

        toast = Toast.makeText(MyActivity.this, "message", Toast.LENGTH_LONG);
        toast.setGravity(Gravity.CENTER  , 0, 0);
        //toast.show();
    }       

    // ShowAlert
    //--------------------------------------------------------
    public void ShowAlert(final String message)
    {
        Log.w("[WOMA]Java", "Toast showAlert(): "+message);

        toast.setText(message);
        toast.show();
    }
} 
