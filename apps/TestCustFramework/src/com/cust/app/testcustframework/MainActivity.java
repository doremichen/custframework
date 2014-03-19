
package com.cust.app.testcustframework;

import com.cust.server.SpecialEffectsService;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.Toast;
import android.app.ListActivity;

import custdroid.hardware.SpecialEffectManager;


public class MainActivity extends ListActivity {

    private static final String TAG = "MainActivity";
    private static final boolean DEBUG = true;
    
    private ListAdapter mAdapter = null;
    
    private static final int ITEM1 = 0;
    private static final int ITEM2 = 1;
    private static final int Exit = 2;
    
    
    private SpecialEffectsService sfe_svr = null;
    
    private final String[] mStrings = new String[]{
        "turn on",
        "turn off",
        "exit"
    };
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        
        sfe_svr = SpecialEffectManager.getSpecialEffectsService(this);
        
        mAdapter = (ListAdapter) new ArrayAdapter<String>(this, 
                android.R.layout.simple_list_item_1,
                mStrings);
        
        this.setListAdapter(mAdapter);
        
        
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        sfe_svr.release();

    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        // TODO Auto-generated method stub
        super.onListItemClick(l, v, position, id);
        
        switch(position) {
            case ITEM1:
//                Toast.makeText(this, getText(R.string.str_list_item1), Toast.LENGTH_SHORT).show();
                sfe_svr.SpecialEffectsOn();
                break;
            case ITEM2:
//                Toast.makeText(this, getText(R.string.str_list_item2), Toast.LENGTH_SHORT).show();
                sfe_svr.SpecialEffectsOff();
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
        // TODO Auto-generated method stub
        super.onPause();
    }

    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        super.onResume();
    }

   
    private void PrintfI(String str) {
        if(DEBUG) Log.i(TAG, str);
    }
    
   

}
