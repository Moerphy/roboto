package org.metafnord.roboto;

import android.content.res.AssetManager;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.opengl.GLES20;
import android.opengl.GLUtils;

import java.io.InputStream;
import java.io.IOException;


public class TextureLoader{
  
  private static Context loaderContext;
  
  public static void setContext( Context c ){
    TextureLoader.loaderContext = c;
  }
  
  public static int[] loadTexture(int textureId, String name){
    AssetManager assetManager = loaderContext.getAssets();
    
    int[] dimensions = new int[2];
    
    Bitmap textureBitmap = null;
    BitmapFactory.Options opts = new BitmapFactory.Options();
    opts.inScaled = false;
    
    try{
      InputStream istr = assetManager.open(name);
      textureBitmap = BitmapFactory.decodeStream(istr, null, opts);
    }catch( IOException e ){
      // TODO logging
      Log.e("robotoJS", "Could not load texture from assets", e);
    }
    
    // todo: bind to textureId
    if( textureBitmap == null ){
      dimensions[0] = 0;
      dimensions[1] = 0;
    }else{
      GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, textureBitmap, 0);
      dimensions[0] = textureBitmap.getWidth();
      dimensions[1] = textureBitmap.getHeight();
    }
        
    return dimensions;
  }
  
}

/*


public static Bitmap getBitmapFromAsset(Context context, String strName) {
    AssetManager assetManager = context.getAssets();

    InputStream istr;
    Bitmap bitmap = null;
    try {
        istr = assetManager.open(strName);
        bitmap = BitmapFactory.decodeStream(istr);
    } catch (IOException e) {
        return null;
    }

    return bitmap;
}
*
public static int loadGLTextureFromBitmap( Bitmap bitmap, GL10 gl ) {

    // Generate one texture pointer
    int[] textureIds = new int[1];
    gl.glGenTextures( 1, textureIds, 0 );

    // bind this texture
    gl.glBindTexture( GL10.GL_TEXTURE_2D, textureIds[0] );

    // Create Nearest Filtered Texture
    gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR );
    gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR );

    gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_S, GL10.GL_REPEAT );
    gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_T, GL10.GL_REPEAT );

    // Use the Android GLUtils to specify a two-dimensional texture image from our bitmap
    GLUtils.texImage2D( GL10.GL_TEXTURE_2D, 0, bitmap, 0 );

    r }eturn textureIds[0];
}
*/
