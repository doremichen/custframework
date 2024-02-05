/**
 * File: MainActivity.java
 * Breif: Test cust framework app
 *
 * Name: Adam chen
 * Date: 
 */

package com.cust.app.testcustframework;

import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.Toast;
import android.app.ListActivity;

import com.cust.framework.hardware.specialEffect.SpecialEffectManager;
import com.cust.framework.hardware.specialEffect.SpecialEffectManager.OnStatusListener;
import com.cust.framework.util.Print;


public class MainActivity extends ListActivity implements OnStatusListener {
    
    private static final String TAG = MainActivity.class.getSimpleName();
    
    private SpecialEffectManager mSPEManager;
    
    private ListAdapter mAdapter;
    
    private static final int ITEM1 = 0;
    private static final int ITEM2 = 1;
    private static final int Exit = 2;
    
    private final String[] mStrings = new String[]{
        "turn on",
        "turn off",
        "exit"
    };
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Print.i(TAG, "[onCreate] enter");
        
        mSPEManager = SpecialEffectManager.getInstance();
        mSPEManager.init(this);
        
        mAdapter = (ListAdapter) new ArrayAdapter<String>(this, 
                android.R.layout.simple_list_item_1,
                mStrings);
        
        this.setListAdapter(mAdapter);
        
        
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mSPEManager.release();

    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        super.onListItemClick(l, v, position, id);
        Print.i(TAG, "onListItemClick: position[" + position + "]");
        switch(position) {
            case ITEM1:
                mSPEManager.turnOn();
                break;
            case ITEM2:
                mSPEManager.turnOff();
                break;
            case Exit:
                finish();
                break;
            default:                
                break;
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    /**
     * As following methods are invoked from SpecialEffectManager controller
     */
    @Override
    public void onError(String str) {
        Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
    }
    
    @Override
    public void onInfo(String str) {
        Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
    }
 

}
