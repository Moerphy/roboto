package org.metafnord.roboto;

import android.os.Bundle;

public class NativeActivity extends android.app.NativeActivity {
  
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    TextureLoader.setContext( this );
    // set up android nativeactivity like normal (calls native code in the process)
    super.onCreate(savedInstanceState);
  }
  
}
