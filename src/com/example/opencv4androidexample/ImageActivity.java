package com.example.opencv4androidexample;

import java.io.File;
import java.io.FileOutputStream;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;

import android.app.Activity;
import android.content.ContentValues;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.provider.MediaStore.Images;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

public class ImageActivity extends Activity{
	private ImageView imageView;
	private EditText editText;
	private Button button;
	private Bitmap bmp;
	
	private Mat src, dst;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_image);
		
		src = MainActivity.mFrameForProc.clone();
		dst = new Mat(src.size(), CvType.CV_8UC4);
		ImageProc.CartoonProcNew(src.getNativeObjAddr(), dst.getNativeObjAddr());
		
		imageView = (ImageView) findViewById(R.id.imageView);
		bmp = Bitmap.createBitmap(dst.width(), dst.height(), Config.ARGB_8888);
		Utils.matToBitmap(dst, bmp);
		imageView.setImageBitmap(bmp);
		editText = (EditText) findViewById(R.id.editText);
		editText.setText("new.png");
		button = (Button) findViewById(R.id.button);
		button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				boolean sdCardExist = Environment.getExternalStorageState()
						.equals(Environment.MEDIA_MOUNTED);
				if (sdCardExist) {
					try {
						//String fileName = "new.png";
						String fileName = editText.getText().toString();
						String path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).getAbsolutePath() + "/";
						File file = new File(path, fileName);
						FileOutputStream out = new FileOutputStream(file);
						
						bmp.compress(Bitmap.CompressFormat.PNG, 90, out);
						out.flush();
						out.close();
						
						ContentValues image = new ContentValues();
						image.put(Images.Media.TITLE, fileName);
						image.put(Images.Media.DISPLAY_NAME, fileName);
						image.put(Images.Media.DATE_TAKEN, System.currentTimeMillis());
						image.put(Images.Media.MIME_TYPE, "image/png");
						image.put(Images.Media.DATA, file.getAbsolutePath());
						ImageActivity.this.getContentResolver().insert(
								MediaStore.Images.Media.EXTERNAL_CONTENT_URI, image);
						
						Toast.makeText(ImageActivity.this, "Image has been saved to " + path, Toast.LENGTH_SHORT).show();
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
				else {
					Toast.makeText(ImageActivity.this, "SD Card Doesn't Exist!", Toast.LENGTH_SHORT).show();
				}
			}
		});
	}
	
}
