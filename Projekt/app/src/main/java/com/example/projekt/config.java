package com.example.projekt;

import org.json.JSONArray;
import org.json.JSONException;

import java.io.IOException;
import java.io.InputStream;

public class config {
    public static String readJSON(UIActivity context) {//parsuje
        String json = null;
        try {
            InputStream in = context.getAssets().open("constants.json");
            int size = in.available();
            byte[] bufor = new byte[size];
            in.read(bufor);
            in.close();
            json = new String(bufor, "UTF-8");
        } catch (IOException ex) {
            ex.printStackTrace();
            return null;
        }
        return json;
    }
    public static void load(UIActivity context) {
        int gravitation;
        int f_PWM;
        //String name;
        try {
            JSONArray jArray = new JSONArray(readJSON(context));
            gravitation=jArray.getJSONObject(0).getInt("number");
            f_PWM = jArray.getJSONObject(1).getInt("number");
            //name=jArray.getJSONObject(1).getString("name2");

        } catch (JSONException e) {
            gravitation=0;
            f_PWM = 0;
        }
        MainActivity.load(gravitation);
        // load(f_PWM);
    }
}
