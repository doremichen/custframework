package com.med.external;

import 	android.content.Context;

import android.renderscript.RenderScript;
import android.renderscript.Allocation;

public class TestRS {

    private RenderScript mRS;
    private Allocation mInAllocation;
    private Allocation mOutAllocation;
    private ScriptC_mono mScript;
    
    public TestRS(Context context) {
        mRS = RenderScript.create(context);
        mScript = new ScriptC_mono(mRS);
    }
    
    private void createScript(Allocation in, Allocation out) {
        mScript = new ScriptC_mono(mRS);
        mScript.forEach_root(in, out);
    }

}